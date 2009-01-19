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
    Calculate agglomeration clusters given a fine matrix.
    Agglomeration clusters give a coarse cluster label for each fine cell.
    Set and return true if the number of cell on coarse level is larger than
    required minimum (nCellsInTopLevel_).

\*---------------------------------------------------------------------------*/

#include "amgSymSolver.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool amgSymSolver::calcAgglomeration(const label fineLevelIndex)
{
    // Algorithm:
    // 1) Create temporary cell-face addressing using a double-pass algorithm.
    //    to create the offset table.
    // 2) loop through all cells and for each cell find the best fit neighbour.
    //    If all neighbours are grouped, "group" the cell on its own.
    // 4) If the number of coarse cells is greater than minimum, set the
    //    list and return true; otherwise return false

    // Get addressing
    const lduMatrix& fineMatrix = matrixLevel(fineLevelIndex);

    const label nEqns = fineMatrix.lduAddr().size();

    const unallocLabelList& upperAddr = fineMatrix.lduAddr().upperAddr();
    const unallocLabelList& lowerAddr = fineMatrix.lduAddr().lowerAddr();

    // Get off-diagonal matrix coefficients
    const scalarField& upper = fineMatrix.upper();

    // For each cell calculate faces
    labelList cellFaces(upperAddr.size() + lowerAddr.size());
    labelList cellFaceOffsets(nEqns + 1);

    // memory management
    {
        labelList nNbrs(nEqns, 0);

        forAll (upperAddr, facei)
        {
            nNbrs[upperAddr[facei]]++;
        }

        forAll (lowerAddr, facei)
        {
            nNbrs[lowerAddr[facei]]++;
        }

        cellFaceOffsets[0] = 0;
        forAll (nNbrs, celli)
        {
            cellFaceOffsets[celli+1] = cellFaceOffsets[celli] + nNbrs[celli];
        }

        // reset the whole list to use as counter
        nNbrs = 0;

        forAll (upperAddr, facei)
        {
            cellFaces
            [
                cellFaceOffsets[upperAddr[facei]] + nNbrs[upperAddr[facei]]
            ] = facei;

            nNbrs[upperAddr[facei]]++;
        }

        forAll (lowerAddr, facei)
        {
            cellFaces
            [
                cellFaceOffsets[lowerAddr[facei]] + nNbrs[lowerAddr[facei]]
            ] = facei;

            nNbrs[lowerAddr[facei]]++;
        }
    }


    // go through the faces and create clusters

    labelField* coarseCellMapPtr = new labelField(nEqns, -1);
    labelField& coarseCellMap = *coarseCellMapPtr;

    label nCoarseCells = 0;

    for (label celli=0; celli<nEqns; celli++)
    {
        if (coarseCellMap[celli] < 0)
        {
            label matchFaceNo = -1;
            scalar maxFaceCoeff = -GREAT;

            // check all faces to find ungrouped neighbour with largest face
            // coefficient
            for
            (
                label faceOs=cellFaceOffsets[celli];
                faceOs<cellFaceOffsets[celli+1];
                faceOs++
            )
            {
                label facei = cellFaces[faceOs];

                // I don't know whether the current cell is owner or neighbour.
                // Therefore I'll check both sides
                if
                (
                    coarseCellMap[upperAddr[facei]] < 0
                 && coarseCellMap[lowerAddr[facei]] < 0
                 && mag(upper[facei]) > maxFaceCoeff
                )
                {
                    // Match found. Pick up all the necessary data
                    matchFaceNo = facei;
                    maxFaceCoeff = mag(upper[facei]);
                }
            }

            if (matchFaceNo >= 0)
            {
                // Make a new group
                coarseCellMap[upperAddr[matchFaceNo]] = nCoarseCells;
                coarseCellMap[lowerAddr[matchFaceNo]] = nCoarseCells;
                nCoarseCells++;
            }
            else
            {
                // No match. Find the best neighbouring cluster and
                // put the cell there
                label clusterMatchFaceNo = -1;
                scalar clusterMaxFaceCoeff = -GREAT;

                for
                (
                    label faceOs=cellFaceOffsets[celli];
                    faceOs<cellFaceOffsets[celli+1];
                    faceOs++
                )
                {
                    label facei = cellFaces[faceOs];

                    if (mag(upper[facei]) > clusterMaxFaceCoeff)
                    {
                        clusterMatchFaceNo = facei;
                        clusterMaxFaceCoeff = mag(upper[facei]);
                    }
                }

                if (clusterMatchFaceNo >= 0)
                {
                    // Add the cell to the best cluster
                    coarseCellMap[celli] = max
                    (
                        coarseCellMap[upperAddr[clusterMatchFaceNo]],
                        coarseCellMap[lowerAddr[clusterMatchFaceNo]]
                    );
                }
            }
        }
    }

    // reverse the map to facilitate later agglomeration

    // for easier substitutions, decrement nCoarseCells by one
    nCoarseCells--;

    forAll (coarseCellMap, celli)
    {
        coarseCellMap[celli] = nCoarseCells - coarseCellMap[celli];
    }

    // The decision on parallel agglomeration needs to be made for the
    // whole gang of processes; otherwise I may end up with a different
    // number of agglomeration levels on different processors.
    // 
    bool moreAgglom = false;

    if (nCoarseCells >= nCellsInTopLevel_)
    {
        moreAgglom = true;
    }

    reduce(moreAgglom, andOp<bool>());

    if (moreAgglom)
    {
        restrictAddressing_.set(fineLevelIndex, coarseCellMapPtr);
    }
    else
    {
        delete coarseCellMapPtr;
    }

    return moreAgglom;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
