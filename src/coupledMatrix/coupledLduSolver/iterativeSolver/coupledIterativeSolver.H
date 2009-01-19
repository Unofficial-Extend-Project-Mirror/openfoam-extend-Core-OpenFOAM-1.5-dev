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

Class
    coupledIterativeSolver

Description
    Virtual base class for coupled iterative solvers

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

SourceFiles
    coupledIterativeSolver.C

\*---------------------------------------------------------------------------*/

#ifndef coupledIterativeSolver_H
#define coupledIterativeSolver_H

#include "coupledLduSolver.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                    Class coupledIterativeSolver Declaration
\*---------------------------------------------------------------------------*/

class coupledIterativeSolver
:
    public coupledLduSolver
{
    // Private data

        //- Control data dictionary
        dictionary dict_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        coupledIterativeSolver(const coupledIterativeSolver&);

        //- Disallow default bitwise assignment
        void operator=(const coupledIterativeSolver&);


protected:

    // Protected data

        //- Solver tolerance
        scalar tolerance_;

        //- Relative tolerance
        scalar relTolerance_;

        //- Maximum number of iterations
        label maxIter_;


    // Protected Member Functions

        //- Return dictionary
        const dictionary& dict() const;

        //- Return normalisation factor
        scalar normFactor() const;


public:

    // Constructors

        //- Construct from matrix components and solver data stream
        coupledIterativeSolver
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

        virtual ~coupledIterativeSolver()
        {}


    // Member Functions

        //- Solve the matrix with this solver
        virtual coupledSolverPerformance solve() = 0;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
