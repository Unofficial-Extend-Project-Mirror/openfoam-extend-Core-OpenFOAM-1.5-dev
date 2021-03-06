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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void primitiveMesh::calcCellCells() const
{
    // Loop through faceCells and mark up neighbours

    if (debug)
    {
        Pout<< "primitiveMesh::calcCellCells() : calculating cellCells"
            << endl;
    }

    // It is an error to attempt to recalculate cellCells
    // if the pointer is already set
    if (ccPtr_)
    {
        FatalErrorIn("primitiveMesh::calcCellCells() const")
            << "cellCells already calculated"
            << abort(FatalError);
    }
    else
    {
        // 1. Count number of internal faces per cell

        labelList ncc(nCells(), 0);

        const labelList& own = faceOwner();
        const labelList& nei = faceNeighbour();

        forAll (nei, faceI)
        {
            ncc[own[faceI]]++;
            ncc[nei[faceI]]++;
        }

        // Create the storage
        ccPtr_ = new labelListList(ncc.size());
        labelListList& cellCellAddr = *ccPtr_;



        // 2. Size and fill cellFaceAddr

        forAll (cellCellAddr, cellI)
        {
            cellCellAddr[cellI].setSize(ncc[cellI]);
        }
        ncc = 0;

        forAll (nei, faceI)
        {
            label ownCellI = own[faceI];
            label neiCellI = nei[faceI];

            cellCellAddr[ownCellI][ncc[ownCellI]++] = neiCellI;
            cellCellAddr[neiCellI][ncc[neiCellI]++] = ownCellI;
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const labelListList& primitiveMesh::cellCells() const
{
    if (!ccPtr_)
    {
        calcCellCells();
    }

    return *ccPtr_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
