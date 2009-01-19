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

void primitiveMesh::calcPointFaces() const
{
    // Loop through faces and mark up points

    if (debug)
    {
        Pout<< "primitiveMesh::calcPointFaces() : calculating pointFaces"
            << endl;
    }

    // It is an error to attempt to recalculate pointFaces
    // if the pointer is already set
    if (pfPtr_)
    {
        FatalErrorIn("primitiveMesh::calcPointFaces()")
            << "pointFaces already calculated"
            << abort(FatalError);
    }
    else
    {
        const faceList& f = faces();

        // 1. Count number of faces per point

        labelList npf(nPoints(), 0);

        forAll (f, faceI)
        {
            const labelList& curPoints = f[faceI];

            forAll (curPoints, pointI)
            {
                label ptI = curPoints[pointI];

                if (ptI < 0 || ptI >= nPoints())
                {
                    FatalErrorIn("primitiveMesh::calcPointFaces()")
                        << "Face " << faceI
                        << " contains vertex labels out of range: "
                        << curPoints << " Max point index = " << nPoints()
                        << abort(FatalError);
                }

                npf[ptI]++;
            }
        }


        // 2. Size and fill pointFaces

        pfPtr_ = new labelListList(npf.size());
        labelListList& pointFaceAddr = *pfPtr_;

        forAll (pointFaceAddr, ptI)
        {
            pointFaceAddr[ptI].setSize(npf[ptI]);
        }
        npf = 0;

        forAll (f, faceI)
        {
            const labelList& curPoints = f[faceI];

            forAll (curPoints, pointI)
            {
                label ptI = curPoints[pointI];

                pointFaceAddr[ptI][npf[ptI]++] = faceI;
            }
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const labelListList& primitiveMesh::pointFaces() const
{
    if (!pfPtr_)
    {
        calcPointFaces();
    }

    return *pfPtr_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
