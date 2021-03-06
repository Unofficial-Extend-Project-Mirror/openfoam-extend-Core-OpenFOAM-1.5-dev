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

\*---------------------------------------------------------------------------*/

#include "nearWallDistNoSearch.H"
#include "fvMesh.H"
#include "wallPoint.H"
#include "wallFvPatch.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::nearWallDistNoSearch::doAll()
{
    const volVectorField& cellCentres = mesh_.C();
    const fvPatchList& patches = mesh_.boundary();

    forAll(patches, patchI)
    {
        fvPatchScalarField& ypatch = operator[](patchI);

        if (patches[patchI].type() == wallFvPatch::typeName)
        {
            const unallocLabelList& faceCells = patches[patchI].faceCells();

            const fvPatchVectorField& patchCentres
                = cellCentres.boundaryField()[patchI];

            const fvsPatchVectorField& Apatch
                = mesh_.Sf().boundaryField()[patchI];

            const fvsPatchScalarField& magApatch
                = mesh_.magSf().boundaryField()[patchI];

            forAll(patchCentres, facei)
            {
                ypatch[facei] =
                (
                    Apatch[facei] &
                    (
                        patchCentres[facei]
                      - cellCentres[faceCells[facei]]
                    )
                )/magApatch[facei];
            }
        }
        else
        {
            ypatch = 0.0;
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::nearWallDistNoSearch::nearWallDistNoSearch(const Foam::fvMesh& mesh)
:
    volScalarField::GeometricBoundaryField
    (
        mesh.boundary(),
        mesh.V(),           // Dummy internal field
        calculatedFvPatchScalarField::typeName
    ),
    mesh_(mesh)
{
    doAll();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::nearWallDistNoSearch::~nearWallDistNoSearch()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::nearWallDistNoSearch::correct()
{
    if (mesh_.changing())
    {
        // Update size of GeometricBoundaryField
        forAll(mesh_.boundary(), patchI)
        {
            operator[](patchI).setSize(mesh_.boundary()[patchI].size());
        }
    }

    doAll();
}


// ************************************************************************* //
