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

    Reads the data description and data portions of a dictionary File.

\*---------------------------------------------------------------------------*/

#include "dictionary.H"
#include "IFstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions * * * * * * * * * * * //

bool dictionary::read(Istream& is, const word& lastEntry)
{
    if (!is.good())
    {
        FatalIOErrorIn("dictionary::read(Istream&, const word&)", is)
            << "Istream not OK for reading dictionary "
            << exit(FatalIOError);

        return false;
    }

    token currToken(is);
    if (currToken != token::BEGIN_BLOCK)
    {
        is.putBack(currToken);
    }

    entry* entryPtr;

    while (!is.eof() && (entryPtr = entry::New(is).ptr()))
    {
        if (entryPtr->keyword() == "FoamFile")
        {
            delete entryPtr;
        }
        else if (entryPtr->keyword() == "include")
        {
            fileName fName(entryPtr->stream());
            fName.expand();

            if (fName.size() && fName[0] != '/')
            {
                fName = fileName(is.name()).path()/fName;
            }

            IFstream fileStream(fName);

            if (fileStream)
            {
                read(fileStream, lastEntry);
            }
            else
            {
                FatalIOErrorIn("dictionary::read(Istream&, const word&)", is)
                    << "Cannot open included file " << fileStream.name()
                    << " while reading dictionary"
                    << exit(FatalIOError);
            }
        }
        else if (!hashedEntries_.insert(entryPtr->keyword(), entryPtr))
        {
            IOWarningIn("dictionary::read(Istream&, const word&)", is)
                << "could not add entry" << endl
                << "    " << *entryPtr
                << "    on line " << is.lineNumber()
                << " of dictionary " << name()
                << endl;

            if (found(entryPtr->keyword()))
            {
                Info<< "    entry already in dictionary : " << endl
                    << lookup(entryPtr->keyword()).info() << endl;
            }

            delete entryPtr;
        }
        else
        {
            append(entryPtr);
        }

        if (debug)
        {
            Info<< "dictionary::read(Istream&, const word&) : "
                << entryPtr->keyword()
                << endl;
        }

        if (&lastEntry != &word::null && entryPtr->keyword() == lastEntry)
        {
            break;
        }
    }

    if (is.bad())
    {
        Info<< "dictionary::read(Istream&, const word&) : "
            << "Istream not OK after reading dictionary " << name()
            << endl;

        return false;
    }

    return true;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

dictionary::dictionary(Istream& is, const word& lastEntry)
:
    name_(is.name())
{
    clear();
    hashedEntries_.clear();
    read(is, lastEntry);
}


autoPtr<dictionary> dictionary::New(Istream& is)
{
    return autoPtr<dictionary>(new dictionary(is));
}


// * * * * * * * * * * * * * * Istream Operator  * * * * * * * * * * * * * * //

Istream& operator>>(Istream& is, dictionary& dict)
{
    dict.clear();
    dict.hashedEntries_.clear();

    dict.read(is);
    return is;
}


// * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * * //

void dictionary::write(Ostream& os, bool subDict) const
{
    if (subDict)
    {
        os << nl << indent << token::BEGIN_BLOCK << incrIndent << nl;
    }

    for
    (
        IDLList<entry>::const_iterator iter = begin();
        iter != end();
        ++iter
    )
    {
        // Write entry & follow with carriage return.
        os << *iter;

        // Check stream before going to next entry.
        if (!os.good())
        {
            WarningIn("dictionary::write(Ostream& os, bool subDict)")
                << "Can't write entry " << (*iter).keyword()
                << " for dictionary " << name()
                << endl;
        }
    }

    if (subDict)
    {
        os << decrIndent << indent << token::END_BLOCK << endl;
    }
}


Ostream& operator<<(Ostream& os, const dictionary& dict)
{
    dict.write(os, true);
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
