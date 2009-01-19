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

//import PatchTool.Mesh.SurfacePatch;
import PatchTool.Util.FoamDataParser;
import PatchTool.Util.FoamDataWriter;

import FoamXServer.PatchToolServer.IPatchToolServer;
import FoamXServer.LongListHolder;
import FoamXServer.LongLongListHolder;
import FoamXServer.FloatListHolder;
import FoamXServer.BoolListHolder;


public class BoundaryMesh
{
    // Corba Server
    IPatchToolServer server_;

    // Cached SurfacePatch list
    SurfacePatch[] patches_;

    // Cached points
    Point3f[] points_;

    // Cached number of edges
    int nEdges_ = -1;

    // Cached number of faces
    int nFaces_ = -1;

    // Cached feature points
    Point3f[] featPoints_;

    // Cached feature edges as labels into featPoints.
    LongListHolder featEdges_;

    // Cached feature edge labels
    LongListHolder featToEdge_;

    // Cached feature segments. Indices into featEdges.
    LongLongListHolder featureSegments_;

    // Cached extra edges
    LongListHolder extraEdgeLabels_;

    // Current selected edges
    EdgeSet edgeSet_;

    // Current selected faces
    FaceSet faceSet_;

    // Currently selected extra edges
    EdgeSet extraEdgeSet_;

    // For every segment the current selection.
    EdgeSet[] featureEdgeSets_;

    // Latest invocation of featuresegments
    double minCos_;

    //--------------------------------------------------------------------------

    /** Construct from Corba server stream */
    public BoundaryMesh(IPatchToolServer server)
    {
        server_ = server;

        // Don't do recalculation
        minCos_ = -10;
    }

    //--------------------------------------------------------------------------

    /** Read from mesh */
    public void read()
    {
        try
        {
            server_.read();

            // Reset all cached quantities
            patches_ = null;

            points_ = null;

            int nEdges = getNEdges();

            int nFaces = getNFaces();

            featPoints_ = null;

            featEdges_ = null;

            featToEdge_ = null;

            featureSegments_ = null;

            extraEdgeLabels_ = null;

            edgeSet_ = new EdgeSet(nEdges);

            faceSet_ = new FaceSet(nFaces);

            extraEdgeSet_  = null;

            featureEdgeSets_ = null;

            System.out.println("Read mesh:");
            System.out.println("    root   : " + server_.caseRoot());
            System.out.println("    case   : " + server_.caseName());
            System.out.println("    faces  : " + nFaces);
            System.out.println("    edges  : " + nEdges);
        }
        catch (FoamXServer.FoamXIOError ioErr)
        {
            System.out.println
            ("BoundaryMesh::read : Caught exception");
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println
            ("BoundaryMesh::read : Caught exception");
        }
    }

    //--------------------------------------------------------------------------

    /** Read from triSurface file */
    public void readTriSurface(File file)
    {
        try
        {
            server_.readTriSurface(file.getPath());

            // Reset all cached quantities
            patches_ = null;

            points_ = null;

            int nEdges = getNEdges();

            int nFaces = getNFaces();

            featPoints_ = null;

            featEdges_ = null;

            featToEdge_ = null;

            featureSegments_ = null;

            extraEdgeLabels_ = null;

            edgeSet_ = new EdgeSet(nEdges);

            faceSet_ = new FaceSet(nFaces);

            extraEdgeSet_  = null;

            featureEdgeSets_ = null;

            System.out.println("Read mesh:");
            System.out.println("    root   : " + server_.caseRoot());
            System.out.println("    case   : " + server_.caseName());
            System.out.println("    faces  : " + nFaces);
            System.out.println("    edges  : " + nEdges);
        }
        catch (FoamXServer.FoamXIOError ioErr)
        {
            System.out.println
            ("BoundaryMesh::readTriSurface : Caught exception");
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println
            ("BoundaryMesh::readTriSurface : Caught exception");
        }
    }

    //--------------------------------------------------------------------------

