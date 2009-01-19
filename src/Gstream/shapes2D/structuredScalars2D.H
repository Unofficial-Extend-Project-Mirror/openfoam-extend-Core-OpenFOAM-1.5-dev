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


SourceFiles
    structuredScalars2DI.H
    structuredScalars2D.C

\*---------------------------------------------------------------------------*/

#ifndef structuredScalars2D_H
#define structuredScalars2D_H

#include "label.H"
#include "scalarList.H"
#include "shape2D.H"
#include "point2D.H"
#include "colourPalette.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Gstream;
class Istream;
class Ostream;

/*---------------------------------------------------------------------------*\
                           Class structuredScalars2D Declaration
\*---------------------------------------------------------------------------*/

class structuredScalars2D
:
    public virtual shape2D
{

    // Private data

        point2D Origin;

        scalarListList data;
        label Xnum;
        label Ynum;

        scalar Xskip;
        scalar Yskip;
        label jump;

        colourPalette palette;


    // Private Member Functions

        //- Disallow default bitwise copy construct and assignment
        structuredScalars2D(const structuredScalars2D&);
        void operator=(const structuredScalars2D&);


public:

    // Constructors

        //- Construct from components
        structuredScalars2D
        (
            const point2D&,
            const scalarListList&,
            const scalar&,
            const scalar&,
            const colourPalette&,
            label = 1
        );

        //- Construct from Istream
        structuredScalars2D(Istream&);


    // Destructor

        ~structuredScalars2D();


    // Member Functions

        // Access

            const point2D& origin() const;

            label xnum() const;
            label ynum() const;

            scalar xskip() const;
            scalar yskip() const;

            label xjump() const;
            label yjump() const;

            const scalarListList& Data() const;
            const scalarList& DataRow(const label&) const;

            colour newColour(const scalar&) const;


        //- Draw
        virtual void draw(Gstream&) const;

        //- Write
        virtual void write(Ostream&) const;


    // Friend Operators

        friend Gstream& operator<<(Gstream&, const structuredScalars2D&);


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const structuredScalars2D&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "structuredScalars2DI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
