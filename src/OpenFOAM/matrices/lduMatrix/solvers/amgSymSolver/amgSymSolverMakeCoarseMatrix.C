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
    Given a level index calculate the next coarser matrix.

\*---------------------------------------------------------------------------*/

#include "amgSymSolver.H"
#include "amgInterfaceField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void amgSymSolver::makeCoarseMatrix(const label fineLevelIndex)
{
    // Construct the coarse matrix and ldu addressing for the next level
    // Algorithm:
    // 1) Loop through all fine faces. If the cluster labels on two sides are
    //    different, this creates a coarse face. Define owner and neighbour
    //    for this face based on cluster IDs.
    // 2) Check if the face has been seen before. If yes, add the coefficient
    //    to the appropriate field (stored with the cell). If no, create a new
    //    face with neighbour ID and add the coefficient
    // 3) Once all the faces have been created, loop through all clusters and
    //    insert the faces in the upper order. At the same time, collect the
    //    owner and neighbour addressing.
    // 4) Agglomerate the diagonal by summing up the fine diagonal

    // Get addressing
    const lduMatrix& fineMatrix = matrixLevel(fineLevelIndex);

    const unallocLabelList& upperAddr = fineMatrix.lduAddr().upperAddr();
    const unallocLabelList& lowerAddr = fineMatrix.lduAddr().lowerAddr();

    // Get off-diagonal matrix coefficients
    const scalarField& upper = fineMatrix.upper();

    // Get restriction map for current level
    const labelField& restrictMap = restrictAddressing_[fineLevelIndex];

    if (restrictMap.size() != fineMatrix.lduAddr().size())
    {
        FatalErrorIn
        (
            "amgSymSolver::makeCoarseMatrix(const label fineLevelIndex)"
        )   << "restrict map does not correspond to fine level. " << endl
            << " Sizes: restrictMap: " << restrictMap.size()
            << " nEqns: " << fineMatrix.lduAddr().size()
            << abort(FatalError);
    }


    // Count the number of coarse cells
    const label nCoarseEqns = max(restrictMap) + 1;

    // Storage for block neighbours and coefficients

    // Guess initial maximum number of neighbours in block
    label maxNnbrs = 10;

    // Number of neighbours per block
    labelList blockNnbrs(nCoarseEqns, 0);

    // Setup initial packed storage for neighbours and coefficients
    labelList blockNbrsData(maxNnbrs*nCoarseEqns);

#   ifdef USEMEMCPY
        scalarList blockCoeffsData(maxNnbrs*nCoarseEqns);
#   else
        scalarList blockCoeffsData(maxNnbrs*nCoarseEqns, 0.0);
#   endif

    // Counter for coarse faces
    label nCoarseFaces = 0;

    // Coarse matrix diagonal
    scalarField coarseDiag(nCoarseEqns, 0.0);

    // Loop through all fine faces
    forAll (upperAddr, fineFacei)
    {
        label rmUpperAddr = restrictMap[upperAddr[fineFacei]];
        label rmLowerAddr = restrictMap[lowerAddr[fineFacei]];

        if (rmUpperAddr == rmLowerAddr)
        {
            // For each fine face inside of the coarse cell, add the fine face
            // coefficient into the diagonal
            // (this will be doubled before inclusion in the final matrix)
            coarseDiag[rmUpperAddr] += upper[fineFacei];
        }
        else
        {
            // this face is a part of a coarse face

            // get coarse owner and neighbour
            const label coarseOwner = min(rmUpperAddr, rmLowerAddr);

            const label coarseNeighbour = max(rmUpperAddr, rmLowerAddr);

            // check the owner block to see if this face has already been found
            label* curBlockNbrs = &blockNbrsData[maxNnbrs*coarseOwner];

            scalar* curBlockFaceCoeffs = &blockCoeffsData[maxNnbrs*coarseOwner];

            bool nbrFound = false;
            label& curBlockNnbrs = blockNnbrs[coarseOwner];

            for (int i=0; i<curBlockNnbrs; i++)
            {
                if (curBlockNbrs[i] == coarseNeighbour)
                {
                    nbrFound = true;

                    curBlockFaceCoeffs[i] += upper[fineFacei];

                    break;
                }
            }

            if (!nbrFound)
            {
                if (curBlockNnbrs >= maxNnbrs)
                {
                    label oldMaxNnbrs = maxNnbrs;
                    maxNnbrs *= 2;

                    blockNbrsData.setSize(maxNnbrs*nCoarseEqns);

#                   ifdef USEMEMCPY
                        blockCoeffsData.setSize(maxNnbrs*nCoarseEqns);
#                   else
                        blockCoeffsData.setSize(maxNnbrs*nCoarseEqns, 0.0);
#                   endif

                    forAllReverse(blockNnbrs, i)
                    {
                        label* oldBlockNbrs = &blockNbrsData[oldMaxNnbrs*i];
                        label* newBlockNbrs = &blockNbrsData[maxNnbrs*i];

                        scalar* oldBlockFaceCoeffs =
                            &blockCoeffsData[oldMaxNnbrs*i];
                        scalar* newBlockFaceCoeffs =
                            &blockCoeffsData[maxNnbrs*i];

                        for (int j=0; j<blockNnbrs[i]; j++)
                        {
                            newBlockNbrs[j] = oldBlockNbrs[j];
                            newBlockFaceCoeffs[j] = oldBlockFaceCoeffs[j];
                        }
                    }

                    curBlockNbrs = &blockNbrsData[maxNnbrs*coarseOwner];

                    curBlockFaceCoeffs = &blockCoeffsData[maxNnbrs*coarseOwner];
                }

                curBlockNbrs[curBlockNnbrs] = coarseNeighbour;
                curBlockFaceCoeffs[curBlockNnbrs] = upper[fineFacei];
                curBlockNnbrs++;

                // new coarse face created
                nCoarseFaces++;
            }
        }
    } // end for all fine faces


    // All coarse faces created. Make the coarse off-diagonal matrix
    labelList coarseOwner(nCoarseFaces);
    labelList coarseNeighbour(nCoarseFaces);
    scalarField coarseUpper(nCoarseFaces);


    // Reorganise the storage of the coefficients into owner-neighbour
    // addressing.

    label coarseFacei = 0;

    forAll (blockNnbrs, blockI)
    {
        label* curBlockNbrs = &blockNbrsData[maxNnbrs*blockI];
        scalar* curBlockFaceCoeffs = &blockCoeffsData[maxNnbrs*blockI];
        label curBlockNnbrs = blockNnbrs[blockI];

        for (int i=0; i<curBlockNnbrs; i++)
        {
            coarseOwner[coarseFacei] = blockI;
            coarseNeighbour[coarseFacei] = curBlockNbrs[i];

            coarseUpper[coarseFacei] = curBlockFaceCoeffs[i];

            coarseFacei++;
        }
    }

    // Clear the temporary storage for the block data
    blockNnbrs.setSize(0);
    blockNbrsData.setSize(0);
    blockCoeffsData.setSize(0);


    // Create coarse-level interfaces

    // Get reference to fine-level interfaces
    const lduInterfaceFieldPtrsList& fineInterfaces =
        interfaceLevel(fineLevelIndex);

    // Get reference to fine-level coefficients
    const FieldField<Field, scalar>& fineInterfaceCoeffs =
        interfaceCoeffsLevel(fineLevelIndex);

    // Create coarse interfaces, addressing and coefficients
    // Set the coarse interfaces and coefficients
    interfaceLevels_.set
    (
        fineLevelIndex,
        new lduInterfaceFieldPtrsList(fineInterfaces.size())
    );

    interfaceCoeffs_.set
    (
        fineLevelIndex,
        new FieldField<Field, scalar>(fineInterfaces.size())
    );

    labelListList coarseInterfaceAddr(fineInterfaces.size());

    // Note: references offset by one since the fine level is stored separately
    // 
    lduInterfaceFieldPtrsList&  coarseInterfaces =
        interfaceLevels_[fineLevelIndex];
    FieldField<Field, scalar>& coarseInterfaceCoeffs =
        interfaceCoeffs_[fineLevelIndex];

    lduInterfacePtrsList coarsePatchInterfaces(coarseInterfaces.size());

    // Initialise transfer of restrict addressing on the interface
    forAll (fineInterfaces, inti)
    {
        if (fineInterfaces.set(inti))
        {
            fineInterfaces[inti]
                .interface().initInternalFieldTransfer(restrictMap, true);
        }
    }

    // Add the coarse level
    forAll (fineInterfaces, inti)
    {
        if (fineInterfaces.set(inti))
        {
            // Coarse level interface
            coarseInterfaces.set
            (
                inti,
                amgInterfaceField::New(fineInterfaces(inti)).ptr()
            );

            // Get the local face-cell addressing
            const unallocLabelList& localAddr = 
                fineMatrix.lduAddr().patchAddr(inti);

            labelField localRestrictAddressing(localAddr.size());

            forAll (localRestrictAddressing, facei)
            {
                localRestrictAddressing[facei] = restrictMap[localAddr[facei]];
            }

            const amgInterfaceField& amgInterface =
                refCast<const amgInterfaceField>(coarseInterfaces[inti]);

            coarsePatchInterfaces.set(inti, &amgInterface);

            // Coefficients and addressing
            coarseInterfaceCoeffs.set
            (
                inti,
                amgInterface.coeffs
                (
                    localRestrictAddressing,
                    fineInterfaces[inti]
                        .interface().internalFieldTransfer(restrictMap),
                    fineInterfaceCoeffs[inti]
                )
            );
            
            coarseInterfaceAddr[inti] = amgInterface.faceCells();
        }
    }
        
    // Matrix restriction done!

    // Set the coarse ldu addressing onto the list
    meshLevels_.set
    (
        fineLevelIndex,
        new lduPrimitiveMesh
        (
            nCoarseEqns,
            coarseOwner,
            coarseNeighbour,
            coarseInterfaceAddr,
            coarsePatchInterfaces,
            fineMatrix.patchSchedule(),
            true
        )
    );

    // Set the coarse level mesh
    matrixLevels_.set
    (
        fineLevelIndex,
        new lduMatrix(meshLevels_[fineLevelIndex])
    );

    // Insert coarse upper
    lduMatrix& coarseMatrix = matrixLevels_[fineLevelIndex];
    coarseMatrix.upper().transfer(coarseUpper);

    // Double the diagonal
    coarseDiag *= 2.0;
    restrictField(coarseMatrix.diag(), fineMatrix.diag(), fineLevelIndex);
    coarseMatrix.diag() += coarseDiag;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