    /** Apply to mesh and write mesh */
    public void write()
    {
        try
        {
            server_.write();
        }
        catch (FoamXServer.FoamXIOError ioErr)
        {
            System.out.println
            ("BoundaryMesh::write : Caught exception");
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println
            ("BoundaryMesh::write : Caught exception");
        }
    }

    //--------------------------------------------------------------------------

    /** Write as triSurface */
    public void writeTriSurface(File file)
    {
        try
        {
            //System.out.println("writeTriSurface : writing to " + file.getName());
            server_.writeTriSurface(file.getPath());
        }
        catch (FoamXServer.FoamXIOError ioErr)
        {
            System.out.println
            ("BoundaryMesh::writeTriSurface : Caught exception");
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println
            ("BoundaryMesh::writeTriSurface : Caught exception");
        }
    }

    //--------------------------------------------------------------------------

    public Point3f[] getPoints()
    {
        if (points_ == null)
        {
            FloatListHolder fxPoints = new FloatListHolder();

            try
            {
                server_.getPoints(fxPoints);
            }
            catch (FoamXServer.FoamXError ioErr)
            {
                System.out.println
                ("BoundaryMesh::getPoints() : Caught exception");
            }

            float[] fPoints = fxPoints.value;

            int nPoints = fPoints.length / 3;

            points_ = new Point3f[nPoints];

            int coordI = 0;

            for (int pointI = 0; pointI < nPoints; pointI++)
            {
                float x = fPoints[coordI++];
                float y = fPoints[coordI++];
                float z = fPoints[coordI++];

                points_[pointI] = new Point3f(x, y, z);
            }

        }
        return points_;
    }

    //--------------------------------------------------------------------------

    public int getNFaces()
    {
        if (nFaces_ == -1)
        {
            try
            {
                nFaces_ = server_.getNFaces();
            }
            catch (FoamXServer.FoamXError fxErr)
            {
                System.out.println
                ("BoundaryMesh::getNFaces : Caught exception");
            }
        }
        return nFaces_;
    }

    //--------------------------------------------------------------------------

    public int getNEdges()
    {
        if (nEdges_ == -1)
        {
            try
            {
                nEdges_ = server_.getNEdges();
            }
            catch (FoamXServer.FoamXError fxErr)
            {
                System.out.println
                ("BoundaryMesh::getNEdges : Caught exception");
            }
        }
        return nEdges_;
    }

    //--------------------------------------------------------------------------

    public int[] getEdges()
    {
        LongListHolder edges = new LongListHolder();

        try
        {
            server_.getEdges(edges);
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println
            ("BoundaryMesh::getEdges : Caught exception");
        }

        return edges.value;
    }

    //--------------------------------------------------------------------------

    public FaceSet getFaceSet()
    {
        return faceSet_;
    }

    //--------------------------------------------------------------------------

    public EdgeSet getEdgeSet()
    {
        return edgeSet_;
    }

    //--------------------------------------------------------------------------

    public void setFeatureEdges(double minCos)
    {
        minCos_ = minCos;

        try
        {
            server_.setFeatureEdges(minCos);
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println
            ("BoundaryMesh::setFeatureEdges : Caught exception");
        }

        // Clear cached feature edge data
        featPoints_ = null;

        featEdges_ = null;

        featToEdge_ = null;

        featureSegments_ = null;

        // Allocate featureSegments and featEdgeSets.
        int[][] featureSegments = getFeatureSegments();

        // Create enough and large enough edge sets for feature edges.
        featureEdgeSets_ = new EdgeSet[featureSegments.length];

        for (int segmentI = 0; segmentI < featureEdgeSets_.length; segmentI++)
        {
            int nFeats = featureSegments[segmentI].length;

            featureEdgeSets_[segmentI] = new EdgeSet(nFeats);
        }
    }

    //--------------------------------------------------------------------------


