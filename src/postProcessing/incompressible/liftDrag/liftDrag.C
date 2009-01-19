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

Description

\*---------------------------------------------------------------------------*/

#include "liftDrag.H"

#include "fvMesh.H"
#include "volFields.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::scalar Foam::liftDrag::dragCoefficient
(
    const volVectorField& U,
    const volScalarField& p,
    const dimensionedScalar& nu,
    const word& patchName,
    const vector& Uinf,
    const scalar& Aref
)
{
    if (mag(Uinf) < VSMALL)
    {
        FatalErrorIn
        (
            "scalar dragCoefficient\n"
            "(\n"
            "    const volVectorField& U,\n"
            "    const volScalarField& p,\n"
            "    const dimensionedScalar& nu,\n"
            "    const word& patchName,\n"
            "    const vector& Uinf,\n"
            "    const scalar& Aref\n"
            ")\n"
        )   << "Uinf is zero."
            << exit(FatalError);
    }

    vector flowDirection = Uinf/mag(Uinf);

    const fvMesh& mesh = p.mesh();

    label patchLabel = -1;

    forAll (mesh.boundary(), patchi)
    {
        if (mesh.boundary()[patchi].name() == patchName)
        {
            patchLabel = patchi;
            break;
        }
    }

    vector pressureForce;
    vector viscousForce;

    if (patchLabel != -1)
    {
        pressureForce = gSum
        (
            p.boundaryField()[patchLabel]
            *mesh.Sf().boundaryField()[patchLabel]
        );

        viscousForce = gSum
        (
          - nu.value()*U.boundaryField()[patchLabel].snGrad()*
            mesh.magSf().boundaryField()[patchLabel]
        );
    }
    else
    {
        pressureForce = vector::zero;
        viscousForce = vector::zero;
    }

    scalar qRef = 0.5*magSqr(Uinf);
    scalar Fref = qRef*Aref;

    vector pressureCoeff = pressureForce/Fref;
    vector viscousCoeff = viscousForce/Fref;


    return ((pressureCoeff + viscousCoeff) & flowDirection);
}


Foam::scalar Foam::liftDrag::turbDragCoefficient
(
    const turbulenceModel& turbulence,
    const volVectorField& U,
    const volScalarField& p,
    const dimensionedScalar& nu,
    const word& patchName,
    const vector& Uinf,
    const scalar& Aref
)
{
    if (mag(Uinf) < VSMALL)
    {
        FatalErrorIn
        (
            "scalar turbDragCoefficient\n"
            "(\n"
            "    const turbulenceModel& turbulence,\n"
            "    const volVectorField& U,\n"
            "    const volScalarField& p,\n"
            "    const dimensionedScalar& nu,\n"
            "    const word& patchName,\n"
            "    const vector& Uinf,\n"
            "    const scalar& Aref\n"
            ")\n"
        )   << "Uinf is zero."
            << exit(FatalError);
    }

    vector flowDirection = Uinf/mag(Uinf);

    const fvMesh& mesh = p.mesh();

    label patchLabel = -1;

    forAll (mesh.boundary(), patchi)
    {
        if (mesh.boundary()[patchi].name() == patchName)
        {
            patchLabel = patchi;
            break;
        }
    }

    vector pressureForce;
    vector viscousForce;
    vector turbForce;

    volScalarField nut = turbulence.nut();

    if (patchLabel != -1)
    {
        pressureForce = gSum
        (
            p.boundaryField()[patchLabel]
            *mesh.Sf().boundaryField()[patchLabel]
        );

        viscousForce = gSum
        (
          - nu.value()*U.boundaryField()[patchLabel].snGrad()*
            mesh.magSf().boundaryField()[patchLabel]
        );

        turbForce = gSum
        (
            -nut.boundaryField()[patchLabel]*
            U.boundaryField()[patchLabel].snGrad()*
            mesh.magSf().boundaryField()[patchLabel]
        );
    }
    else
    {
        pressureForce = vector::zero;
        viscousForce = vector::zero;
        turbForce = vector::zero;
    }

    scalar qRef = 0.5*magSqr(Uinf);
    scalar Fref = qRef*Aref;

    vector pressureCoeff = pressureForce/Fref;
    vector viscousCoeff = viscousForce/Fref;
    vector turbCoeff = turbForce/Fref;


    return ((pressureCoeff + viscousCoeff + turbCoeff) & flowDirection);
}


