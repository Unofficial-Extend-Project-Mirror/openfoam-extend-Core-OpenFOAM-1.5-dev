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

#include "displacementLaplacianFvMotionSolver.H"
#include "motionDiffusivity.H"
#include "fvmLaplacian.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(displacementLaplacianFvMotionSolver, 0);

    addToRunTimeSelectionTable
    (
        fvMotionSolver,
        displacementLaplacianFvMotionSolver,
        dictionary
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::displacementLaplacianFvMotionSolver::displacementLaplacianFvMotionSolver
(
    const polyMesh& mesh,
    Istream& msData
)
:
    fvMotionSolver(mesh),
    points0_
    (
        pointIOField
        (
            IOobject
            (
                "points",
                time().constant(),
                polyMesh::meshSubDir,
                mesh,
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            )
        )
    ),
    pointDisplacement_
    (
        IOobject
        (
            "pointDisplacement",
            fvMesh_.time().timeName(),
            fvMesh_,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        pointMesh_
    ),
    cellDisplacement_
    (
        IOobject
        (
            "cellDisplacement",
            mesh.time().timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fvMesh_,
        dimensionedVector
        (
            "cellDisplacement",
            pointDisplacement_.dimensions(),
            vector::zero
        ),
        cellMotionBoundaryTypes<vector>(pointDisplacement_.boundaryField())
    ),
    diffusivityPtr_
    (
        motionDiffusivity::New(*this, lookup("diffusivity"))
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::displacementLaplacianFvMotionSolver::
~displacementLaplacianFvMotionSolver()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::tmp<Foam::pointField> 
Foam::displacementLaplacianFvMotionSolver::curPoints() const
{
    // The points have moved so before interpolation update 
    // volPointInterpolation accordingly
    vpi_.movePoints();

    vpi_.interpolate(cellDisplacement_, pointDisplacement_);

    tmp<pointField> tcurPoints
    (
        points0_ + pointDisplacement_.internalField()
    );

    twoDCorrectPoints(tcurPoints());

    return tcurPoints;
}


void Foam::displacementLaplacianFvMotionSolver::solve()
{
    diffusivityPtr_->correct();
    pointDisplacement_.boundaryField().updateCoeffs();

    Foam::solve
    (
        fvm::laplacian
        (
            diffusivityPtr_->operator()(),
            cellDisplacement_,
            "laplacian(diffusivity,cellDisplacement)"
        )
    );
}


// ************************************************************************* //
