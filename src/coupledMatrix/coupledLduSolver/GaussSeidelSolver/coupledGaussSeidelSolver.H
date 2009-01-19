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

Class
    coupledGaussSeidelSolver

Description
    Symmetric Gauss-Seidel solver for coupled matrix

SourceFiles
    coupledGaussSeidelSolver.C

\*---------------------------------------------------------------------------*/

#ifndef coupledGaussSeidelSolver_H
#define coupledGaussSeidelSolver_H

#include "coupledIterativeSolver.H"
#include "coupledGaussSeidelPrecon.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class coupledGaussSeidelSolver Declaration
\*---------------------------------------------------------------------------*/

class coupledGaussSeidelSolver
:
    public coupledIterativeSolver
{
    // Private Data

        // Gauss-Seidel preconditioner
        coupledGaussSeidelPrecon gs_;

        //- Number of sweeps before evaluating residual
        label nResSweeps_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        coupledGaussSeidelSolver(const coupledGaussSeidelSolver&);

        //- Disallow default bitwise assignment
        void operator=(const coupledGaussSeidelSolver&);


public:

    //- Runtime type information
    TypeName("GaussSeidel");


    // Constructors

        //- Construct from matrix
        coupledGaussSeidelSolver
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
        );


    // Destructor

        virtual ~coupledGaussSeidelSolver()
        {}


    // Member Functions

        //- Solve the matrix with this solver
        virtual coupledSolverPerformance solve();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
