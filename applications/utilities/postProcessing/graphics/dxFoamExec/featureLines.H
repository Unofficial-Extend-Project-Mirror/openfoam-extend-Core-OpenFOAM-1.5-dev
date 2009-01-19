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
    featureLines

Description

SourceFiles
    featureLines.C

\*---------------------------------------------------------------------------*/

#ifndef featureLines_H
#define featureLines_H

#include "polyMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{


/*---------------------------------------------------------------------------*\
                           Class featureLines Declaration
\*---------------------------------------------------------------------------*/

class featureLines
{
    // Private data

        const polyPatch& patch_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        featureLines(const featureLines&);

        //- Disallow default bitwise assignment
        void operator=(const featureLines&);


public:

    // Constructors

        //- Construct given polyPatch
        featureLines(const polyPatch&);


    // Member Functions

        // Access

            //- Return the set of feature lines on the patch corresponding to
            //  the given feature angle
            edgeList extractEdges(const scalar featureAngle);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
