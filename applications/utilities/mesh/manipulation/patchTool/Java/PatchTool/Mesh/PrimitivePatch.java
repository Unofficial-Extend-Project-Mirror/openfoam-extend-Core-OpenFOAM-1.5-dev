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

package PatchTool.Mesh;

import java.io.*;

import javax.media.j3d.*;
import javax.vecmath.Point3f;
import javax.vecmath.Vector3f;

public class PrimitivePatch
{
    BoundaryMesh mesh_;

    String patchName_;

    // Each 3 vertices is one triangle.
    Point3f[] points_;

    // Each 3 vertices is one triangle.
    int[] triFaces_;

    // From triangle to patch face
    int[] triToPoly_;

    // From patch face to triangle start
    int[] polyToTri_;

    // From patch face to number of triangles
    int[] nTris_;


    //--------------------------------------------------------------------------

    /** Construct from components
      */
    public PrimitivePatch
    (
        BoundaryMesh mesh,
        String patchName,
        int[] triFaces,
        int[] triToPoly,
        int[] polyToTri,
        int[] nTris
    )
    {
        mesh_ = mesh;
        patchName_ = patchName;
        triFaces_ = triFaces;
        triToPoly_ = triToPoly;
        polyToTri_ = polyToTri;
        nTris_ = nTris;
    }

    //--------------------------------------------------------------------------

    /** Construct from SurfacePatch (face slice (startFace, nFaces)).
      * Triangulates faces 
      */
    public PrimitivePatch(BoundaryMesh mesh, String patchName)
    {
        mesh_ = mesh;
        patchName_ = patchName;

        SurfacePatch patch = mesh_.getPatch(patchName_);

        int size = patch.getSize();
        int startFaceI = patch.getStart();

        nTris_ = new int[size];

        int totalNTris = mesh_.getNTris(startFaceI, size, nTris_);

        triFaces_ = new int[3*totalNTris];

        int nPoints = mesh_.getNPoints(startFaceI, size);

        int[] localToGlobal = new int[nPoints];

        mesh_.triangulateLocal
        (
            startFaceI,
            size,
            totalNTris,
            triFaces_,
            localToGlobal
        );

        System.out.println
        (
            "Creating patch "
            + patch.getName()
            + " size:" + size
            + " start:" + startFaceI
        );

        // Construct local coordinates
        Point3f[] meshPoints = mesh_.getPoints();

        points_ = new Point3f[nPoints];

        for (int i = 0; i < nPoints; i++)
        {
            points_[i] = meshPoints[localToGlobal[i]];
        }        

        // Fill polyToTri (start of triangles) and triToPoly

        polyToTri_ = new int[size];
        triToPoly_ = new int[totalNTris];

        int triI = 0;

        for (int patchFaceI = 0; patchFaceI < size; patchFaceI++)
        {
            polyToTri_[patchFaceI] = triI;

            int nTris = nTris_[patchFaceI];

            for (int localTri = 0; localTri < nTris; localTri++)
            {
                triToPoly_[triI++] = patchFaceI;
            }
        }

        if (triI != totalNTris)
        {
            System.out.println("*** PrimitivePatch problem");
        }
    }

    //--------------------------------------------------------------------------

    /** Reference to patch-local points
      */
    public Point3f[] getPoints()
    {
        //return mesh_.getPoints();
        return points_;
    }

    //--------------------------------------------------------------------------

    public int getSize()
    {
        // Map through to SurfacePatch
        return mesh_.getPatch(patchName_).getSize();
    }

    //--------------------------------------------------------------------------

    /** Get triangle vertices. Every three vertices is one triangle.
      */
    public int[] getTriFaces()
    {
        return triFaces_;
    }

    //--------------------------------------------------------------------------

    /** Get triangle to face addressing
      */
    public int[] getTriToPoly()
    {
        return triToPoly_;
    }

    //--------------------------------------------------------------------------

    /** Get face to triangle start addressing
      */
    public int[] getPolyToTri()
    {
        return polyToTri_;
    }

    //--------------------------------------------------------------------------

    /** Get number of triangles for face
      */
    public int[] getNTris()
    {
        return nTris_;
    }

    //--------------------------------------------------------------------------

    /** Get SurfacePatch
      */
    public SurfacePatch getSurfacePatch()
    {
        return mesh_.getPatch(patchName_);
    }

    //--------------------------------------------------------------------------

    /** Get boundary mesh
      */
    public BoundaryMesh getMesh()
    {
        return mesh_;
    }

    //--------------------------------------------------------------------------
}
