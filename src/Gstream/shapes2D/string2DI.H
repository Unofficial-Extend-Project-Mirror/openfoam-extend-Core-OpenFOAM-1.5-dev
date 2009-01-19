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
    string2D

Description
    2D string.

\*---------------------------------------------------------------------------*/

#include "Gstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
inline string2D::string2D(const point2D& s, const string& c)
:
    start(s),
    characters(c),
    direction(1.0, 0.0),
    Horizontal(true)
{}


inline string2D::string2D(const point2D& s, const string& c, const point2D& d)
:
    start(s),
    characters(c),
    direction(d),
    Horizontal(false)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool string2D::horizontal() const
{
    return Horizontal;
}

inline scalar string2D::x() const
{
    return start.x();
}

inline scalar string2D::y() const
{
    return start.y();
}

inline const string& string2D::text() const
{
    return characters;
}

inline const point2D& string2D::startPt() const
{
    return start;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void string2D::operator=(const string2D& a)
{
    characters = a.characters;
    start = a.start;
    direction = a.direction;
    Horizontal = a.Horizontal;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline Gstream& operator<<(Gstream& gs, const string2D& s2D)
{
    gs.draw(s2D);
    return gs;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
