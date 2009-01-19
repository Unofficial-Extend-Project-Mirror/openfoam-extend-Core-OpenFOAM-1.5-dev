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
    primitiveMesh

Description
    Cell-face mesh analysis engine

SourceFiles
    primitiveMeshI.H
    primitiveMesh.C
    primitiveMeshClear.C
    primitiveMeshCellCells.C
    primitiveMeshEdgeCells.C
    primitiveMeshPointCells.C
    primitiveMeshCells.C
    primitiveMeshEdgeFaces.C
    primitiveMeshPointFaces.C
    primitiveMeshCellEdges.C
    primitiveMeshPointEdges.C
    primitiveMeshPointPoints.C
    primitiveMeshEdges.C
    primitiveMeshCellCentresAndVols.C
    primitiveMeshFaceCentresAndAreas.C
    primitiveMeshEdgeVectors.C
    primitiveMeshCheck.C
    primitiveMeshCheckMotion.C
    primitiveMeshFindCell.C

\*---------------------------------------------------------------------------*/

#ifndef primitiveMesh_H
#define primitiveMesh_H

#include "edgeList.H"
#include "pointField.H"
#include "SubField.H"
#include "SubList.H"
#include "faceList.H"
#include "cellList.H"
#include "cellShapeList.H"
#include "labelList.H"
#include "boolList.H"
#include "labelHashSet.H"
#include "Map.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class primitiveMesh Declaration
\*---------------------------------------------------------------------------*/

class primitiveMesh
{
    // Permanent data

        // Primitive size data

            //- Number of points
            label nPoints_;

            //- Number of edges
            mutable label nEdges_;

            //- Number of internal faces
            label nInternalFaces_;

            //- Number of faces
            label nFaces_;

            //- Number of cells
            label nCells_;


        // Minimum mesh data

            //- Points
            pointField::subField points_;

            //- Faces
            faceList::subList faces_;

            //- Face-owner cells
            labelList::subList faceOwner_;

            //- Face-neighbour cells
            labelList::subList faceNeighbour_;

            //- Have the primitives been cleared
            bool clearedPrimitives_;


        // Mesh motion

            //- Old points
            mutable pointField::subField* oldPointsPtr_;


        // Shapes

            //- Cell shapes
            mutable cellShapeList* cellShapesPtr_;

            //- Edges
            mutable edgeList* edgesPtr_;


        // Connectivity

            //- Cell-cells
            mutable labelListList* ccPtr_;

            //- Edge-cells
            mutable labelListList* ecPtr_;

            //- Point-cells
            mutable labelListList* pcPtr_;

            //- Cell-faces
            mutable cellList* cfPtr_;

            //- Edge-faces
            mutable labelListList* efPtr_;

            //- Point-faces
            mutable labelListList* pfPtr_;

            //- Cell-edges
            mutable labelListList* cePtr_;

            //- Face-edges
            mutable labelListList* fePtr_;

            //- Point-edges
            mutable labelListList* pePtr_;

            //- Point-points
            mutable labelListList* ppPtr_;

            //- Cell-points
            mutable labelListList* cpPtr_;


        // Geometric data

            //- Cell centres
            mutable vectorField* cellCentresPtr_;

            //- Face centres
            mutable vectorField* faceCentresPtr_;

            //- Cell volumes
            mutable scalarField* cellVolumesPtr_;

            //- Face areas
            mutable vectorField* faceAreasPtr_;


    // Private member functions

        //- Disallow construct as copy
        primitiveMesh(const primitiveMesh&);

        //- Disallow default bitwise assignment
        void operator=(const primitiveMesh&);


        // Private member functions to calculate demand driven data

        // Topological calculations

            //- Calculate cell shapes
            void calcCellShapes() const;

            //- Calculate cell-cell addressing
            void calcCellCells() const;

            //- Calculate cell-edge addressing
            void calcEdgeCells() const;

            //- Calculate point-cell addressing
            void calcPointCells() const;

            //- Calculate cell-face addressing
            void calcCells() const;

            //- Calculate face-edge addressing
            void calcEdgeFaces() const;

            //- Calculate point-face addressing
            void calcPointFaces() const;

            //- Calculate edge list
            void calcCellEdges() const;

            //- Calculate point-edge addressing
            void calcPointEdges() const;

            //- Calculate point-point addressing
            void calcPointPoints() const;

            //- Calculate cell-point addressing
            void calcCellPoints() const;

            //- During edge calculation, a larger set of data is assembled.
            // Create and destroy as a set, using clearOutEdges()
            void calcEdges() const;
            void clearOutEdges();


        // Geometrical calculations

            //- Calculate face centres and areas
            void calcFaceCentresAndAreas() const;
            void makeFaceCentresAndAreas
            (
                const pointField& p,
                vectorField& fCtrs,
                vectorField& fAreas
            ) const;

