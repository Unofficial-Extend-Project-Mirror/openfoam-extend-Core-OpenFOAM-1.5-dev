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
    liftDrag

Description
    Calculates the lift and drag for each time. 

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "liftDrag.H"
#include "wallFvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

#   include "addTimeOptions.H"
#   include "setRootCase.H"

#   include "createTime.H"

    // Get times list
    instantList Times = runTime.times();

    // set startTime and endTime depending on -time and -latestTime options
#   include "checkTimeOptions.H"

    runTime.setTime(Times[startTime], startTime);

#   include "createMesh.H"

    const fvPatchList& patches = mesh.boundary();

#   include "createNu.H"

    for (label i=startTime; i<endTime; i++)
    {
        runTime.setTime(Times[i], i);

        Info<< "Time = " << runTime.timeName() << endl;

        IOobject UHeader
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        );

        IOobject pHeader
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        );

        // Check U exists
        if (UHeader.headerOk() && pHeader.headerOk())
        {
            mesh.readUpdate();

            Info<< "    Reading U\n" << endl;
            volVectorField U(UHeader, mesh);

            Info<< "    Reading p\n" << endl;
            volScalarField p(pHeader, mesh);

            vector Uav = vector::zero;

            forAll(U.boundaryField(), patchI)
            {
                if (U.boundaryField()[patchI].fixesValue())
                {
                    Uav += average(U.boundaryField()[patchI]);
                }
            }

            Info << "Inlet velocity: " << Uav << endl;

            forAll (patches, patchI)
            {
                if (isType<wallFvPatch>(patches[patchI]))
                {
                    // Reference area
                    scalar Aref = gSum
                    (
                        (Uav & mesh.Sf().boundaryField()[patchI])*
                        pos(Uav & mesh.Sf().boundaryField()[patchI])
                    );

                    // Reference length
                    boundBox patchBounds
                    (
                        mesh.boundaryMesh()[patchI].localPoints()
                    );

                    scalar Lref =
                        mag(Uav & (patchBounds.max() - patchBounds.min()))
                        /(mag(Uav) + VSMALL);

                    Info<< "Wall patch " << patchI
                        << " named " << patches[patchI].name() << " : " << nl
                        << "Reference area: " << Aref
                        << " Reference length: " << Lref
                        << " Drag coefficient: "
                        << liftDrag::dragCoefficient
                           (
                               U,
                               p,
                               nu,
                               patches[patchI].name(),
                               Uav,
                               Aref
                           )
                        << " Lift coefficient: "
                        << liftDrag::liftCoefficient
                           (
                               U,
                               p,
                               nu,
                               patches[patchI].name(),
                               Uav,
                               Aref
                           )
                        << " Moment coefficient: "
                        << liftDrag::momentCoefficient
                           (
                               U,
                               p,
                               nu,
                               patches[patchI].name(),
                               Uav,
                               Aref,
                               Lref
                           )
                        
                        << endl;
                }
            }
        }
        else
        {
            Info<< "    No p" << endl;
        }

        Info<< endl;
    }

    return(0);
}


// ************************************************************************* //
