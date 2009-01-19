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

#include "cyclicPointPatch.H"
#include "pointBoundaryMesh.H"
#include "addToRunTimeSelectionTable.H"
#include "pointMesh.H"
#include "globalPointPatch.H"
#include "edgeList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(cyclicPointPatch, 0);

addToRunTimeSelectionTable
(
    facePointPatch,
    cyclicPointPatch,
    polyPatch
);


void Foam::cyclicPointPatch::calcMeshPoints() const
{
    const edgeList& cp = cyclicPolyPatch_.coupledPoints();
    const labelList& mp = cyclicPolyPatch_.meshPoints();

    // If there are no global points create a 1->1 map
    if (!boundaryMesh().mesh().globalData().nGlobalPoints())
    {
        nonGlobalPatchPointsPtr_ = new labelList(mp.size());
        labelList& ngpp = *nonGlobalPatchPointsPtr_;
        forAll(ngpp, i)
        {
            ngpp[i] = i;
        }

        meshPointsPtr_ = new labelList(cyclicPolyPatch_.meshPoints());

        transformPairsPtr_ = new edgeList(cp);
    }
    else
    {
        // Get reference to shared points
        const labelList& sharedPoints = 
            boundaryMesh().globalPatch().meshPoints();

        nonGlobalPatchPointsPtr_ = new labelList(mp.size());
        labelList& ngpp = *nonGlobalPatchPointsPtr_;

        meshPointsPtr_ = new labelList(mp.size());
        labelList& meshPoints_ = *meshPointsPtr_;

        labelList pointMap(mp.size(), -1);

        label noFiltPoints = 0;

        forAll (mp, pointI)
        {
            label curP = mp[pointI];

            bool found = false;

            forAll (sharedPoints, sharedI)
            {
                if (sharedPoints[sharedI] == curP)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                pointMap[pointI] = noFiltPoints;
                ngpp[noFiltPoints] = pointI;
                meshPoints_[noFiltPoints] = curP;
                noFiltPoints++;
            }
        }

        ngpp.setSize(noFiltPoints);
        meshPoints_.setSize(noFiltPoints);


        transformPairsPtr_ = new edgeList(cp.size());
        edgeList& transformPairs_ = *transformPairsPtr_;

        label noFiltPointPairs = 0;

        forAll(cp, i)
        {
            if (pointMap[cp[i][0]] != -1 && pointMap[cp[i][1]] != -1)
            {
                transformPairs_[noFiltPointPairs][0] = pointMap[cp[i][0]];
                transformPairs_[noFiltPointPairs][1] = pointMap[cp[i][1]];
                noFiltPointPairs++;
            }
            else if (pointMap[cp[i][0]] == -1 && pointMap[cp[i][1]] != -1)
            {
                FatalErrorIn("cyclicPointPatch::calcMeshPoints() const")
                    << "Point " << cp[i][0] << "of point-pair " << i
                    << " is a global point but the other point "
                    << cp[i][1] << " is not"
                    << exit(FatalError);
            }
            else if (pointMap[cp[i][0]] != -1 && pointMap[cp[i][1]] == -1)
            {
                FatalErrorIn("cyclicPointPatch::calcMeshPoints() const")
                    << "Point " << cp[i][1] << "of point-pair " << i
                    << " is a global point but the other point "
                    << cp[i][0] << " is not"
                    << exit(FatalError);
            }
        }

        transformPairs_.setSize(noFiltPointPairs);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

cyclicPointPatch::cyclicPointPatch
(
    const polyPatch& patch,
    const pointBoundaryMesh& bm
)
:
    coupledFacePointPatch(patch, bm),
    cyclicPolyPatch_(refCast<const cyclicPolyPatch>(patch)),
    transformPairsPtr_(NULL)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

cyclicPointPatch::~cyclicPointPatch()
{
    deleteDemandDrivenData(transformPairsPtr_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const edgeList& cyclicPointPatch::transformPairs() const
{
    if (!transformPairsPtr_)
    {
        calcMeshPoints();
    }

    return *transformPairsPtr_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
