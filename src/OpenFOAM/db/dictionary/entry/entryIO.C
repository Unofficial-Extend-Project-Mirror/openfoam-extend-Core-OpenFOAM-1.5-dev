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

Description
    Entry constructor from Istream and Ostream output operator.

\*---------------------------------------------------------------------------*/

#include "primitiveEntry.H"
#include "dictionaryEntry.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

//- Construct on freestore from Istream
autoPtr<entry> entry::New(Istream& is)
{
    is.fatalCheck("primitiveEntry::primitiveEntry(Istream& is)");

    // Get the keyword
    token keywordToken(is);

    // If end of file return empty tokenList without error
    if (is.eof() || !keywordToken.isWord())
    {
        return autoPtr<entry>(NULL);
    }
    else if (keywordToken.isWord())
    {
        token nextToken(is);
        is.putBack(nextToken);

        if (nextToken == token::BEGIN_BLOCK)
        {
            return autoPtr<entry>
            (
                new dictionaryEntry(keywordToken.wordToken(), is)
            );
        }
        else
        {
            return autoPtr<entry>
            (
                new primitiveEntry(keywordToken.wordToken(), is)
            );
        }
    }
    else
    {
        FatalIOErrorIn("primitiveEntry::primitiveEntry(Istream&)", is)
            << "bad keyword " << keywordToken.info()
            << exit(FatalIOError);

        return autoPtr<entry>(NULL);
    }
}


// * * * * * * * * * * * * * Ostream operator  * * * * * * * * * * * * * * * //

Ostream& operator<<(Ostream& os, const entry& e)
{
    e.write(os);
    return os;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