Foam::scalar Foam::liftDrag::lesDragCoefficient
(
    const LESmodel& sgsModel,
    const volVectorField& U,
    const volScalarField& p,
    const dimensionedScalar& nu,
    const word& patchName,
    const vector& Uinf,
    const scalar& Aref
)
{
    if (mag(Uinf) < VSMALL)
    {
        FatalErrorIn
        (
            "scalar lesDragCoefficient\n"
            "(\n"
            "    const LESmodel& sgsModel,\n"
            "    const volVectorField& U,\n"
            "    const volScalarField& p,\n"
            "    const dimensionedScalar& nu,\n"
            "    const word& patchName,\n"
            "    const vector& Uinf,\n"
            "    const scalar& Aref\n"
            ")\n"
        )   << "Uinf is zero."
            << exit(FatalError);
    }

    vector flowDirection = Uinf/mag(Uinf);

    const fvMesh& mesh = p.mesh();

    label patchLabel = -1;

    forAll (mesh.boundary(), patchi)
    {
        if (mesh.boundary()[patchi].name() == patchName)
        {
            patchLabel = patchi;
            break;
        }
    }

    vector pressureForce;
    vector viscousForce;
    vector turbForce;

    if (patchLabel != -1)
    {
        pressureForce = gSum
        (
            p.boundaryField()[patchLabel]
            *mesh.Sf().boundaryField()[patchLabel]
        );

        viscousForce = gSum
        (
          - nu.value()*U.boundaryField()[patchLabel].snGrad()*
            mesh.magSf().boundaryField()[patchLabel]
        );

        turbForce = gSum
        (
          - mesh.Sf().boundaryField()[patchLabel]
          & sgsModel.B()().boundaryField()[patchLabel]
        );
    }
    else
    {
        pressureForce = vector::zero;
        viscousForce = vector::zero;
        turbForce = vector::zero;
    }

    scalar qRef = 0.5*magSqr(Uinf);
    scalar Fref = qRef*Aref;

    vector pressureCoeff = pressureForce/Fref;
    vector viscousCoeff = viscousForce/Fref;
    vector turbCoeff = turbForce/Fref;


    return ((pressureCoeff + viscousCoeff + turbCoeff) & flowDirection);
}


Foam::vector Foam::liftDrag::liftCoefficient
(
    const volVectorField& U,
    const volScalarField& p,
    const dimensionedScalar& nu,
    const word& patchName,
    const vector& Uinf,
    const scalar& Aref
)
{
    if (mag(Uinf) < VSMALL)
    {
        FatalErrorIn
        (
            "vector liftCoefficient\n"
            "(\n"
            "    const volVectorField& U,\n"
            "    const volScalarField& p,\n"
            "    const dimensionedScalar& nu,\n"
            "    const word& patchName,\n"
            "    const vector& Uinf,\n"
            "    const scalar& Aref\n"
            ")\n"
        )   << "Uinf is zero."
            << exit(FatalError);
    }

    vector flowDirection = Uinf/mag(Uinf);

    const fvMesh& mesh = p.mesh();

    label patchLabel = -1;

    forAll (mesh.boundary(), patchi)
    {
        if (mesh.boundary()[patchi].name() == patchName)
        {
            patchLabel = patchi;
            break;
        }
    }

    vector pressureForce;
    vector viscousForce;

    if (patchLabel != -1)
    {
        pressureForce = gSum
        (
            p.boundaryField()[patchLabel]
            *mesh.Sf().boundaryField()[patchLabel]
        );

        viscousForce = gSum
        (
            -nu.value()*U.boundaryField()[patchLabel].snGrad()
            *mesh.magSf().boundaryField()[patchLabel]
        );
    }
    else
    {
        pressureForce = vector::zero;
        viscousForce = vector::zero;
    }

    scalar qRef = 0.5*magSqr(Uinf);
    scalar Fref = qRef*Aref;

    vector pressureCoeff = pressureForce/Fref;
    vector viscousCoeff = viscousForce/Fref;

    return
        (pressureCoeff + viscousCoeff)
      - flowDirection*(flowDirection & (pressureCoeff + viscousCoeff));
}


Foam::vector Foam::liftDrag::turbLiftCoefficient
(
    const turbulenceModel& turbulence,
    const volVectorField& U,
    const volScalarField& p,
    const dimensionedScalar& nu,
    const word& patchName,
    const vector& Uinf,
    const scalar& Aref
)
{
    if (mag(Uinf) < VSMALL)
    {
        FatalErrorIn
        (
            "vector turbLiftCoefficient\n"
            "(\n"
            "    const turbulenceModel& turbulence,\n"
            "    const volVectorField& U,\n"
            "    const volScalarField& p,\n"
            "    const dimensionedScalar& nu,\n"
            "    const word& patchName,\n"
            "    const vector& Uinf,\n"
            "    const scalar& Aref\n"
            ")\n"
        )   << "Uinf is zero."
            << exit(FatalError);
    }

    vector flowDirection = Uinf/mag(Uinf);

    const fvMesh& mesh = p.mesh();

    label patchLabel = -1;

    forAll (mesh.boundary(), patchi)
    {
        if (mesh.boundary()[patchi].name() == patchName)
        {
            patchLabel = patchi;
            break;
        }
    }

    vector pressureForce;
    vector viscousForce;
    vector turbForce;

    volScalarField nut = turbulence.nut();

    if (patchLabel != -1)
    {
        pressureForce = gSum
        (
            p.boundaryField()[patchLabel]
            *mesh.Sf().boundaryField()[patchLabel]
        );

        viscousForce = gSum
        (
          - nu.value()*U.boundaryField()[patchLabel].snGrad()*
            mesh.magSf().boundaryField()[patchLabel]
        );

        turbForce = gSum
        (
            -nut.boundaryField()[patchLabel]*
            U.boundaryField()[patchLabel].snGrad()*
            mesh.magSf().boundaryField()[patchLabel]
        );
    }
    else
    {
        pressureForce = vector::zero;
        viscousForce = vector::zero;
        turbForce = vector::zero;
    }

    scalar qRef = 0.5*magSqr(Uinf);
    scalar Fref = qRef*Aref;

    vector pressureCoeff = pressureForce/Fref;
    vector viscousCoeff = viscousForce/Fref;
    vector turbCoeff = turbForce/Fref;

    return
        (pressureCoeff + viscousCoeff + turbCoeff)
      - flowDirection
      * (flowDirection & (pressureCoeff + viscousCoeff + turbForce));
}


