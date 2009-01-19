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
    line

Description
    A line primitive.

SourceFiles
    lineI.H

\*---------------------------------------------------------------------------*/

#ifndef line_H
#define line_H

#include "vector.H"
#include "pointHit.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * Forward declaration of template friend fuctions * * * * * * * //

template<class Point, class PointRef> class line;

class pointHit;
class Istream;
class Ostream;

template<class Point, class PointRef>
inline Istream& operator>>
(
    Istream&,
    line<Point, PointRef>&
);

template<class Point, class PointRef>
inline Ostream& operator<<
(
    Ostream&,
    const line<Point, PointRef>&
);

/*---------------------------------------------------------------------------*\
                           Class line Declaration
\*---------------------------------------------------------------------------*/

template<class Point, class PointRef>
class line
{
    // Private data

        PointRef a_, b_;


public:

    // Constructors

        //- Construct from two points
        inline line(const Point& start, const Point& end);

        //- Construct from Istream
        inline line(Istream&);


    // Member functions

        // Access

            //- Return first vertex
            inline PointRef start() const;

            //- Return second vertex
            inline PointRef end() const;


        // Properties

            //- Return centre (centroid)
            inline Point centre() const;

            //- Return scalar magnitude
            inline scalar mag() const;

            //- Return start-end vector
            inline vector vec() const;

            //- Return nearest distance to line from a given point
            //  If the nearest point is on the line, return a hit
            pointHit nearestDist(const point& p) const;

            //- Return nearest distance from line to line. Returns distance
            //  and sets both points (one on *this, one on the provided
            //  linePointRef
            scalar nearestDist
            (
                const line<point, const point&>& edge,
                point& thisPoint,
                point& edgePoint
            ) const;


    // Ostream operator

        friend Istream& operator>> <Point, PointRef>
        (
            Istream&,
            line&
        );

        friend Ostream& operator<< <Point, PointRef>
        (
            Ostream&,
            const line&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "lineI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
