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
    dictionary

Description
    A dictionary is a list of keyword definitions. A keyword definition
    consists of a keyword followed by any number of values 
    (e.g. words and numbers).  Dictionary is the base class for IOdictionary
    It serves the purpose of a bootstrap dictionary for the objectRegistry data
    dictionaries, since unlike the IOdictionary class, it does not use a
    objectRegistry itself to work.

SourceFiles
    dictionary.C
    dictionaryIO.C

\*---------------------------------------------------------------------------*/

#ifndef dictionary_H
#define dictionary_H

#include "entry.H"
#include "IDLList.H"
#include "fileName.H"
#include "ITstream.H"
#include "HashTable.H"
#include "className.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class dictionary Declaration
\*---------------------------------------------------------------------------*/

class dictionary
:
    public IDLList<entry>
{
    // Private data

        //- Dictionary name
        fileName name_;

        //- HashTable of the enries held on the DL-list for quick lookup
        HashTable<entry*> hashedEntries_;

        //- Add a new entry
        void add(entry*);

        //- Read dictionary from Istream
        bool read(Istream&, const word& = word::null);


public:

    // Declare name of the class and it's debug switch
    ClassName("dictionary");


    //- Null dictionary
    static const dictionary null;


    // Constructors

        //- Construct null
        dictionary();

        //- Construct from Istream, reading entries until lastEntry or EOF
        dictionary(Istream&, const word& lastEntry = word::null);

        //- Construct as copy
        dictionary(const dictionary&);

        //- Construct and return clone
        autoPtr<dictionary> clone() const;

        //- Construct on freestore from Istream
        static autoPtr<dictionary> New(Istream& is);


    // Desstructor

        ~dictionary();


    // Member functions

        //- Return the dictionary name
        const fileName& name() const
        {
            return name_;
        }

        //- Return the dictionary name
        fileName& name()
        {
            return name_;
        }

        //- Return line number of first token in dictionary
        label startLineNumber() const;

        //- Return line number of last token in dictionary
        label endLineNumber() const;

        //- Clear the dictionary
        void clear();


        // Search and lookup

            //- Search dictionary for given keyword
            bool found(const word& keyword) const;

            //- Find and return an entry data stream
            const entry& lookupEntry(const word&) const;

            //- Find and return an entry data stream
            ITstream& lookup(const word&) const;

            //- Check if entry is a sub-dictionary
            bool isDict(const word&) const;

            //- Find and return a sub-dictionary
            const dictionary& subDict(const word&) const;

            //- Return the table of contents
            wordList toc() const;


        // Editing

            //- Add an entry
            void add(const entry&);

            //- Add a token entry
            void add(const word& keyword, const token&);

            //- Add a word entry
            void add(const word& keyword, const word&);

            //- Add a string entry
            void add(const word& keyword, const Foam::string&);

            //- Add a label entry
            void add(const word& keyword, const label);

            //- Add a scalar entry
            void add(const word& keyword, const scalar);

            //- Add an entry constructed from a ITstream
            void add(const word& keyword, const ITstream&);

            //- Add an entry constructed from a tokenList
            void add(const word& keyword, const tokenList& tokens);

            //- Add a T entry
            template<class T>
            void add(const word& keyword, const T&);

            //- Add a dictionary entry
            void add(const word& keyword, const dictionary&);

            //- Remove an entry specified by keyword
            bool remove(const word& keyword);

            //- Change the keyword for an entry,
            //  optionally forcing overwrite of an existing entry
            bool changeKeyword
            (
                const word& oldKeyword,
                const word& newKeyword,
                bool forceOverwrite = false
            );


        // Write 

            void write(Ostream& os, bool subDict = true) const;


    // Member Operators

        //- Find and return entry
        ITstream& operator[](const word&) const;

        void operator=(const dictionary&);

        //- Include entries from the given dictionary.
        //  Warn, but do not overwrite existing entries
        void operator+=(const dictionary&);

        //- Conditionally include entries from the given dictionary.
        //  Do not overwrite existing entries.
        void operator|=(const dictionary&);

        //- Unconditionally include entries from the given dictionary.
        //  Overwrite existing entries.
        void operator<<=(const dictionary&);


    // IOstream operators

        //- Read dictionary from Istream
        friend Istream& operator>>(Istream&, dictionary&);

        //- Write dictionary to Ostream
        friend Ostream& operator<<(Ostream&, const dictionary&);
};


// Global Operators

// Combine dictionaries starting from the entries in dict one and then including
// those from dict2.
// Warn, but do not overwrite the entries from dict1.
dictionary operator+(const dictionary& dict1, const dictionary& dict2);
   
// Combine dictionaries starting from the entries in dict one and then including
// those from dict2.
// Do not overwrite the entries from dict1.
dictionary operator|(const dictionary& dict1, const dictionary& dict2);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "dictionaryTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
