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

Application
    flowType

Description
    Calculates and writes the flowType of velocity field U.
    The -noWrite option has no meaning.

    The flow type parameter is obtained according to the following equation:
    @verbatim
                 |D| - |Omega|
        lambda = -------------
                 |D| + |Omega|

        -1 = rotational flow
         0 = simple shear flow
         1 = planar extensional flow
    @endverbatim

\*---------------------------------------------------------------------------*/

#include "calc.H"
#include "fvc.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::calc(const argList& args, const Time& runTime, const fvMesh& mesh)
{
    IOobject Uheader
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
    );

    if (Uheader.headerOk())
    {
        Info<< "    Reading U" << endl;
        volVectorField U(Uheader, mesh);

        volTensorField gradU = fvc::grad(U);
        volScalarField magD = mag(symm(gradU));
        volScalarField magOmega = mag(skew(gradU));
        dimensionedScalar smallMagD("smallMagD", magD.dimensions(), SMALL);

        Info<< "    Calculating flowType" << endl;

        volScalarField flowType
        (
            IOobject
            (
                "flowType",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ
            ),
            (magD - magOmega)/(magD + magOmega + smallMagD)
        );

        flowType.write();
    }
    else
    {
        Info<< "    No U" << endl;
    }
}


// ************************************************************************* //
