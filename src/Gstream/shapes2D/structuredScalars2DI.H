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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const point2D& structuredScalars2D::origin() const
{
    return Origin;
}

inline label structuredScalars2D::xnum() const
{
    return Xnum;
}

inline label structuredScalars2D::ynum() const
{
    return Ynum;
}

inline scalar structuredScalars2D::xskip() const
{
    return Xskip;
}

inline scalar structuredScalars2D::yskip() const
{
    return Yskip;
}

inline label structuredScalars2D::xjump() const
{
    return jump;
}

inline label structuredScalars2D::yjump() const
{
    return jump;
}

inline const scalarListList& structuredScalars2D::Data() const
{
    return data;
}

inline const scalarList& structuredScalars2D::DataRow(const label& i) const
{
    return data[i];
}

inline colour structuredScalars2D::newColour(const scalar& val) const
{
    label thisval = label(val*scalar(palette.size()-1));
    return palette[thisval];
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
