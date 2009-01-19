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
    PrimitiveEntry constructor from Istream and Ostream output operator.

\*---------------------------------------------------------------------------*/

#include "primitiveEntry.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

void primitiveEntry::readData(Istream& is)
{
    is.fatalCheck("primitiveEntry::readData(Istream& is)");

    label keywordLineNumber = is.lineNumber();

    label i = 0;
    label blockCount = 0;
    token currToken;

    if
    (
        is.read(currToken)
     && currToken.good()
     && currToken != token::END_STATEMENT
    )
    {
        newElmt(i++) = currToken;

        if
        (
            currToken == token::BEGIN_BLOCK
         || currToken == token::BEGIN_LIST
        )
        {
            blockCount++;
        }

        while
        (
            is.read(currToken)
         && currToken.good()
         && !(currToken == token::END_STATEMENT && blockCount == 0)
        )
        {
            if
            (
                currToken == token::BEGIN_BLOCK
             || currToken == token::BEGIN_LIST
            )
            {
                blockCount++;
            }
            else if
            (
                currToken == token::END_BLOCK
             || currToken == token::END_LIST
            )
            {
                blockCount--;
            }

            newElmt(i++) = currToken;
        }
    }

    if (!currToken.good())
    {
        FatalIOErrorIn("primitiveEntry::readData(Istream& is)", is) 
            << "ill defined primitiveEntry starting at keyword '"
            << keyword() << '\''
            << " on line " << keywordLineNumber
            << " and ending at line " << is.lineNumber()
            << exit(FatalIOError);
    }

    is.fatalCheck("primitiveEntry::readData(Istream& is)");

    setSize(i);
}


primitiveEntry::primitiveEntry(const word& key, Istream& is)
:
    entry(key),
    ITstream
    (
        is.name() + "::" + key,
        tokenList(10),
        is.format(),
        is.version()
    )
{
    readData(is);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void primitiveEntry::write(Ostream& os) const
{
    os.writeKeyword(keyword());

    for (label i=0; i<size(); i++)
    {
        os << operator[](i);

        if (i < size()-1)
        {
            os << token::SPACE;
        }
    }
    
    os << token::END_STATEMENT << endl;
}


// * * * * * * * * * * * * * Ostream operator  * * * * * * * * * * * * * * * //

#if defined (__GNUC__)
template<>
#endif
Ostream& operator<<(Ostream& os, const InfoProxy<primitiveEntry>& ip)
{
    const primitiveEntry& e = ip.t_;

    e.print(os);

    const label nPrintTokens = 10;

    os  << "    primitiveEntry '" << e.keyword() << "' comprises ";

    for (label i=0; i<min(e.size(), nPrintTokens); i++)
    {
        os  << nl << "        " << e[i].info();
    }

    if (e.size() > nPrintTokens)
    {
        os  << " ...";
    }
    
    os  << endl;

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
