/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

\*---------------------------------------------------------------------------*/

package PatchTool.Util;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.awt.event.ActionEvent;
import javax.swing.event.*;
import javax.media.j3d.*;
import javax.vecmath.*;
import com.sun.j3d.utils.geometry.Sphere;
import com.sun.j3d.utils.picking.PickTool;
import com.sun.j3d.utils.picking.PickCanvas;
import com.sun.j3d.utils.picking.PickResult;
import com.sun.j3d.utils.picking.PickIntersection;

import PatchTool.Shapes.PickableShape;
import PatchTool.Events.MeshPickEvent;
import PatchTool.Events.PickListener;

public class PickDragBehavior extends Behavior
{
    WakeupCriterion[] mouseEvents_;
    WakeupOr mouseCriterion_;
    int x_, y_;
    int x_last_, y_last_;
    double x_angle_, y_angle_;
    double x_factor_, y_factor_;

    Transform3D modelTrans_ = new Transform3D();
    Transform3D transformX_ = new Transform3D();
    Transform3D transformY_ = new Transform3D();

    Canvas3D canvas3D_;
    BranchGroup branchGroup_;
    TransformGroup transformGroup_;

    PickCanvas pickCanvas_;

    Vector3d transVector_ = new Vector3d(0.0, 0.0, 0.0);

    public PickDragBehavior
    (
        Canvas3D canvas3D,
        BranchGroup branchGroup,
        TransformGroup transformGroup
    )
    {
        canvas3D_ = canvas3D;
        branchGroup_ = branchGroup;
        transformGroup_ = transformGroup;

        transVector_.x = 0.05;
        transVector_.y = 0.05;
        transVector_.z = 0.005;

        pickCanvas_ =  new PickCanvas(canvas3D_, branchGroup);
        pickCanvas_.setMode(PickTool.GEOMETRY_INTERSECT_INFO); 
        pickCanvas_.setTolerance(2.0f);

    }

    //--------------------------------------------------------------------------

    public void initialize()
    {
        x_ = 0;
        y_ = 0;
        x_last_ = 0;
        y_last_ = 0;
        x_angle_ = 0;
        y_angle_ = 0;
        x_factor_ = .02;
        y_factor_ = .02;

        mouseEvents_ = new WakeupCriterion[2];
        mouseEvents_[0] = new WakeupOnAWTEvent(MouseEvent.MOUSE_DRAGGED);
        mouseEvents_[1] = new WakeupOnAWTEvent(MouseEvent.MOUSE_PRESSED);
        mouseCriterion_ = new WakeupOr(mouseEvents_);
        wakeupOn (mouseCriterion_);
    }

    //--------------------------------------------------------------------------

    public void reset()
    {
        x_ = 0;
        y_ = 0;
        x_last_ = 0;
        y_last_ = 0;
        x_angle_ = 0;
        y_angle_ = 0;
        x_factor_ = .02;
        y_factor_ = .02;

        modelTrans_.setIdentity();
        modelTrans_.setScale(1.0);
        transformGroup_.setTransform(modelTrans_);
    }

    //--------------------------------------------------------------------------

