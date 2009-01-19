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
    Direct solver for coupled diagonal lduMatrices.

\*---------------------------------------------------------------------------*/

#include "coupledDiagonalSolver.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(coupledDiagonalSolver, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from matrix
Foam::coupledDiagonalSolver::coupledDiagonalSolver
(
    const word& fieldName,
    FieldField<Field, scalar>& x,
    const coupledLduMatrix& matrix,
    const FieldField<Field, scalar>& b,
    const PtrList<FieldField<Field, scalar> >& bouCoeffs,
    const PtrList<FieldField<Field, scalar> >& intCoeffs,
    const lduInterfaceFieldPtrsListList& interfaces,
    const direction cmpt
)
:
    coupledLduSolver
    (
        fieldName,
        x,
        matrix,
        b,
        bouCoeffs,
        intCoeffs,
        interfaces,
        cmpt
    ),
    dp_
    (
        matrix,
        bouCoeffs,
        intCoeffs,
        interfaces,
        cmpt
    )
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::coupledSolverPerformance Foam::coupledDiagonalSolver::solve()
{
    dp_.solve(x_, b_);

    return coupledSolverPerformance
    (
        typeName,
        fieldName_,
        0,
        0,
        0,
        true,
        false
    );
}


// ************************************************************************* //
