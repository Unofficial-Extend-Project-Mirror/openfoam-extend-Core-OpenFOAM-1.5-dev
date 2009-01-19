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

#include "error.H"

#include "polygon2D.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// from Istream
polygon2D::polygon2D(Istream& is)
:
    vertices(is)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

polygon2D::~polygon2D()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void polygon2D::draw(Gstream& gs) const
{
    gs.draw(*this);
}


void polygon2D::write(Ostream& os) const
{
    os << *this;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void polygon2D::operator=(const polygon2D& a)
{
    if (vertices.size() != a.vertices.size())
    {
        FatalErrorIn("polygon2D::operator=(const polygon2D& a)")
            << "unequal polygons"
            << abort(FatalError);
    }

    for (label i=0; i< vertices.size(); i++)
    {
        vertices[i] = a.vertices[i];
    }
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Ostream& operator<<(Ostream& os, const polygon2D& p)
{
    os << p.vertices;

    // Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const polygon2D&)");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
