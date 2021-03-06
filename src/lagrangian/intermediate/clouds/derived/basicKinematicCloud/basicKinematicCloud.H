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
    Foam::basicKinematicCloud

Description
    Cloud class to introduce kinematic parcels

SourceFiles
    basicKinematicCloud.C

\*---------------------------------------------------------------------------*/

#ifndef basicKinematicCloud_H
#define basicKinematicCloud_H

#include "KinematicCloud.H"
#include "basicKinematicParcel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class basicKinematicCloud Declaration
\*---------------------------------------------------------------------------*/

class basicKinematicCloud
:
    public KinematicCloud<basicKinematicParcel>
{

    // Private member functions

        //- Disallow default bitwise copy construct
        basicKinematicCloud(const basicKinematicCloud&);

        //- Disallow default bitwise assignment
        void operator=(const basicKinematicCloud&);


public:

    //- Runtime type information
    TypeName("basicKinematicCloud");


    // Constructors

        //- Construct from components
        basicKinematicCloud
        (
            const word& cloudType,
            const volPointInterpolation& vpi,
            const volScalarField& rho,
            const volVectorField& U,
            const volScalarField& mu,
            const dimensionedVector& g
        );


    //- Destructor

        ~basicKinematicCloud();


    // Member functions

        //- Write fields
        void writeFields() const;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
