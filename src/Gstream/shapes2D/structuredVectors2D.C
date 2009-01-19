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
    structuredVectors2D

Description


\*---------------------------------------------------------------------------*/

#include "error.H"

#include "structuredVectors2D.H"
#include "arrow2D.H"
#include "Gstream.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
structuredVectors2D::structuredVectors2D
(
    const point2D& StartAt,
    List<List <point2D> >& dat,
    const scalar& xskip,
    const scalar& yskip,
    label Jump,
    scalar vscale
)
:
    startat(StartAt),
    Xskip(xskip),
    Yskip(yskip),
    Xnum(dat[0].size()),
    Ynum(dat.size()),
    Xjump(Jump),
    Yjump(Xjump),
    vectorScale(vscale),
    vects(dat)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

structuredVectors2D::~structuredVectors2D()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void structuredVectors2D::draw(Gstream& gs) const
{
    scalar ycoord(startat.y());

    for (label i=0; i<Ynum; i+= Yjump)
    {
        scalar xcoord(startat.x());
        for (label j=0; j<Xnum; j+= Xjump)
        {
            arrow2D arrow
            (
                point2D(xcoord,ycoord),
                vects[i][j],
                vectorScale
            );
            arrow.draw(gs);

            xcoord += Xskip*Xjump;
        }
        ycoord += Yskip*Yjump;
    }
}


void structuredVectors2D::write(Ostream& os) const
{
    os << *this;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

Gstream& operator<<(Gstream& gs, const structuredVectors2D& a)
{
    a.draw(gs);
    return gs;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Ostream& operator<<(Ostream& os, const structuredVectors2D&)
{
    notImplemented("Ostream& operator<<(Ostream& os, const structuredVectors2D&)");
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
