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
    fileName

Description
    A class for handling file names.
    A fileName can be  constructed from a char* or a word.
    FileNames may be concatenated by adding a '/' separator.
    FileNames may be decomposed into the path, name or component list.
    FileNames may be interogated for type and access mode.

SourceFiles
    fileName.C
    fileNameIO.C

\*---------------------------------------------------------------------------*/

#ifndef fileName_H
#define fileName_H

#include "word.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

template<class T> class List;
typedef List<word> wordList;

/*---------------------------------------------------------------------------*\
                           Class fileName Declaration
\*---------------------------------------------------------------------------*/

class fileName
:
    public string
{
    // Private member functions

        //- Strip invalid characters from this word
        inline void stripInvalid();


public:

    //- Enumerations to handle file types and modes.
    enum Type
    {
        UNDEFINED,
        FILE,
        DIRECTORY,
        LINK
    };

    //- Debug switch
    static int debug;

    //- Null fileName
    static const fileName null;


    // Constructors

        //- Construct null
        inline fileName();

        //- Construct as copy
        inline fileName(const fileName& fn);

        //- Construct as copy of word
        inline fileName(const word& w);

        //- Construct as copy of string
        inline fileName(const string& s);

        //- Construct as copy of std::string
        inline fileName(const std::string& s);

        //- Construct as copy of character array
        inline fileName(const char*);

        //- Construct by concatenating elements of wordList separated by '/'
        explicit fileName(const wordList&);

        //- Construct from Istream
        fileName(Istream&);


    // Member functions

        //- Is this character valid for a fileName
        inline static bool valid(char);


        // Decomposition

            //- Return file name (part beyond last /)
            word name() const;

            //- Return directory path name (part before last /)
            fileName path() const;

            //- Return file name without extension (part before last .)
            fileName lessExt() const;

            //- Return file name extension (part after last .)
            word ext() const;

            //- Return path components as wordList
            wordList components(const char delimiter='/') const;

            //- Return a component of the path
            word component(const size_t, const char delimiter='/') const;


        // Interogation

            //- Return file type
            Type type() const;


    // Member operators

        // Assignment

            void operator=(const fileName&);
            void operator=(const word&);
            void operator=(const string&);
            void operator=(const std::string&);
            void operator=(const char*);


    // IOstream operators

        friend Istream& operator>>(Istream&, fileName&);
        friend Ostream& operator<<(Ostream&, const fileName&);
};


//- Assemble words and fileNames as pathnames by adding a '/' separator
fileName operator/(const string&, const string&);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fileNameI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
