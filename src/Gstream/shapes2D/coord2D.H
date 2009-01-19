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
    Coordinate in 2D.

SourceFiles
    coord2DI.H

\*---------------------------------------------------------------------------*/

#ifndef coord2D_H
#define coord2D_H

#include "label.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Gstream;


/*---------------------------------------------------------------------------*\
                           Class coord2D Declaration
\*---------------------------------------------------------------------------*/

class coord2D
{
    // Private data

    label X;
    label Y;

public:

    // Constructors

        //- Construct from components
        coord2D(const label, const label);

        //- Construct as copy
        coord2D(const coord2D&);


    // Destructor

        ~coord2D();


    // Member Functions

        // Access

            label x() const;
            label y() const;


    // Member Operators

        void operator=(const coord2D&);


    // Friend Operators

        //- addition + subtraction
        inline friend coord2D operator+(const coord2D&, const coord2D&);
        inline friend coord2D operator-(const coord2D&, const coord2D&);

        //- modulo division
        inline friend coord2D operator/(const coord2D&, const int&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// include inline implementaions

#include "coord2DI.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
