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
    laminar

Description
    Dummy turbulence model for laminar flow.

SourceFiles
    laminar.C
    laminarCorrect.C

\*---------------------------------------------------------------------------*/

#ifndef compressibleLaminar_H
#define compressibleLaminar_H

#include "turbulenceModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace compressible
{
namespace turbulenceModels
{

/*---------------------------------------------------------------------------*\
                           Class laminar Declaration
\*---------------------------------------------------------------------------*/

class laminar
:
    public turbulenceModel
{

public:

    //- Runtime type information
    TypeName("laminar");

    // Constructors

        //- from components
        laminar
        (
            const volScalarField& rho,
            const volVectorField& U,
            const surfaceScalarField& phi,
            basicThermo& thermophysicalModel
        );


    // Destructor

        ~laminar()
        {}


    // Member Functions

        tmp<volScalarField> mut() const
        {
            return tmp<volScalarField>
            (
                new volScalarField
                (
                    IOobject
                    (
                        "mut",
                        runTime_.timeName(),
                        mesh_,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE
                    ),
                    mesh_,
                    dimensionedScalar("mut", mu().dimensions(), 0.0)
                )
            );
        }

        tmp<volScalarField> muEff() const
        {
            return tmp<volScalarField>(new volScalarField("muEff", mu()));
        }

        tmp<volScalarField> alphaEff() const
        {
            return tmp<volScalarField>(new volScalarField("alphaEff", alpha()));
        }

        tmp<volScalarField> k() const
        {
            return tmp<volScalarField>
            (
                new volScalarField
                (
                    IOobject
                    (
                        "k",
                        runTime_.timeName(),
                        mesh_,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE
                    ),
                    mesh_,
                    dimensionedScalar("k", sqr(U_.dimensions()), 0.0)
                )
            );
        }

        tmp<volScalarField> epsilon() const
        {
            return tmp<volScalarField>
            (
                new volScalarField
                (
                    IOobject
                    (
                        "epsilon",
                        runTime_.timeName(),
                        mesh_,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE
                    ),
                    mesh_,
                    dimensionedScalar
                    (
                        "epsilon", sqr(U_.dimensions())/dimTime, 0.0
                    )
                )
            );
        }

        tmp<volSymmTensorField> R() const
        {
            return tmp<volSymmTensorField>
            (
                new volSymmTensorField
                (
                    IOobject
                    (
                        "R",
                        runTime_.timeName(),
                        mesh_,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE
                    ),
                    mesh_,
                    dimensionedSymmTensor
                    (
                        "R", sqr(U_.dimensions()), symmTensor::zero
                    )
                )
            );
        }

        tmp<fvVectorMatrix> divRhoR(volVectorField& U) const;

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
