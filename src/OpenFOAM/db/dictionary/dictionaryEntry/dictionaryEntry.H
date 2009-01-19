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
    dictionaryEntry

Description
    A keyword and a list of tokens is a 'dictionaryEntry'.
    An dictionaryEntry can be read, written and printed, and the types and
    values of its tokens analysed.  A dictionaryEntry is a high-level building
    block for data description.  It is a front-end for the token parser.
    A list of entries can be used as a set of keyword syntax elements,
    for example.

SourceFiles
    dictionaryEntry.C
    dictionaryEntryIO.C

\*---------------------------------------------------------------------------*/

#ifndef dictionaryEntry_H
#define dictionaryEntry_H

#include "entry.H"
#include "dictionary.H"
#include "InfoProxy.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class dictionaryEntry Declaration
\*---------------------------------------------------------------------------*/

class dictionaryEntry
:
    public entry,
    public dictionary
{
    // Private member functions

        void readData(Istream&);


public:

    // Constructors

        //- Construct from Istream
        dictionaryEntry(Istream&);

        //- Construct from keyword and a Istream
        dictionaryEntry(const word& keyword, Istream&);

        //- Construct from keyword and a dictionary
        dictionaryEntry(const word& keyword, const dictionary&);

        autoPtr<entry> clone() const
        {
            return autoPtr<entry>(new dictionaryEntry(*this));
        }


    // Member functions

        //- Return the dictionary name
        const fileName& name() const
        {
            return dictionary::name();
        }

        //- Return the dictionary name
        fileName& name()
        {
            return dictionary::name();
        }

        //- Return line number of first token in dictionary
        label startLineNumber() const;

        //- Return line number of last token in dictionary
        label endLineNumber() const;

        //- Return token stream if this entry is a dictionary entry
        ITstream& stream() const;

        //- Return true because this entry is a dictionary
        bool isDict() const
        {
            return true;
        }

        //- Return token stream if this entry is a dictionary entry
        const dictionary& dict() const;

        // Write
        void write(Ostream&) const;

        //- Return info proxy.
        //  Used to print token information to a stream
        InfoProxy<dictionaryEntry> info() const
        {
            return *this;
        }


    // Ostream operator

        friend Ostream& operator<<(Ostream&, const dictionaryEntry&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
