/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2004-6 H. Jasak All rights reserved
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
    Virtual base class for coupled iterative solvers

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*---------------------------------------------------------------------------*/

#include "coupledIterativeSolver.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from matrix and solver data stream
Foam::coupledIterativeSolver::coupledIterativeSolver
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
    dict_(solverData),
    tolerance_(readScalar(dict_.lookup("tolerance"))),
    relTolerance_(readScalar(dict_.lookup("relativeTolerance"))),
    maxIter_(readLabel(dict().lookup("maxIterations")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::dictionary& Foam::coupledIterativeSolver::dict() const
{
    return dict_;
}


Foam::scalar Foam::coupledIterativeSolver::normFactor() const
{
    typedef FieldField<Field, scalar> scalarFieldField;

    // Calculate reference value of x
    scalar xRef = gAverage(x_);

    scalarFieldField pA(x_.size());
    scalarFieldField wA(x_.size());
    scalarFieldField xRefField(x_.size());

    forAll (x_, rowI)
    {
        pA.set(rowI, new scalarField(x_[rowI].size(), 0));
        wA.set(rowI, new scalarField(x_[rowI].size(), 0));
        xRefField.set(rowI, new scalarField(x_[rowI].size(), xRef));
    }

    // Calculate A.x and A.xRefField
    matrix_.Amul(wA, x_, bouCoeffs_, interfaces_, cmpt_);
    matrix_.Amul(pA, xRefField, bouCoeffs_, interfaces_, cmpt_);

    // Calculate the normalisation factor
    scalar normFactor =
        gSum(mag(wA - pA) + mag(b_ - pA)) + lduMatrix::small_;

    if (coupledLduMatrix::debug >= 2)
    {
        Info<< "Iterative solver normalisation factor = " << normFactor << endl;
    }

    return normFactor;
}


// ************************************************************************* //
