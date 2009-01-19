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
    Restriction, prolongation and scaling for the amg solver.

\*---------------------------------------------------------------------------*/

#include "amgSymSolver.H"
#include "vector2D.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void amgSymSolver::restrictField
(
    scalarField& result,
    const scalarField& fineField,
    const label fineLevelIndex
) const
{
    const labelField& fineToCoarse = restrictAddressing_[fineLevelIndex];

    if (fineField.size() != fineToCoarse.size())
    {
        FatalErrorIn
        (
            "scalarField amgSymSolver::restrictField(const scalarField& f, "
            "const label fineLevelIndex) const"
        )   << "field does not correspond to level " << fineLevelIndex
            << " sizes: field = " << fineField.size() << " level = "
            << fineToCoarse.size()
            << abort(FatalError);
    }

    result = 0.0;

    forAll (fineField, i)
    {
        result[fineToCoarse[i]] += fineField[i];
    }
}


void amgSymSolver::prolongField
(
    scalarField& result,
    const scalarField& f,
    const label coarseLevelIndex
) const
{
    const labelField& fineToCoarse = restrictAddressing_[coarseLevelIndex];

    forAll(fineToCoarse, i)
    {
        result[i] = f[fineToCoarse[i]];
    }
}


scalar amgSymSolver::scalingFactor
(
    const scalarField& x,
    const scalarField& b,
    const scalarField& A
) const
{
    scalar scalingFactorNum = 0.0;
    scalar scalingFactorDenom = 0.0;

    forAll(x, i)
    {
        scalingFactorNum += b[i]*x[i];
        scalingFactorDenom += A[i]*x[i];
    }

    vector2D scalingVector(scalingFactorNum, scalingFactorDenom);
    reduce(scalingVector, sumOp<vector2D>());

    scalar factor = 1.0;

    if
    (
        scalingVector.x()*scalingVector.y() <= 0
     || mag(scalingVector.x()) < mag(scalingVector.y())
    )
    {
        factor = 1.0;
    }
    else if (mag(scalingVector.x()) > 2*mag(scalingVector.y()))
    {
        factor = 2.0;
    }
    else
    {
        factor = scalingVector.x()/stabilise(scalingVector.y(), VSMALL);
    }

    return factor;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
