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

SourceFiles
    structuredVectors2DI.H
    structuredVectors2D.C

\*---------------------------------------------------------------------------*/

#ifndef structuredVectors2D_H
#define structuredVectors2D_H

#include "List.H"
#include "point2D.H"
#include "shape2D.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Gstream;
class Istream;
class Ostream;

/*---------------------------------------------------------------------------*\
                           Class structuredVectors2D Declaration
\*---------------------------------------------------------------------------*/

class structuredVectors2D
:
    public virtual shape2D
{

    // Private data

        point2D startat;

        scalar Xskip;
        scalar Yskip;

        label Xnum;
        label Ynum;
        label Xjump;
        label Yjump;

        scalar vectorScale;

        List< List <point2D> >& vects;

    // Private Member Functions

        //- Disallow default bitwise copy construct and assignment
        structuredVectors2D(const structuredVectors2D&);
        void operator=(const structuredVectors2D&);

public:

    // Constructors

        //- Construct from components
        structuredVectors2D
        (
            const point2D&,
            List< List <point2D> >&,
            const scalar&,
            const scalar&,
            label = 1,
            scalar = 1.0
        );


    // Destructor

        ~structuredVectors2D();


    // Member Functions

        // Access

            const point2D& origin() const;

            label xnum() const;
            label ynum() const;

            scalar xskip() const;
            scalar yskip() const;

            scalar vscale() const;

            const List<List<point2D> >& Vects() const;


        //- Draw
        virtual void draw(Gstream&) const;

        //- Write
        virtual void write(Ostream&) const;


    // Friend Operators

        friend Gstream& operator<<(Gstream&, const structuredVectors2D&);


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const structuredVectors2D&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "structuredVectors2DI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
