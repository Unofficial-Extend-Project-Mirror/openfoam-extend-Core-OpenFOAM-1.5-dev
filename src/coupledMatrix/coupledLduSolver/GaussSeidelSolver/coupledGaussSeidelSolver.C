/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2006-7 H. Jasak All rights reserved
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
    Symmetric Gauss-Seidel solver for coupled diagonal lduMatrices.

\*---------------------------------------------------------------------------*/

#include "coupledGaussSeidelSolver.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(coupledGaussSeidelSolver, 0);

    coupledLduSolver::addsymMatrixConstructorToTable<coupledGaussSeidelSolver>
        addGaussSeidelSolverSymMatrixConstructorToTable_;

    coupledLduSolver::addasymMatrixConstructorToTable<coupledGaussSeidelSolver>
        addGaussSeidelSolverAsymMatrixConstructorToTable_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from matrix
Foam::coupledGaussSeidelSolver::coupledGaussSeidelSolver
(
    const word& fieldName,
    FieldField<Field, scalar>& x,
    const coupledLduMatrix& matrix,
    const FieldField<Field, scalar>& b,
    const PtrList<FieldField<Field, scalar> >& bouCoeffs,
    const PtrList<FieldField<Field, scalar> >& intCoeffs,
    const lduInterfaceFieldPtrsListList& interfaces,
    const direction cmpt,
    Istream& solverData
)
:
    coupledIterativeSolver
    (
        fieldName,
        x,
        matrix,
        b,
        bouCoeffs,
        intCoeffs,
        interfaces,
        cmpt,
        solverData
    ),
    gs_
    (
        matrix,
        bouCoeffs,
        intCoeffs,
        interfaces,
        cmpt
    ),
    nResSweeps_(readLabel(dict().lookup("nResSweeps")))
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::coupledSolverPerformance Foam::coupledGaussSeidelSolver::solve()
{
    // Prepare solver performance
    coupledSolverPerformance solverPerf(typeName, fieldName_);

    scalar norm = this->normFactor();

    FieldField<Field, scalar> wA(x_.size());

    forAll (x_, rowI)
    {
        wA.set(rowI, new scalarField(x_[rowI].size(), 0));
    }


    // Calculate initial residual. Note: for efficiency, swapping sign
    matrix_.Amul(wA, x_, bouCoeffs_, interfaces_, cmpt_);
    wA -= b_;

    solverPerf.initialResidual() = gSumMag(wA)/norm;
    solverPerf.finalResidual() = solverPerf.initialResidual();

    if (!solverPerf.checkConvergence(tolerance_, relTolerance_))
    {
        do
        {
            for (label i = 0; i < nResSweeps_; i++)
            {
                gs_.solve(x_, b_);

                solverPerf.nIterations()++;
            }

            // Re-calculate residual
            matrix_.Amul(wA, x_, bouCoeffs_, interfaces_, cmpt_);
            wA -= b_;

            solverPerf.finalResidual() = gSumMag(wA)/norm;

        } while
        (
            solverPerf.nIterations() < maxIter_
         && !(solverPerf.checkConvergence(tolerance_, relTolerance_))
        );
    }

    return solverPerf;
}


// ************************************************************************* //
