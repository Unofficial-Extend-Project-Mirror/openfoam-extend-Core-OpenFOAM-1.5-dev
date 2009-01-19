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
    rectangle2D

Description
    2D rectangle.

\*---------------------------------------------------------------------------*/

#include "Gstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
inline rectangle2D::rectangle2D(const point2D p1, const point2D p2)
:
    fstcorner(p1),
    sndcorner(p2)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const point2D& rectangle2D::first() const
{
    return fstcorner;
}


inline const point2D& rectangle2D::second() const
{
    return sndcorner;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void rectangle2D::operator=(const rectangle2D& a)
{
    fstcorner = a.fstcorner;
    sndcorner = a.sndcorner;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline Gstream& operator<<(Gstream& gs, const rectangle2D& r)
{
    gs.draw(r);
    return gs;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
