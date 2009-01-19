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
    Symmetric Gauss-Seidel smoother for coupled diagonal lduMatrices.

\*---------------------------------------------------------------------------*/

#include "coupledGaussSeidelSmoother.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(coupledGaussSeidelSmoother, 0);

    addToRunTimeSelectionTable
    (
        coupledLduSmoother,
        coupledGaussSeidelSmoother,
        word
    );
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from matrix
Foam::coupledGaussSeidelSmoother::coupledGaussSeidelSmoother
(
    const coupledLduMatrix& matrix,
    const PtrList<FieldField<Field, scalar> >& bouCoeffs,
    const PtrList<FieldField<Field, scalar> >& intCoeffs,
    const lduInterfaceFieldPtrsListList& interfaces,
    const direction cmpt
)
:
    coupledLduSmoother
    (
        matrix,
        bouCoeffs,
        intCoeffs,
        interfaces,
        cmpt
    ),
    gs_
    (
        matrix,
        bouCoeffs,
        intCoeffs,
        interfaces,
        cmpt
    )
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::coupledGaussSeidelSmoother::solve
(
    FieldField<Field, scalar>& x,
    const FieldField<Field, scalar>& b,
    FieldField<Field, scalar>& xBuffer
) const
{
    gs_.solve(x, b);
}


// ************************************************************************* //
