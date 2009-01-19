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

#include "entry.H"
#include "OStringStream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from keyword
entry::entry(const word& keyword)
:
    keyword_(keyword)
{}


// Construct as copy
entry::entry(const entry& e)
:
    IDLList<entry>::link(),
    keyword_(e.keyword_)
{}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void entry::operator=(const entry& e)
{
    // check for assignment to self
    if (this == &e)
    {
        FatalErrorIn("entry::operator=(const entry&)")
            << "attempted assignment to self"
            << abort(FatalError);
    }

    keyword_ = e.keyword_;
}


bool entry::operator==(const entry& e) const
{
    if (keyword_ != e.keyword_)
    {
        return false;
    }
    else
    {
        OStringStream oss1;
        oss1 << *this;

        OStringStream oss2;
        oss2 << e;

        return oss1.str() == oss2.str();
    }
}


bool entry::operator!=(const entry& e) const
{
    return !operator==(e);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
