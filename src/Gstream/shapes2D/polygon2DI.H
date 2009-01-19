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
    polygon2D

Description
    General 2D polygon.

\*---------------------------------------------------------------------------*/

#include "Gstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
inline polygon2D::polygon2D
(
    const point2D& v0,
    const point2D& v1,
    const point2D& v2
)
:
    vertices(3)
{
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

inline const List<point2D>& polygon2D::verts() const
{
    return vertices;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline Gstream& operator<<(Gstream& gs, const polygon2D& p)
{
    gs.draw(p);
    return gs;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
