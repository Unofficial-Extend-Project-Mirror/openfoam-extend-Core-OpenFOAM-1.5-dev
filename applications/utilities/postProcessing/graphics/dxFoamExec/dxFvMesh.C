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

#include "dxFvMesh.H"
#include "foamValid.H"
#include "dxDatabase.H"
#include "Ostream.H"
#include "long.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

void dxFvMesh::writeStats(Foam::Ostream& os) const
{
    os
        << "   cells       : " << nCells() << endl
        << "   points      : " << nPoints() << endl
        << "   faces       : " << nFaces() << endl
        << "   bounding box: " << bounds() << endl << endl;

    os  << "   patches:" << endl
        << "       label  name  size" << endl
        << "       -----  ----  ----" << endl;

    label displayLabel = 0;

    forAll (boundaryMesh(), patchi)
    {
        const polyPatch& patch = boundaryMesh()[patchi];

        if (valid(patch))
        {
            os  << "        " << displayLabel << "  " << patch.name()
                << "  " << patch.size() << endl;

            displayLabel++;
        }
        else
        {
            os  << "        skipping patch " << patch.name()
                << " (empty or wedge)" << endl;
        }
    }

    
    os  << endl
        << "    faceZones:" << endl
        << "       label  name  size" << endl
        << "       -----  ----  ----" << endl;

    forAll(faceZones(), zoneI)
    {
        const faceZone& zone = faceZones()[zoneI];

        os  << "        " << displayLabel << "  " << zone.name()
            << "  " << zone.size() << endl;
        
        displayLabel++;
    }
    os<< endl;
    
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
dxFvMesh::dxFvMesh
(
    dxDatabase& db,
    IOobject::readOption rOpt,
    IOobject::writeOption wOpt
)
:
    fvMesh
    (
        IOobject
        (
            fvMesh::defaultRegion,
            db.timeName(),
            db,
            rOpt,
            wOpt
        )
    ),
    pointMesh_(*this),
    volPointInterpolation_(*this, pointMesh_),
    facesDir_(db.findInstance(meshDir(), "faces")),
    pointsDir_(db.findInstance(meshDir(), "points")),
    addPointCellLabels_(0),
    superCells_(0),
    nBfaces_(0),
    superFacePatches_(0),
    superFaces_(0),
    nZFaces_(0),
    superZFaceZones_(0),
    superZFaces_(0),
    totalNCells_(0),
    tetMeshPtr_(NULL),
    dxBfaceCons_(NULL),
    dxConnections_(createDxConnections()),
    dxPositions_(createDxPositions()),
    dxMesh_(createDxMesh())
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

dxFvMesh::~dxFvMesh()
{
    Info<< "dxFvMesh::dxFvMesh : deleting dxFvMesh "
        << long(this) << endl;

    DXDelete(reinterpret_cast<Object>(dxMesh_));

    if (tetMeshPtr_)
    {
        delete tetMeshPtr_;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void dxFvMesh::setTetMesh()
{
    if (!tetMeshPtr_)
    {
        tetMeshPtr_ = new tetPolyMesh(*this);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
