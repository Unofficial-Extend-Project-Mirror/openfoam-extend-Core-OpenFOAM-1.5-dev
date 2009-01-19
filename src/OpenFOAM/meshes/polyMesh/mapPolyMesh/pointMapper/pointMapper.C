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

#include "pointMapper.H"
#include "demandDrivenData.H"
#include "mapPolyMesh.H"
#include "polyMesh.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::pointMapper::calcAddressing() const
{
    if (directAddrPtr_ || insertedPointLabelsPtr_)
    {
        FatalErrorIn("void pointMapper::calcAddressing() const")
            << "Addressing already calculated."
            << abort(FatalError);
    }

    directAddrPtr_ = new labelList(mpm_.pointMap());
    labelList& directAddr = *directAddrPtr_;

    // Reset the size of addressing list to contain only live points
    directAddr.setSize(mesh_.nPoints());

    insertedPointLabelsPtr_ = new labelList(directAddr.size());
    labelList& insertedPoints = *insertedPointLabelsPtr_;

    label nInsertedPoints = 0;

    forAll (directAddr, pointI)
    {
        if (directAddr[pointI] < 0)
        {
            // Found inserted point
            directAddr[pointI] = 0;
            insertedPoints[nInsertedPoints] = pointI;
            nInsertedPoints++;
        }
    }

    insertedPoints.setSize(nInsertedPoints);
}


void Foam::pointMapper::clearOut()
{
    deleteDemandDrivenData(directAddrPtr_);
    deleteDemandDrivenData(insertedPointLabelsPtr_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
Foam::pointMapper::pointMapper(const mapPolyMesh& mpm)
:
    mesh_(mpm.mesh()),
    mpm_(mpm),
    insertedPoints_(true),
    directAddrPtr_(NULL),
    insertedPointLabelsPtr_(NULL)
{
    // Check for inserted points
    if (min(mpm_.pointMap()) < 0)
    {
        insertedPoints_ = true;
    }
    else
    {
        insertedPoints_ = false;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::pointMapper::~pointMapper()
{
    clearOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::label Foam::pointMapper::size() const
{
    return mesh_.nPoints();
}


Foam::label Foam::pointMapper::sizeBeforeMapping() const
{
    return mpm_.nOldPoints();
}


const Foam::unallocLabelList& Foam::pointMapper::directAddressing() const
{
    if (!directAddrPtr_)
    {
        calcAddressing();
    }

    return *directAddrPtr_;
}


const Foam::labelList& Foam::pointMapper::insertedObjectLabels() const
{
    if (!insertedPointLabelsPtr_)
    {
        if (!insertedObjects())
        {
            // There are no inserted points
            insertedPointLabelsPtr_ = new labelList(0);
        }
        else
        {
            calcAddressing();
        }
    }

    return *insertedPointLabelsPtr_;
}
        

// ************************************************************************* //
