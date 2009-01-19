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

void primitiveMesh::calcCellPoints() const
{
    if (debug)
    {
        Pout<< "primitiveMesh::calcCellPoints() : "
            << "calculating cellPoints"
            << endl;
    }
    
    // It is an error to attempt to recalculate pointPoints
    // if the pointer is already set
    if (cpPtr_)
    {
        FatalErrorIn("primitiveMesh::calcCellPoints() const")
            << "cellPoints already calculated"
            << abort(FatalError);
    }
    else
    {
        // Need pointCells to calculate cellPoints
        const labelListList& pc = pointCells();

        cpPtr_ = new labelListList(nCells());
        labelListList& cp = *cpPtr_;

        labelList cellPointCounter(nCells(), 0);

        forAll(pc, pci)
        {
            forAll(pc[pci], ci)
            {
                label cellI = pc[pci][ci];
                cellPointCounter[cellI]++;
            }
        }

        labelList cellPointIndex(nCells(),-1);
        forAll(pc, pci)
        {
            forAll(pc[pci], ci)
            {
                label cellI = pc[pci][ci];
                if(cellPointIndex[cellI] == -1)
                {
                    cp[cellI].setSize(cellPointCounter[cellI]);
                }

                cellPointIndex[cellI]++;
                cp[cellI][cellPointIndex[cellI]] = pci;
            }
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const labelListList& primitiveMesh::cellPoints() const
{
    if (!cpPtr_)
    {
        calcCellPoints();
    }
    
    return *cpPtr_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
