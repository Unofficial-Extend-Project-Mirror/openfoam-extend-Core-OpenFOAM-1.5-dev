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
    Foam::uniform

Description
    uniform pdf

SourceFiles
    uniformI.H
    uniform.C
    uniformIO.C

\*---------------------------------------------------------------------------*/

#ifndef uniform_H
#define uniform_H

#include "pdf.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class uniform Declaration
\*---------------------------------------------------------------------------*/

class uniform
:
    public pdf
{
    // Private data

        dictionary pdfDict_;
        //- min and max values of the distribution
        scalar minValue_;
        scalar maxValue_;

        // help-variable to avoid always having to calculate max-min
        scalar range_;


public:

    //- Runtime type information
        TypeName("uniform");


    // Constructors

        //- Construct from components
        uniform
        (
            const dictionary& dict,
            Random& rndGen
        );


    // Destructor

        ~uniform();


    // Member Functions

    scalar sample() const;

    scalar minValue() const;
    scalar maxValue() const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//#include "uniformI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
