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
    Preconditioned Conjugate Gradient solver

\*---------------------------------------------------------------------------*/

#include "coupledBicgSolver.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(coupledBicgSolver, 0);

    coupledLduSolver::addasymMatrixConstructorToTable<coupledBicgSolver>
        addBicgSolverSymMatrixConstructorToTable_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from matrix
Foam::coupledBicgSolver::coupledBicgSolver
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
    preconPtr_
    (
        coupledLduPrecon::New
        (
            matrix,
            bouCoeffs,
            intCoeffs,
            interfaces,
            cmpt,
            dict().subDict("preconditioner")
        )
    )
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::coupledSolverPerformance Foam::coupledBicgSolver::solve()
{
    // Prepare solver performance
    coupledSolverPerformance solverPerf(typeName, fieldName_);

    scalar norm = this->normFactor();

    FieldField<Field, scalar> wA(x_.size());

    forAll (x_, rowI)
    {
        wA.set(rowI, new scalarField(x_[rowI].size(), 0));
    }


    // Calculate initial residual
    matrix_.Amul(wA, x_, bouCoeffs_, interfaces_, cmpt_);
    FieldField<Field, scalar> rA(b_ - wA);

    solverPerf.initialResidual() = gSumMag(rA)/norm;
    solverPerf.finalResidual() = solverPerf.initialResidual();

    if (!solverPerf.checkConvergence(tolerance_, relTolerance_))
    {
        scalar rho = matrix_[0].great_;
        scalar rhoOld = rho;

        scalar alpha, beta, wApT;

        FieldField<Field, scalar> pA(x_.size());
        FieldField<Field, scalar> pT(x_.size());

        FieldField<Field, scalar> wT(x_.size());
        FieldField<Field, scalar> rT = rA;

        forAll (pA, rowI)
        {
            pA.set(rowI, new scalarField(x_[rowI].size(), 0));
            pT.set(rowI, new scalarField(x_[rowI].size(), 0));

            wT.set(rowI, new scalarField(x_[rowI].size(), 0));
        }


        do
        {
            rhoOld = rho;

            // Execute preconditioning
            preconPtr_->solve(wA, rA);
            preconPtr_->solve(wT, rT);

            // Update search directions
            rho = gSumProd(wA, rT);

            beta = rho/rhoOld;

            forAll (pA, rowI)
            {
                scalarField& curPA = pA[rowI];
                const scalarField& curWA = wA[rowI];

                forAll (curPA, i)
                {
                    curPA[i] = curWA[i] + beta*curPA[i];
                }
            }

            forAll (pT, rowI)
            {
                scalarField& curPT = pT[rowI];
                const scalarField& curWT = wT[rowI];

                forAll (curPT, i)
                {
                    curPT[i] = curWT[i] + beta*curPT[i];
                }
            }

            // Update preconditioned residual
            matrix_.Amul(wA, pA, bouCoeffs_, interfaces_, cmpt_);
            matrix_.Amul(wT, pT, bouCoeffs_, interfaces_, cmpt_);

            wApT = gSumProd(wA, pT);


            // Check for singularity
            if (solverPerf.checkSingularity(mag(wApT)/norm))
            {
                break;
            }

            // Update solution and residual
            alpha = rho/wApT;

            forAll (x_, rowI)
            {
                scalarField& curX = x_[rowI];
                const scalarField& curPA = pA[rowI];

                forAll (curX, i)
                {
                    curX[i] += alpha*curPA[i];
                }
            }

            forAll (rA, rowI)
            {
                scalarField& curRA = rA[rowI];
                const scalarField& curWA = wA[rowI];

                forAll (curRA, i)
                {
                    curRA[i] -= alpha*curWA[i];
                }
            }

            forAll (rT, rowI)
            {
                scalarField& curRT = rT[rowI];
                const scalarField& curWT = wT[rowI];

                forAll (curRT, i)
                {
                    curRT[i] -= alpha*curWT[i];
                }
            }

            solverPerf.finalResidual() = gSumMag(rA)/norm;
        } while
        (
            solverPerf.nIterations()++ < maxIter_
         && !(solverPerf.checkConvergence(tolerance_, relTolerance_))
        );
    }

    return solverPerf;
}


// ************************************************************************* //
