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

\*---------------------------------------------------------------------------*/

#include "boundaryPatch.H"
#include "dictionary.H"
#include "Ostream.H"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
Foam::boundaryPatch::boundaryPatch
(
    const word& name,
    const label index,
    const label size,
    const label start,
    const word& physicalType
)
:
    patchIdentifier(name, index, physicalType),
    size_(size),
    start_(start)
{}


// Construct as copy
Foam::boundaryPatch::boundaryPatch(const boundaryPatch& sp)
:
    patchIdentifier(sp.name(), sp.index(), sp.physicalType()),
    size_(sp.size()),
    start_(sp.start())
{}


// Construct from Istream
Foam::boundaryPatch::boundaryPatch(Istream& is, const label index)
:
    patchIdentifier(is, index),
    size_(readLabel(is)),
    start_(readLabel(is))
{}


// Construct from dictionary
Foam::boundaryPatch::boundaryPatch
(
    const word& name,
    const dictionary& dict,
    const label index
)
:
    patchIdentifier(name, dict, index),
    size_(readLabel(dict.lookup("size"))),
    start_(readLabel(dict.lookup("start")))
{}


// Clone
Foam::autoPtr<Foam::boundaryPatch> Foam::boundaryPatch::clone() const
{
    return autoPtr<boundaryPatch>(new boundaryPatch(*this));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::boundaryPatch::~boundaryPatch()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Write
void Foam::boundaryPatch::write(Ostream& os) const
{
    os  << nl;
    patchIdentifier::write(os);
    os  << nl;
    os << size_ << nl;
    os << start_ << nl;
}


void Foam::boundaryPatch::writeDict(Ostream& os) const
{
    patchIdentifier::writeDict(os);

    os << "    size " << size_ << token::END_STATEMENT << nl;

    os << "    start " << start_ << token::END_STATEMENT << nl;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

Foam::Ostream& Foam::operator<<(Ostream& os, const boundaryPatch& sp)
{
    sp.write(os);
    os.check("Ostream& operator<<(Ostream& f, const boundaryPatch&)");
    return os;
}


// ************************************************************************* //
