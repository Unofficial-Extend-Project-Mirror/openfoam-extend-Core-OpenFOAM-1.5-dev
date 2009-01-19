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
    coupledNoPrecon

Description
    No preconditioning

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

SourceFiles
    coupledNoPrecon.C

\*---------------------------------------------------------------------------*/

#ifndef coupledNoPrecon_H
#define coupledNoPrecon_H

#include "coupledLduPrecon.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class coupledNoPrecon Declaration
\*---------------------------------------------------------------------------*/

class coupledNoPrecon
:
    public coupledLduPrecon
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        coupledNoPrecon(const coupledNoPrecon&);

        //- Disallow default bitwise assignment
        void operator=(const coupledNoPrecon&);


public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from components
        coupledNoPrecon
        (
            const coupledLduMatrix& matrix,
            const PtrList<FieldField<Field, scalar> >& bouCoeffs,
            const PtrList<FieldField<Field, scalar> >& intCoeffs,
            const lduInterfaceFieldPtrsListList& interfaces,
            const direction cmpt,
            const dictionary& dict
        )
        :
            coupledLduPrecon
            (
                matrix,
                bouCoeffs,
                intCoeffs,
                interfaces,
                cmpt
            )
        {}


    // Destructor

        virtual ~coupledNoPrecon()
        {}


    // Member Functions

        //- Execute preconditioning
        virtual void solve
        (
            FieldField<Field, scalar>& x,
            const FieldField<Field, scalar>& b
        ) const
        {
            x = b;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
