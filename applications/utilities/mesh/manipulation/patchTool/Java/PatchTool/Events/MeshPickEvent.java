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

Description

\*---------------------------------------------------------------------------*/

package PatchTool.Events;

import com.sun.j3d.utils.picking.PickTool;
import com.sun.j3d.utils.picking.PickCanvas;
import com.sun.j3d.utils.picking.PickResult;
import com.sun.j3d.utils.picking.PickIntersection;
//import javax.media.j3d.*;
import javax.vecmath.Point3d;

import PatchTool.Shapes.PickableShape;


public class MeshPickEvent
    extends java.util.EventObject
{
    protected PickableShape shape_;

    protected PickResult pickResult_;

    protected int faceI_;

    protected int edgeI_;

    protected int vertI_;

    protected Point3d point_;

    /** MeshPickEvent constructor. */
    public MeshPickEvent
    (
        Object source,
        PickableShape shape,
        PickResult pickResult,
        int faceI,
        int edgeI,
        int vertI,
        Point3d point
    )
    {
        super(source);
        shape_ = shape;
        pickResult_ = pickResult;
        faceI_ = faceI;
        edgeI_ = edgeI;
        vertI_ = vertI;
        point_ = point;
    }

    public String toString()
    {
        return "MeshPickEvent : " + shape_ + " " + pickResult_;
    }

    public PickableShape getShape()
    {
        return shape_;
    }

    public PickResult getPickResult()
    {
        return pickResult_;
    }

    public int getFace()
    {
        return faceI_;
    }

    public int getEdge()
    {
        return edgeI_;
    }

    public int getVertex()
    {
        return vertI_;
    }

    public Point3d getPoint()
    {
        return point_;
    }
}



