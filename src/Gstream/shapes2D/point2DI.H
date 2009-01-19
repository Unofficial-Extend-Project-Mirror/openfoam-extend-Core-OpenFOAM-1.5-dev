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
    point2D

Description


\*---------------------------------------------------------------------------*/

#include "error.H"

#include "point2D.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct as null
inline point2D::point2D()
:
    X(0.0),
    Y(0.0)
{}


// Construct from components
inline point2D::point2D(const scalar XX, const scalar YY)
:
    X(XX),
    Y(YY)
{}


// Construct from point (uses only x and y components)
inline point2D::point2D(const point& p)
:
    X(p.x()),
    Y(p.y())
{}


// Construct from Istream
inline point2D::point2D(Istream& is)
{
    //- Read beginning of point2D
    is.readBegin("point2D");

    is >> X >> Y;

    //- Read end of point2D
    is.readEnd("point2D");

    //- Check state of Istream
    is.check("point2D::point2D(Istream& is)");
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

inline point2D::~point2D()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline scalar point2D::x() const
{
    return X;
}


inline scalar point2D::y() const
{
    return Y;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void point2D::operator=(const point2D& a)
{
    X = a.X;
    Y = a.Y;
}

inline void point2D::operator*=(const scalar& val)
{
    X *= val;
    Y *= val;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline point2D operator+(const point2D& p1, const point2D& p2)
{
    return point2D(p1.X + p2.X, p1.Y + p2.Y);
}


inline point2D operator-(const point2D& p1, const point2D& p2)
{
    return point2D(p1.X - p2.X , p1.Y - p2.Y);
}


inline point2D operator*(const scalar& s, const point2D& p)
{
    return point2D(s*p.X, s*p.Y);
}


inline bool operator==(const point2D& p1, const point2D& p2)
{
    return (p1.X == p2.X && p1.Y == p2.Y);
}


inline bool operator!=(const point2D& p1, const point2D& p2)
{
    return !(p1 == p2);
}


inline scalar distance(const point2D& p1, const point2D& p2)
{
    return sqrt(sqr(p1.X-p2.X) + sqr(p1.Y-p2.Y));
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

inline Ostream& operator<<(Ostream& os, const point2D& p)
{
    os  << token::BEGIN_LIST
        << p.X
        << token::SPACE
        << p.Y
        << token::END_LIST;

    //- Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const point2D&)");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
