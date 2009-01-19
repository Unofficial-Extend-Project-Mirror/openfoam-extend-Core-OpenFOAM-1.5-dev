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
    NamedEnum

Description

SourceFiles
    NamedEnum.C

\*---------------------------------------------------------------------------*/

#ifndef NamedEnum_H
#define NamedEnum_H

#include "HashTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class NamedEnum Declaration
\*---------------------------------------------------------------------------*/

template<class Enum, int nEnum>
class NamedEnum
:
    public HashTable<int>
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        NamedEnum(const NamedEnum&);

        //- Disallow default bitwise assignment
        void operator=(const NamedEnum&);


public:

    // Static data members

        //- The set of names corresponding to the enumeration Enum
        static const char* names[nEnum];


    // Constructors

        //- Construct from names
        NamedEnum();


    // Member Functions

        //- Read a word from Istream and return the corresponding
        //  enumeration element
        const Enum read(Istream& is) const
        {
            word name(is);

            HashTable<int>::const_iterator iter = find(name);

            if (iter == HashTable<int>::end())
            {
                FatalIOErrorIn
                (
                    "NamedEnum<Enum, nEnum>::read(Istream& is) const",
                    is
                ) << name << " is not in enumeration " << toc()
                  << exit(FatalIOError);
            }

            return Enum(iter());
        }


    // Member Operators

        //- Return the enumeration element corresponding to the given name
        const Enum operator[](const char* name) const
        {
            return Enum(HashTable<int>::operator[](name));
        }

        //- Return the enumeration element corresponding to the given name
        const Enum operator[](const word& name) const
        {
            return Enum(HashTable<int>::operator[](name));
        }

        //- Return the name or the given enumeration element
        const char* operator[](const Enum e) const
        {
            return names[e];
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "NamedEnum.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
