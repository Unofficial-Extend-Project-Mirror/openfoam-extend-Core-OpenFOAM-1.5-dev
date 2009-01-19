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

#include "inverseDistanceDiffusivity.H"
#include "addToRunTimeSelectionTable.H"
#include "patchWave.H"
#include "labelHashSet.H"
#include "surfaceInterpolate.H"
#include "zeroGradientFvPatchFields.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(inverseDistanceDiffusivity, 0);

    addToRunTimeSelectionTable
    (
        motionDiffusivity,
        inverseDistanceDiffusivity,
        Istream
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::inverseDistanceDiffusivity::inverseDistanceDiffusivity
(
    const fvMotionSolver& mSolver,
    Istream& mdData
)
:
    uniformDiffusivity(mSolver, mdData),
    patchNames_(mdData)
{
    correct();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::inverseDistanceDiffusivity::~inverseDistanceDiffusivity()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::tmp<Foam::scalarField> Foam::inverseDistanceDiffusivity::y() const
{
    const polyMesh& mesh = mSolver().mesh();
    const polyBoundaryMesh& bdry = mesh.boundaryMesh();

    wordList allPatchNames = bdry.names();

    labelHashSet patchSet(bdry.size());

    forAll(patchNames_, i)
    {
        // Treat the given patch names as wild-cards and search the set
        // of all patch names for matches
        labelList patchIDs = findStrings(patchNames_[i], allPatchNames);

        if (patchIDs.size() == 0)
        {
            WarningIn("inverseDistanceDiffusivity::y()")
                << "Cannot find any patch names matching " << patchNames_[i]
                << endl;
        }

        forAll(patchIDs, j)
        {
            patchSet.insert(patchIDs[j]);
        }
    }

    if (patchSet.size() > 0)
    {
        return tmp<scalarField>
        (
            new scalarField(patchWave(mesh, patchSet, false).distance())
        );
    }
    else
    {
        return tmp<scalarField>(new scalarField(mesh.nCells(), 1.0));
    }
}


void Foam::inverseDistanceDiffusivity::correct()
{
    const fvMesh& mesh = mSolver().mesh();

    volScalarField y_
    (
        IOobject
        (
            "y",
            mesh.time().timeName(),
            mesh
        ),
        mesh,
        dimless,
        zeroGradientFvPatchScalarField::typeName
    );
    y_.internalField() = y();
    y_.correctBoundaryConditions();

    faceDiffusivity_ = 1.0/fvc::interpolate(y_);
}


// ************************************************************************* //
