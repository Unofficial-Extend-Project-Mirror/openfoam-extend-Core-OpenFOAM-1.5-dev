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

#include "dictionaryEntry.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from keyword and ITstream
dictionaryEntry::dictionaryEntry(const word& key, const dictionary& dict)
:
    entry(key),
    dictionary(dict)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Return line number of first token in dictionary
label dictionaryEntry::startLineNumber() const
{
    if (size())
    {
        return first()->startLineNumber();
    }
    else
    {
        return -1;
    }
}

//- Return line number of last token in dictionary
label dictionaryEntry::endLineNumber() const
{
    if (size())
    {
        return last()->endLineNumber();
    }
    else
    {
        return -1;
    }
}


//- Return token stream if this entry is a dictionary entry
ITstream& dictionaryEntry::stream() const
{
    FatalIOErrorIn("ITstream& primitiveEntry::stream() const", *this)
        << "Attempt to return dictionary entry as a primitive"
        << abort(FatalIOError);

    return lookup("");
}


//- Return token stream if this entry is a dictionary entry
const dictionary& dictionaryEntry::dict() const
{
    return *this;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
