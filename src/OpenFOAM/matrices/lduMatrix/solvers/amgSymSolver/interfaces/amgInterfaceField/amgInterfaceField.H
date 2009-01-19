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
    amgInterfaceField

Description
    Abstract base class for AMG agglomerated interfaces.

SourceFiles
    amgInterfaceField.C
    newAmgInterfaceField.C

\*---------------------------------------------------------------------------*/

#ifndef amgInterfaceField_H
#define amgInterfaceField_H

#include "lduInterface.H"
#include "lduInterfaceField.H"
#include "autoPtr.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class amgInterfaceField Declaration
\*---------------------------------------------------------------------------*/

class amgInterfaceField
:
    public lduInterface,
    public lduInterfaceField
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        amgInterfaceField(const amgInterfaceField&);

        //- Disallow default bitwise assignment
        void operator=(const amgInterfaceField&);


public:

    //- Runtime type information
    TypeName("amgInterfaceField");


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            autoPtr,
            amgInterfaceField,
            lduInterface,
            (const lduInterfaceField* fineInterfacePtr),
            (fineInterfacePtr)
        );


    // Selectors

        //- Return a pointer to a new interface created on freestore given
        //  the fine interface
        static autoPtr<amgInterfaceField> New
        (
            const lduInterfaceField* fineInterfacePtr
        );


    // Constructors

        //- Construct from fine interface
        amgInterfaceField(const lduInterfaceField* lducfPtr)
        :
            lduInterfaceField(static_cast<const lduInterface&>(*this))
        {}


    // Member Functions

        //- Return size
        virtual label size() const = 0;

        //- Return faceCell addressing
        virtual const unallocLabelList& faceCells() const = 0;

        //- Return interface coefficients given colouring and fine coeffs
        virtual tmp<scalarField> coeffs
        (
            const labelField& localRestrictAddressing,
            const labelField& neighbourRestrictAddressing,
            const scalarField& fineCoeffs
        ) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
