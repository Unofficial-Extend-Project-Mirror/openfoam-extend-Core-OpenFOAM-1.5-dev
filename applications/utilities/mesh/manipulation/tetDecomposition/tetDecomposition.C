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
    Takes a mesh and decomposes it into tetrahedra using a face-cell centre 
    decomposition.

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "Time.H"
#include "polyMesh.H"
#include "tetPolyMesh.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{

#   include "setRootCase.H"
#   include "createTime.H"

    polyMesh mesh
    (
        IOobject
        (
            polyMesh::defaultRegion,
            runTime.timeName(),
            runTime
        )
    );

    tetPolyMesh getTets(mesh);

    runTime++;

    Info << "Creating tet mesh. Number of tets: " << getTets.nTets() << endl;

    polyMesh newMesh
    (
        IOobject
        (
            polyMesh::defaultRegion,
            runTime.timeName(),
            runTime
        ),
        getTets.points(),
        getTets.tetCells(),
        getTets.boundary().boundaryTriFaces(),
        mesh.boundaryMesh().names(),
        mesh.boundaryMesh().types(),
        polyPatch::typeName,
        mesh.boundaryMesh().physicalTypes()
    );

    Info << "Writing polyMesh" << endl;
    newMesh.write();

    Info << "End\n" << endl;

    return 0;
}


// ************************************************************************* //
