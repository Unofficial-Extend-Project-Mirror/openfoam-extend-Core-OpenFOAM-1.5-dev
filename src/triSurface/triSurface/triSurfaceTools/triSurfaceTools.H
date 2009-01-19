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

Class
    triSurfaceTools

Description

SourceFiles
    triSurfaceTools.C

\*---------------------------------------------------------------------------*/

#ifndef triSurfaceTools_H
#define triSurfaceTools_H

#include "boolList.H"
#include "pointField.H"
#include "DynamicList.H"
#include "labelHashSet.H"
#include "FixedList.H"
#include "vector2D.H"
#include "triPointRef.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Class forward declarations
class triSurface;
class edge;
class labelledTri;
class polyBoundaryMesh;

/*---------------------------------------------------------------------------*\
                           Class triSurfaceTools Declaration
\*---------------------------------------------------------------------------*/

namespace triSurfaceTools
{
    //- Write pointField to OBJ format file
    void writeOBJ
    (
        const fileName& fName,
        const pointField& pts
    );

    //- Write vertex subset to OBJ format file
    void writeOBJ
    (
        const triSurface& surf,
        const fileName& fName,
        const boolList& markedVerts
    );

    //- Get all triangles using edge endpoint
    void getVertexTriangles
    (
        const triSurface& surf,
        const label edgeI,
        labelList& edgeTris
    );

    //- Get all vertices (local numbering) connected to vertices of edge
    labelList getVertexVertices
    (
        const triSurface& surf,
        const edge& e
    );

    //- Get all edges of triangle in face ordering (elem 0 is edge between
    //  f[0] .. f[1], elem1 between f[1]..f[2] etc)
    FixedList<label, 3> sortedFaceEdges
    (
        const triSurface& surf,
        const label faceI
    );

    //- Order vertices consistent with face
    void orderVertices
    (
        const labelledTri& f,
        const label v1,
        const label v2,
        label& vA,
        label& vB
    );

    //- Get face connected to edge not faceI
    label otherFace
    (
        const triSurface& surf,
        const label faceI,
        const label edgeI
    );

    //- Get the two edges on faceI counterclockwise after edgeI
    void otherEdges
    (
        const triSurface& surf,
        const label faceI,
        const label edgeI,
        label& e1,
        label& e2
    );

    //- Get the two vertices (local numbering) on faceI counterclockwise vertI
    void otherVertices
    (
        const triSurface& surf,
        const label faceI,
        const label vertI,
        label& vert1I,
        label& vert2I
    );

    //- Get edge opposite vertex (local numbering)
    label oppositeEdge
    (
        const triSurface& surf,
        const label faceI,
        const label vertI
    );

    //- Get vertex (local numbering) opposite edge
    label oppositeVertex
    (
        const triSurface& surf,
        const label faceI,
        const label edgeI
    );

    //- Returns edge label connecting v1, v2 (local numbering)
    label getEdge
    (
        const triSurface& surf,
        const label vert1I,
        const label vert2I
    );

    //- Return index of triangle (or -1) using all three edges
    label getTriangle
    (
        const triSurface& surf,
        const label e0I,
        const label e1I,
        const label e2I
    );

    //- Returns element in edgeIndices with minimum length
    label minEdge
    (
        const triSurface& surf,
        const labelList& edgeIndices
    );

    //- Returns element in edgeIndices with minimum length
    label maxEdge
    (
        const triSurface& surf,
        const labelList& edgeIndices
    );

    scalar faceCosAngle
    (
        const point& pStart,
        const point& pEnd,
        const point& pLeft,
        const point& pRight
    );

    //- Estimate angle of vec in coordinate system (e0, e1, e0^e1).
    //  Is guaranteed to return increasing number but is not correct
    //  angle. Used for sorting angles.
    //  All input vectors need to be normalized.
    scalar pseudoAngle
    (
        const vector& e0,
        const vector& e1,
        const vector& vec
    );


    //- faces to collapse because of edge collapse
    labelHashSet getCollapsedFaces
    (
        const triSurface& surf,
        label edgeI
    );

    // Return value of faceUsed for faces using vertI (local numbering).
    // Used internally.
    label vertexUsesFace
    (
        const triSurface& surf,
        const labelHashSet& faceUsed,
        const label vertI
    );

    // Get new connections between faces (because of edge collapse) in form of
    // tables:
    //  - given edge get other edge
    //  - given edge get other face
    // A face using point v1 on edge will get connected to a face using point v2
    // if they share a common vertex (but not a common edge since then the
    // triangles collapse to nothing)
    void getMergedEdges
    (
        const triSurface& surf,
        const label edgeI,
        const labelHashSet& collapsedFaces,
        HashTable<label, label, Hash<label> >& edgeToEdge,
        HashTable<label, label, Hash<label> >& edgeToFace
    );

    //- Calculates (cos of) angle across edgeI of faceI,
    //  taking into account updated addressing (resulting from edge collapse)
    scalar edgeCosAngle
    (
        const triSurface& surf,
        const label v1,
        const point& pt,
        const labelHashSet& collapsedFaces,
        const HashTable<label, label, Hash<label> >& edgeToEdge,
        const HashTable<label, label, Hash<label> >& edgeToFace,
        const label faceI,
        const label edgeI
    );

    //- Calculate minimum (cos of) edge angle using addressing from collapsing
    //  edge to v1 at pt. Returns 1 if v1 is on edge without neighbours
    //  (and hence no edge angle can be defined)
    scalar collapseMinCosAngle
    (
        const triSurface& surf,
        const label v1,
        const point& pt,
        const labelHashSet& collapsedFaces,
        const HashTable<label, label, Hash<label> >& edgeToEdge,
        const HashTable<label, label, Hash<label> >& edgeToFace
    );

