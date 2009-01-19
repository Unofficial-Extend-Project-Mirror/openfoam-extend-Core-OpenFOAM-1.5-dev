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

#include "patchIdentifier.H"
#include "dictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
patchIdentifier::patchIdentifier
(
    const word& name,
    const label index,
    const word& physicalType
)
:
    name_(name),
    boundaryIndex_(index),
    physicalType_(physicalType)
{}


// Construct from Istream
patchIdentifier::patchIdentifier(Istream& is, const label index)
:
    name_(is),
    boundaryIndex_(index)
{}


// Construct from dictionary
patchIdentifier::patchIdentifier
(
    const word& name,
    const dictionary& dict,
    const label index
)
:
    name_(name),
    boundaryIndex_(index)
{
    if (dict.found("physicalType"))
    {
        dict.lookup("physicalType") >> physicalType_;
    }
}


// Construct from geometric patch, resetting the index
patchIdentifier::patchIdentifier
(
    const patchIdentifier& p,
    const label index
)
:
    name_(p.name_),
    boundaryIndex_(index),
    physicalType_(p.physicalType_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

patchIdentifier::~patchIdentifier()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Write
void patchIdentifier::write(Ostream& os) const
{
    os  << nl << name_;

    if (physicalType_.size())
    {
        os  << nl << physicalType_;
    }
}


void patchIdentifier::writeDict(Ostream& os) const
{
    if (physicalType_.size())
    {
        os  << "    physicalType " << physicalType_ << token::END_STATEMENT
            << nl;
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

Ostream& operator<<(Ostream& os, const patchIdentifier& gp)
{
    gp.write(os);
    os.check("Ostream& operator<<(Ostream& f, const patchIdentifier& gp)");
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
