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
    structuredScalars2D

Description


\*---------------------------------------------------------------------------*/

#include "error.H"

#include "structuredScalars2D.H"
#include "Gstream.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// from components
structuredScalars2D::structuredScalars2D
(
    const point2D& o,
    const scalarListList& dat,
    const scalar& xskip,
    const scalar& yskip,
    const colourPalette& pal,
    label Jump
)
:
    Origin(o),
    data(dat),
    Xnum(data[0].size()),
    Ynum(data.size()),
    Xskip(xskip),
    Yskip(yskip),
    jump(Jump),
    palette(pal)
{
    for (label i=0; i<Ynum; i++)
    {
        for (label j=0; j<Xnum; j++)
        {
            if (data[i][j]>1.0 || data[i][j]<0.0)
            {
                FatalErrorIn
                (
                    "structuredScalars2D::structuredScalars2D"
                    "(const point2D& o, const scalarListList& dat, "
                    "const scalar& xskip, const scalar& yskip, "
                    "const colourPalette& pal, label Jump"
                )   << "incorrect range"
                    << abort(FatalError);
            }
        }
    }
}


// from Istream
structuredScalars2D::structuredScalars2D(Istream& is)
:
    Origin(is),
    data(is),
    Xnum(data[0].size()),
    Ynum(data.size()),
    Xskip(readScalar(is)),
    Yskip(readScalar(is)),
    jump(readLabel(is)),
    palette(is)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

structuredScalars2D::~structuredScalars2D()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void structuredScalars2D::draw(Gstream& gs) const
{
    gs.draw(*this);
}


void structuredScalars2D::write(Ostream& os) const
{
    os << *this;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

Gstream& operator<<(Gstream& gs, const structuredScalars2D& sS)
{
    gs.draw(sS);
    return gs;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //


Ostream& operator<<(Ostream& os, const structuredScalars2D& ss2D)
{
    os  << ss2D.Origin << endl
        << ss2D.data << endl
        << ss2D.Xskip << endl
        << ss2D.Yskip << endl
        << ss2D.jump << endl
        << ss2D.palette << endl;

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
