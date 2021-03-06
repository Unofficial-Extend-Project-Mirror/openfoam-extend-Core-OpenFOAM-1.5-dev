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
    Foam::cloud

Description
    A cloud is a collection of lagrangian particles

SourceFiles
    cloud.C

\*---------------------------------------------------------------------------*/

#ifndef cloud_H
#define cloud_H

#include "objectRegistry.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                           Class cloud Declaration
\*---------------------------------------------------------------------------*/

class cloud
:
    public objectRegistry
{

    // Private Member Functions

        //- Disallow default bitwise copy construct
        cloud(const cloud&);

        //- Disallow default bitwise assignment
        void operator=(const cloud&);


public:

    //- Runtime type information
    TypeName("cloud");


    // Constructors

        //- Construct for the given objectRegistry and named cloud instance
        cloud
        (
            const objectRegistry& obr,
            const word& cloudName = "defaultCloud"
        );


    // Destructor

        virtual ~cloud();


    // Member Functions

        // Edit

            //- Remap the cells of particles corresponding to the
            //  mesh topology change
            virtual void autoMap(const mapPolyMesh&) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
