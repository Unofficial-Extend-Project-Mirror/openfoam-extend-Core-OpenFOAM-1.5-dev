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
    Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

Class
    LunPressure

Description

SourceFiles
    LunPressure.C

\*---------------------------------------------------------------------------*/

#ifndef LunPressure_H
#define LunPressure_H

#include "granularPressureModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class LunPressure Declaration
\*---------------------------------------------------------------------------*/

class LunPressure
:
    public granularPressureModel
{

public:

        //- Runtime type information
        TypeName("Lun");


    // Constructors

        //- Construct from components
        LunPressure(const dictionary& dict);


    // Destructor

        ~LunPressure();


    // Member Functions

        tmp<volScalarField> granularPressureCoeff
        (
            const volScalarField& alpha,
            const volScalarField& g0,
            const dimensionedScalar& rhoa,
            const dimensionedScalar& e
        ) const;

        tmp<volScalarField> granularPressureCoeffPrime
        (
            const volScalarField& alpha,
            const volScalarField& g0,
            const volScalarField& g0prime,
            const dimensionedScalar& rhoa,
            const dimensionedScalar& e
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
