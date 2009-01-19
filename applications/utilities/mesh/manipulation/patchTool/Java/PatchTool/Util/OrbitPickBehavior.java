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

import javax.media.j3d.*;
import com.sun.j3d.utils.picking.PickTool;
import com.sun.j3d.utils.picking.PickResult;
import com.sun.j3d.utils.picking.PickIntersection;
import com.sun.j3d.utils.picking.behaviors.PickMouseBehavior;
import java.util.*;
import java.awt.*;
import java.awt.Event;
import java.awt.AWTEvent;
import java.awt.event.MouseEvent;
import javax.vecmath.*;

import PatchTool.InteractiveNodes.InteractiveNode;
import PatchTool.Shapes.PickableShape;

public class OrbitPickBehavior extends PickMouseBehavior
{
    protected static final int ILLEGALACTION = 0;
    protected static final int PICKACTION = 1;
    protected static final int UNPICKACTION = 2;
    protected static final boolean debug = false;

    protected int action = ILLEGALACTION;

    public OrbitPickBehavior
    (
        Canvas3D canvas,
        BranchGroup root,
        Bounds bounds
    )
    {
        super(canvas, root, bounds);
        this.setSchedulingBounds(bounds);
        root.addChild(this);
        pickCanvas.setMode(PickTool.GEOMETRY_INTERSECT_INFO);
        //pickCanvas.setMode(PickTool.BOUNDS);
        pickCanvas.setTolerance(2.0f);
    }

    // Register with event handler
    public void initialize() {

        conditions = new WakeupCriterion[2];
        conditions[0] = new WakeupOnAWTEvent(Event.MOUSE_MOVE);
        conditions[1] = new WakeupOnAWTEvent(Event.MOUSE_DOWN);
        wakeupCondition = new WakeupOr(conditions);

        wakeupOn(wakeupCondition);
    }
  

    // Called at wakeup. Discard all but last event.
    public void processStimulus (Enumeration criteria)
    {
        WakeupCriterion wakeup;
        AWTEvent[] evt = null;
        int xpos = 0, ypos = 0;

        while(criteria.hasMoreElements())
        {
            wakeup = (WakeupCriterion)criteria.nextElement();
            if (wakeup instanceof WakeupOnAWTEvent)
            {
                evt = ((WakeupOnAWTEvent)wakeup).getAWTEvent();
            }
        }

        if (evt[0] instanceof MouseEvent)
        {
            mevent = (MouseEvent) evt[0];

            if
            (
                (
                    mevent.getID()==MouseEvent.MOUSE_PRESSED |
                    mevent.getID()==MouseEvent.MOUSE_CLICKED
                )
             && mevent.isShiftDown()
            )
            {
                xpos = mevent.getPoint().x;
                ypos = mevent.getPoint().y;

                int modifiers = mevent.getModifiers();

                action = ILLEGALACTION;
                if ((modifiers & mevent.BUTTON1_MASK) != 0)
                {
                    action = PICKACTION;
                    updateScene(xpos, ypos);
                }
                if ((modifiers & mevent.BUTTON2_MASK) != 0)
                {
                    // ?
                }
                if ((modifiers & mevent.BUTTON3_MASK) != 0)
                {
                    action = UNPICKACTION;
                    updateScene(xpos, ypos);
                }
            }
        }
        wakeupOn (wakeupCondition);
    }


    // Do something based on (un)picking action.
    public void updateScene(int xpos, int ypos)
    {
        try
        {

            PickResult pickResult = null;
    	    Shape3D shape = null;

    	    pickCanvas.setShapeLocation(xpos, ypos);

    	    pickResult = pickCanvas.pickClosest();
            if (pickResult != null)
            {
                shape = (Shape3D)pickResult.getNode(PickResult.SHAPE3D);
                if (shape instanceof PickableShape)
                {
                    PickableShape pickableShape = (PickableShape)shape;

                    if (action == PICKACTION)
                    {
                        pickableShape.picked(pickResult);
                    }
                    else if (action == UNPICKACTION)
                    {
                        pickableShape.unpicked(pickResult);
                    }
                }
            }
        }
        catch(CapabilityNotSetException ex)
        {
            System.out.println(ex);
        }
    }



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

            if (debug)
            {
                System.out.println("minDist  : " + minDist);
                System.out.println("minIndex : " + minIndex);
            }
            return minIndex;
        }
        else
        {
            System.out.println("**\n** Illegal number of intersections:" + nInters);
            return -1;
        }
    }



    // Utility function to print shape info.
    static void printShape(Object subShape)
    {
        if (subShape.getClass() == javax.media.j3d.Text3D.class)
        {
            Text3D subText = (Text3D)subShape;
            System.out.println("Text3D:" + subText.getString());
        }   
        else if (subShape.getClass() == javax.media.j3d.TriangleStripArray.class)
        {
            TriangleStripArray strip = (TriangleStripArray)subShape;

            int nVert = strip.getVertexCount();
            System.out.println("triangleStripArray of length" + nVert);
            Point3d[] vertices = new Point3d[nVert];
            for (int i = 0; i < vertices.length; i++)
            {
                 vertices[i] = new Point3d();
            }
            strip.getCoordinates(0, vertices);
            for(int i =  0; i < vertices.length; i++)
            {
                System.out.println("vertex " + vertices[i]);
            }
        }
        else if (subShape.getClass() == javax.media.j3d.IndexedTriangleArray.class)
        {
            IndexedTriangleArray strip = (IndexedTriangleArray)subShape;

            int nVert = strip.getVertexCount();
            System.out.println("IndexedTriangleArray of length" + nVert);
            Point3d[] vertices = new Point3d[nVert];
            for (int i = 0; i < vertices.length; i++)
            {
                 vertices[i] = new Point3d();
            }
            strip.getCoordinates(0, vertices);
            for(int i =  0; i < vertices.length; i++)
            {
                System.out.println("vertex " + vertices[i]);
            }
        }
        else if (subShape.getClass() == javax.media.j3d.TriangleArray.class)
        {
            TriangleArray strip = (TriangleArray)subShape;

            int nVert = strip.getVertexCount();
            System.out.println("triangleArray of length" + nVert);
            Point3d[] vertices = new Point3d[nVert];
            for (int i = 0; i < vertices.length; i++)
            {
                 vertices[i] = new Point3d();
            }
            strip.getCoordinates(0, vertices);

            for(int i =  0; i < vertices.length; i++)
            {
                System.out.println("vertex " + vertices[i]);
            }
        }
        else
        {
            System.out.println("unknown shape" + subShape);
        }
    }

}
