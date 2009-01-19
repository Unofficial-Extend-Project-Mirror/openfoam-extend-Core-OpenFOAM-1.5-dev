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
    lineStyle

Description

SourceFiles
    lineStyle.C

\*---------------------------------------------------------------------------*/

#ifndef lineStyle_H
#define lineStyle_H

#include "word.H"
#include "scalarList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Istream;
class Ostream;

/*---------------------------------------------------------------------------*\
                           Class lineStyle Declaration
\*---------------------------------------------------------------------------*/

class lineStyle
{
    // Private data

        word name_;
        scalar lineWidth_;
        scalar dashLength_;
        scalarList dashStyle_;

public:

    // Constructors

        //- Construct null
        lineStyle();

        //- Construct from components
        lineStyle
        (
            const word&,
            const scalar,
            const scalar,
            const scalarList&
        );

        //- Construct from Istream
        lineStyle(Istream&);

        //- Construct as copy
        lineStyle(const lineStyle&);


    // Destructor

        ~lineStyle();


    // Member Functions

        // Access

            inline scalar thickness() const;
            inline scalar patternLength() const;
            inline const scalarList& pattern() const;


    // Member Operators

        void operator=(const lineStyle&);


    // IOstream Operators

        friend Istream& operator>>(Istream&, lineStyle&);
        friend Ostream& operator<<(Ostream&, const lineStyle&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline scalar lineStyle::thickness() const
{
    return lineWidth_;
}

inline scalar lineStyle::patternLength() const
{
    return dashLength_;
}

inline const scalarList& lineStyle::pattern() const
{
    return dashStyle_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
