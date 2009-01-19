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

import org.j3d.util.interpolator.ColorInterpolator;

import PatchTool.InteractiveNodes.ColoredSurfaceNode;
import PatchTool.Util.BinaryFoamFile;

public class ColoredSurface extends Shape3D implements PickableShape
{
    double creaseAngle_;

    public ColoredSurface
    (
        ColoredSurfaceNode node,
        double creaseAngle,
        String pointsFileName,
        String facesFileName,
        String valuesFileName,
        float min,
        float max
    )
    {
        // Store reference to input data
        creaseAngle_ = creaseAngle;

        System.out.println("Loading files ...");
        Point3f[] coords = BinaryFoamFile.readPoint3fArray(pointsFileName);
        int[] triFaces = BinaryFoamFile.readIntArray(facesFileName);
        float[] values = BinaryFoamFile.readFloatArray(valuesFileName);


        // Auto colour
        min = values[0];
        max = values[0];
        for(int i = 0; i < values.length; i++)
        {
            min = Math.min(min, values[i]);
            max = Math.max(max, values[i]);
        }
        System.out.println("min=" + min + "  max=" + max);

//        for(int i = 0; i < coords.length; i++)
//        {
//            System.out.println("i=" + i + " Coord=" + coords[i]);
//        }
//        for(int i = 0; i < triFaces.length; i++)
//        {
//            System.out.println("i=" + i + " TriFaces=" + triFaces[i]);
//        }
//        for(int i = 0; i < values.length; i++)
//        {
//            System.out.println("i=" + i + " Values=" + values[i]);
//        }


        System.out.println("Colouring ...");
        Color3f[] colors = new Color3f[values.length];
        valuesToVertexColors(values, colors, min, max);

        int[] colorIndices = new int[triFaces.length];


        System.out.println("Setting color indices ...");
        if (values.length == coords.length)
        {
            //- Vertex wise data
            for(int i = 0; i < triFaces.length; i++)
            {
                colorIndices[i] = triFaces[i];
            }
        }
        else if (3*values.length == triFaces.length)
        {
            //- Face wise data
            int vertI = 0;
            for(int faceI = 0; faceI < triFaces.length/3; faceI++)
            {
                colorIndices[vertI++] = faceI;
                colorIndices[vertI++] = faceI;
                colorIndices[vertI++] = faceI;
            }
        }
        else
        {
             System.out.println("**Illegal length of values" + values.length);
        }


        GeometryInfo geometryInfo =
            new GeometryInfo(GeometryInfo.TRIANGLE_ARRAY);
        System.out.println("Setting coordinates ...");
        geometryInfo.setCoordinates(coords);
        geometryInfo.setCoordinateIndices(triFaces);

        System.out.println("Setting colours ...");
        geometryInfo.setColors(colors);
        geometryInfo.setColorIndices(colorIndices);

        //- Generate normals
        System.out.println("Setting normals ...");
        NormalGenerator ng = new NormalGenerator(creaseAngle_);
        ng.generateNormals(geometryInfo);

        //- Make shape and set various
        System.out.println("Making geometryArray...");
        IndexedGeometryArray array = geometryInfo.getIndexedGeometryArray();

        this.setGeometry(array);
        //PickTool.setCapabilities(this, PickTool.INTERSECT_FULL);
        //this.setCapability(Shape3D.ALLOW_GEOMETRY_READ);
        //this.setCapability(Shape3D.ALLOW_APPEARANCE_READ);
        //array.setCapability(IndexedGeometryArray.ALLOW_COLOR_INDEX_WRITE);
        this.setCapability(Shape3D.ALLOW_BOUNDS_READ);
        System.out.println("Done making shape...");
    }

    // Convert values into colors
    static void valuesToVertexColors
    (
        float[] values, Color3f[] colors, float min, float max
    )
    {
        if (values.length != colors.length)
        {
            System.out.println
            (
                "Lengths different:" + values.length + " " + colors.length
            );
        }
        if (values.length == 0)
        {
            return;
        }

        ColorInterpolator ci = new ColorInterpolator(3);
        ci.addRGBKeyFrame(min            , 0.0f, 0.0f, 1.0f, 1.0f);
        float temp = (float)(0.5*min+0.5*max);
        ci.addRGBKeyFrame(temp, 0.0f, 1.0f, 0.0f, 1.0f);
        ci.addRGBKeyFrame(max            , 1.0f, 0.0f, 0.0f, 1.0f);

        for(int i = 0; i < values.length; i++)
        {
            float[] rgbColor = ci.floatRGBValue(values[i]);
            colors[i] = new Color3f(rgbColor[0], rgbColor[1], rgbColor[2]);
        }

    }


    //--------------------------------------------------------------------------
    //---- PickableShape Interface
    //--------------------------------------------------------------------------

    // Callback for if picked.
    public void picked(PickResult pickResult)
    {
        System.out.println("\n\nPicked on ColoredSurface ");
    }

    //--------------------------------------------------------------------------

    // Callback for if unpicked.
    public void unpicked(PickResult pickResult)
    {
        System.out.println("\n\nUnpicked on ColoredSurface ");
    }

    // Callback for if picked.
    public void pickedAll(PickResult pickResult)
    {
        System.out.println("\n\nPickedAll on ColoredSurface ");
    }

    //--------------------------------------------------------------------------

    // Callback for if unpicked.
    public void unpickedAll(PickResult pickResult)
    {
        System.out.println("\n\nUnpickedAll on ColoredSurface ");
    }

    //--------------------------------------------------------------------------

    // Completely remove
    public void remove()
    {
        System.out.println("remove of ColoredSurface");

        if (this.numGeometries() != 0)
        {
            this.removeGeometry(0);
        }
    }

    //--------------------------------------------------------------------------
}
