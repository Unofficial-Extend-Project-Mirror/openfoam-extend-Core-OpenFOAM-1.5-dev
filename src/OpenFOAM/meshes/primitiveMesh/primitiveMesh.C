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

#include "primitiveMesh.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(primitiveMesh, 0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct null
primitiveMesh::primitiveMesh()
:
    nPoints_(0),
    nEdges_(-1),
    nInternalFaces_(0),
    nFaces_(0),
    nCells_(0),

    points_(UList<point>(), nPoints_),
    faces_(UList<face>(), nFaces_),
    faceOwner_(UList<label>(), nFaces_),
    faceNeighbour_(UList<label>(), nInternalFaces_),
    clearedPrimitives_(false),

    oldPointsPtr_(NULL),

    cellShapesPtr_(NULL),
    edgesPtr_(NULL),
    ccPtr_(NULL),
    ecPtr_(NULL),
    pcPtr_(NULL),

    cfPtr_(NULL),
    efPtr_(NULL),
    pfPtr_(NULL),

    cePtr_(NULL),
    fePtr_(NULL),
    pePtr_(NULL),
    ppPtr_(NULL),
    cpPtr_(NULL),

    cellCentresPtr_(NULL),
    faceCentresPtr_(NULL),
    cellVolumesPtr_(NULL),
    faceAreasPtr_(NULL)
{}


// Construct from components
// WARNING: ASSUMES CORRECT ORDERING OF DATA. 
primitiveMesh::primitiveMesh
(
    const label nPoints,
    const label nInternalFaces,
    const label nFaces,
    const label nCells,
    const pointField& p,
    const faceList& f,
    const labelList& own,
    const labelList& nei
)
:
    nPoints_(nPoints),
    nEdges_(-1),
    nInternalFaces_(nInternalFaces),
    nFaces_(nFaces),
    nCells_(nCells),

    points_(p, nPoints_),
    faces_(f, nFaces_),
    faceOwner_(own, nFaces_),
    faceNeighbour_(nei, nInternalFaces_),
    clearedPrimitives_(false),

    oldPointsPtr_(NULL),

    cellShapesPtr_(NULL),
    edgesPtr_(NULL),
    ccPtr_(NULL),
    ecPtr_(NULL),
    pcPtr_(NULL),

    cfPtr_(NULL),
    efPtr_(NULL),
    pfPtr_(NULL),

    cePtr_(NULL),
    fePtr_(NULL),
    pePtr_(NULL),
    ppPtr_(NULL),
    cpPtr_(NULL),

    cellCentresPtr_(NULL),
    faceCentresPtr_(NULL),
    cellVolumesPtr_(NULL),
    faceAreasPtr_(NULL)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

primitiveMesh::~primitiveMesh()
{
    clearAll();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void primitiveMesh::reset
(
    const label nPoints,
    const label nInternalFaces,
    const label nFaces,
    const label nCells,
    const pointField& p,
    const faceList& f,
    const labelList& own,
    const labelList& nei
)
{
    clearAll();

    nPoints_ = nPoints;
    nEdges_ = -1;
    nInternalFaces_ = nInternalFaces;
    nFaces_ = nFaces;
    nCells_ = nCells;

    points_ = pointField::subField(p, nPoints_);
    faces_ = faceList::subList(f, nFaces_);
    faceOwner_ = labelList::subList(own, nFaces_);
    faceNeighbour_ = labelList::subList(nei, nInternalFaces_);
    clearedPrimitives_ = false;
}


void primitiveMesh::reset
(
    const label nPoints,
    const label nInternalFaces,
    const label nFaces,
    const label nCells,
    const pointField& p,
    const faceList& f,
    const labelList& own,
    const labelList& nei,
    cellList& c
)
{
    reset
    (
        nPoints,
        nInternalFaces,
        nFaces,
        nCells,
        p,
        f,
        own,
        nei
    );

    cfPtr_ = new cellList(c, true);
}


const pointField& primitiveMesh::points() const
{
    if (clearedPrimitives_)
    {
        FatalErrorIn("const pointField& primitiveMesh::points() const")
            << "points deallocated"
            << abort(FatalError);
    }

    return points_;
}


const faceList& primitiveMesh::faces() const
{
    if (clearedPrimitives_)
    {
        FatalErrorIn("const faceList& primitiveMesh::faces() const")
            << "faces deallocated"
            << abort(FatalError);
    }

    return faces_;
}


const pointField& primitiveMesh::oldPoints() const
{
    if (!oldPointsPtr_)
    {
        FatalErrorIn("const pointField& primitiveMesh::oldPoints() const")
            << "oldPoints not allocated"
            << abort(FatalError);
    }

    return *oldPointsPtr_;
}


// Move points
tmp<scalarField> primitiveMesh::movePoints
(
    const pointField& newPoints,
    const pointField& oldPoints
)
{
    if (newPoints.size() <  nPoints() || oldPoints.size() < nPoints())
    {
        FatalErrorIn
        (
            "primitiveMesh::movePoints(const pointField& newPoints, "
            "const pointField& oldPoints)"
        )   << "Cannot move points: size of given point list smaller "
            << "than the number of active points"
            << abort(FatalError);
    }

    // Create swept volumes
    const faceList& f = faces();

    // Grab new points
    points_ = pointField::subField(newPoints, nPoints());

    // Slice old points
    deleteDemandDrivenData(oldPointsPtr_);
    oldPointsPtr_ = new pointField::subField(oldPoints, nPoints());
    const pointField::subField& oldP = *oldPointsPtr_;

    tmp<scalarField> tsweptVols(new scalarField(f.size()));
    scalarField& sweptVols = tsweptVols();

    forAll (f, faceI)
    {
        sweptVols[faceI] = f[faceI].sweptVol(oldP, points_);
    }

    // Force recalculation of all geometric data with new points
    clearGeom();

    return tsweptVols;
}


const cellShapeList& primitiveMesh::cellShapes() const
{
    if (!cellShapesPtr_)
    {
        calcCellShapes();
    }

    return *cellShapesPtr_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
