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
    line2D

Description
    2D line.

\*---------------------------------------------------------------------------*/

#include "Gstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
inline line2D::line2D(const point2D& p1, const point2D& p2)
:
    sttpoint(p1),
    endpoint(p2)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const point2D& line2D::start() const
{
    return sttpoint;
}


inline const point2D& line2D::end() const
{
    return endpoint;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void line2D::operator=(const line2D& a)
{
    sttpoint = a.sttpoint;
    endpoint = a.endpoint;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline Gstream& operator<<(Gstream& xout, const line2D& l)
{
    xout.draw(l);
    return xout;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
