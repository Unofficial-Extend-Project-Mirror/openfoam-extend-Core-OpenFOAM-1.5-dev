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
    point2D

Description
    2D point.

SourceFiles
    point2DI.H

\*---------------------------------------------------------------------------*/

#ifndef point2D_H
#define point2D_H

#include "error.H"
#include "point.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Istream;
class Ostream;

/*---------------------------------------------------------------------------*\
                           Class point2D Declaration
\*---------------------------------------------------------------------------*/

class point2D
{
    // Private data

        scalar X;
        scalar Y;

public:

    // Constructors

        //- Construct as null
        point2D();

        //- Construct from components
        point2D(const scalar, const scalar);

        //- Construct from point (uses only x and y components)
        point2D(const point&);

        //- Construct from Istream
        point2D(Istream&);


    // Destructor

        ~point2D();


    // Member Functions

        // Access

            scalar x() const;
            scalar y() const;


    // Member Operators

        void operator=(const point2D&);

        void operator*=(const scalar&);


    // Friend Operators

        //- addition
        inline friend point2D operator+(const point2D&, const point2D&);

        //- subtraction
        inline friend point2D operator-(const point2D&, const point2D&);

        //- multiplication by a scalar
        inline friend point2D operator*(const scalar&, const point2D&);

        inline friend bool operator==(const point2D&, const point2D&);
        inline friend bool operator!=(const point2D&, const point2D&);

        //- distance between point2Ds
        inline friend scalar distance(const point2D&, const point2D&);


    // Ostream Operator

        inline friend Ostream& operator<<(Ostream&, const point2D&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- For the benefit of old compilers instantiating  ListIO.C
inline Istream& operator>>(Istream& is, point2D&)
{
    notImplemented("Istream& operator<<(Istream&, point2D&)");
    return is;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "point2DI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
