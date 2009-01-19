/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2006 H. Jasak All rights reserved
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
    coupledLduSmoother

Description
    Coupled LDU matrix smoother virtual base class.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

SourceFiles
    coupledLduSmoother.C

\*---------------------------------------------------------------------------*/

#ifndef coupledLduSmoother_H
#define coupledLduSmoother_H

#include "coupledLduMatrix.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                    Class coupledLduSmoother Declaration
\*---------------------------------------------------------------------------*/

class coupledLduSmoother
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        coupledLduSmoother(const coupledLduSmoother&);

        //- Disallow default bitwise assignment
        void operator=(const coupledLduSmoother&);


protected:

    // Protected data

        //- Matrix reference
        const coupledLduMatrix& matrix_;

        //- Boundary coefficients
        const PtrList<FieldField<Field, scalar> >& bouCoeffs_;

        //- Internal coefficients
        const PtrList<FieldField<Field, scalar> >& intCoeffs_;

        //- Interfaces
        const lduInterfaceFieldPtrsListList& interfaces_;

        //- Solving direction
        const direction cmpt_;


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            autoPtr,
            coupledLduSmoother,
            word,
            (
                const coupledLduMatrix& matrix,
                const PtrList<FieldField<Field, scalar> >& bouCoeffs,
                const PtrList<FieldField<Field, scalar> >& intCoeffs,
                const lduInterfaceFieldPtrsListList& interfaces,
                const direction cmpt
            ),
            (
                matrix,
                bouCoeffs,
                intCoeffs,
                interfaces,
                cmpt
            )
        );


    // Constructors

        //- Construct from components
        coupledLduSmoother
        (
            const coupledLduMatrix& matrix,
            const PtrList<FieldField<Field, scalar> >& bouCoeffs,
            const PtrList<FieldField<Field, scalar> >& intCoeffs,
            const lduInterfaceFieldPtrsListList& interfaces,
            const direction cmpt
        )
        :
            matrix_(matrix),
            bouCoeffs_(bouCoeffs),
            intCoeffs_(intCoeffs),
            interfaces_(interfaces),
            cmpt_(cmpt)
        {}


    // Selectors

        //- Select given matrix data and type
        static autoPtr<coupledLduSmoother> New
        (
            const word& smootherName,
            const coupledLduMatrix& matrix,
            const PtrList<FieldField<Field, scalar> >& bouCoeffs,
            const PtrList<FieldField<Field, scalar> >& intCoeffs,
            const lduInterfaceFieldPtrsListList& interfaces,
            const direction cmpt
        );


    // Destructor

        virtual ~coupledLduSmoother()
        {}


    // Member Functions

        //- Execute smoothing.
        //  Buffer is used for residual calculation scratch space
        virtual void solve
        (
            FieldField<Field, scalar>& x,
            const FieldField<Field, scalar>& b,
            FieldField<Field, scalar>& xBuffer
        ) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
