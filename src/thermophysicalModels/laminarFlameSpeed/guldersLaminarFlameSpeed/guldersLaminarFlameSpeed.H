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
    guldersLaminarFlameSpeed

Description

SourceFiles
    guldersLaminarFlameSpeed.C


\*---------------------------------------------------------------------------*/

#ifndef guldersLaminarFlameSpeed_H
#define guldersLaminarFlameSpeed_H

#include "laminarFlameSpeed.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class guldersLaminarFlameSpeed Declaration
\*---------------------------------------------------------------------------*/

class guldersLaminarFlameSpeed
:
    public laminarFlameSpeed
{
    // Private Data

        dictionary coeffsDict_;

        scalar W_;
        scalar eta_;
        scalar xi_;
        scalar f_;
        scalar alpha_;
        scalar beta_;


    // Private member functions

        inline scalar SuRef
        (
            scalar phi
        ) const;

        inline scalar Su0pTphi
        (
            scalar p,
            scalar Tu,
            scalar phi,
            scalar Yres
        ) const;

        tmp<volScalarField> Su0pTphi
        (
            const volScalarField& p,
            const volScalarField& Tu,
            scalar phi
        ) const;

        tmp<volScalarField> Su0pTphi
        (
            const volScalarField& p,
            const volScalarField& Tu,
            const volScalarField& phi
        ) const;

        //- Construct as copy (not implemented)
        guldersLaminarFlameSpeed(const guldersLaminarFlameSpeed&);

        void operator=(const guldersLaminarFlameSpeed&);


public:

    //- Runtime type information
    TypeName("Gulders");

    // Constructors

        //- Construct from dictionary and hhuCombustionThermo
        guldersLaminarFlameSpeed
        (
            const dictionary&,
            const hhuCombustionThermo&
        );


    // Destructor

        ~guldersLaminarFlameSpeed();


    // Member functions

        //- Return the laminar flame speed [m/s]
        tmp<volScalarField> operator()() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