    public Point3f[] getFeaturePoints()
    {
        if (featPoints_ == null)
        {
            FloatListHolder fxPoints = new FloatListHolder();

            try
            {
                server_.getFeaturePoints(fxPoints);
            }
            catch (FoamXServer.FoamXError ioErr)
            {
                System.out.println
                ("BoundaryMesh::getFeaturePoints() : Caught exception");
            }

            float[] fPoints = fxPoints.value;

            int nPoints = fPoints.length / 3;

            featPoints_ = new Point3f[nPoints];

            int coordI = 0;

            for (int pointI = 0; pointI < nPoints; pointI++)
            {
                float x = fPoints[coordI++];
                float y = fPoints[coordI++];
                float z = fPoints[coordI++];

                featPoints_[pointI] = new Point3f(x, y, z);
            }

        }
        return featPoints_;
    }

    //--------------------------------------------------------------------------

    public int[] getFeatureEdges()
    {
        if (featEdges_ == null)
        {
            featEdges_ = new LongListHolder();

            try
            {
                server_.getFeatureEdges(featEdges_);
            }
            catch (FoamXServer.FoamXError fxErr)
            {
                System.out.println
                ("BoundaryMesh::getFeatureEdges : Caught exception");
            }
        }
        return featEdges_.value;
    }

    //--------------------------------------------------------------------------

    public int[] getFeatureToEdge()
    {
        if (featToEdge_ == null)
        {
            featToEdge_ = new LongListHolder();

            try
            {
                server_.getFeatureToEdge(featToEdge_);
            }
            catch (FoamXServer.FoamXError fxErr)
            {
                System.out.println
                ("BoundaryMesh::getFeatureEdges : Caught exception");
            }
        }
        return featToEdge_.value;
    }

    //--------------------------------------------------------------------------

    public int[][] getFeatureSegments()
    {
        if (featureSegments_ == null)
        {
            featureSegments_ = new LongLongListHolder();

            try
            {
                server_.getFeatureSegments(featureSegments_);
            }
            catch (FoamXServer.FoamXError fxErr)
            {
                System.out.println
                ("BoundaryMesh::getFeatureSegments : Caught exception");
            }
        }
        return featureSegments_.value;
    }

    //--------------------------------------------------------------------------

    public EdgeSet[] getFeatureEdgeSets()
    {
        return featureEdgeSets_;
    }

    //--------------------------------------------------------------------------

    public void setExtraEdges(int edgeI)
    {
        try
        {
            // Clear cached labels
            extraEdgeLabels_ = null;

            server_.setExtraEdges(edgeI);
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println
            ("BoundaryMesh::setExtraEdges : Caught exception");
        }

        extraEdgeSet_ = new EdgeSet(getExtraEdges().length);
    }
    
    //--------------------------------------------------------------------------

    public int[] getExtraEdges()
    {
        if (extraEdgeLabels_ == null)
        {
            extraEdgeLabels_ = new LongListHolder();

            try
            {
                server_.getExtraEdges(extraEdgeLabels_);
            }
            catch (FoamXServer.FoamXError fxErr)
            {
                System.out.println
                ("BoundaryMesh::getExtraEdges : Caught exception");
            }
        }
        return extraEdgeLabels_.value;
    }

    //--------------------------------------------------------------------------

    public EdgeSet getExtraEdgeSet()
    {
        return extraEdgeSet_;
    }

    //--------------------------------------------------------------------------

    /** Add empty patch at end of patches */
    public void addPatch(String patchName)
    {
        try
        {
            server_.addPatch(patchName);

            // Delete cached patches
            patches_ = null;
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println
            ("BoundaryMesh::addPatch : Caught exception");
        }
    }

    //--------------------------------------------------------------------------

    /** Delete named patch. Has to have 0 faces! */
    public void deletePatch(String patchName)
    {
        try
        {
            server_.deletePatch(patchName);

            // Delete cached patches
            patches_ = null;
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println
            ("BoundaryMesh::deletePatch : Caught exception");
        }
    }

    //--------------------------------------------------------------------------

