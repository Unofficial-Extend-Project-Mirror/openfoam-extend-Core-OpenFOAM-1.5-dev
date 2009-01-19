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
    Agglomerated algebraic multigrid solver tuned for the FV elliptic matrices.

\*---------------------------------------------------------------------------*/

#include "amgSymSolver.H"
#include "ICCG.H"
#include "GaussSeidelSmoother.H"
#include "SubField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

lduSolverPerformance amgSymSolver::solve
(
    scalarField& x,
    const scalarField& b,
    const direction cmpt
) const
{
    // If no levels are created, bail out by calling ICCG
    if (matrixLevels_.size() < 1)
    {
        if (debug >= 2)
        {
            Pout<< "lduSolverPerformance amgSymSolver::solve : "
                << "No coarse levels. Matrix too small for AMG; "
                << "Reverting to CG."
                << endl;
        }

        return ICCG
        (
            fieldName(),
            matrix_,
            coupleBouCoeffs_,
            coupleIntCoeffs_,
            interfaces_,
            tolerance(),
            relTolerance()
        ).solve(x, b, cmpt);
    }


    // Setup class containing solver performance data
    lduSolverPerformance solverPerf(typeName, fieldName());

    // Calculate A.x and A.xRef
    scalarField Ax(x.size());
    matrix_.Amul(Ax, x, coupleBouCoeffs_, interfaces_, cmpt);

    // Create the storage for the fineCorrection which may be used as a
    // temporary in normFactor
    scalarField fineCorrection(x.size());

    // Calculate normalisation factor
    scalar normFactor = this->normFactor(x, b, Ax, fineCorrection, cmpt);

    if (debug >= 2)
    {
        Info<< "   Normalisation factor = " << normFactor << endl;
    }

    // Calculate initial residual field
    scalarField fineResidual(b - Ax);

    // Calculate residual magnitude
    solverPerf.initialResidual() = gSumMag(fineResidual)/normFactor;
    solverPerf.finalResidual() = solverPerf.initialResidual();


    // Check convergence, solve if not converged

    if (!solverPerf.checkConvergence(tolerance(), relTolerance()))
    {
        // Create coarse fields
        PtrList<scalarField> coarseX(matrixLevels_.size());

        // Create coarse Bs
        PtrList<scalarField> coarseB(matrixLevels_.size());

        forAll (matrixLevels_, levelI)
        {
            coarseX.set
            (
                levelI,
                new scalarField(meshLevels_[levelI].size(), 0.0)
            );

            coarseB.set
            (
                levelI,
                new scalarField(meshLevels_[levelI].size(), 0.0)
            );
        }

        const label topLevel = matrixLevels_.size() - 1;

        if (debug >= 3)
        {
            Pout<< "preparatory operations " << cpu_.cpuTimeIncrement() << endl;
        }

        do
        {
            if (matrixLevels_.size() > 0)
            {
                // Restrict residual for the next level up
                restrictField(coarseB[0], fineResidual, 0);

                // Solver V-cycle

                // Residual restriction (going to coarser levels)
                for (label levelI = 0; levelI < topLevel; levelI++)
                {
                    // Residual is equal to b
                    restrictField
                    (
                        coarseB[levelI + 1],
                        coarseB[levelI],
                        levelI + 1
                    );
                }

                // Top level

                if (debug >= 3)
                {
                    Pout<< "restriction " << cpu_.cpuTimeIncrement() << endl;
                }

                // clean up the field
                coarseX[topLevel] = 0;

                lduSolverPerformance coarseSolverPerf = ICCG
                (
                    "topLevelCorr",
                    matrixLevels_[topLevel],
                    interfaceCoeffs_[topLevel],
                    interfaceCoeffs_[topLevel],
                    interfaceLevels_[topLevel],
                    tolerance(),
                    relTolerance()
                ).solve(coarseX[topLevel], coarseB[topLevel], cmpt);

                if (debug >= 2)
                {
                    coarseSolverPerf.print();

                    Pout<< "Scaling factors: ";
                }

                // Post-smoothing (going to finer levels)
                for (label levelI = topLevel - 1; levelI >= 0; levelI--)
                {
                    // Note:
                    // This form of scaling assumes there is no pre-smoothing
                    // For full form see scaling for the finest mesh

                    prolongField
                    (
                        coarseX[levelI],
                        coarseX[levelI + 1],
                        levelI + 1
                    );

                    if (debug >= 3)
                    {
                        Pout<< "prolongation " << cpu_.cpuTimeIncrement()
                            << endl;
                    }

                    // Calculate scaling factor
                    scalarField::subField ACf
                    (
                        Ax,
                        coarseX[levelI].size()
                    );

                    matrixLevels_[levelI].Amul
                    (
                        reinterpret_cast<scalarField&>(ACf),
                        coarseX[levelI],
                        interfaceCoeffs_[levelI],
                        interfaceLevels_[levelI],
                        cmpt
                    );

                    scalar sf = scalingFactor
                    (
                        coarseX[levelI],
                        coarseB[levelI],
                        ACf
                    );

                    if (debug >= 2)
                    {
                        Pout<< sf << " ";
                    }

                    coarseX[levelI] *= sf;

                    if (debug >= 3)
                    {
                        Pout<< "scaling " << cpu_.cpuTimeIncrement() << endl;
                    }

                    // Smooth the solution
                    GaussSeidelSmoother::smooth
                    (
                        coarseX[levelI],
                        matrixLevels_[levelI],
                        coarseB[levelI],
                        interfaceCoeffs_[levelI],
                        interfaceLevels_[levelI],
                        cmpt,
                        nPostSweeps_
                    );

                    if (debug >= 3)
                    {
                        Pout<< "smoothing " << cpu_.cpuTimeIncrement() << endl;
                    }
                }

                // Prolong the finest level correction
                prolongField(fineCorrection, coarseX[0], 0);

                if (debug >= 3)
                {
                    Pout<< "finest prolongation " << cpu_.cpuTimeIncrement()
                        << endl;
                }

                // Calculate fine scaling factor
                matrix_.Amul
                (
                    Ax,
                    fineCorrection,
                    coupleBouCoeffs_,
                    interfaces_,
                    cmpt
                );

                scalar fsf = scalingFactor
                (
                    fineCorrection,
                    fineResidual,
                    Ax
                );

                if (debug >= 2)
                {
                    Pout<< fsf << " ";
                }

                forAll(x, i)
                {
                    x[i] += fsf*fineCorrection[i];
                }
            }

            if (debug >= 3)
            {
                Pout<< "finest scaling " << cpu_.cpuTimeIncrement() << endl;
            }

            // smooth fine matrix
            GaussSeidelSmoother::smooth
            (
                x,
                matrix_,
                b,
                coupleBouCoeffs_,
                interfaces_,
                cmpt,
                nBottomSweeps_
            );

            if (debug >= 3)
            {
                Pout<< "finest smoothing " << cpu_.cpuTimeIncrement() << endl;
            }

            // Calculate fine residual
            matrix_.Amul(Ax, x, coupleBouCoeffs_, interfaces_, cmpt);
            fineResidual = b;
            fineResidual -= Ax;

            solverPerf.finalResidual() = gSumMag(fineResidual)/normFactor;
            solverPerf.nIterations()++;
            if (debug >= 3)
            {
                Pout<< "finest residual " << cpu_.cpuTimeIncrement() << endl;
            }
        } while (!stop(solverPerf));
    }

    return solverPerf;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
