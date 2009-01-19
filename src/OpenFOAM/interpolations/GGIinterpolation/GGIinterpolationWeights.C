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
    Mass-conservative face interpolation of face data between two
    primitivePatches

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*---------------------------------------------------------------------------*/

#include "GGIinterpolation.H"
#include "objectHit.H"
#include "boolList.H"
#include "DynamicList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class MasterPatch, class SlavePatch>
label GGIinterpolation<MasterPatch, SlavePatch>::maxIter_
(
    debug::optimisationSwitch("GGImaxIter", 5)
);


template<class MasterPatch, class SlavePatch>
scalar GGIinterpolation<MasterPatch, SlavePatch>::areaErrorTol_
(
    debug::tolerances("GGIAreaErrorTol", 0.01)
);


template<class MasterPatch, class SlavePatch>
scalar GGIinterpolation<MasterPatch, SlavePatch>::sumFacetTol_
(
    debug::tolerances("GGISumFacetTol", 0.5)
);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class MasterPatch, class SlavePatch>
void GGIinterpolation<MasterPatch, SlavePatch>::calcAddressing() const
{
    if
    (
        masterAddrPtr_
     || masterWeightsPtr_
     || slaveAddrPtr_
     || slaveWeightsPtr_
    )
    {
        FatalErrorIn
        (
            "void GGIinterpolation<MasterPatch, SlavePatch>::"
            "calcAddressing() const"
        )   << "Addressing already calculated"
            << abort(FatalError);
    }

    if (debug)
    {
        Info << "GGI: projecting face centres" << endl;
    }

//     const labelListList& masterPatchFaceFaces = masterPatch_.faceFaces();

    // Calculate area magnitudes
    scalarField masterMagAreas(masterPatch_.size());
    scalarField slaveMagAreas(slavePatch_.size());

    // Memory management
    {
        const pointField& masterPatchPoints = masterPatch_.points();

        forAll (masterPatch_, faceI)
        {
            masterMagAreas[faceI] =
                masterPatch_[faceI].mag(masterPatchPoints);
        }

        const pointField& slavePatchPoints = slavePatch_.points();

        forAll (slavePatch_, faceI)
        {
            slaveMagAreas[faceI] =
                slavePatch_[faceI].mag(slavePatchPoints);
        }
    }


    // Create the dynamic lists to hold the addressing
    List<DynamicList<label> > masterNbrs(masterPatch_.size());
    List<DynamicList<label> > slaveNbrs(slavePatch_.size());

    // Using scalar importance factors in weights adjustment
    List<DynamicList<scalar> > masterFactors(masterPatch_.size());
    List<DynamicList<scalar> > slaveFactors(slavePatch_.size());

    // Project in both directions

    // Algorithm:
    // 1) Establish symmetric addressing
    //
    //    For all master faces, add the slave that has been hit as a neighbour
    //    and for the hit neighbour add the master.
    //    For all slave faces, check if the hit face exists in the addressing.
    //    If not, add it at both places
    //
    // 2) Calculate weights
    //
    //    Count the number of master and slave weights and
    //    slave weight offset: this is the dimension of the system
    //
    // 3) Calculate error
    //    The error for each facet is the difference between the weighted
    //    master and slave area.  If the error is smaller than the limit,
    //    weights are accepted.

    labelList masterHits(masterPatch_.size(), -1);
    labelList slaveHits(slavePatch_.size(), -1);

    // Memory management
    {
        List<objectHit> projMasterToSlave =
            masterPatch_.projectFaceCentres
            (
                slavePatch_,
                masterPatch_.faceNormals(),
                alg_,
                dir_
            );

        List<objectHit> projSlaveToMaster =
            slavePatch_.projectFaceCentres
            (
                masterPatch_,
                slavePatch_.faceNormals(),
                alg_,
                dir_
            );

        forAll (projMasterToSlave, masterFaceI)
        {
            if (projMasterToSlave[masterFaceI].hit())
            {
                // A hit exists
                const label slaveFaceI =
                    projMasterToSlave[masterFaceI].hitObject();

                // Record a hit
                masterHits[masterFaceI] = slaveFaceI;

                // Add hit at both locations
                masterNbrs[masterFaceI].append(slaveFaceI);
                masterFactors[masterFaceI].append(1.0);

                slaveNbrs[slaveFaceI].append(masterFaceI);
                slaveFactors[slaveFaceI].append(1.0);
            }
        }

        forAll (projSlaveToMaster, slaveFaceI)
        {
            if (projSlaveToMaster[slaveFaceI].hit())
            {
                // A hit exists
                const label masterFaceI =
                    projSlaveToMaster[slaveFaceI].hitObject();

                // Record a hit
                slaveHits[slaveFaceI] = masterFaceI;

                // Search for neighbour and insert if not present
                const DynamicList<label>& cmn = masterNbrs[masterFaceI];

                bool found = false;

                forAll (cmn, cmnI)
                {
                    if (cmn[cmnI] == slaveFaceI)
                    {
                        found = true;
                    }
                }

                if (!found)
                {
                    // New pair found, insert it
                    masterNbrs[masterFaceI].append(slaveFaceI);
                    masterFactors[masterFaceI].append(1.0);

                    slaveNbrs[slaveFaceI].append(masterFaceI);
                    slaveFactors[slaveFaceI].append(1.0);
                }
            }
        }
    }

    // Get face-face addressing for patches
    const labelListList& masterFaceFaces = masterPatch_.faceFaces();
    const labelListList& slaveFaceFaces = slavePatch_.faceFaces();

    masterWeightsPtr_ = new scalarListList(masterPatch_.size());
    slaveWeightsPtr_ = new scalarListList(slavePatch_.size());

    // Weights adjustment loop
    // Note: the loop breaks only aftre the weights have been calculated
    // HJ, 29/Aug/2007
    scalar maxError = GREAT;
    label iter = 0;

    for (;;)
    {
        // Calculate weights

        // Master side
        scalarListList& masterWeights = *masterWeightsPtr_;

        scalarField masterSumFacets(masterPatch_.size(), 0);

        forAll (masterWeights, masterFaceI)
        {
            const DynamicList<label>& curNbrs = masterNbrs[masterFaceI];
            const DynamicList<scalar>& curFactors = masterFactors[masterFaceI];

            // Weights are over-written, no need to reset to zero
            scalarList& curWeights = masterWeights[masterFaceI];
            curWeights.setSize(curNbrs.size());

            scalar weightedSumFacets = 0;

            forAll (curNbrs, nbrI)
            {
                masterSumFacets[masterFaceI] += slaveMagAreas[curNbrs[nbrI]];

                weightedSumFacets +=
                    curFactors[nbrI]*slaveMagAreas[curNbrs[nbrI]];
            }

            forAll (curWeights, nbrI)
            {
                curWeights[nbrI] =
                    curFactors[nbrI]*slaveMagAreas[curNbrs[nbrI]]
                    /weightedSumFacets;
            }
        }

        // Slave side
        scalarListList& slaveWeights = *slaveWeightsPtr_;

        scalarField slaveSumFacets(slavePatch_.size(), 0);

        forAll (slaveWeights, slaveFaceI)
        {
            const DynamicList<label>& curNbrs = slaveNbrs[slaveFaceI];
            const DynamicList<scalar>& curFactors = slaveFactors[slaveFaceI];

            // Weights are over-written, no need to reset to zero
            scalarList& curWeights = slaveWeights[slaveFaceI];
            curWeights.setSize(curNbrs.size());

            scalar weightedSumFacets = 0;

            forAll (curNbrs, nbrI)
            {
                slaveSumFacets[slaveFaceI] += masterMagAreas[curNbrs[nbrI]];

                weightedSumFacets +=
                    curFactors[nbrI]*masterMagAreas[curNbrs[nbrI]];
            }

            forAll (curWeights, nbrI)
            {
                curWeights[nbrI] =
                    curFactors[nbrI]*masterMagAreas[curNbrs[nbrI]]
                    /weightedSumFacets;
            }
        }

        // Increment the iteration count and check the exit criterion
        iter++;

        if (maxError < areaErrorTol_ || iter >= maxIter_)
        {
            break;
        }

        maxError = 0;

        // Calculate the interpolation error in facet area

        // Warning:
        // At this stage, the weights are allocated to fixed size, but
        // in the iterative process, the neighbourhood will be enriched.
        // The job involves checking existing weights:
        // ALL LOOPING CONTROLS SHOULD BE DONE ON WEIGHTS LISTS
        // This will avoid visiting parts of dynamic lists that have been
        // enriched in the current adjustment sweep
        // HJ, 1/Aug/2007
        // Error codes
        // 0 = no error
        // 1 = error, adjusted weights
        // 2 = error, adjusted neighbourhood, reset weights
        labelList errorInMaster(masterNbrs.size(), 0);
        labelList errorInSlave(slaveNbrs.size(), 0);

        // Looping on master side visits all facet pairs
        // HJ, 6/Aug/2007
        forAll (masterNbrs, masterFaceI)
        {
            const DynamicList<label>& curMasterNbrs = masterNbrs[masterFaceI];
            const scalarList& curMasterWeights = masterWeights[masterFaceI];

            DynamicList<scalar>& curMasterFactors = masterFactors[masterFaceI];

            forAll (curMasterWeights, nbrI)
            {
                const label slaveFaceI = curMasterNbrs[nbrI];

                // Find the slave weight
                const DynamicList<label>& curSlaveNbrs = slaveNbrs[slaveFaceI];
                const scalarList& curSlaveWeights = slaveWeights[slaveFaceI];

                DynamicList<scalar>& curSlaveFactors = slaveFactors[slaveFaceI];

                bool foundNbr = false;

                forAll (curSlaveWeights, snI)
                {
                    if (curSlaveNbrs[snI] == masterFaceI)
                    {
                        // Found a pair
                        foundNbr = true;

                        scalar areaError =
                            curMasterWeights[nbrI]*masterMagAreas[masterFaceI]
                          - curSlaveWeights[snI]*slaveMagAreas[slaveFaceI];

                        scalar error =
                            mag(areaError)/
                            Foam::max
                            (
                                masterMagAreas[masterFaceI],
                                slaveMagAreas[slaveFaceI]
                            );

                        maxError = Foam::max(error, maxError);

                        if (error > areaErrorTol_)
                        {
                            errorInMaster[masterFaceI] = 1;
                            errorInSlave[slaveFaceI] = 1;

                            if (areaError > 0)
                            {
                                // Adjust master weight
                                scalar curFacetArea =
                                    curMasterWeights[nbrI]*
                                    masterMagAreas[masterFaceI];

                                curMasterFactors[nbrI] *=
                                    (curFacetArea - areaError)/curFacetArea;

                                // Limiting, should not be necessary
                                curMasterFactors[nbrI] =
                                    Foam::max
                                    (
                                        0,
                                        Foam::min(curMasterFactors[nbrI], 1)
                                    );
                            }
                            else
                            {
                                // Adjust slave weight
                                scalar curFacetArea =
                                    curSlaveWeights[snI]*
                                    slaveMagAreas[slaveFaceI];

                                // Warning: error < 0
                                curSlaveFactors[snI] *=
                                    (curFacetArea + areaError)/
                                    curFacetArea;

                                // Limiting, should not be necessary
                                curSlaveFactors[snI] =
                                    Foam::max
                                    (
                                        0,
                                        Foam::min(curSlaveFactors[snI], 1)
                                    );
                            }

//                             Info<< "Error (" << masterFaceI << " "
//                                 << slaveFaceI << ") = " << error
//                                 << " sign: " << areaError
//                                 << " f: " << curMasterFactors << " "
//                                 << curSlaveFactors << endl;
                        }

                        break;
                    }
                }

                if (!foundNbr)
                {
                    FatalErrorIn
                    (
                        "void GGIinterpolation<MasterPatch, SlavePatch>::"
                        "calcAddressing() const"
                    )   << " Pair (" << masterFaceI << " " << slaveFaceI
                        << ") not found: assymetric addressing."
                        << abort(FatalError);
                }
            }
        }

        // Run enrichment

        // Enrich master side
        forAll (masterNbrs, masterFaceI)
        {
            //  If face area is smaller than required overshoot,
            //  add more neighbours using faceFaces

            if (errorInMaster[masterFaceI] > 0)
            {
                const DynamicList<label>& curMasterNbrs =
                    masterNbrs[masterFaceI];

                const scalarList& curMasterWeights = masterWeights[masterFaceI];

                if
                (
                    (
                        (1 + sumFacetTol_)*masterMagAreas[masterFaceI]
                      > masterSumFacets[masterFaceI]
                    )
                 || curMasterNbrs.size() < 2
                )
                {
                    // Enrich the neighbourhood

                    // Master side: Warning: loop on weights
                    forAll (curMasterWeights, nbrI)
                    {
                        // For each slave face, grab its slave neighbours and
                        // add to the list, avoiding duplicates
                        const labelList& sff =
                            slaveFaceFaces[curMasterNbrs[nbrI]];

                        bool found = false;

                        forAll (sff, sffI)
                        {
                            // Check all neighbours: looping on current nbrs
                            forAll (curMasterNbrs, cnI)
                            {
                                if (curMasterNbrs[cnI] == sff[sffI])
                                {
                                    // Face already exists as neighbour.
                                    // Skip it
                                    found = true;
                                    break;
                                }
                            }

                            if (!found)
                            {
                                // Find the appropriate slave
                                const label slaveFaceI = sff[sffI];

                                if
                                (
                                    (
                                        (1 + sumFacetTol_)*
                                        slaveMagAreas[slaveFaceI]
                                      > slaveSumFacets[slaveFaceI]
                                    )
                                 || curMasterNbrs.size() < 2
                                )
                                {
                                    // Found a pair: Append new face
                                    masterNbrs[masterFaceI].append(slaveFaceI);
                                    masterFactors[masterFaceI].append(1.0);

                                    slaveNbrs[slaveFaceI].append(masterFaceI);
                                    slaveFactors[slaveFaceI].append(1.0);

                                    // Mark the weights to be reset
                                    errorInMaster[masterFaceI] = 2;
                                    errorInSlave[slaveFaceI] = 2;

//                                     Info<< "Added m(" << masterFaceI
//                                         << " " << slaveFaceI << ")" << endl;
                                }
                            }
                        }
                    } // End current master neighbours
                } // End neighbourhood enrichment
            }
        }

        // Enrich slave side
        forAll (slaveNbrs, slaveFaceI)
        {
            //  If face area is smaller than required overshoot,
            //  add more neighbours using faceFaces

            const DynamicList<label>& curSlaveNbrs = slaveNbrs[slaveFaceI];
            const scalarList& curSlaveWeights = slaveWeights[slaveFaceI];

            if (errorInSlave[slaveFaceI])
            {
                // Enriching the neighbourhood is not allowed in the last pass
                if
                (
                    (
                        (1 + sumFacetTol_)*slaveMagAreas[slaveFaceI]
                      > slaveSumFacets[slaveFaceI]
                    )
                 || curSlaveNbrs.size() < 2
                )
                {
                    // Enrich the neighbourhood

                    // Slave side: Warning: loop on weights
                    forAll (curSlaveWeights, nbrI)
                    {
                        // For each master face, grab its master neighbours and
                        // add to the list, avoiding duplicates
                        const labelList& mff =
                            masterFaceFaces[curSlaveNbrs[nbrI]];

                        bool found = false;

                        forAll (mff, mffI)
                        {
                            // Check all neighbours: looping on current nbrs
                            forAll (curSlaveNbrs, cnI)
                            {
                                if (curSlaveNbrs[cnI] == mff[mffI])
                                {
                                    // Face already exists as neighbour.
                                    // Skip it
                                    found = true;
                                    break;
                                }
                            }

                            if (!found)
                            {
                                // Find the appropriate master
                                const label masterFaceI = mff[mffI];

                                if
                                (
                                    (
                                        (1 + sumFacetTol_)*
                                        masterMagAreas[masterFaceI]
                                      > masterSumFacets[masterFaceI]
                                    )
                                 || curSlaveNbrs.size() < 2
                                )
                                {
                                    // Found a pair: Append new face
                                    slaveNbrs[slaveFaceI].append(masterFaceI);
                                    slaveFactors[slaveFaceI].append(1.0);

                                    masterNbrs[masterFaceI].append(slaveFaceI);
                                    masterFactors[masterFaceI].append(1.0);

                                    // Mark the weights to be reset
                                    errorInMaster[masterFaceI] = 2;
                                    errorInSlave[slaveFaceI] = 2;

//                                     Info<< "Added s(" << masterFaceI
//                                         << " " << slaveFaceI << ")" << endl;
                                }
                            }
                        }
                    } // End current slave neighbours
                } // End neighbourhood enrichment
            }
        }

        // Reset the master weights
        forAll (errorInMaster, masterFaceI)
        {
            if (errorInMaster[masterFaceI] == 2)
            {
                DynamicList<scalar>& curMasterFactors =
                    masterFactors[masterFaceI];

                forAll (curMasterFactors, nbrI)
                {
                    curMasterFactors[nbrI] = 1.0;
                }
            }
        }

        // Reset the slave weights
        forAll (errorInSlave, slaveFaceI)
        {
            if (errorInSlave[slaveFaceI] == 2)
            {
                DynamicList<scalar>& curSlaveFactors =
                    slaveFactors[slaveFaceI];

                forAll (curSlaveFactors, nbrI)
                {
                    curSlaveFactors[nbrI] = 1.0;
                }
            }
        }

        if (debug)
        {
            Info<< "Iteration " << iter << ", max error = " << maxError << endl;
        }
    }

    if (debug)
    {
        Info<< "Final weights, max error = " << maxError << endl;
    }

    // Pack the addressing into standard list
    masterAddrPtr_ = new labelListList(masterPatch_.size());
    labelListList& ma = *masterAddrPtr_;

    forAll (ma, mfI)
    {
        ma[mfI].transfer(masterNbrs[mfI].shrink());
    }

    slaveAddrPtr_ = new labelListList(slavePatch_.size());
    labelListList& sa = *slaveAddrPtr_;

    forAll (sa, sfI)
    {
        sa[sfI].transfer(slaveNbrs[sfI].shrink());
    }

    const scalarListList& mw = *masterWeightsPtr_;

    // Check addressing and weights: Debug only
    if (debug)
    {
        forAll (ma, mfI)
        {
            if (ma[mfI].size() != mw[mfI].size())
            {
                FatalErrorIn
                (
                    "void GGIinterpolation<MasterPatch, SlavePatch>::"
                    "calcAddressing() const"
                )   << "Master sizes different for face " << mfI
                    << ": " << ma[mfI].size() << " " << mw[mfI].size()
                    << abort(FatalError);
            }
        }

        const scalarListList& sw = *slaveWeightsPtr_;

        forAll (sa, sfI)
        {
            if (sa[sfI].size() != sw[sfI].size())
            {
                FatalErrorIn
                (
                    "void GGIinterpolation<MasterPatch, SlavePatch>::"
                    "calcAddressing() const"
                )   << "Slave sizes different for face " << sfI
                    << ": " << sa[sfI].size() << " " << sw[sfI].size()
                    << abort(FatalError);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