    public void processStimulus(Enumeration criteria)
    {
        WakeupCriterion wakeup;
        AWTEvent[] event = null;
        MouseEvent mevent;
        int id;
        int dx, dy;

        // Discard all but last event
        while (criteria.hasMoreElements())
        {
            wakeup = (WakeupCriterion) criteria.nextElement();
            if (wakeup instanceof WakeupOnAWTEvent)
            {
                event = ((WakeupOnAWTEvent)wakeup).getAWTEvent();
            }
        }

        // Handle last mouse event
        if (event[0] instanceof MouseEvent)
        {
            mevent = (MouseEvent) event[0];

            id = event[0].getID();

            if (id == MouseEvent.MOUSE_DRAGGED)
            {
                x_ = mevent.getX();
                y_ = mevent.getY();

                dx = x_ - x_last_;
                dy = y_ - y_last_;

                // rotate
	        if (rotate(mevent))
                {
                    x_angle_ = dy * y_factor_;
                    y_angle_ = dx * x_factor_;

                    transformX_.rotX(x_angle_);
                    transformY_.rotY(y_angle_);

                    transformGroup_.getTransform(modelTrans_);
                    Matrix4d mat = new Matrix4d();

                    // Remember old matrix
                    modelTrans_.get(mat);

                    // Translate to origin
                    modelTrans_.setTranslation(transVector_);
                    modelTrans_.mul(transformX_, modelTrans_);
                    modelTrans_.mul(transformY_, modelTrans_);

                    // Set old translation back
                    Vector3d translation =
                        new Vector3d(mat.m03,mat.m13, mat.m23);
                    modelTrans_.setTranslation(translation);
                }
                else if (translate(mevent))
                {
		    transformGroup_.getTransform(modelTrans_);
                    modelTrans_.get(transVector_);

                    transVector_.x += dx*0.05;
                    transVector_.y -= dy*0.05;

                    modelTrans_.setTranslation(transVector_);
                }
                else if (zoom(mevent))
                {
                    double scale = modelTrans_.getScale();

                    if (dy > 0.0)
                    {
                        scale *= (1 + dy*0.01);
                    }
                    else
                    {
                        scale /= (1 - dy*0.01);
                    }

                    scale = Math.max(1E-6, scale);

                    modelTrans_.setScale(scale);
                }

                transformGroup_.setTransform(modelTrans_);

                x_last_ = x_;
                y_last_ = y_;
            }
            else if ((id == MouseEvent.MOUSE_PRESSED) && pick(mevent))
            {
                x_ = x_last_ = mevent.getX();
                y_ = y_last_ = mevent.getY();

                //
                // Picking
                //

                pickCanvas_.setShapeLocation(mevent);
                PickResult pickResult = pickCanvas_.pickClosest();

                if (pickResult != null)
                {
                    Shape3D shape =
                        (Shape3D)pickResult.getNode(PickResult.SHAPE3D);

                    if (shape instanceof PickableShape)
                    {
                        handlePickAction
                        (
                            mevent,
                            (PickableShape)shape,
                            pickResult
                        );
                    }
                }
            }
            else
            {
                x_ = x_last_ = mevent.getX();
                y_ = y_last_ = mevent.getY();
            }
        }
        wakeupOn (mouseCriterion_);
    }

    //--------------------------------------------------------------------------

    boolean rotate(MouseEvent evt)
    {
        if (evt.getModifiers() == evt.BUTTON1_MASK)
        {
            return true;
        }
        else
        {
	    return false;
        }
    }

    //--------------------------------------------------------------------------

    boolean zoom(MouseEvent evt)
    {
        if (evt.getModifiers() == evt.BUTTON2_MASK)
        {
            return true;
        }
        else
        {
	    return false;
        }
    }

    //--------------------------------------------------------------------------

    boolean translate(MouseEvent evt)
    {
        if (evt.getModifiers() == evt.BUTTON3_MASK)
        {
            return true;
        }
        else
        {
	    return false;
        }
    }

    //--------------------------------------------------------------------------

    boolean pick(MouseEvent evt)
    {
        if (evt.isShiftDown() ^ (evt.isControlDown()))
        {
            return true;
        }
        else
        {
	    return false;
        }
    }

    //--------------------------------------------------------------------------

    void handlePickAction
    (
        MouseEvent mevent,
        PickableShape shape,
        PickResult pickResult
    )
    {
        int modifiers = mevent.getModifiers();

        if ((modifiers & mevent.BUTTON1_MASK) != 0)
        {
            if (mevent.isShiftDown())
            {
                shape.picked(pickResult);
            }
            else if (mevent.isControlDown())
            {
                shape.pickedAll(pickResult);
            }
        }
        else if ((modifiers & mevent.BUTTON3_MASK) != 0)
        {
            if (mevent.isShiftDown())
            {
                shape.unpicked(pickResult);
            }
            else if (mevent.isControlDown())
            {
                shape.unpickedAll(pickResult);
            }
        }
    }

    //--------------------------------------------------------------------------

    // Utility function get nearest of multiple intersections
    public static int getNearestIndex(PickResult pickResult)
    {
        int nInters = pickResult.numIntersections();

        if (nInters == 1)
        {
            return 0;
        }
        else if (nInters > 1)
        {
            double minDist=1E30;

            int minIndex=-1;

            for(int i = 0; i < nInters; i++)
            {
                PickIntersection pi = pickResult.getIntersection(i);
                double dist = pi.getDistance();
                if (dist < minDist)
                {
                    minDist = pi.getDistance();
                    minIndex = pi.getGeometryArrayIndex();
                }
            }

            return minIndex;
        }
        else
        {
            System.out.println("**\n** Illegal number of intersections:" + nInters);
            return -1;
        }
    }

    //--------------------------------------------------------------------------
}
