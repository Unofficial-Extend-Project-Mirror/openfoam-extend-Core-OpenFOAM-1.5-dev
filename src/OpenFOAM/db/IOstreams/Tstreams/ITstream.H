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
    Foam::ITstream

Description
    Input token stream.

SourceFiles
    ITread.C
    ITprint.C

\*---------------------------------------------------------------------------*/

#ifndef ITstream_H
#define ITstream_H

#include "Istream.H"
#include "tokenList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class ITstream Declaration
\*---------------------------------------------------------------------------*/

class ITstream
:
    public Istream,
    public tokenList
{
    // Private data

        //- Name of ITstream
        fileName name_;

        //- Index of token currently being read
        label tokenIndex_;


public:

    // Constructors

        //- Construct from components
        ITstream
        (
            const string& name,
            const tokenList& tokens,
            streamFormat format=ASCII,
            versionNumber version=currentVersion
        )
        :
            Istream(format, version),
            tokenList(tokens),
            name_(name),
            tokenIndex_(0)
        {
            setOpened();
            setGood();
        }


        //- Construct as copy
        ITstream(const ITstream& its)
        :
            Istream(ASCII, currentVersion),
            tokenList(its),
            name_(its.name_),
            tokenIndex_(0)
        {
            setOpened();
            setGood();
        }


    // Member functions

        // Inquiry

            //- Return the name of the stream
            const fileName& name() const
            {
                return name_;
            }

            //- Return non-const access to the name of the stream
            fileName& name()
            {
                return name_;
            }

            //- Return the current token index
            label tokenIndex() const
            {
                return tokenIndex_;
            }

            //- Return non-const access to the current token index
            label& tokenIndex()
            {
                return tokenIndex_;
            }

            //- Return the number of remaining tokens
            label nRemainingTokens() const
            {
                return size() - tokenIndex_;
            }

            //- Return flags of output stream
            ios_base::fmtflags flags() const
            {
                return ios_base::fmtflags(0);
            }


        // Read functions

            //- Return next token from stream
            Istream& read(token&);

            //- Read a character
            Istream& read(char&);

            //- Read a word
            Istream& read(word&);

            // Read a string (including enclosing double-quotes)
            Istream& read(string&);

            //- Read a label
            Istream& read(label&);

            //- Read a floatScalar
            Istream& read(floatScalar&);

            //- Read a doubleScalar
            Istream& read(doubleScalar&);

            //- Read binary block
            Istream& read(char*, std::streamsize);

            //- Rewind and return the stream so that it may be read again
            Istream& rewind();


        // Edit

            //- Set flags of stream
            ios_base::fmtflags flags(const ios_base::fmtflags)
            {
                return ios_base::fmtflags(0);
            }


        // Print

            //- Print description of IOstream to Ostream
            void print(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
