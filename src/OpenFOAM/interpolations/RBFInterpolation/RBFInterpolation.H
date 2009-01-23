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
    RBFInterpolation

Description
    Radial basis function interpolation class

Author
    Frank Bos, TU Delft.  All rights reserved.

SourceFiles
    RBFInterpolationI.H
    RBFInterpolation.C
    RBFInterpolationIO.C

\*---------------------------------------------------------------------------*/

#ifndef RBFInterpolation_H
#define RBFInterpolation_H

#include "dictionary.H"
#include "RBFFunction.H"
#include "simpleMatrix.H"
#include "polyMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes

/*---------------------------------------------------------------------------*\
                        Class RBFInterpolation Declaration
\*---------------------------------------------------------------------------*/

class RBFInterpolation
{
    // Private data

        //- Mesh reference
        const polyMesh& mesh_;

        //- Dictionary
        const dictionary& dict_;

        //- Reference to control points
        const vectorField& controlPoints_;

        //- Rerefence to all points
        const vectorField& allPoints_;

        //- RBF function
        autoPtr<RBFFunction> RBF_;

        //- Interpolation matrix
        mutable scalarMatrix* BPtr_;

        //- Inner cut-off radius
        scalar innerRadius_;

        //- Outer cut-off radius
        scalar outerRadius_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        RBFInterpolation(const RBFInterpolation&);

        //- Disallow default bitwise assignment
        void operator=(const RBFInterpolation&);


        //- Return interpolation matrix
        const scalarMatrix& B() const;

        //- Calculate interpolation matrix
        void calcB() const;

        //- Clear out
        void clearOut();


public:

    // Constructors

        //- Construct from components
        RBFInterpolation
        (
            const polyMesh& mesh,
            const dictionary& dict,
            const vectorField& controlPoints,
            const vectorField& allPoints
        );


    // Destructor

        ~RBFInterpolation();


    // Member Functions

        //- Interpolate
        template<class Type>
        tmp<Field<Type> > interpolate(const Field<Type>& ctrlField) const;

        //- Move points
        void movePoints();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "RBFInterpolationTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