            //- Calculate cell centres and volumes
            void calcCellCentresAndVols() const;
            void makeCellCentresAndVols
            (
                const vectorField& fCtrs,
                const vectorField& fAreas,
                vectorField& cellCtrs,
                scalarField& cellVols
            ) const;

            //- Calculate edge vectors
            void calcEdgeVectors() const;


    // Helper functions for mesh checking

            //- Check if all points on face are shared with another face.
            bool checkDuplicateFaces
            (
                const label,
                const Map<label>&,
                label& nBaffleFaces,
                labelHashSet*
            ) const;

            //- Check that shared points are in consecutive order.
            bool checkCommonOrder
            (
                const label,
                const Map<label>&,
                labelHashSet*
            ) const;


    // Static data members

        //- Static data to control mesh checking

            //- Cell closedness warning threshold
            //  set as the fraction of un-closed area to closed area
            static scalar closedThreshold_;

            //- Aspect ratio warning threshold
            static scalar aspectThreshold_;

            //- Non-orthogonality warning threshold in deg
            static scalar nonOrthThreshold_;

            //- Skewness warning threshold
            static scalar skewThreshold_;


protected:

        //- Construct null
        primitiveMesh();


public:

        // Static data

            ClassName("primitiveMesh");

            //- Estimated number of cells per edge
            static const unsigned cellsPerEdge_ = 4;

            //- Estimated number of cells per point
            static const unsigned cellsPerPoint_ = 8;

            //- Estimated number of faces per cell
            static const unsigned facesPerCell_ = 6;

            //- Estimated number of faces per edge
            static const unsigned facesPerEdge_ = 4;

            //- Estimated number of faces per point
            static const unsigned facesPerPoint_ = 12;

            //- Estimated number of edges per cell
            static const unsigned edgesPerCell_ = 12;

            //- Estimated number of edges per cell
            static const unsigned edgesPerFace_ = 4;

            //- Estimated number of edges per point
            static const unsigned edgesPerPoint_ = 6;

            //- Estimated number of points per cell
            static const unsigned pointsPerCell_ = 8;

            //- Estimated number of points per face
            static const unsigned pointsPerFace_ = 4;


    // Constructors

        //- Construct from components
        primitiveMesh
        (
            const label nPoints,
            const label nInternalFaces,
            const label nFaces,
            const label nCells,
            const pointField& points,
            const faceList& faces,
            const labelList& faceOwner,
            const labelList& faceNeighbour
        );


    // Destructor

        virtual ~primitiveMesh();


    // Member Functions

        //- Reset this primitiveMesh given a complete set of data
        void reset
        (
            const label nPoints,
            const label nInternalFaces,
            const label nFaces,
            const label nCells,
            const pointField& points,
            const faceList& faces,
            const labelList& faceOwner,
            const labelList& faceNeighbour
        );

        //- Reset this primitiveMesh given a complete set of data and cells
        void reset
        (
            const label nPoints,
            const label nInternalFaces,
            const label nFaces,
            const label nCells,
            const pointField& points,
            const faceList& faces,
            const labelList& faceOwner,
            const labelList& faceNeighbour,
            cellList& cells
        );


        // Access

            // Mesh size parameters

                inline label nPoints() const;
                inline label nEdges() const;
                inline label nInternalFaces() const;
                inline label nFaces() const;
                inline label nCells() const;


            // Primitive mesh data

                //- Return mesh points
                const pointField& points() const;

                //- Return faces
                const faceList& faces() const;

                //- Face owner addresing
                inline const labelList& faceOwner() const;

                //- Face neighbour addressing
                inline const labelList& faceNeighbour() const;

                //- Return cell shapes
                const cellShapeList& cellShapes() const;

                //- Return mesh edges
                const edgeList& edges() const;

                //- Helper function to calculate cell-face addressing from
                //  face-cell addressing. If nCells is not provided it will
                //  scan for the maximum.
                static void calcCells
                (
                    cellList&,
                    const unallocLabelList& own,
                    const unallocLabelList& nei,
                    const label nCells = -1
                );


            // Mesh motion

                //- Return old points
                const pointField& oldPoints() const;


            // Return mesh connectivity

                const labelListList& cellCells() const;
                // faceCells given as faceOwner and faceNeighbour
                const labelListList& edgeCells() const;
                const labelListList& pointCells() const;

                const cellList& cells() const;
                // faceFaces considered unnecessary
                const labelListList& edgeFaces() const;
                const labelListList& pointFaces() const;

                const labelListList& cellEdges() const;
                const labelListList& faceEdges() const;
                // edgeEdges considered unnecessary
                const labelListList& pointEdges() const;
                const labelListList& pointPoints() const;
                const labelListList& cellPoints() const;


            // Geometric data (raw!)

                const vectorField& cellCentres() const;
                const vectorField& faceCentres() const;
                const scalarField& cellVolumes() const;
                const vectorField& faceAreas() const;


            // Mesh motion

                //- Move points, returns volumes swept by faces in motion
                tmp<scalarField> movePoints
                (
                    const pointField& p,
                    const pointField& oldP
                );


