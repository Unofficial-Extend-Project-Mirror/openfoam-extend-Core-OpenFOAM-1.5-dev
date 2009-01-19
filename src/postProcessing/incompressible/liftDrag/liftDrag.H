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

Global
    liftDrag

Description
    Calculates the lift and drag coefficients over a patch given by a name
    from the given velocity and pressure field.

SourceFiles
    liftDrag.C

\*---------------------------------------------------------------------------*/

#ifndef liftDrag_H
#define liftDrag_H

#include "label.H"
#include "volFieldsFwd.H"
#include "dimensionedTypes.H"
#include "incompressible/transportModel/transportModel.H"
#include "incompressible/turbulenceModel/turbulenceModel.H"
#include "incompressible/LESmodel/LESmodel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace liftDrag
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

scalar dragCoefficient
(
    const volVectorField& U,
    const volScalarField& p,
    const dimensionedScalar& nu,
    const word& patchName,
    const vector& Uinf,
    const scalar& Aref
);
    
scalar turbDragCoefficient
(
    const turbulenceModel& turbulence,
    const volVectorField& U,
    const volScalarField& p,
    const dimensionedScalar& nu,
    const word& patchName,
    const vector& Uinf,
    const scalar& Aref
);
    
scalar lesDragCoefficient
(
    const LESmodel& sgsModel,
    const volVectorField& U,
    const volScalarField& p,
    const dimensionedScalar& nu,
    const word& patchName,
    const vector& Uinf,
    const scalar& Aref
);

vector liftCoefficient
(
    const volVectorField& U,
    const volScalarField& p,
    const dimensionedScalar& nu,
    const word& patchName,
    const vector& Uinf,
    const scalar& Aref
);

vector turbLiftCoefficient
(
    const turbulenceModel& turbulence,
    const volVectorField& U,
    const volScalarField& p,
    const dimensionedScalar& nu,
    const word& patchName,
    const vector& Uinf,
    const scalar& Aref
);

vector lesLiftCoefficient
(
    const LESmodel& sgsModel,
    const volVectorField& U,
    const volScalarField& p,
    const dimensionedScalar& nu,
    const word& patchName,
    const vector& Uinf,
    const scalar& Aref
);

vector momentCoefficient
(
    const volVectorField& U,
    const volScalarField& p,
    const dimensionedScalar& nu,
    const word& patchName,
    const vector& Uinf,
    const scalar& Aref,
    const scalar& Lref
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace liftDrag

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
