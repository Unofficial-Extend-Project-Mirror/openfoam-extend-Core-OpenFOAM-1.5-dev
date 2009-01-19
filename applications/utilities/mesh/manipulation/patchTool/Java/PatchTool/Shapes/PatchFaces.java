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

import java.util.*;
import javax.swing.*;

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
import javax.vecmath.Point3d;

import PatchTool.PatchTool;
import PatchTool.Mesh.BoundaryMesh;
import PatchTool.Mesh.PrimitivePatch;
import PatchTool.Mesh.SurfacePatch;
import PatchTool.Mesh.FaceSet;
import PatchTool.InteractiveNodes.PatchFacesNode;
import PatchTool.Util.PickDragBehavior;
import PatchTool.Events.ElementEvent;
import PatchTool.Events.ElementSelectListener;
import PatchTool.Shapes.EditPanels.PolygonPropsDlg;
import PatchTool.Shapes.EditPanels.PolygonPropsPanel;

public class PatchFaces
    extends Shape3D
    implements PickableShape, ElementSelectListener
{
    static final int UNPICKEDCOLOR = 0;
    static final int PICKEDCOLOR = 1;

    PatchTool pp_;
    PrimitivePatch patch_;
    Color3f[] colors_;
    double creaseAngle_;
    int[] colorIndices_;

    public PatchFaces
    (
        PatchTool pp,
        PatchFacesNode PatchFacesNode,
        PrimitivePatch patch,
        Color3f color,
        double creaseAngle
    )
    {
        // Store reference to input data
        pp_ = pp;
        patch_ = patch;
        creaseAngle_ = creaseAngle;

        Point3f[] coords = patch.getPoints();
        int[] triFaces = patch.getTriFaces();

        GeometryInfo geometryInfo =
            new GeometryInfo(GeometryInfo.TRIANGLE_ARRAY);
        geometryInfo.setCoordinateIndices(triFaces);
        geometryInfo.setCoordinates(coords);


        colors_ = new Color3f[2];
        colors_[UNPICKEDCOLOR] = color;
        colors_[PICKEDCOLOR] = new Color3f(0.1f, 0.8f, 0.1f);
        geometryInfo.setColors(colors_);

        //- Start off with all triangles unpickedcolor
        colorIndices_ = new int[triFaces.length];
        Arrays.fill(colorIndices_, UNPICKEDCOLOR);
        geometryInfo.setColorIndices(colorIndices_);

        //- Generate normals
        NormalGenerator ng = new NormalGenerator(creaseAngle_);
        ng.generateNormals(geometryInfo);

        geometryInfo.recomputeIndices();
        geometryInfo.compact();

        //- Make shape and set various
        IndexedGeometryArray array = geometryInfo.getIndexedGeometryArray();

        this.setGeometry(array);

        // Set default appearance (with full editing capabilities)
        this.setAppearance(pp_.getPolyAppearance());

        // Set all capabilities

        PickTool.setCapabilities(this, PickTool.INTERSECT_FULL);

        array.setCapability(IndexedGeometryArray.ALLOW_COLOR_WRITE);
        array.setCapability(IndexedGeometryArray.ALLOW_COLOR_INDEX_WRITE);

        this.setCapability(Shape3D.ALLOW_GEOMETRY_WRITE);
        this.setCapability(Shape3D.ALLOW_APPEARANCE_READ);
        this.setCapability(Shape3D.ALLOW_BOUNDS_READ);

        // Make sure we display current faceSet

        FaceSet fs = patch_.getMesh().getFaceSet();
        fs.addElementSelectListener(this);
    }

    //--------------------------------------------------------------------------
    //---- PickableShape Interface
    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------

    // Utility function for handling picks.
    private void handlePicks(PickResult pickResult, boolean select)
    {
        IndexedGeometryArray array = (IndexedGeometryArray)this.getGeometry();

        int nearest = PickDragBehavior.getNearestIndex(pickResult);
        PickIntersection pi = pickResult.getIntersection(nearest);

        // Get mesh face index
        int[] indices = pi.getPrimitiveVertexIndices();

        int triIndex = indices[0] / 3;  // 3 labels per triangle

        // Get mesh vertex nearest to pick point
        int nearestIndex = pi.getClosestVertexIndex();
        int nearestVertI = array.getCoordinateIndex(indices[0] + nearestIndex);

        int patchFaceI = patch_.getTriToPoly()[triIndex];

        SurfacePatch sp = patch_.getSurfacePatch();

        int meshFaceI = patchFaceI + sp.getStart();

        Point3d pt = pi.getPointCoordinates();

        //System.out.println
        //(
        //    "Picked on:\n"
        //  + "    Coord:" + pt
        //  + "    Patch:" + sp.getName()
        //  + "    PatchFace:" + patchFaceI
        //  + "    MeshFace:" + meshFaceI
        //  + "    Nearest Vertex:" + nearestVertI
        //);


        //
        // Handle according to global pick mode
        //

        FaceSet fs = patch_.getMesh().getFaceSet();

        if ((pp_.getPickMode() & PatchTool.SELECT_SINGLE) != 0)
        {
            if (select)
            {
                fs.select(meshFaceI);
            }
            else
            {
                fs.deselect(meshFaceI);
            }
        }
        else if ((pp_.getPickMode() & PatchTool.SELECT_ALL) != 0)
        {
            // Cheat: work not on copy but on array itself.
            boolean[] selected = fs.getSelected();

            int startFaceI = sp.getStart();

            int endFaceI = startFaceI + sp.getSize();

            if (select)
            {
                for (int faceI = startFaceI; faceI < endFaceI; faceI++)
                {
                    selected[faceI] = true;
                }
            }
            else
            {
                for (int faceI = startFaceI; faceI < endFaceI; faceI++)
                {
                    selected[faceI] = false;
                }
            }

            fs.setSelected(selected);
        }

        //
        // Warn PatchTool.and any listeners (using mesh indices, 
        // not local indices)
        //

        if (select)
        {
            pp_.firePicked(this, pickResult, meshFaceI, -1, nearestVertI, pt);
        }
        else
        {
            pp_.fireUnpicked(this, pickResult, meshFaceI, -1, nearestVertI, pt);
        }
    }

    //--------------------------------------------------------------------------

    // Callback for if picked.
    public void picked(PickResult pickResult)
    {
        // Select based on pickResult
        handlePicks(pickResult, true);
    }

    //--------------------------------------------------------------------------

    // Callback for if unpicked.
    public void unpicked(PickResult pickResult)
    {
        // Deselect based on pickResult
        handlePicks(pickResult, false);
    }

    //--------------------------------------------------------------------------

    // Callback for if picked all.
    public void pickedAll(PickResult pickResult)
    {
        // Change to select-all mode and select all faces.
        int oldPickMode = pp_.getPickMode();

        pp_.setPickMode(PatchTool.SELECT_ALL);

        handlePicks(pickResult, true);

        pp_.setPickMode(oldPickMode);
    }


    //--------------------------------------------------------------------------

    // Callback for if unpicked all.
    public void unpickedAll(PickResult pickResult)
    {
        // Change to select-all mode and deselect all faces.
        int oldPickMode = pp_.getPickMode();

        pp_.setPickMode(PatchTool.SELECT_ALL);

        handlePicks(pickResult, false);

        pp_.setPickMode(oldPickMode);
    }

    //--------------------------------------------------------------------------

    // Completely remove
    public void remove()
    {
        FaceSet fs = patch_.getMesh().getFaceSet();

        fs.removeElementSelectListener(this);

        if (this.numGeometries() != 0)
        {
            this.removeGeometry(0);
        }
    }

    //--------------------------------------------------------------------------

    // Edit
    public void edit()
    {
        PolygonPropsDlg editDlg = new 
            PolygonPropsDlg
            (
                null,
                colors_[UNPICKEDCOLOR]
            );

        if (editDlg.getState() == PolygonPropsDlg.OK)
        {
            PolygonPropsPanel editPnl = editDlg.getPolygonPropsPanel();

            try
            {
                Color3f color = editPnl.getColor3f();

                // Check change of color
                if (!color.equals(colors_[UNPICKEDCOLOR]))
                {
                    System.out.println("Changing color to " + color);

                    IndexedGeometryArray array =
                        (IndexedGeometryArray)this.getGeometry();

                    colors_[UNPICKEDCOLOR] = color;

                    array.setColors(0, colors_);
                }
            }
            catch (NumberFormatException ex)
            {
                System.out.println("** Format exception:" + ex);
            }
        }
    }

    //--------------------------------------------------------------------------
    //---- ElementSelectListener Implementation
    //--------------------------------------------------------------------------

    public void elementSelected(ElementEvent evt)
    {
        // Color all triangles of face

        SurfacePatch sp = patch_.getSurfacePatch();

        IndexedGeometryArray array = (IndexedGeometryArray)this.getGeometry();

        int meshFaceI = evt.getElem();

        if
        (
            (meshFaceI >= sp.getStart())
         && (meshFaceI < sp.getStart() + sp.getSize())
        )
        {
            int patchFaceI = meshFaceI - sp.getStart();


            int nTris = patch_.getNTris()[patchFaceI];

            int triStart = patch_.getPolyToTri()[patchFaceI];

            int vertI = 3 * triStart;

            for(int triI = 0; triI < nTris; triI++)
            {
                array.setColorIndex(vertI++, PICKEDCOLOR);
                array.setColorIndex(vertI++, PICKEDCOLOR);
                array.setColorIndex(vertI++, PICKEDCOLOR);
            }
        }
    }

    //--------------------------------------------------------------------------

    public void elementDeselected(ElementEvent evt)
    {
        // Color all triangles of face

        SurfacePatch sp = patch_.getSurfacePatch();

        IndexedGeometryArray array = (IndexedGeometryArray)this.getGeometry();

        int meshFaceI = evt.getElem();

        if
        (
            (meshFaceI >= sp.getStart())
         && (meshFaceI < sp.getStart() + sp.getSize())
        )
        {
            int patchFaceI = meshFaceI - sp.getStart();

            int nTris = patch_.getNTris()[patchFaceI];

            int triStart = patch_.getPolyToTri()[patchFaceI];

            int vertI = 3 * triStart;

            for(int triI = 0; triI < nTris; triI++)
            {
                array.setColorIndex(vertI++, UNPICKEDCOLOR);
                array.setColorIndex(vertI++, UNPICKEDCOLOR);
                array.setColorIndex(vertI++, UNPICKEDCOLOR);
            }
        }
    }

    //--------------------------------------------------------------------------

    public void selectedSetChanged(ElementEvent evt)
    {
        SurfacePatch sp = patch_.getSurfacePatch();

        // Get array of selected faces
        boolean[] selected = patch_.getMesh().getFaceSet().getSelected();

        int meshFaceI = sp.getStart();

        for (int patchFaceI = 0; patchFaceI < sp.getSize(); patchFaceI++)
        {
            int vertI = 3*patch_.getPolyToTri()[patchFaceI];

            int nTris = patch_.getNTris()[patchFaceI];

            if (selected[meshFaceI])
            {
                for (int triI = 0; triI < nTris; triI++)
                {
                    colorIndices_[vertI++] = PICKEDCOLOR;
                    colorIndices_[vertI++] = PICKEDCOLOR;
                    colorIndices_[vertI++] = PICKEDCOLOR;
                }
            }
            else
            {
                for (int triI = 0; triI < nTris; triI++)
                {
                    colorIndices_[vertI++] = UNPICKEDCOLOR;
                    colorIndices_[vertI++] = UNPICKEDCOLOR;
                    colorIndices_[vertI++] = UNPICKEDCOLOR;
                }
            }
            meshFaceI++;
        }

        // Update geometry.
        IndexedGeometryArray array = (IndexedGeometryArray)this.getGeometry();

        array.setColorIndices(0, colorIndices_);
    }

    //--------------------------------------------------------------------------
}
