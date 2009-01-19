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
    Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

\*---------------------------------------------------------------------------*/

#include "primitiveMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void primitiveMesh::calcEdgeFaces() const
{
    if (debug)
    {
        Pout<< "primitiveMesh::calcEdgeFaces() : "
            << "calculating edgeFaces"
            << endl;
    }

    // It is an error to attempt to recalculate edges
    // if the pointer is already set
    if (efPtr_)
    {
        FatalErrorIn("primitiveMesh::calcEdges() const")
            << "edgeFaces already calculated"
            << abort(FatalError);
    }
    else
    {
        const labelListList& fEdgs = faceEdges();

        labelList nef(nEdges(), 0);

        forAll (fEdgs, faceI)
        {
            const labelList& curEdges = fEdgs[faceI];

            forAll (curEdges, edgeI)
            {
                label eI = curEdges[edgeI];

                nef[eI]++;
            }
        }

        efPtr_ = new labelListList(nef.size());
        labelListList& edgeFaceAddr = *efPtr_;

        forAll(edgeFaceAddr, edgeI)
        {
            edgeFaceAddr[edgeI].setSize(nef[edgeI]);
        }
        nef = 0;


        forAll (fEdgs, faceI)
        {
            const labelList& curEdges = fEdgs[faceI];

            forAll (curEdges, edgeI)
            {
                label eI = curEdges[edgeI];

                edgeFaceAddr[eI][nef[eI]++] = faceI;
            }
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const labelListList& primitiveMesh::edgeFaces() const
{
    if (!efPtr_)
    {
        calcEdgeFaces();
    }

    return *efPtr_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
