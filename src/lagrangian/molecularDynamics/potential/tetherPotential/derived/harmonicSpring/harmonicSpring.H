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
    Foam::tetherPotentials::harmonicSpring

Description


SourceFiles
    harmonicSpring.C

\*---------------------------------------------------------------------------*/

#ifndef harmonicSpring_H
#define harmonicSpring_H

#include "tetherPotential.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace tetherPotentials
{

/*---------------------------------------------------------------------------*\
                           Class harmonicSpring Declaration
\*---------------------------------------------------------------------------*/

class harmonicSpring
:
    public tetherPotential
{
    // Private data

        dictionary harmonicSpringCoeffs_;

        scalar springConstant_;


public:

    //- Runtime type information
    TypeName("harmonicSpring");


    // Constructors

        //- Construct from components
        harmonicSpring
        (
            const word& name,
            const dictionary& tetherPotentialProperties
        );


    // Destructor

        ~harmonicSpring()
        {}


    // Member Functions

        scalar energy(const scalar r) const;

        scalar force(const scalar r) const;

        //- Read transportProperties dictionary
        bool read(const dictionary& tetherPotentialProperties);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tetherPotentials
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
