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
    colour

Description


\*---------------------------------------------------------------------------*/

#include "colour.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct null
inline colour::colour()
:
    name_(""),
    r_(0),
    g_(0),
    b_(0)
{}


// Construct from components
inline colour::colour
(
    const word& name,
    const scalar r,
    const scalar g,
    const scalar b
)
:
    name_(name),
    r_(r),
    g_(g),
    b_(b)
{}


// Construct from Istream
inline colour::colour(Istream& is)
:
    name_(is)
{
    //- Read beginning of colour
    is.readBegin("colour");

    is >> r_ >> g_ >> b_;

    //- Read end of colour
    is.readEnd("colour");

    //- Check state of Istream
    is.check("colour::colour(Istream& is)");
}


// Construct as copy
inline colour::colour(const colour& c)
:
    name_(c.name_),
    r_(c.r_),
    g_(c.g_),
    b_(c.b_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

inline colour::~colour()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const word& colour::name() const
{
    return name_;
}

inline scalar colour::r() const
{
    return r_;
}

inline scalar colour::g() const
{
    return g_;
}

inline scalar colour::b() const
{
    return b_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void colour::operator=(const colour& c)
{
    name_ = c.name_;
    r_ = c.r_;
    g_ = c.g_;
    b_ = c.b_;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline bool operator==(const colour& c1, const colour& c2)
{
    return
    (
        c1.name_ == c2.name_
     && c1.r_ == c2.r_
     && c1.g_ == c2.g_
     && c1.b_ == c2.b_
    );
}


inline bool operator!=(const colour& c1, const colour& c2)
{
    return !(c1 == c2);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

inline Istream& operator>>(Istream& is, colour& c)
{
    is >> c.name_;

    //- Read beginning of colour
    is.readBegin("colour");

    is >> c.r_ >> c.g_ >> c.b_;

    //- Read end of colour
    is.readEnd("colour");

    //- Check state of Istream
    is.check("Istream& operator>>(Istream&, colour&)");

    return is;
}


inline Ostream& operator<<(Ostream& os, const colour& c)
{
    os  << c.name_
        << token::SPACE
        << token::BEGIN_LIST
        << c.r_
        << token::SPACE
        << c.g_
        << token::SPACE
        << c.b_
        << token::END_LIST;

    //- Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const colour&)");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
