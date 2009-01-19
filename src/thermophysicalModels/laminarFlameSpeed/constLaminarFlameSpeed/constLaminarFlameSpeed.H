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
    constLaminarFlameSpeed

Description

SourceFiles
    selectLaminarFlameSpeed.C


\*---------------------------------------------------------------------------*/

#ifndef constLaminarFlameSpeed_H
#define constLaminarFlameSpeed_H

#include "laminarFlameSpeed.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class constLaminarFlameSpeed Declaration
\*---------------------------------------------------------------------------*/

class constLaminarFlameSpeed
:
    public laminarFlameSpeed
{

private:

    // Const laminar flame speed

        dimensionedScalar Su_;


    // Private member functions

        //- Construct as copy (not implemented)
        constLaminarFlameSpeed(const constLaminarFlameSpeed&);
        void operator=(const constLaminarFlameSpeed&);


public:

    //- Runtime type information
    TypeName("const");


    // Constructors

        //- Construct from dictionary and hhuCombustionThermo
        constLaminarFlameSpeed
        (
            const dictionary&,
            const hhuCombustionThermo&
        );


    // Destructor

        ~constLaminarFlameSpeed();


    // Member functions

        //- Return the laminar flame speed [m/s]
        tmp<volScalarField> operator()() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
