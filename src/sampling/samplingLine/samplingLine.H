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
    samplingLine

Description
    This class samples the given volScalarField along a line specified
    with its start, end and the number of points

SourceFiles
    samplingLine.C
    pointAdressing.C

\*---------------------------------------------------------------------------*/

#ifndef samplingLine_H
#define samplingLine_H

#include "point.H"
#include "pointAddressing.H"
#include "primitiveFields.H"
#include "SLList.H"
#include "volFieldsFwd.H"
#include "volMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class samplingLine Declaration
\*---------------------------------------------------------------------------*/

class samplingLine
{
    // Private data

        const fvMesh& mesh_;

        point start_, end_;
        label nPoints_;

        vectorField points_;
        scalarField parametricX_;
        SLList<pointAddressing> addressingData_;


    // Private Member Functions

        //- Calculate addressing
        void calcAddressing();

        //- Checks if the point is inside the cell
        bool checkPoint(const point&, label, const vectorField&) const;


    // Dissallow default bitwise operator= and copy construct

        samplingLine(const samplingLine&);
        samplingLine& operator=(const samplingLine&);


public:

    // Constructors

        //- Construct from components
        samplingLine
        (
            const fvMesh& mesh,
            const point& start,
            const point& end,
            const label noPoints = 50
        );


        //- Construct from mesh and Istream
        samplingLine
        (
            const fvMesh& mesh,
            Istream&
        );


    // Member Functions

        // Access

            const vectorField& points() const
            {
                return points_;
            }

            const scalarField& parametricX() const
            {
                return parametricX_;
            }


        // Interpolate

            //- Uses the nearest cell value as the interpolate
            scalarField simpleInterpolate(const volScalarField& f) const;

            //- Calculates the cell gradient of f and calls next function
            scalarField interpolate(const volScalarField& f) const;

            //- Uses the cell gradient to perform a second order interpolate
            scalarField interpolate
            (
                const volScalarField& f,
                const volVectorField& gradf
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