    /** Add empty patch at end of patches */
    public void changePatchType(String patchName, String type)
    {
        try
        {
            server_.changePatchType(patchName, type);

            // Delete cached patches
            patches_ = null;
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println
            ("BoundaryMesh::changePatchType : Caught exception");
        }
    }

    //--------------------------------------------------------------------------

    /** Change patch of faces. Reorders faces! */
    public void changeFaces(int[] patchIDs, int[] oldToNew)
    {
        LongListHolder fxOldToNew = new LongListHolder();

        try
        {
            server_.changeFaces(patchIDs, fxOldToNew);

            for (int i = 0; i < fxOldToNew.value.length; i++)
            {
                oldToNew[i] = fxOldToNew.value[i];
            }

            //
            // Delete cached data
            //

            // patches will be different
            patches_ = null;

            // edges will be renumbered
            featPoints_ = null;
            featEdges_ = null;
            featToEdge_ = null;
            featureSegments_ = null;
            extraEdgeLabels_ = null;

            faceSet_.clear();
            edgeSet_.clear();

            if (extraEdgeSet_ != null)
            {
                extraEdgeSet_.clear();
            }
            if (featureEdgeSets_ != null)
            {
                //Q: can we clear or should we delete whole set? Depends on
                //   whether featedges only rely on geometric info (constant
                //   with repatching) or on patch info as well.
                for (int setI = 0; setI < featureEdgeSets_.length; setI++)
                {
                    featureEdgeSets_[setI].clear();
                }
            }

            // Recalculate featureEdges if were valid before
            if (Math.abs(minCos_) <= 1.0)
            {
                setFeatureEdges(minCos_);
            }
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println
            ("BoundaryMesh::changeFaces : Caught exception");
        }

        faceSet_.reorder(oldToNew);
    }

    //--------------------------------------------------------------------------

    public SurfacePatch[] getPatches()
    {
        if (patches_ == null)
        {
            FoamXServer.SurfacePatchListHolder fxPatches =
                new FoamXServer.SurfacePatchListHolder();

            try
            {
                server_.getPatches(fxPatches);
            }
            catch (FoamXServer.FoamXError fxErr)
            {
                System.out.println
                ("BoundaryMesh::getPatches : Caught exception");
            }

            patches_ = new SurfacePatch[fxPatches.value.length];

            for (int patchI = 0; patchI < patches_.length; patchI++)
            {
                FoamXServer.SurfacePatch fxPatch = fxPatches.value[patchI];

                patches_[patchI] =
                    new SurfacePatch
                    (
                        fxPatch.name,
                        fxPatch.geometricType,
                        fxPatch.size,
                        fxPatch.start
                    );
            }
        }
        return patches_;
    }

    //--------------------------------------------------------------------------

    /** Index of patch in patch list */
    public int getPatchIndex(String patchName)
    {
        SurfacePatch[] patches = getPatches();

        for (int patchI = 0; patchI < patches.length; patchI++)
        {
            if (patches[patchI].getName().equals(patchName))
            {
                return patchI;
            }
        }

        return -1;
    }

    //--------------------------------------------------------------------------

    /** Get patch by name */
    public SurfacePatch getPatch(String patchName)
    {
        SurfacePatch[] patches = getPatches();

        for (int patchI = 0; patchI < patches.length; patchI++)
        {
            if (patches[patchI].getName().equals(patchName))
            {
                return patches[patchI];
            }
        }

        return null;
    }

    //--------------------------------------------------------------------------

    /** Get patchID for face. Don't use server but do local */
    public int whichPatch(int faceI)
    {
        SurfacePatch[] patches = getPatches();

        for (int patchI = 0; patchI < patches.length; patchI++)
        {
            SurfacePatch sp = patches[patchI];

            if
            (
                (faceI >= sp.getStart())
             && (faceI < (sp.getStart() + sp.getSize()))
            )
            {
                return patchI;
            }
        }
        return -1;
    }

