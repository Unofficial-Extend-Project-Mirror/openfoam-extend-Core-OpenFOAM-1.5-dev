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
package PatchTool.Shapes;

//import com.sun.j3d.utils.geometry.GeometryInfo;
//import com.sun.j3d.utils.geometry.NormalGenerator;
//import com.sun.j3d.utils.geometry.Stripifier;
//import com.sun.j3d.utils.geometry.Triangulator;
//import com.sun.j3d.utils.picking.PickIntersection;
import com.sun.j3d.utils.picking.PickResult;
//import com.sun.j3d.utils.picking.PickTool;

import javax.media.j3d.*;
import javax.vecmath.Color3f;
import javax.vecmath.Point3f;
import javax.vecmath.Vector3f;

import PatchTool.InteractiveNodes.ColorBarNode;
import PatchTool.Util.Colorizer;

public class ColorBar extends Shape3D implements PickableShape
{
    // Create coloured triangles.
    public ColorBar
    (
        final ColorBarNode node,
        final Colorizer ci,
        final float min,
        final float max
    )
    {
        this.setGeometry(ci.colorBar(min, max));
        this.setCapability(Shape3D.ALLOW_BOUNDS_READ);
    }

    //--------------------------------------------------------------------------
    //---- PickableShape Interface
    //--------------------------------------------------------------------------

    // Callback for if picked.
    public void picked(PickResult pickResult)
    {
        System.out.println("\n\nPicked on ColorBar ");
    }

    //--------------------------------------------------------------------------

    // Callback for if unpicked.
    public void unpicked(PickResult pickResult)
    {
        System.out.println("\n\nUnpicked on ColorBar ");
    }

    //--------------------------------------------------------------------------

    // Callback for if picked.
    public void pickedAll(PickResult pickResult)
    {
        System.out.println("\n\nPickedAll on ColorBar ");
    }

    //--------------------------------------------------------------------------

    // Callback for if unpicked.
    public void unpickedAll(PickResult pickResult)
    {
        System.out.println("\n\nUnpickedAll on ColorBar ");
    }

    //--------------------------------------------------------------------------

    // Completely remove
    public void remove()
    {
        System.out.println("remove of ColorBar");

        if (this.numGeometries() != 0)
        {
            this.removeGeometry(0);
        }
    }

    //--------------------------------------------------------------------------
}
