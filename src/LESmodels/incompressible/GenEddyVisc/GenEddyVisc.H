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
    GenEddyVisc

Description
    General base class for all models that can be implemented as
    an eddy viscosity, i.e. algebraic and one-equation models.

    Contains fields for k (SGS turbulent kinetic energy), gamma
    (modelled viscosity) and epsilon (SGS dissipation).

SourceFiles
    GenEddyVisc.C

\*---------------------------------------------------------------------------*/

#ifndef GenEddyVisc_H
#define GenEddyVisc_H

#include "LESmodel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace LESmodels
{

/*---------------------------------------------------------------------------*\
                           Class GenEddyVisc Declaration
\*---------------------------------------------------------------------------*/

class GenEddyVisc
:
    virtual public LESmodel
{
    // Private Member Functions

        // Disallow default bitwise copy construct and assignment
        GenEddyVisc(const GenEddyVisc&);
        GenEddyVisc& operator=(const GenEddyVisc&);


protected:

        dimensionedScalar ce_;

        volScalarField nuSgs_;


public:

    // Constructors

        //- Construct from components
        GenEddyVisc
        (
            const volVectorField& U,
            const surfaceScalarField& phi,
            transportModel& transport
        );


    // Destructor

        virtual ~GenEddyVisc()
        {}


    // Member Functions

        //- Evaluate B. This is done on the fly : its not usually necessary,
        //  but will be if we want to compare with experiments or DNS
        virtual tmp<volSymmTensorField> B() const;

        //- Return SGS kinetic energy
        virtual tmp<volScalarField> k() const = 0;

        //- Return sub-grid disipation rate
        virtual tmp<volScalarField> epsilon() const
        {
            return ce_*k()*sqrt(k())/delta();
        }

        //- Return the SGS viscosity.
        virtual tmp<volScalarField> nuSgs() const
        {
            return nuSgs_;
        }

        //- Returns divergence of B : i.e. the additional term in the
        //  filtered NSE.
        virtual tmp<fvVectorMatrix> divB(volVectorField& U) const;

        //- Correct Eddy-Viscosity and related properties
        virtual void correct(const tmp<volTensorField>& gradU);

        //- Read turbulenceProperties dictionary
        bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace LESmodels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
