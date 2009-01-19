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

SourceFiles
    polygon2DI.H
    polygon2D.C

\*---------------------------------------------------------------------------*/

#ifndef polygon2D_H
#define polygon2D_H

#include "shape2D.H"
#include "point2D.H"
#include "List.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Gstream;
class Istream;
class Ostream;

/*---------------------------------------------------------------------------*\
                           Class polygon2D Declaration
\*---------------------------------------------------------------------------*/

class polygon2D
:
    public virtual shape2D
{
    // Private data

        List<point2D> vertices;

public:

    // Constructors

        //- Construct from components
        inline polygon2D
        (
            const point2D&,
            const point2D&,
            const point2D&
        );


        //- Construct from Istream
        polygon2D(Istream&);


    // Destructor

        virtual ~polygon2D();


    // Member Functions

        //- Access
        inline const List<point2D>& verts() const;

        //- Draw
        virtual void draw(Gstream&) const;

        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        inline void operator=(const polygon2D&);


    // Friend Operators

        inline friend Gstream& operator<<(Gstream&, const polygon2D&);


    // IOstream Operators

        inline friend Ostream& operator<<(Ostream&, const polygon2D&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "polygon2DI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
