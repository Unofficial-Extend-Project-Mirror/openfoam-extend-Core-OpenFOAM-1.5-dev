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

#include "coupledFacePointPatch.H"
#include "pointBoundaryMesh.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(coupledFacePointPatch, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::coupledFacePointPatch::coupledFacePointPatch
(
    const polyPatch& patch,
    const pointBoundaryMesh& bm
)
:
    facePointPatch(patch, bm),
    coupledPointPatch(bm),
    coupledPolyPatch_(refCast<const coupledPolyPatch>(patch)),
    nonGlobalPatchPointsPtr_(NULL),
    meshPointsPtr_(NULL)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::coupledFacePointPatch::~coupledFacePointPatch()
{
    deleteDemandDrivenData(nonGlobalPatchPointsPtr_);
    deleteDemandDrivenData(meshPointsPtr_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::labelList& Foam::coupledFacePointPatch::nonGlobalPatchPoints() const
{
    if (!nonGlobalPatchPointsPtr_)
    {
        calcMeshPoints();
    }

    return *nonGlobalPatchPointsPtr_;
}


const Foam::labelList& Foam::coupledFacePointPatch::meshPoints() const
{
    if (!meshPointsPtr_)
    {
        calcMeshPoints();
    }

    return *meshPointsPtr_;
}


const Foam::pointField& Foam::coupledFacePointPatch::localPoints() const
{
    notImplemented("coupledFacePointPatch::localPoints() const");
    return Field<point>::null();
}


const Foam::vectorField& Foam::coupledFacePointPatch::pointNormals() const
{
    notImplemented("coupledFacePointPatch::pointNormals() const");
    return Field<vector>::null();
}


// ************************************************************************* //
