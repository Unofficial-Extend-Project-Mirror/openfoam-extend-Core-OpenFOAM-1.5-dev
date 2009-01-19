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
    diagonalSolver

Description
    Diagonal-only solver for coupled matrix

SourceFiles
    diagonalSolver.C

\*---------------------------------------------------------------------------*/

#ifndef coupledDiagonalSolver_H
#define coupledDiagonalSolver_H

#include "coupledLduSolver.H"
#include "coupledDiagonalPrecon.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class coupledDiagonalSolver Declaration
\*---------------------------------------------------------------------------*/

class coupledDiagonalSolver
:
    public coupledLduSolver
{
    // Private Data

        // Diagonal preconditioner
        coupledDiagonalPrecon dp_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        coupledDiagonalSolver(const coupledDiagonalSolver&);

        //- Disallow default bitwise assignment
        void operator=(const coupledDiagonalSolver&);


public:

    //- Runtime type information
    TypeName("diagonal");


    // Constructors

        //- Construct from matrix
        coupledDiagonalSolver
        (
            const word& fieldName,
            FieldField<Field, scalar>& x,
            const coupledLduMatrix& matrix,
            const FieldField<Field, scalar>& b,
            const PtrList<FieldField<Field, scalar> >& bouCoeffs,
            const PtrList<FieldField<Field, scalar> >& intCoeffs,
            const lduInterfaceFieldPtrsListList& interfaces,
            const direction cmpt
        );


    // Destructor

        virtual ~coupledDiagonalSolver()
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
