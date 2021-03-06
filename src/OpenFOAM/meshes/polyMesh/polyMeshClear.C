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

#include "polyMesh.H"
#include "primitiveMesh.H"
#include "globalMeshData.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::polyMesh::removeBoundary()
{
    if (debug)
    {
        Info<< "void polyMesh::removeBoundary(): "
            << "Removing boundary patches."
            << endl;
    }

    // Remove the point zones
    boundary_.clear();
    boundary_.setSize(0);

    clearOut();
}


void Foam::polyMesh::removeZones()
{
    if (debug)
    {
        Info<< "void polyMesh::removeZones(): "
            << "Removing point, face and cell zones."
            << endl;
    }

    // Remove the zones
    pointZones_.clear();
    pointZones_.setSize(0);

    faceZones_.clear();
    faceZones_.setSize(0);

    cellZones_.clear();
    cellZones_.setSize(0);

    clearOut();
}


void Foam::polyMesh::clearGeom()
{
    if (debug)
    {
        Info<< "void polyMesh::clearGeom() : "
            << "clearing geometric data"
            << endl;
    }

    primitiveMesh::clearGeom();

    forAll (boundary_, patchI)
    {
        boundary_[patchI].clearGeom();
    }
}


void Foam::polyMesh::clearAddressing()
{
    if (debug)
    {
        Info<< "void polyMesh::clearAddressing() : "
            << "clearing topology"
            << endl;
    }

    primitiveMesh::clearAddressing();

    // parallelData depends on the processorPatch ordering so force
    // recalculation
    deleteDemandDrivenData(globalMeshDataPtr_);
}


void Foam::polyMesh::clearPrimitives()
{
    resetMotion();

    allPoints_.setSize(0);
    allFaces_.setSize(0);
    owner_.setSize(0);
    neighbour_.setSize(0);

    clearedPrimitives_ = true;
}


void Foam::polyMesh::clearOut()
{
    clearGeom();
    clearAddressing();
}


// ************************************************************************* //
