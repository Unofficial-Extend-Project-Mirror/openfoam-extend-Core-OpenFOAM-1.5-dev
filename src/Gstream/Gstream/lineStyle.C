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
    lineStyle

Description

\*---------------------------------------------------------------------------*/

#include "error.H"

#include "lineStyle.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct null
lineStyle::lineStyle()
:
    name_("Continuous"),
    lineWidth_(1.0),
    dashLength_(1.0),
    dashStyle_(1)
{
    dashStyle_[0] = 1.0;
}

// Construct from components
lineStyle::lineStyle
(
    const word& nm,
    const scalar lw,
    const scalar dl,
    const scalarList& ds
)
:
    name_(nm),
    lineWidth_(lw),
    dashLength_(dl),
    dashStyle_(ds.size())
{
    forAll(dashStyle_, j)
    {
        dashStyle_[j] = ds[j];
    }
}


// Construct from Istream
lineStyle::lineStyle(Istream& is)
:
    name_(is)
{
    //- Read beginning of colour
    is.readBegin("lineStyle");

    is >> lineWidth_ >> dashLength_ >> dashStyle_;

    //- Read end of colour
    is.readEnd("lineStyle");

    //- Check state of Istream
    is.check("lineStyle::lineStyle(Istream& is)");
}


// Construct as copy
lineStyle::lineStyle(const lineStyle& ls)
:
    name_(ls.name_),
    lineWidth_(ls.lineWidth_),
    dashLength_(ls.dashLength_),
    dashStyle_(ls.dashStyle_.size())
{
    forAll(dashStyle_, j)
    {
        dashStyle_[j] = ls.dashStyle_[j];
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

lineStyle::~lineStyle()
{}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void lineStyle::operator=(const lineStyle& ls)
{
    name_ = ls.name_;
    lineWidth_ = ls.lineWidth_;
    dashLength_ = ls.dashLength_;

    forAll(dashStyle_, j)
    {
        dashStyle_[j] = ls.dashStyle_[j];
    }
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Istream& operator>>(Istream& is, lineStyle& ls)
{
    is >> ls.name_;

    //- Read beginning of lineStyle
    is.readBegin("lineStyle");

    is >> ls.lineWidth_ >> ls.dashLength_ >> ls.dashStyle_;

    //- Read end of lineStyle
    is.readEnd("lineStyle");

    //- Check state of Istream
    is.check("Istream& operator>>(Istream&, lineStyle&)");

    return is;
}


Ostream& operator<<(Ostream& os, const lineStyle& ls)
{
    os  << ls.name_
        << token::SPACE
        << token::BEGIN_LIST
        << ls.lineWidth_
        << token::SPACE
        << ls.dashLength_
        << token::SPACE
        << ls.dashStyle_
        << token::END_LIST;

    //- Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const lineStyle&)");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
