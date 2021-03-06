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
    Foam::layeredEngineMesh

Description
    Foam::layeredEngineMesh

SourceFiles
    layeredEngineMesh.C

\*---------------------------------------------------------------------------*/

#ifndef layeredEngineMesh_H
#define layeredEngineMesh_H

#include "engineMesh.H"
#include "dimensionedScalar.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class layeredEngineMesh Declaration
\*---------------------------------------------------------------------------*/

class layeredEngineMesh
:
    public engineMesh
{
    // Private data

        dimensionedScalar pistonLayers_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        layeredEngineMesh(const layeredEngineMesh&);

        //- Disallow default bitwise assignment
        void operator=(const layeredEngineMesh&);


public:

    //- Runtime type information
    TypeName("layered");


    // Constructors

        //- Construct from IOobject
        layeredEngineMesh(const IOobject& io);


    // Destructor

        ~layeredEngineMesh();


    // Member Functions

        // Edit

            void move();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
