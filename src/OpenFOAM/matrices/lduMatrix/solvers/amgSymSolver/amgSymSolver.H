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

Class
    amgSymSolver

Description
    Agglomerated algebraic multigrid solver tuned for the FV elliptic matrices.

  Characteristics:
      - Specific for elliptic matrices (symmetric positive definite, diagonally
        equal or better).
      - Choice of smoother: Gauss-Seidel (assymetric).
      - Restriction operator: summation (zero-order).
      - Prolongation operator: injection (zero-order).
      - Agglomeration algorithm: single-pass pairwise agglomeration with change
        of direction between levels.
      - Coarse matrix creation: central coefficient: summation of fine grid
        central coefficients with the removal of intra-cluster face;
        off-diagonal coefficient: summation of off-diagonal faces.
      - Coarse matrix scaling: performed by correction scaling, using steepest
        descent optimisation.
      - Type of cycle: Saw-tooth (V-cycle with no pre-smoothing). Top-level
        matrices below 100 equations solved using CG.

SourceFiles
    amgSymSolver.C
    amgSymSolverCalcAgglomeration.C
    amgSymSolverMakeCoarseMatrix.C
    amgSymSolverOperations.C
    amgSymSolverSolve.C

\*---------------------------------------------------------------------------*/

#ifndef amgSymSolver_H
#define amgSymSolver_H

#include "lduMatrix.H"
#include "PtrList.H"
#include "labelField.H"
#include "lduPrimitiveMesh.H"
#include "primitiveFields.H"
#include "lduInterfaceFieldPtrsList.H"
#include "cpuTime.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class amgSymSolver Declaration
\*---------------------------------------------------------------------------*/

class amgSymSolver
:
    public lduMatrix::solver
{
    // Static data members

        //- Max number of levels
        static label maxLevels_;

        //- Max number of cycles
        static label maxCycles_;

        //- Number of post-smoothing sweeps
        static label nPostSweeps_;

        //- Number of smoothing sweeps on finest mesh
        static label nBottomSweeps_;


    // Private data

        //- Min number of cells in top level
        label nCellsInTopLevel_;

        //- Restrict addressing array
        // Note: restrict addressing array n tells you how to grab the next
        // finer level and create the n-th level of the hierarchy. It has the
        // same size as meshLevels; the zeroth level tells you how to grab the
        // finest matrix and create the zeroth level of the hierarchy
        PtrList<labelField> restrictAddressing_;

        //- Hierarchy of meshes
        PtrList<lduPrimitiveMesh> meshLevels_;

        //- Hierarchy of matrix levels
        PtrList<lduMatrix> matrixLevels_;

        //- Hierarchy of interfaces.
        //  Warning! Needs to be cleared by hand.  
        PtrList<lduInterfaceFieldPtrsList> interfaceLevels_;

        //- Hierarchy of interfaced interface coefficients
        PtrList<FieldField<Field, scalar> > interfaceCoeffs_;

        //- Timing
        mutable cpuTime cpu_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        amgSymSolver(const amgSymSolver&);

        //- Disallow default bitwise assignment
        void operator=(const amgSymSolver&);

        //- Return the dictionary data-stream constructed from the old-style
        //  data-stream.  Needed for backward compatibility
        IStringStream solverDataStream
        (
            const word& solverName,
            Istream& solverData
        ) const;

        //- Simplified access to matrix level
        const lduMatrix& matrixLevel(const label i) const;

        //- Simplified access to interface level
        const lduInterfaceFieldPtrsList& interfaceLevel
        (
            const label i
        ) const;

        //- Simplified access to interface coeffs level
        const FieldField<Field, scalar>& interfaceCoeffsLevel
        (
            const label i
        ) const;

        //- Calculate agglomeration and return success
        bool calcAgglomeration(const label fineLevelIndex);

        //- Assemble coarse matrix
        void makeCoarseMatrix(const label fineLevelIndex);

        //- Restrict cell field
        void restrictField
        (
            scalarField& result,
            const scalarField& f,
            const label fineLevelIndex
        ) const;

        //- Prolong cell field
        void prolongField
        (
            scalarField& result,
            const scalarField& f,
            const label coarseLevelIndex
        ) const;

        //- Do a Jacobi iteration on the coarseX using ACf and
        //  return the scaling factor
        scalar scalingFactor
        (
            const scalarField& coarseX,
            const scalarField& coarseB,
            const scalarField& ACf
        ) const;

        //- Dummy readControls, the actual read is done by the read function
        //  to maintain backward compatability
        virtual void readControls()
        {}


public:

    //- Runtime type information
    TypeName("AMG");


    // Constructors

        //- Construct from lduMatrix
        amgSymSolver
        (
            const word& fieldName,
            const lduMatrix& matrix,
            const FieldField<Field, scalar>& coupleBouCoeffs,
            const FieldField<Field, scalar>& coupleIntCoeffs,
            const lduInterfaceFieldPtrsList& interfaces,
            Istream& solverData
        );


    // Destructor

        ~amgSymSolver();


    // Member Functions

        //- Set maximum number of level
        static label setMaxLevels(const label mLevels)
        {
            label oldMaxLevels = maxLevels_;
            maxLevels_ = mLevels;

            return oldMaxLevels;
        }

        //- Set maximum number of cycles
        static label setMaxCycles(const label mCycles)
        {
            label oldMaxCycles = maxCycles_;
            maxCycles_ = mCycles;

            return oldMaxCycles;
        }

        //- Set number of post-smoothing sweeps
        static label setPostSweeps(const label pSweeps)
        {
            label oldPostSweeps = nPostSweeps_;
            nPostSweeps_ = pSweeps;

            return oldPostSweeps;
        }

        //- Set number of bottom-smoothing sweeps
        static label setBottomSweeps(const label bSweeps)
        {
            label oldBottomSweeps = nBottomSweeps_;
            nBottomSweeps_ = bSweeps;

            return oldBottomSweeps;
        }

        //- Read and reset the solver parameters from the given stream
        void read(Istream& solverData);

        //- Solve
        lduSolverPerformance solve
        (
            scalarField& x,
            const scalarField& b,
            const direction cmpt=0
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
