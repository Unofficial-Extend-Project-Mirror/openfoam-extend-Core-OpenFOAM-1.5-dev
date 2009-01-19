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
    porousZone

Description
    porous zone definition based on cell zones and parameters obtained
    from a control dictionary constructed from the given stream.
    The orientation of the porous region is defined either by the local
    e1/e2 vectors, or by axis (e3) and direction (e1) as per coordinateSystems.
    Implemented porosity models:

        powerLaw (C0/C1 parameters)
            S = - rho * C0 * |U|^((C1 - 1) / 2) * U

        Darcy-Forchheimer (d/f parameters)
            S = - (mu * U * d + rho/2 * |U| * f) * U

    The model does not include correction of the temporal terms.

SourceFiles
    porousZone.C

\*---------------------------------------------------------------------------*/

#ifndef porousZone_H
#define porousZone_H

#include "dictionary.H"
#include "wordList.H"
#include "labelList.H"
#include "dimensionedScalar.H"
#include "dimensionedTensor.H"
#include "primitiveFieldsFwd.H"
#include "volFieldsFwd.H"
#include "fvMatricesFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class fvMesh;

/*---------------------------------------------------------------------------*\
                           Class porousZone Declaration
\*---------------------------------------------------------------------------*/

class porousZone
{
    // Private data

        //- Reference to the finite volume mesh this zone is part of
        const fvMesh& mesh_;

        //- Name of this zone
        word name_;

        //- Dictionary containing the parameters
        dictionary dict_;

        //- Cell zone ID
        label cellZoneID_;

        //- powerLaw coefficient C0
        dimensionedScalar C0_;

        //- powerLaw coefficient C1
        dimensionedScalar C1_;

        //- Darcy coefficient 
        dimensionedTensor D_;

        //- Forchheimer coefficient
        dimensionedTensor F_;


    // Private Member Functions

        //- Power-law resistance
        template<class RhoFieldType>
        void addPowerLawResistance
        (
            scalarField& Udiag,
            const labelList& cells,
            const scalarField& V,
            const RhoFieldType& rho,
            const vectorField& U
        ) const;

        //- Viscous and inertial resistance
        template<class RhoFieldType>
        void addViscousInertialResistance
        (
            scalarField& Udiag,
            vectorField& Usource,
            const labelList& cells,
            const scalarField& V,
            const RhoFieldType& rho,
            const scalarField& mu,
            const vectorField& U
        ) const;


        //- Power-law resistance
        template<class RhoFieldType>
        void addPowerLawResistance
        (
            tensorField& AU,
            const labelList& cells,
            const RhoFieldType& rho,
            const vectorField& U
        ) const;

        //- Viscous and inertial resistance
        template<class RhoFieldType>
        void addViscousInertialResistance
        (
            tensorField& AU,
            const labelList& cells,
            const RhoFieldType& rho,
            const scalarField& mu,
            const vectorField& U
        ) const;


        //- Disallow default bitwise copy construct
        porousZone(const porousZone&);

        //- Disallow default bitwise assignment
        void operator=(const porousZone&);


public:

    // Constructors

        //- Construct from fvMesh and Istream
        porousZone(const fvMesh& mesh, Istream& is);

        //- Return clone
        autoPtr<porousZone> clone() const
        {
            notImplemented("autoPtr<porousZone> clone() const");
            return autoPtr<porousZone>(NULL);
        }

        //- Return a pointer to a new porousZone created on freestore
        //  from Istream
        class iNew
        {
            const fvMesh& mesh_;

        public:

            iNew(const fvMesh& mesh)
            :
                mesh_(mesh)
            {}

            autoPtr<porousZone> operator()(Istream& is) const
            {
                return autoPtr<porousZone>(new porousZone(mesh_, is));
            }
        };


    // Member Functions

        // Access

            const word& zoneName() const
            {
                return name_;
            }

            label zoneId() const
            {
                return cellZoneID_;
            }

            const dictionary& dict() const
            {
                return dict_;
            }


        //- Add the viscous and inertial resistance force contribution
        //  to the momentum equation
        void addResistance(fvVectorMatrix& UEqn) const;

        //- Add the viscous and inertial resistance force contribution
        //  to the tensorial diagonal.
        //  Optionally correct the processor BCs of AU.
        void addResistance
        (
            const fvVectorMatrix& UEqn,
            volTensorField& AU,
            bool correctAUpBC = true
        ) const;

        //- Write the porousZone dictionary
        void writeDict(Ostream&, bool subDict = true) const;


    // Ostream Operator

        friend Ostream& operator<<(Ostream&, const porousZone&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "porousZoneTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
