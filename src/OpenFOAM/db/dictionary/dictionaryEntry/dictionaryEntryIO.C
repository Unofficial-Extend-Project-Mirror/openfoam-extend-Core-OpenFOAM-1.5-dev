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
    DictionaryEntry constructor from Istream and Ostream output operator.

\*---------------------------------------------------------------------------*/

#include "dictionaryEntry.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

dictionaryEntry::dictionaryEntry(Istream& is)
:
    entry(is),
    dictionary(is)
{
    is.fatalCheck
    (
        "dictionaryEntry::dictionaryEntry(Istream& is)"
    );
}


dictionaryEntry::dictionaryEntry(const word& key, Istream& is)
:
    entry(key),
    dictionary(is)
{
    name() += "::" + key;

    is.fatalCheck
    (
        "dictionaryEntry::dictionaryEntry(const word& keyword, Istream& is)"
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void dictionaryEntry::write(Ostream& os) const
{
    os.writeKeyword(keyword());
    dictionary::write(os);
}


// * * * * * * * * * * * * * Ostream operator  * * * * * * * * * * * * * * * //

Ostream& operator<<(Ostream& os, const dictionaryEntry& de)
{
    de.write(os);
    return os;
}


#if defined (__GNUC__)
template<>
#endif
Ostream& operator<<(Ostream& os, const InfoProxy<dictionaryEntry>& ip)
{
    const dictionaryEntry& e = ip.t_;

    os  << "    dictionaryEntry '" << e.keyword() << "'" << endl;

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
