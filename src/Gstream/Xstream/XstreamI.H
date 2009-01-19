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
    Xstream

Description


\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline int Xstream::x(const point2D& p)
{
    return
    (
        int(scale().x()*p.x() + centre().x()*width().x())
    );
}


inline int Xstream::y(const point2D& p)
{
    return
    (
        width().y() - int(scale().y()*p.y() + centre().y()*width().y())
    );
}


inline double Xstream::x(const coord2D& c)
{
    return
    (
        double(c.x() - centre().x()*width().x())/scale().x()
    );
}


inline double Xstream::y(const coord2D& c)
{
    return
    (
        double(width().y() - c.y() - centre().y()*width().y())/scale().y()
    );
}


inline int Xstream::waitForEvent()
{
    GEvent dummy;

    int choice = waitForEvent(dummy);

    if (dummy.whatHappened() == GEvent::KPRESS)
    {
        return 0;
    }

    return choice;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
