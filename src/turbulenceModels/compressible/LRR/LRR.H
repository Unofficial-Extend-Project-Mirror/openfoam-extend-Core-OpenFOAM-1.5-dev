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
    LRR

Description
    Launder, Reece and Rodi Reynolds-stress turbulence model.

SourceFiles
    LRR.C
    LRRcorrect.C

\*---------------------------------------------------------------------------*/

#ifndef compressibleLRR_H
#define compressibleLRR_H

#include "turbulenceModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace compressible
{
namespace turbulenceModels
{

/*---------------------------------------------------------------------------*\
                           Class LRR Declaration
\*---------------------------------------------------------------------------*/

class LRR
:
    public turbulenceModel
{
    // Private data

        dimensionedScalar Cmu;

        dimensionedScalar Clrr1;
        dimensionedScalar Clrr2;

        dimensionedScalar C1;
        dimensionedScalar C2;
        dimensionedScalar Cs;
        dimensionedScalar Ceps;

        scalar couplingFactor_;

        dimensionedScalar alphaR;
        dimensionedScalar alphaEps;
        dimensionedScalar alphah;

        volSymmTensorField R_;
        volScalarField k_;
        volScalarField epsilon_;
        volScalarField mut_;


public:

    //- Runtime type information
    TypeName("LRR");

    // Constructors

        //- from components
        LRR
        (
            const volScalarField& rho,
            const volVectorField& U,
            const surfaceScalarField& phi,
            basicThermo& thermophysicalModel
        );


    // Destructor

        ~LRR(){}


    // Member Functions

        tmp<volScalarField> mut() const
        {
            return mut_;
        }

        //- Return the effective diffusivity for R
        tmp<volScalarField> DREff() const
        {
            return tmp<volScalarField> 
            (
                new volScalarField("DREff", alphaR*mut_ + mu())
            );
        }

        //- Return the effective diffusivity for epsilon
        tmp<volScalarField> DepsilonEff() const
        {
            return tmp<volScalarField> 
            (
                new volScalarField("DepsilonEff", alphaEps*mut_ + mu())
            );
        }

        //- Return the effective turbulent thermal diffusivity
        tmp<volScalarField> alphaEff() const
        {
            return tmp<volScalarField> 
            (
                new volScalarField("alphaEff", alphah*mut_ + alpha())
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

        tmp<fvVectorMatrix> divRhoR(volVectorField& U) const;

        tmp<fvScalarMatrix> divRhoUh(volScalarField& h) const;

        void correct();

        //- Read turbulenceProperties dictionary
        bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace turbulenceModels
} // End namespace compressible
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
