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
    arrow2D

Description
    2D arrow.

\*---------------------------------------------------------------------------*/

#include "Gstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline void arrow2D::createHead(const scalar headratio)
{
    scalar headwidth(0.5);
    point2D back(headratio*(tail-head));

    point2D norm(-headwidth*back.y(), headwidth*back.x());

    head1 = head + back + norm;
    head2 = head + back - norm;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
inline arrow2D::arrow2D
(
    const point2D t,
    const point2D h,
    const scalar lengthratio,
    const scalar headratio
)
:
    tail(t),
    head(t+lengthratio*h),
    head1(0.0,0.0),
    head2(0.0,0.0)
{
    createHead(headratio);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const point2D& arrow2D::Head() const
{
    return head;
}

inline const point2D& arrow2D::Tail() const
{
    return tail;
}

inline const point2D& arrow2D::Head1() const
{
    return head1;
}

inline const point2D& arrow2D::Head2() const
{
    return head2;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void arrow2D::operator=(const arrow2D& a)
{
    tail = a.tail;
    head = a.head;
    head1 = a.head1;
    head2 = a.head2;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline Gstream& operator<<(Gstream& gs, arrow2D& a)
{
    a.draw(gs);
    return gs;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
