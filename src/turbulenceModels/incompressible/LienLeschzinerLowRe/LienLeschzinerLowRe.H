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
    LienLeschzinerLowRe

Description
    Lien and Leschziner low-Reynolds k-epsilon turbulence model.

SourceFiles
    LienLeschzinerLowRe.C
    LienLeschzinerLowReCorrect.C

\*---------------------------------------------------------------------------*/

#ifndef LienLeschzinerLowRe_H
#define LienLeschzinerLowRe_H

#include "turbulenceModel.H"
#include "wallDist.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace turbulenceModels
{

/*---------------------------------------------------------------------------*\
                           Class LienLeschzinerLowRe Declaration
\*---------------------------------------------------------------------------*/

class LienLeschzinerLowRe
:
    public turbulenceModel
{
    // Private data

        dimensionedScalar C1;
        dimensionedScalar C2;
        dimensionedScalar alphak;
        dimensionedScalar alphaEps;
        dimensionedScalar Cmu;

        dimensionedScalar Am;
        dimensionedScalar Aepsilon;
        dimensionedScalar Amu;

        volScalarField k_;
        volScalarField epsilon_;

        wallDist y_;

        volScalarField yStar_;

        volScalarField nut_;


public:

    TypeName("LienLeschzinerLowRe");

    // Constructors

        //- from components
        LienLeschzinerLowRe
        (
            const volVectorField& U,
            const surfaceScalarField& phi,
            transportModel& transport
        );


    // Destructor

        ~LienLeschzinerLowRe(){}


    // Member Functions

        tmp<volScalarField> nut() const
        {
            return nut_;
        }

        //- Return the effective diffusivity for k
        tmp<volScalarField> DkEff() const
        {
            return tmp<volScalarField> 
            (
                new volScalarField("DkEff", alphak*nut_ + nu())
            );
        }

        //- Return the effective diffusivity for epsilon
        tmp<volScalarField> DepsilonEff() const
        {
            return tmp<volScalarField> 
            (
                new volScalarField("DepsilonEff", alphaEps*nut_ + nu())
            );
        }

        tmp<volScalarField> k() const
        {
            return k_;
        }

        tmp<volScalarField> epsilon() const
        {
            return epsilon_;
        }

        tmp<volSymmTensorField> R() const;

        tmp<fvVectorMatrix> divR(volVectorField& U) const;

        void correct();

        //- Read turbulenceProperties dictionary
        bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace turbulenceModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
