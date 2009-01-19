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
import javax.media.j3d.*;
import javax.vecmath.*;
import com.sun.j3d.utils.geometry.Sphere;
import com.sun.j3d.utils.picking.PickTool;
import com.sun.j3d.utils.picking.PickResult;
import com.sun.j3d.utils.picking.PickIntersection;

import PatchTool.Shapes.PickableShape;

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
    Transform3D translation_ = new Transform3D();

    Canvas3D canvas3D_;
    BranchGroup branchGroup_;
    TransformGroup transformGroup_;

    PickRay pickRay_ = new PickRay();
    SceneGraphPath sceneGraphPath_[];
    boolean parallel_;
    Vector3d transVector_;

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

        parallel_ = true;
    }

    public void initialize()
    {
        System.out.println("PickDragBehavior: initializing");

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

    public void processStimulus(Enumeration criteria)
    {
        WakeupCriterion wakeup;
        AWTEvent[] event;
        int id;
        int dx, dy;

        while (criteria.hasMoreElements())
        {
            wakeup = (WakeupCriterion) criteria.nextElement();
            if (wakeup instanceof WakeupOnAWTEvent)
            {
                event = ((WakeupOnAWTEvent)wakeup).getAWTEvent();
                for (int i=0; i<event.length; i++)
                {
                    id = event[i].getID();
                    if (id == MouseEvent.MOUSE_DRAGGED)
                    {
                        x_ = ((MouseEvent)event[i]).getX();
                        y_ = ((MouseEvent)event[i]).getY();

                        dx = x_ - x_last_;
                        dy = y_ - y_last_;

                        // rotate
	                if (rotate((MouseEvent)event[i]))
                        {

                            x_angle_ = dy * y_factor_;
                            y_angle_ = dx * x_factor_;

                            transformX_.rotX(x_angle_);
                            transformY_.rotY(y_angle_);

                            modelTrans_.mul(transformX_, modelTrans_);
                            modelTrans_.mul(transformY_, modelTrans_);
                        }
                        else if (translate((MouseEvent)event[i]))
                        {
		            transformGroup_.getTransform(modelTrans_);
                            modelTrans_.get(transVector_);

                            transVector_.x += dx*0.1;
                            transVector_.y -= dy*0.1;

                            modelTrans_.setTranslation(transVector_);
                        }
                        else if (zoom((MouseEvent)event[i]))
                        {
                            double scale = modelTrans_.getScale();
                            scale += dy*0.01;

                            scale = Math.max(1E-6, scale);
                            //System.out.println("scale:" + scale);
                            modelTrans_.setScale(scale);
                        }

                        transformGroup_.setTransform(modelTrans_);

                        x_last_ = x_;
                        y_last_ = y_;
                    }
                    else if (id == MouseEvent.MOUSE_PRESSED)
                    {
                        x_ = x_last_ = ((MouseEvent)event[i]).getX();
                        y_ = y_last_ = ((MouseEvent)event[i]).getY();

                        //
                        // Picking
                        //

                        Point3d eyePos = new Point3d();
                        canvas3D_.getCenterEyeInImagePlate(eyePos);

                        Point3d mousePos = new Point3d();
                        canvas3D_.getPixelLocationInImagePlate(x_, y_, mousePos);

                        Transform3D transform3D = new Transform3D();
                        canvas3D_.getImagePlateToVworld(transform3D);

                        transform3D.transform(eyePos);
                        transform3D.transform(mousePos);

                        Vector3d mouseVec;
                        if (parallel_)
                        {
                            mouseVec = new Vector3d(0.f, 0.f, -1.f);
                        }
                        else
                        {
                            mouseVec = new Vector3d();
                            mouseVec.sub(mousePos, eyePos);
                            mouseVec.normalize();
                        }

                        pickRay_.set(mousePos, mouseVec);
                        sceneGraphPath_ = branchGroup_.pickAllSorted(pickRay_);

                        if (sceneGraphPath_ != null)
                        {
                            for (int j=0; j<sceneGraphPath_.length; j++)
                            {
                                System.out.println("Picked:" + sceneGraphPath_[j]);
                                if (sceneGraphPath_[j] != null)
                                {
                                    Node node = sceneGraphPath_[j].getObject();

                                    if (node instanceof PickableShape)
                                    {
                                        PickResult pickRes =  new PickResult
                                        (
                                            sceneGraphPath_[j],
                                            pickRay_
                                        );

                                        ((PickableShape)node).picked(pickRes);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        wakeupOn (mouseCriterion_);
    }

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
}
