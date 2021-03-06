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
    Foam::functionEntries::includeEntry

Description
    Specify an include file when reading dictionaries, expects a
    single string to follow.

    An example of the @c \#include directive:
    @verbatim
        include "includeFile"
    @endverbatim

    The usual expansion of environment variables and other constructs (eg,
    the @c ~OpenFOAM/ expansion) is retained.

See Also
    fileName, string::expand()

SourceFiles
    includeEntry.C

\*---------------------------------------------------------------------------*/

#ifndef includeEntry_H
#define includeEntry_H

#include "functionEntry.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionEntries
{

/*---------------------------------------------------------------------------*\
                           Class includeEntry Declaration
\*---------------------------------------------------------------------------*/

class includeEntry
:
    public functionEntry
{
    // Private Member Functions

        //- Read the include fileName from Istream, expand and return
        static fileName includeFileName(Istream& is);

        //- Disallow default bitwise copy construct
        includeEntry(const includeEntry&);

        //- Disallow default bitwise assignment
        void operator=(const includeEntry&);


public:

    //- Runtime type information
    TypeName("include");


    // Member Functions

        //- Execute the functionEntry in a sub-dict context
        static bool execute
        (
            dictionary& parentDict,
            Istream& is
        );

        //- Execute the functionEntry in a primitiveEntry context
        static bool execute
        (
            const dictionary& parentDict,
            primitiveEntry& entry,
            Istream& is
        );

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionEntries
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
