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
    arrow2D

Description
    2D arrow.

SourceFiles
    arrow2DI.H
    arrow2D.C

\*---------------------------------------------------------------------------*/

#ifndef arrow2D_H
#define arrow2D_H

#include "shape2D.H"
#include "point2D.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Gstream;
class Istream;
class Ostream;


/*---------------------------------------------------------------------------*\
                           Class arrow2D Declaration
\*---------------------------------------------------------------------------*/

class arrow2D
:
    public virtual shape2D
{

    // Private data

        point2D tail;
        point2D head;
        point2D head1;
        point2D head2;

    // Private Member Functions

        void createHead(const scalar);

public:

    // Constructors

        //- Construct from components
        arrow2D
        (
            const point2D,
            const point2D,
            const scalar = 1.0,
            const scalar = 0.1
        );

        //- Construct from Istream
        arrow2D(Istream&);


    // Destructor

        virtual ~arrow2D();


    // Member Functions

        // Access

        const point2D& Head() const;
        const point2D& Tail() const;
        const point2D& Head1() const;
        const point2D& Head2() const;

        //- Draw
        virtual void draw(Gstream&) const;

        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        void operator=(const arrow2D&);


    // Friend Operators

        friend Gstream& operator<<(Gstream&, const arrow2D&);


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const arrow2D&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "arrow2DI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
