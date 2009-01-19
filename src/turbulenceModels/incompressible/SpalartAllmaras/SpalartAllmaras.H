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
    SpalartAllmaras

Description
    Spalart-Allmaras 1-eqn mixing-length model for external flows.

    "A One-Equation Turbulence Model for Aerodynamic Flows"
    P.R. Spalart,
    S.R. Allmaras
    Boeing Commercial Airplane Group
    P.O. Box 3707, MS 7H-96
    Seattle, WA 98124-2207
    USA.

SourceFiles
    SpalartAllmaras.C
    SpalartAllmarasCorrect.C

\*---------------------------------------------------------------------------*/

#ifndef SpalartAllmaras_H
#define SpalartAllmaras_H

#include "turbulenceModel.H"
#include "wallDist.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace turbulenceModels
{

/*---------------------------------------------------------------------------*\
                           Class SpalartAllmaras Declaration
\*---------------------------------------------------------------------------*/

class SpalartAllmaras
:
    public turbulenceModel
{
    // Private data

        dimensionedScalar alphaNut;

        dimensionedScalar Cb1;
        dimensionedScalar Cb2;
        dimensionedScalar Cw1;
        dimensionedScalar Cw2;
        dimensionedScalar Cw3;
        dimensionedScalar Cv1;
        dimensionedScalar Cv2;

        volScalarField nuTilda_;
        volScalarField nut_;

        wallDist d_;


    // Private member functions

        tmp<volScalarField> chi() const;
        tmp<volScalarField> fv1(const volScalarField& chi) const;
        tmp<volScalarField> fv2
        (
            const volScalarField& chi,
            const volScalarField& fv1
        ) const;
        tmp<volScalarField> fv3
        (
            const volScalarField& chi,
            const volScalarField& fv1
        ) const;
        tmp<volScalarField> fw(const volScalarField& Stilda) const;


public:

    //- Runtime type information
    TypeName("SpalartAllmaras");


    // Constructors

        //- from components
        SpalartAllmaras
        (
            const volVectorField& U,
            const surfaceScalarField& phi,
            transportModel& transport
        );


    // Destructor

        ~SpalartAllmaras()
        {}


    // Member Functions

        tmp<volScalarField> nut() const
        {
            return nut_;
        }

        //- Return the effective diffusivity for nuTilda
        tmp<volScalarField> DnuTildaEff() const
        {
            return tmp<volScalarField> 
            (
                new volScalarField("DnuTildaEff", alphaNut*nuTilda_ + nu())
            );
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
                        mesh_
                    ),
                    mesh_,
                    dimensionedScalar("0", dimensionSet(0, 2, -2, 0, 0), 0)
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
                        "epslion",
                        runTime_.timeName(),
                        mesh_
                    ),
                    mesh_,
                    dimensionedScalar("0", dimensionSet(0, 2, -3, 0, 0), 0)
                )
            );
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
