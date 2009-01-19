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
    LaunderGibsonRSTM

Description
    Launder-Gibson Reynolds stress turbulence model

SourceFiles
    LaunderGibsonRSTM.C
    LaunderGibsonRSTMcorrect.C

\*---------------------------------------------------------------------------*/

#ifndef LaunderGibsonRSTM_H
#define LaunderGibsonRSTM_H

#include "turbulenceModel.H"
#include "wallDistReflection.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace turbulenceModels
{

/*---------------------------------------------------------------------------*\
                           Class LaunderGibsonRSTM Declaration
\*---------------------------------------------------------------------------*/

class LaunderGibsonRSTM
:
    public turbulenceModel
{
    // Private data

        dimensionedScalar Cmu;

        dimensionedScalar Clg1;
        dimensionedScalar Clg2;

        dimensionedScalar C1;
        dimensionedScalar C2;
        dimensionedScalar Cs;
        dimensionedScalar Ceps;
        dimensionedScalar alphaR;
        dimensionedScalar alphaEps;

        dimensionedScalar C1Ref;
        dimensionedScalar C2Ref;

        scalar couplingFactor_;

        wallDistReflection yr_;

        volSymmTensorField R_;
        volScalarField k_;
        volScalarField epsilon_;
        volScalarField nut_;


public:

    //- Runtime type information
    TypeName("LaunderGibsonRSTM");

    // Constructors

        //- from components
        LaunderGibsonRSTM
        (
            const volVectorField& U,
            const surfaceScalarField& phi,
            transportModel& transport
        );


    // Destructor

        ~LaunderGibsonRSTM(){}


    // Member Functions

        // Access

        tmp<volScalarField> nut() const
        {
            return nut_;
        }

        //- Return the effective diffusivity for R
        tmp<volScalarField> DREff() const
        {
            return tmp<volScalarField> 
            (
                new volScalarField("DREff", alphaR*nut_ + nu())
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


        tmp<volSymmTensorField> R() const
        {
            return R_;
        }

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
