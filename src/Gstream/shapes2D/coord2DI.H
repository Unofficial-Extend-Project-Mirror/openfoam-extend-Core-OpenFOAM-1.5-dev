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
    coord2D

Description


\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
inline coord2D::coord2D(const label XX, const label YY)
:
    X(XX),
    Y(YY)
{}


// Construct as copy
inline coord2D::coord2D(const coord2D& c)
:
    X(c.X),
    Y(c.Y)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

inline coord2D::~coord2D()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline label coord2D::x() const
{
    return X;
}


inline label coord2D::y() const
{
    return Y;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void coord2D::operator=(const coord2D& a)
{
    X = a.X;
    Y = a.Y;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline coord2D operator+(const coord2D& p1, const coord2D& p2)
{
    return coord2D(p1.X + p2.X, p1.Y + p2.Y);
}


inline coord2D operator-(const coord2D& p1, const coord2D& p2)
{
    return coord2D(p1.X - p2.X, p1.Y - p2.Y);
}


inline coord2D operator/(const coord2D& p, const label& div)
{
    return coord2D(label(p.x()/div), label(p.y()/div));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
