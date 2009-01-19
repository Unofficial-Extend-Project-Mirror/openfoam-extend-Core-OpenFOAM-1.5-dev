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
    colour

Description


SourceFiles
    colourI.H

\*---------------------------------------------------------------------------*/

#ifndef colour_H
#define colour_H

#include "scalar.H"
#include "word.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Istream;
class Ostream;

/*---------------------------------------------------------------------------*\
                           Class colour Declaration
\*---------------------------------------------------------------------------*/

class colour
{
    // Private data

        word name_;
        scalar r_, g_, b_;

public:

    // Constructors

        //- Construct null
        inline colour();

        //- Construct from components
        inline colour
        (
            const word& nam,
            const scalar r,
            const scalar g,
            const scalar b
        );

        //- Construct from Istream
        inline colour(Istream&);

        //- Construct as copy
        inline colour(const colour&);


    // Destructor

        inline ~colour();


    // Member Functions

        // Access

            inline const word& name() const;
            inline scalar r() const;
            inline scalar g() const;
            inline scalar b() const;


    // Member Operators

        inline void operator=(const colour&);


    // Friend Operators

        inline friend bool operator==(const colour&, const colour&);
        inline friend bool operator!=(const colour&, const colour&);


    // IOstream Operators

        inline friend Istream& operator>>(Istream&, colour&);
        inline friend Ostream& operator<<(Ostream&, const colour&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// include inline implementaions

#include "colourI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