    //--------------------------------------------------------------------------

    /** Simple triangulation of subset of faces. 
      *     nTris[faceI] : number of triangles for faceI
      *     triVerts[i .. i+nTris] : vertices (each three form triangle) from
      *     triangulation.
      * Returns total number of triangles.
      */
    public int getNTris
    (
        int startFaceI,
        int nFaces,
        int[] nTris
    )
    {
        LongListHolder fxNTris = new LongListHolder();

        int totalNTris = 0;

        try
        {
            totalNTris =
                server_.getNTris
                (
                    startFaceI,
                    nFaces,
                    fxNTris
                );

            for (int i = 0; i < fxNTris.value.length; i++)
            {
                nTris[i] = fxNTris.value[i];
            }
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println("BoundaryMesh::getNTris : Caught exception");
        }
        return totalNTris;
    }


    public void triangulate
    (
        int startFaceI,
        int nFaces,
        int totalNTris,
        int[] triVerts
    )
    {
        LongListHolder fxTriVerts = new LongListHolder();

        try
        {
            server_.triangulate
            (
                startFaceI,
                nFaces,
                totalNTris,
                fxTriVerts
            );
            for (int i = 0; i < fxTriVerts.value.length; i++)
            {
                triVerts[i] = fxTriVerts.value[i];
            }
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println("BoundaryMesh::triangulate : Caught exception");
        }
    }

    //--------------------------------------------------------------------------

    /** Same but in local coordinates. Additionally returns map from local
      * to global points ('patch.meshPoints()')
      */

    public int getNPoints(int startFaceI, int nFaces)
    {
        int nPoints = 0;

        try
        {
            nPoints = server_.getNPoints(startFaceI, nFaces);
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println("BoundaryMesh::getNPoints : Caught exception");
        }
        return nPoints;
    }

    public void triangulateLocal
    (
        int startFaceI,
        int nFaces,
        int totalNTris,
        int[] triVerts,
        int[] localToGlobal
    )
    {
        LongListHolder fxTriVerts = new LongListHolder();
        LongListHolder fxLocalToGlobal = new LongListHolder();

        try
        {
            server_.triangulateLocal
            (
                startFaceI,
                nFaces,
                totalNTris,
                fxTriVerts,
                fxLocalToGlobal
            );
            for (int i = 0; i < fxTriVerts.value.length; i++)
            {
                triVerts[i] = fxTriVerts.value[i];
            }
            for (int i = 0; i < fxLocalToGlobal.value.length; i++)
            {
                localToGlobal[i] = fxLocalToGlobal.value[i];
            }
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println
            ("BoundaryMesh::triangulateLocal : Caught exception");
        }
    }


    //--------------------------------------------------------------------------

    /**  From faceI mark all faces given by currently selected edges
      *  (in all edgeSets)
      */
    public void markLoopFaces(int faceI, boolean[] visited)
    {
        // All edges currently selected.
        boolean[] markedEdge = BoundaryMeshUtils.getMarkedEdges(this);

        // Pack markedEdge(boolList) into labelList
        int nMarked = 0;

        for (int edgeI = 0; edgeI < markedEdge.length; edgeI++)
        {
            if (markedEdge[edgeI])
            {
                nMarked++;
            }
        }
        int[] markedEdges = new int[nMarked];

        nMarked = 0;

        for (int edgeI = 0; edgeI < markedEdge.length; edgeI++)
        {
            if (markedEdge[edgeI])
            {
                markedEdges[nMarked++] = edgeI;
            }
        }

        BoolListHolder fxVisited = new BoolListHolder();

        try
        {
            server_.markFaces(markedEdges, faceI, fxVisited);
        }
        catch (FoamXServer.FoamXError fxErr)
        {
            System.out.println
            ("BoundaryMesh::markLoopFaces : Caught exception");
        }

        for (int i = 0; i < fxVisited.value.length; i++)
        {
            visited[i] = fxVisited.value[i];
        }
    }
}