            //- Return true if given face label is internal to the mesh
            inline bool isInternalFace(const label faceIndex) const;


            // Topological checks

                //- Check cell zip-up
                bool checkCellsZipUp
                (
                    const bool report = false,
                    labelHashSet* setPtr = NULL
                ) const;

                //- Check uniqueness of face vertices
                bool checkFaceVertices
                (
                    const bool report = false,
                    labelHashSet* setPtr = NULL
                ) const;

                //- Check face-face connectivity
                bool checkFaceFaces
                (
                    const bool report = false,
                    labelHashSet* setPtr = NULL
                ) const;

                //- Check face ordering
                bool checkUpperTriangular
                (
                    const bool report = false,
                    labelHashSet* setPtr = NULL
                ) const;


            // Geometric checks

                //- Check boundary for closedness
                bool checkClosedBoundary(const bool report = false) const;

                //- Check cells for closedness
                bool checkClosedCells
                (
                    const bool report = false,
                    labelHashSet* setPtr = NULL,
                    labelHashSet* highAspectSetPtr = NULL
                ) const;

                //- Check for negative face areas
                bool checkFaceAreas
                (
                    const bool report = false,
                    labelHashSet* setPtr = NULL
                ) const;

                //- Check for negative cell volumes
                bool checkCellVolumes
                (
                    
                    const bool report = false,
                    labelHashSet* setPtr = NULL
                ) const;

                //- Check for non-orthogonality
                bool checkFaceOrthogonality
                (
                    const bool report = false,
                    labelHashSet* setPtr = NULL
                ) const;

                //- Check face pyramid volume
                bool checkFacePyramids
                (
                    const bool report = false,
                    const scalar minPyrVol = -SMALL,
                    labelHashSet* setPtr = NULL
                ) const;

                //- Check face skewness
                bool checkFaceSkewness
                (
                    const bool report = false,
                    labelHashSet* setPtr = NULL
                ) const;

                //- Check face angles
                bool checkFaceAngles
                (
                    const bool report = false,
                    const scalar maxSin = 10,    // In degrees
                    labelHashSet* setPtr = NULL
                ) const;

                //- Check face warpage: decompose face and check ratio between
                //  magnitude of sum of triangle areas and sum of magnitude of
                //  triangle areas.
                bool checkFaceFlatness
                (
                    const bool report,
                    const scalar warnFlatness,  // When to include in set.
                    labelHashSet* setPtr
                ) const;

                //- Check for unused points
                bool checkPoints
                (
                    const bool report = false,
                    labelHashSet* setPtr = NULL
                ) const;

                //- Check for point-point-nearness,
                //  e.g. colocated points which may be part of baffles.
                bool checkPointNearness
                (
                    const bool report,
                    const scalar reportDistSqr,
                    labelHashSet* setPtr = NULL
                ) const;

                //- Check edge length
                bool checkEdgeLength
                (
                    const bool report,
                    const scalar minLenSqr,
                    labelHashSet* setPtr = NULL
                ) const;

                //- Check cell determinant
                bool checkCellDeterminant
                (
                    const bool report = false,
                    labelHashSet* setPtr = NULL
                ) const;


            //- Check mesh topology for correctness.
            //  Returns false for no error.
            bool checkTopology(const bool report = false) const;

            //- Check mesh geometry (& implicitly topology) for correctness. 
            //  Returns false for no error.
            bool checkGeometry(const bool report = false) const;

            //- Check mesh for correctness. Returns false for no error.
            bool checkMesh(const bool report = false) const;

            //- Check mesh motion for correctness given motion points
            bool checkMeshMotion
            (
                const pointField& newPoints,
                const bool report = false
            ) const;


            //- Set the closedness ratio warning threshold
            static scalar setClosedThreshold(const scalar);

            //- Set the aspect ratio warning threshold
            static scalar setAspectThreshold(const scalar);

            //- Set the non-orthogonality warning threshold in degrees
            static scalar setNonOrthThreshold(const scalar);

            //- Set the skewness warning threshold as percentage
            //  of the face area vector
            static scalar setSkewThreshold(const scalar);


        // Useful derived info

            //- Is the point in the cell bounding box
            bool pointInCellBB(const point& p, label celli) const;

            //- Is the point in the cell
            bool pointInCell(const point& p, label celli) const;

            //- Find the cell with the nearest cell centre to location
            label findNearestCell(const point& location) const;

            //- Find cell enclosing this location (-1 if not in mesh)
            label findCell(const point& location) const;


        //  Storage management

            //- Print a list of all the currently allocated mesh data
            void printAllocated() const;

            //- Clear geometry
            void clearGeom();

            //- Clear topological data
            void clearAddressing();

            //- Clear all geometry and addressing unnecessary for CFD
            void clearOut();

            //- Clear primitive data.  Access to points and faces
            //  will be denied
            void clearPrimitives();

            //- Clear everyting primitive, geometry and addressing
            void clearAll();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "primitiveMeshI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