Foam::vector Foam::liftDrag::lesLiftCoefficient
(
    const LESmodel& sgsModel,
    const volVectorField& U,
    const volScalarField& p,
    const dimensionedScalar& nu,
    const word& patchName,
    const vector& Uinf,
    const scalar& Aref
)
{
    if (mag(Uinf) < VSMALL)
    {
        FatalErrorIn
        (
            "vector lesLiftCoefficient\n"
            "(\n"
            "    const LESmodel& sgsModel,\n"
            "    const volVectorField& U,\n"
            "    const volScalarField& p,\n"
            "    const dimensionedScalar& nu,\n"
            "    const word& patchName,\n"
            "    const vector& Uinf,\n"
            "    const scalar& Aref\n"
            ")\n"
        )   << "Uinf is zero."
            << exit(FatalError);
    }

    vector flowDirection = Uinf/mag(Uinf);

    const fvMesh& mesh = p.mesh();

    label patchLabel = -1;

    forAll (mesh.boundary(), patchi)
    {
        if (mesh.boundary()[patchi].name() == patchName)
        {
            patchLabel = patchi;
            break;
        }
    }

    vector pressureForce;
    vector viscousForce;
    vector turbForce;

    if (patchLabel != -1)
    {
        pressureForce = gSum
        (
            p.boundaryField()[patchLabel]
            *mesh.Sf().boundaryField()[patchLabel]
        );

        viscousForce = gSum
        (
          - nu.value()*U.boundaryField()[patchLabel].snGrad()*
            mesh.magSf().boundaryField()[patchLabel]
        );

        turbForce = gSum
        (
          - mesh.Sf().boundaryField()[patchLabel]
          & sgsModel.B()().boundaryField()[patchLabel]
        );
    }
    else
    {
        pressureForce = vector::zero;
        viscousForce = vector::zero;
        turbForce = vector::zero;
    }

    scalar qRef = 0.5*magSqr(Uinf);
    scalar Fref = qRef*Aref;

    vector pressureCoeff = pressureForce/Fref;
    vector viscousCoeff = viscousForce/Fref;
    vector turbCoeff = turbForce/Fref;

    return
        (pressureCoeff + viscousCoeff + turbCoeff)
      - flowDirection
      * (flowDirection & (pressureCoeff + viscousCoeff + turbForce));
}


Foam::vector Foam::liftDrag::momentCoefficient
(
    const volVectorField& U,
    const volScalarField& p,
    const dimensionedScalar& nu,
    const word& patchName,
    const vector& Uinf,
    const scalar& Aref,
    const scalar& Lref
)
{
    if (mag(Uinf) < VSMALL)
    {
        FatalErrorIn
        (
            "vector momentCoefficient\n"
            "(\n"
            "    const volVectorField& U,\n"
            "    const volScalarField& p,\n"
            "    const dimensionedScalar& nu,\n"
            "    const word& patchName,\n"
            "    const vector& Uinf,\n"
            "    const scalar& Aref,\n"
            "    const scalar& Lref\n"
            ")\n"
        )   << "Uinf is zero."
            << exit(FatalError);
    }

    const fvMesh& mesh = p.mesh();

    label patchLabel = -1;

    forAll (mesh.boundary(), patchi)
    {
        if (mesh.boundary()[patchi].name() == patchName)
        {
            patchLabel = patchi;
            break;
        }
    }

    vector pressureForceMoment;
    vector viscousForceMoment;

    if (patchLabel != -1)
    {
        pressureForceMoment = gSum
        (
            mesh.Cf().boundaryField()[patchLabel]^
            (p.boundaryField()[patchLabel]
            *mesh.Sf().boundaryField()[patchLabel])
        );

        viscousForceMoment = gSum
        (
            mesh.Cf().boundaryField()[patchLabel] ^
            (-nu.value()*U.boundaryField()[patchLabel].snGrad()
            *mesh.magSf().boundaryField()[patchLabel])
        );
    }
    else
    {
        pressureForceMoment = vector::zero;
        viscousForceMoment = vector::zero;
    }

    scalar qRef = 0.5*magSqr(Uinf);
    scalar Fref = qRef*Aref;
    scalar Mref = Fref*Lref;

    vector pressureCoeff = pressureForceMoment/Mref;
    vector viscousCoeff = viscousForceMoment/Mref;

    return (pressureCoeff + viscousCoeff);
}


// ************************************************************************* //
