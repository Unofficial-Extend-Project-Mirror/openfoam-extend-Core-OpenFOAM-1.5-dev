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

\*---------------------------------------------------------------------------*/

#include "primitiveEntry.H"
#include "dictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from keyword and ITstream
primitiveEntry::primitiveEntry(const word& key, const ITstream& tokens)
:
    entry(key),
    ITstream(tokens)
{
    name() += "::" + keyword();
}


// Construct from keyword and a token
primitiveEntry::primitiveEntry(const word& keyword, const token& t)
:
    entry(keyword),
    ITstream(keyword, tokenList(1, t))
{}


// Construct from keyword and tokenList
primitiveEntry::primitiveEntry(const word& keyword, const tokenList& tokens)
:
    entry(keyword),
    ITstream(keyword, tokens)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Return line number of first token in dictionary
label primitiveEntry::startLineNumber() const
{
    if (size())
    {
        return operator[](0).lineNumber();
    }
    else
    {
        return -1;
    }
}

//- Return line number of last token in dictionary
label primitiveEntry::endLineNumber() const
{
    if (size())
    {
        return operator[](size()-1).lineNumber();
    }
    else
    {
        return -1;
    }
}


//- Return token stream if this entry is a primitive entry
ITstream& primitiveEntry::stream() const
{
    ITstream& dataStream = const_cast<primitiveEntry&>(*this);
    dataStream.rewind();
    return dataStream;
}


//- Return token stream if this entry is a primitive entry
const dictionary& primitiveEntry::dict() const
{
    FatalErrorIn("const dictionary& primitiveEntry::dict() const")
        << "Attempt to return primitive entry " << info()
        << " as a sub-dictionary"
        << abort(FatalError);

    return dictionary::null;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
