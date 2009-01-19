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
    probes

Description
    Set of locations to sample.
    Call probes.write() to sample&write files.

SourceFiles
    probes.C

\*---------------------------------------------------------------------------*/

#ifndef IOprobes_H
#define IOprobes_H

#include "probes.H"
#include "IOdictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class probes Declaration
\*---------------------------------------------------------------------------*/

class IOprobes
:
    public IOdictionary,
    public probes
{
    // Private Member Functions

        //- Disallow default bitwise copy construct and assignment
        IOprobes(const IOprobes&);
        void operator=(const IOprobes&);


public:

    // Constructors

        //- Construct for given objectRegistry and dictionary
        IOprobes
        (
            const objectRegistry& obr,
            const fileName& dictName = "probesDict"
        );


    // Destructor

        virtual ~IOprobes();


    // Member Functions

        //- Read the probes
        virtual bool read();

        //- Update for changes of mesh
        void updateMesh(const mapPolyMesh&)
        {
            read();
        }

        //- Update for changes of mesh
        void movePoints(const pointField&)
        {
            read();
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