    //- Like collapseMinCosAngle but return true for value < minCos 
    bool collapseCreatesFold
    (
        const triSurface& surf,
        const label v1,
        const point& pt,
        const labelHashSet& collapsedFaces,
        const HashTable<label, label, Hash<label> >& edgeToEdge,
        const HashTable<label, label, Hash<label> >& edgeToFace,
        const scalar minCos
    );

//    //- Checks if edge collapse creates triangles on top of each other
//    bool collapseCreatesDuplicates
//    (
//        const triSurface& surf,
//        const label edgeI,
//        const HashTable<bool, label, Hash<label> >& collapsedFaces
//    );

    //- Face collapse status.
    //  anyEdge: any edge can be collapsed
    //  noEdge: no edge can be collapsed
    //  collapsed: already collapsed
    //  >0: edge label that can be collapsed
    static const label ANYEDGE = -1;
    static const label NOEDGE = -2;
    static const label COLLAPSED = -3;

    void protectNeighbours
    (
        const triSurface& surf,
        const label vertI,
        labelList& faceStatus
    );

    //- Create new triSurface by collapsing edges to edge mids.
    triSurface collapseEdges
    (
        const triSurface& surf,
        const labelList& collapsableEdges
    );

    //- Create new triSurface by collapsing edges to specified
    //  positions. faceStatus allows
    //  explicit control over which faces need to be protected (see above).
    //  faceStatus gets updated to protect collapsing already collapsed faces.
    triSurface collapseEdges
    (
        const triSurface& surf,
        const labelList& collapsableEdges,
        const pointField& edgeMids,
        labelList& faceStatus
    );

    enum refineType
    {
        NONE,
        RED,
        GREEN
    };

    void calcRefineStatus
    (
        const triSurface& surf,
        const label faceI,
        labelList& refine
    );

    void greenRefine
    (
        const triSurface& surf,
        const label faceI,
        const label edgeI,
        const label newPointI,
        DynamicList<labelledTri>& newFaces
    );

    //- Refine edges by splitting to opposite vertex
    triSurface greenRefine
    (
        const triSurface& surf,
        const labelList& refineEdges
    );

    triSurface doRefine
    (
        const triSurface& surf,
        const labelList& refineStatus
    );

    //- Refine face by splitting all edges. Neighbouring face is greenRefine'd.
    triSurface redGreenRefine
    (
        const triSurface& surf,
        const labelList& refineFaces
    );

    //- Merge points within distance
    triSurface mergePoints
    (
        const triSurface& surf,
        const scalar mergeTol
    );

    //- Triangle (unit) normal. If nearest point to triangle on edge use edge
    //  normal (calculated on the fly); if on vertex use vertex normal.
    //  Uses planarTol.
    vector surfaceNormal
    (
        const triSurface& surf,
        const label nearestFaceI,
        const point& nearestPt
    );

    //- on which side of surface
    enum sideType
    {
        UNKNOWN,    // cannot be determined (e.g. non-manifold)
        INSIDE,     // inside
        OUTSIDE     // outside
    };

    //- if nearest point is on edgeI, determine on which side of surface sample
    //  is.
    sideType edgeSide
    (
        const triSurface& surf,
        const point& sample,
        const point& nearestPoint,
        const label edgeI
    );

    //- Given nearest point (to sample) on surface determines which side
    //  sample is. Uses either face normal, edge normal or point normal
    //  (non-trivial). Feed in output of e.g. triangle::classify.
    sideType surfaceSide
    (
        const triSurface& surf,
        const point& sample,
        const label nearestFaceI,   // nearest face
        const point& nearestPt      // nearest point on nearest face
    );


    //- Simple triangulation of (selected patches of) boundaryMesh. Needs
    //  polyMesh (or polyBoundaryMesh) since only at this level are the
    //  triangles on neighbouring patches connected.
    triSurface triangulate
    (
        const polyBoundaryMesh& mBesh,
        const labelHashSet& includePatches,
        const bool verbose = false
    );

    //- Face-centre triangulation of (selected patches of) boundaryMesh. Needs
    //  polyMesh (or polyBoundaryMesh) since only at this level are the
    //  triangles on neighbouring patches connected.
    triSurface triangulateFaceCentre
    (
        const polyBoundaryMesh& mBesh,
        const labelHashSet& includePatches,
        const bool verbose = false
    );

    //- Calculate linear interpolation weights for point (guaranteed to be
    //  inside triangle)
    void calcInterpolationWeights
    (
        const triPointRef&,
        const point&,
        FixedList<scalar, 3>& weights
    );

    // Calculate weighting factors from samplePts to triangle it is in.
    // Uses linear search to find triangle.
    // Vertices are:
    //   (a b c)  : vertices of the triangle abc the point is in
    // or if the point is outside all triangles:
    //   (a b -1) : the edge ab the point is nearest to.
    //   (a -1 -1) : the vertex a the point is nearest to
    void calcInterpolationWeights
    (
        const triSurface& s,
        const pointField& samplePts,
        List<FixedList<label, 3> >& verts,
        List<FixedList<scalar, 3> >& weights
    );

    //- Do unconstrained Delaunay of points. Returns triSurface with 3D points
    //  with z=0. All triangles in region 0.
    triSurface delaunay2D(const List<vector2D>&);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
