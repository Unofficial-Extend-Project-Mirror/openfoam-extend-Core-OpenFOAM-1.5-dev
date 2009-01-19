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
    Combine the current agglomeration level with the previous one

\*---------------------------------------------------------------------------*/

#include "amgSymSolver.H"
#include "amgInterfaceField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void amgSymSolver::combineLevels(const label curLevel)
{
    label prevLevel = curLevel - 1;

    // Map the restrictAddressing from the coarser level into the previous
    // finer level
    const labelList& curResAddr = restrictAddressing_[curLevel];
    labelList& prevResAddr = restrictAddressing_[prevLevel];
    forAll(prevResAddr, i)
    {
        prevResAddr[i] = curResAddr[prevResAddr[i]];
    }

    // Delete the restrictAddressing for the coarser level
    restrictAddressing_.set(curLevel, NULL);

    // Delete the matrix addressing and coefficients from the previous level
    // and replace with the corresponding entried from the coarser level
    meshLevels_.set(prevLevel, meshLevels_.set(curLevel, NULL));
    matrixLevels_.set(prevLevel, matrixLevels_.set(curLevel, NULL));

    // Same for the lduInterfaceFields taking care to delete the sub-entries
    // held on List<T*>
    lduInterfaceFieldPtrsList& prevInterLevel = interfaceLevels_[prevLevel];
    forAll (prevInterLevel, i)
    {
        if (prevInterLevel.set(i))
        {
            delete prevInterLevel(i);
        }
    }
    interfaceLevels_.set(prevLevel, interfaceLevels_.set(curLevel, NULL));

    // Same for the interface coefficients
    interfaceCoeffs_.set(prevLevel, interfaceCoeffs_.set(curLevel, NULL));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
