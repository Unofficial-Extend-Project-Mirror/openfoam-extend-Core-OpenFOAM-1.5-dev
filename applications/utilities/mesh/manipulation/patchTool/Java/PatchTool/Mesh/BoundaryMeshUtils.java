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

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.util.*;

import javax.media.j3d.*;
import javax.vecmath.Point3f;
import javax.vecmath.Vector3f;


public class BoundaryMeshUtils
{

    //--------------------------------------------------------------------------

    /**  Get all selected edges from current mesh.
      */
    static public boolean[] getMarkedEdges(BoundaryMesh mesh)
    {
        // All edges currently selected.
        boolean[] markedEdge = new boolean[mesh.getNEdges()];

        Arrays.fill(markedEdge, false);

        // Make copy of edgeSet
        if (mesh.getEdgeSet() != null)
        {
            boolean[] selectedEdges = mesh.getEdgeSet().getSelected();

            for (int edgeI = 0; edgeI < selectedEdges.length; edgeI++)
            {
                if (selectedEdges[edgeI])
                {
                    markedEdge[edgeI] = selectedEdges[edgeI];
                }
            }
        }

        // Make copy of extraEdgeSet
        if (mesh.getExtraEdgeSet() != null)
        {
            boolean[] selectedExtraEdges =
                mesh.getExtraEdgeSet().getSelected();

            int[] extraEdges = mesh.getExtraEdges();

            for (int eLabel = 0; eLabel < selectedExtraEdges.length; eLabel++)
            {
                if (selectedExtraEdges[eLabel])
                {
                    int edgeI = extraEdges[eLabel];

                    markedEdge[edgeI] = true;
                }
            }
        }

        // Append feature edge sets
        EdgeSet[] featureEdgeSets = mesh.getFeatureEdgeSets();

        if (featureEdgeSets != null)
        {
            int[][] featureSegments = mesh.getFeatureSegments();

            int[] featureToEdge = mesh.getFeatureToEdge();

            for
            (
                int segmentI = 0;
                segmentI < featureEdgeSets.length;
                segmentI++
            )
            {
                boolean[] selectedFeats =
                    featureEdgeSets[segmentI].getSelected();

                int[] featEdgeLabels = featureSegments[segmentI];


                for (int eLabel = 0; eLabel < selectedFeats.length; eLabel++)
                {
                    if (selectedFeats[eLabel])
                    {
                        int featEdgeI = featEdgeLabels[eLabel];

                        int edgeI = featureToEdge[featEdgeI];

                        markedEdge[edgeI] = true;
                    }
                }
            }
        }

        return markedEdge;
    }

    //--------------------------------------------------------------------------

    /**  Select all feature edges
      */
    static public void selectAllFeatureEdges(BoundaryMesh mesh)
    {
        // Append feature edge sets
        EdgeSet[] featureEdgeSets = mesh.getFeatureEdgeSets();

        if (featureEdgeSets != null)
        {
            int[][] featureSegments = mesh.getFeatureSegments();

            int[] featureEdges = mesh.getFeatureEdges();

            for
            (
                int segmentI = 0;
                segmentI < featureEdgeSets.length;
                segmentI++
            )
            {
                boolean[] selectedFeats =
                    featureEdgeSets[segmentI].getSelected();

                int[] featEdgeLabels = featureSegments[segmentI];

                for (int eLabel = 0; eLabel < selectedFeats.length; eLabel++)
                {
                    selectedFeats[eLabel] = true;
                }

                featureEdgeSets[segmentI].setSelected(selectedFeats);
            }
        }
    }


    //--------------------------------------------------------------------------

    /**  Get patchID for all faces after changing selected to newPatchI.
      *  Sets patchID to new patch for face. Returns number of faces changed
      */
    static public int getPatchIDs
    (
        BoundaryMesh mesh,
        boolean[] selected,
        int newPatchID,
        int[] patchID
    )
    {
        SurfacePatch[] patches = mesh.getPatches();

        int nChanged = 0;

        for (int patchI = 0; patchI < patches.length; patchI++)
        {
            SurfacePatch sp = patches[patchI];

            for
            (
                int faceI = sp.getStart();
                faceI < sp.getStart() + sp.getSize();
                faceI++
            )
            {
                if (selected[faceI])
                {
                    // Change
                    patchID[faceI] = newPatchID;

                    if (newPatchID != patchI)
                    {
                        nChanged++;
                    }
                }
                else
                {
                    // Keep patchID
                    patchID[faceI] = patchI;
                }
            }
        }
        return nChanged;
    }

}
