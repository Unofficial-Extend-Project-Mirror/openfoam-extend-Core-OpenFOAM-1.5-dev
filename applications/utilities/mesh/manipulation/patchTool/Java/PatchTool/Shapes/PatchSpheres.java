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

import com.sun.j3d.utils.geometry.GeometryInfo;
import com.sun.j3d.utils.geometry.NormalGenerator;
import com.sun.j3d.utils.geometry.Stripifier;
import com.sun.j3d.utils.geometry.Triangulator;
import com.sun.j3d.utils.picking.PickIntersection;
import com.sun.j3d.utils.picking.PickResult;
import com.sun.j3d.utils.picking.PickTool;

import javax.media.j3d.*;
import javax.vecmath.Color3f;

import javax.vecmath.Point3f;

import PatchTool.Mesh.PrimitivePatch;
import PatchTool.InteractiveNodes.PatchSpheresNode;
import PatchTool.Util.PickDragBehavior;

public class PatchSpheres extends Shape3D implements PickableShape
{
    static int[] tetStrips =
    {
        4
    };
    static Point3f tetCoords =
    {
        new Point3f(0.0f, 0.0f, 0.0f),
        new Point3f(0.0f, 0.0f, 0.0f),
        new Point3f(0.0f, 0.0f, 0.0f),
        new Point3f(0.0f, 0.0f, 0.0f)
    };
    static float[] components = new float[3];
    static float[] tetComponents = new float[3];

    static final int UNPICKEDCOLOR = 0;
    static final int PICKEDCOLOR = 1;


    PrimitivePatch patch_;
    Color3f color_;

    public PatchSpheres
    (
        PatchSpheresNode PatchSpheresNode,
        PrimitivePatch patch,
        Color3f color
    )
    {
        // Store reference to input data
        patch_ = patch;
        color_ = color;

        // Color table
        Color3f[] colors = new Color3f[2];
        colors[UNPICKEDCOLOR] = color_;
        colors[PICKEDCOLOR] = new Color3f(0.5f, 1.0f, 0.2f);


        Point3f[] coords = patch_.getPoints();

        for(int i = 0; i < coords.length; i++)
        {
            Geometry sphere = makeSphere(coords[i]);
            this.insertGeometry(sphere, i);
        }
        PickTool.setCapabilities(this, PickTool.INTERSECT_FULL);

        this.setCapability(Shape3D.ALLOW_APPEARANCE_READ);
        this.setCapability(Shape3D.ALLOW_BOUNDS_READ);
    }

    //--------------------------------------------------------------------------

    Geometry makeSphere(Point3f coord, float dist)
    {
        // Actually make tet
        array = new TriangeStripArray
        (
            4,
            TriangeStripArray.COORDINATES,
            tetStrips
        );

        coord.get(components);

        tetCoords[0].get(tetComponents);
        tetComponents[0] = components[0];
        tetComponents[1] = components[1] - dist;
        tetComponents[2] = components[2] - dist;
        tetCoords[0].set(tetComponents);

        tetCoords[1].get(tetComponents);
        tetComponents[0] = components[0] - dist;
        tetComponents[1] = components[1] - dist;
        tetComponents[2] = components[2] + dist;
        tetCoords[1].set(tetComponents);

        tetCoords[2].get(tetComponents);
        tetComponents[0] = components[0] + dist;
        tetComponents[1] = components[1] - dist;
        tetComponents[2] = components[2] + dist;
        tetCoords[2].set(tetComponents);

        tetCoords[3].get(tetComponents);
        tetComponents[0] = components[0] - dist;
        tetComponents[1] = components[1] + dist;
        tetComponents[2] = components[2] - dist;
        tetCoords[3].set(tetComponents);

        
    }


    //--------------------------------------------------------------------------
    //---- PickableShape Interface
    //--------------------------------------------------------------------------

    // Callback for if picked.
    public void picked(PickResult pickResult)
    {
        IndexedPointArray array = (IndexedPointArray)this.getGeometry();

        int nearest = PickDragBehavior.getNearestIndex(pickResult);
        PickIntersection pi = pickResult.getIntersection(nearest);

        SurfacePatch sp = patch_.getSurfacePatch();

        System.out.println
        (
            "Picked on Point:" + pi.getPointCoordinates()
          + " Patch:" + sp.getName()
        );
    }

    //--------------------------------------------------------------------------

    // Callback for if unpicked.
    public void unpicked(PickResult pickResult)
    {
        System.out.println("\n\nUnpicked on patch " + patch_.getPatchNo());

        IndexedPointArray array = (IndexedPointArray)this.getGeometry();

        int nearest = PickDragBehavior.getNearestIndex(pickResult);
        PickIntersection pi = pickResult.getIntersection(nearest);

        SurfacePatch sp = patch_.getSurfacePatch();

        System.out.println
        (
            "Unpicked on Point:" + pi.getPointCoordinates()
          + " Patch:" + sp.getName()
        );
    }

    //--------------------------------------------------------------------------

    // Callback for if picked.
    public void pickedAll(PickResult pickResult)
    {
        SurfacePatch sp = patch_.getSurfacePatch();

        System.out.println("\n\nPickedAll on patch " + sp.getName());
    }

    //--------------------------------------------------------------------------

    // Callback for if unpicked.
    public void unpickedAll(PickResult pickResult)
    {
        SurfacePatch sp = patch_.getSurfacePatch();

        System.out.println("\n\nUnpickedAll on patch " + sp.getName());
    }

    //--------------------------------------------------------------------------

    // Completely remove
    public void remove()
    {
        //System.out.println("remove:" + patch_.getPatchNo());
        if (this.numGeometries() != 0)
        {
            this.removeGeometry(0);
        }
    }

    //--------------------------------------------------------------------------
}
