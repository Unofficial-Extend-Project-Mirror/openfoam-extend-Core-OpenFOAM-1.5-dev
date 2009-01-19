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
    LES model for laminar flow.  It simply returns laminar properties.

SourceFiles
    laminar.C

\*---------------------------------------------------------------------------*/

#ifndef laminar_H
#define laminar_H

#include "LESmodel.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace LESmodels
{

/*---------------------------------------------------------------------------*\
                           Class laminar Declaration
\*---------------------------------------------------------------------------*/

class laminar
:
    public LESmodel
{
    // Private member functions

        // Disallow default bitwise copy construct and assignment
        laminar(const laminar&);
        laminar& operator=(const laminar&);       


public:

    //- Runtime type information
    TypeName("laminar");


    // Constructors

        //- Constructor from components
        laminar
        (
            const volVectorField& U,
            const surfaceScalarField& phi,
            transportModel& transport
        );


    // Destructor

        ~laminar()
        {}


    // Member Functions

        //- Evaluate B. This is done on the fly : its not usually necessary,
        //  but will be if we want to compare with experiments or DNS
        tmp<volSymmTensorField> B() const
        {
            return tmp<volSymmTensorField>
            (
                new volSymmTensorField
                (
                    IOobject
                    (
                        "B",
                        runTime_.timeName(),
                        mesh_,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE
                    ),
                    mesh_,
                    dimensionedSymmTensor
                    (
                        "B", sqr(U().dimensions()), symmTensor::zero
                    )
                )
            );
        }
        
        //- Return SGS kinetic energy
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
                    dimensionedScalar("k", sqr(U().dimensions()), 0.0)
                )
            );
        }

        //- Return sub-grid disipation rate
        tmp<volScalarField> epsilon() const;

        //- Return SGS viscosity
        tmp<volScalarField> nuSgs() const
        {
            return tmp<volScalarField>
            (
                new volScalarField
                (
                    IOobject
                    (
                        "nuSgs",
                        runTime_.timeName(),
                        mesh_,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE
                    ),
                    mesh_,
                    dimensionedScalar("nuSgs", nu().dimensions(), 0.0)
                )
            );
        }       

        //- Return the effective viscosity
        tmp<volScalarField> nuEff() const
        {
            return tmp<volScalarField>
            (
                new volScalarField("nuEff", nu())
            );
        }

        //- Returns divergence of B : i.e. the additional term in the
        //  filtered NSE.
        tmp<fvVectorMatrix> divB(volVectorField& U) const;

        //- Read turbulenceProperties dictionary
        bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace LESmodels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
