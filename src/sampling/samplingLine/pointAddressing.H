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
    pointAddressing

Description
    This class contains the nearest cell label and the difference vector
    for an arbitrary point

SourceFiles
    pointAddressing.C

\*---------------------------------------------------------------------------*/

#ifndef pointAddressing_H
#define pointAddressing_H

#include "label.H"
#include "vector.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class pointAddressing Declaration
\*---------------------------------------------------------------------------*/

class pointAddressing
{
    // Private data

        label nearestCell_;

        vector diffVector_;


public:

    // Constructors

        //- Construct from components
        pointAddressing(label, const vector&);

        //- Construct from Istream
        pointAddressing(Istream&);

        //- Clone
        autoPtr<pointAddressing> clone() const;


    // Member Functions

        label nearestCell() const
        {
            return nearestCell_;
        }

        vector diffVector() const
        {
            return diffVector_;
        }


    // Ostream Operator

        friend Ostream& operator<<(Ostream&, const pointAddressing&);
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
