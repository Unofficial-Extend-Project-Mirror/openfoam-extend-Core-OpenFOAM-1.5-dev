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

SourceFiles
    rectangle2DI.H
    rectangle2D.C

\*---------------------------------------------------------------------------*/

#ifndef rectangle2D_H
#define rectangle2D_H

#include "shape2D.H"
#include "point2D.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Gstream;
class Istream;
class Ostream;

/*---------------------------------------------------------------------------*\
                           Class rectangle2D Declaration
\*---------------------------------------------------------------------------*/

class rectangle2D
:
    public virtual shape2D
{
    // Private data

        point2D fstcorner;
        point2D sndcorner;


public:

    // Constructors

        //- Construct from components
        rectangle2D(const point2D, const point2D);

        //- Construct from Istream
        rectangle2D(Istream&);


    // Destructor

        virtual ~rectangle2D();


    // Member Functions

        // Access

            const point2D& first() const;
            const point2D& second() const;


        //- Draw
        virtual void draw(Gstream&) const;

        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        inline void operator=(const rectangle2D&);


    // Friend Operators

        inline friend Gstream& operator<<(Gstream&, const rectangle2D&);


    // IOstream Operators

        inline friend Ostream& operator<<(Ostream&, const rectangle2D&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "rectangle2DI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
