/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anispulation  |
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
    renumberMesh

Description
    Renumbers the cell list in order to reduce the bandwidth, reading and
    renumbering all fields from all the time directories.

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "IOobjectList.H"
#include "fvMeshBandCompression.H"

using namespace Foam;

// Renumbering GeometricField
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template<class GeometricField>
void RenumberFields
(
    const fvMeshBandCompression& mesh,
    const fvMesh& newMesh,
    const IOobjectList& objects
)
{
    // Search list of objects for volScalarFields
    IOobjectList fields(objects.lookupClass(GeometricField::typeName));

    for
    (
        IOobjectList::iterator fieldIter = fields.begin();
        fieldIter != fields.end();
        ++fieldIter
    )
    {
        Info<< "    renumbering " << fieldIter()->name()
            << endl;

        // Read field
        GeometricField theta(*fieldIter(), mesh);

        // Renumber field
        GeometricField thetaCpy
        (
            IOobject
            (
                fieldIter()->name(),
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE,
                false
            ),
            mesh.renumber(newMesh, theta)
        );

        // Write field
        thetaCpy.write();
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::noParallel();
#   include "addTimeOptions.H"

#   include "setRootCase.H"
#   include "createTime.H"

    // Get times list
    instantList Times = runTime.times();

    // set startTime and endTime depending on -time and -latestTime options
#   include "checkTimeOptions.H"

    runTime.setTime(Times[startTime], startTime);

    fvMeshBandCompression mesh
    (
        IOobject
        (
            fvMeshBandCompression::defaultRegion,
            runTime.timeName(),
            runTime
        )
    );

    const unallocLabelList& oldFownerC = mesh.faceOwner();
    const unallocLabelList& oldFneighbourC = mesh.faceNeighbour();

    label band = 0;

    forAll(oldFneighbourC, faceI)
    {
        label diff = oldFneighbourC[faceI] - oldFownerC[faceI];

        if (diff > band)
        {
            band = diff;
        }
    }

    Info << "Mesh size: " << mesh.nCells()
        << "    band before renumbering: " << band << endl;

    // create a new mesh
    fvMesh* newMeshPtr = mesh.renumberedMesh();
    fvMesh& newMesh = *newMeshPtr;

    newMesh.objectRegistry::write();

    // check the new bandwidth
    band = 0;
    const unallocLabelList& newFownerC = newMesh.owner();
    const unallocLabelList& newFneighbourC = newMesh.neighbour();

    forAll(newFownerC, faceI)
    {
        label diff = newFneighbourC[faceI] - newFownerC[faceI];

        if (diff > band)
        {
            band = diff;
        }
    }

    Info << "        Band after renumbering: " << band << endl;

    for (label i=startTime; i<endTime; i++)
    {
        runTime.setTime(Times[i], i);

        Info<< "Time = " << runTime.timeName() << endl;

        // Search for list of objects for this time
        IOobjectList objects(mesh, runTime.timeName());

        RenumberFields<volScalarField>(mesh, newMesh, objects);
        RenumberFields<volVectorField>(mesh, newMesh, objects);
        RenumberFields<volSphericalTensorField>(mesh, newMesh, objects);
        RenumberFields<volSymmTensorField>(mesh, newMesh, objects);
        RenumberFields<volTensorField>(mesh, newMesh, objects);
    }

    Info<< "\nEnd.\n" << endl;

    return 0;
}


// ************************************************************************* //
