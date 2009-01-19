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
    string2D

Description
    2D string.

SourceFiles
    string2DI.H
    string2D.C

\*---------------------------------------------------------------------------*/

#ifndef string2D_H
#define string2D_H

#include "shape2D.H"
#include "string.H"
#include "point2D.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Gstream;
class Istream;
class Ostream;


/*---------------------------------------------------------------------------*\
                           Class string2D Declaration
\*---------------------------------------------------------------------------*/

class string2D
:
    public virtual shape2D
{

    // Private data

        point2D start;
        string characters;
        point2D direction;

        bool Horizontal;

public:

    // Constructors

        //- Construct from components
        inline string2D(const point2D&, const string&);
        inline string2D(const point2D&, const string&, const point2D&);

        //- Construct from Istream
        string2D(Istream&);


    // Destructor

        virtual ~string2D();


    // Member Functions

        // Access

            inline bool horizontal() const;
            inline scalar x() const;
            inline scalar y() const;
            inline const string& text() const;
            inline const point2D& startPt() const;


        //- Draw
        virtual void draw(Gstream&) const;

        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        inline void operator=(const string2D&);


    // Friend Operators

        inline friend Gstream& operator<<(Gstream&, const string2D&);


    // IOstream Operators

        inline friend Ostream& operator<<(Ostream&, const string2D&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "string2DI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
