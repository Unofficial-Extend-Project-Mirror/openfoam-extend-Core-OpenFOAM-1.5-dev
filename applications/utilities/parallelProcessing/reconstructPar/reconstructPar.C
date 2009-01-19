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
    reconstructPar

Description
    Reconstructs a mesh and fields of a case that is decomposed for parallel
    execution of FOAM.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "IOobjectList.H"
#include "processorMeshes.H"
#include "fvFieldReconstructor.H"
#include "pointFieldReconstructor.H"
#include "tetPointFieldReconstructor.H"
#include "reconstructLagrangian.H"

#include "faCFD.H"
#include "faMesh.H"
#include "processorFaMeshes.H"
#include "faFieldReconstructor.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::noParallel();
#   include "addTimeOptions.H"
#   include "setRootCase.H"
#   include "createTime.H"

    Info<< "Create mesh\n" << endl;
    fvMesh mesh
    (
        IOobject
        (
            fvMesh::defaultRegion,
            runTime.timeName(),
            runTime
        )
    );

    IOdictionary decompositionDict
    (
        IOobject
        (
            "decomposeParDict",
            runTime.system(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    int nProcs(readInt(decompositionDict.lookup("numberOfSubdomains")));


    // Create the processor databases
    PtrList<Time> databases(nProcs);

    forAll (databases, procI)
    {
        databases.set
        (
            procI,
            new Time
            (
                Time::controlDictName,
                args.rootPath(),
                args.caseName()/fileName(word("processor") + name(procI))
            )
        );
    }


    // Get times list from first database
    const instantList Times = databases[0].times();

#   include "checkTimeOptions.H"

    // Set all times (on reconstructed mesh and on processor meshes)
    runTime.setTime(Times[startTime], startTime);
    mesh.readUpdate();

    forAll (databases, procI)
    {
        databases[procI].setTime(Times[startTime], startTime);
    }

    // Read all meshes and addressing to reconstructed mesh
    processorMeshes procMeshes(databases);

    // Foam version 2.1 changes the addressing of faces in faceProcAddressing
    // The following code checks and modifies the addressing for cases where
    // the decomposition has been done with the foam2.0 and earlier tools, but
    // the reconstruction is attempted with version 2.1 or later
    // 
#   include "checkFaceAddressingComp.H"

    // Loop over all times
    for (label timei = startTime; timei < endTime; timei++)
    {
        // Set time for global database
        runTime.setTime(Times[timei], timei);

        Info << "Time = " << runTime.timeName() << endl << endl;

        // Set time for all databases

        forAll (databases, procI)
        {
            databases[procI].setTime(Times[timei], timei);
        }

        // Check if any new meshes need to be read.
        fvMesh::readUpdateState meshStat = mesh.readUpdate();

        fvMesh::readUpdateState procStat = procMeshes.readUpdate();

        if (procStat == fvMesh::POINTS_MOVED)
        {
            // Reconstruct the points for moving mesh cases and write them out
            procMeshes.reconstructPoints(mesh);
        }
        else if (meshStat != procStat)
        {
            WarningIn(args.executable())
                << "readUpdate for the reconstructed mesh:" << meshStat << nl
                << "readUpdate for the processor meshes  :" << procStat << nl
                << "These should be equal or your addressing"
                << " might be incorrect."
                << " Please check your time directories for any "
                << "mesh directories." << endl;
        }


        // Get list of objects from processor0 database
        IOobjectList objects(procMeshes.meshes()[0], databases[0].timeName());


        // If there are any FV fields, reconstruct them

        if
        (
            objects.lookupClass(volScalarField::typeName).size()
         || objects.lookupClass(volVectorField::typeName).size()
         || objects.lookupClass(volSphericalTensorField::typeName).size()
         || objects.lookupClass(volSymmTensorField::typeName).size()
         || objects.lookupClass(volTensorField::typeName).size()
         || objects.lookupClass(surfaceScalarField::typeName).size()
        )
        {
            Info << "Reconstructing FV fields" << nl << endl;

            fvFieldReconstructor fvReconstructor
            (
                mesh,
                procMeshes.meshes(),
                procMeshes.faceProcAddressing(),
                procMeshes.cellProcAddressing(),
                procMeshes.boundaryProcAddressing()
            );

            fvReconstructor.reconstructFvVolumeFields<scalar>(objects);
            fvReconstructor.reconstructFvVolumeFields<vector>(objects);
            fvReconstructor.reconstructFvVolumeFields<sphericalTensor>(objects);
            fvReconstructor.reconstructFvVolumeFields<symmTensor>(objects);
            fvReconstructor.reconstructFvVolumeFields<tensor>(objects);

            fvReconstructor.reconstructFvSurfaceFields<scalar>(objects);
        }
        else
        {
            Info << "No FV fields" << nl << endl;
        }


        // If there are any point fields, reconstruct them
        if
        (
            objects.lookupClass(pointScalarField::typeName).size()
         || objects.lookupClass(pointVectorField::typeName).size()
         || objects.lookupClass(pointSphericalTensorField::typeName).size()
         || objects.lookupClass(pointSymmTensorField::typeName).size()
         || objects.lookupClass(pointTensorField::typeName).size()
        )
        {
            Info << "Reconstructing point fields" << nl << endl;

            pointMesh pMesh(mesh);
            PtrList<pointMesh> pMeshes(procMeshes.meshes().size());

            forAll (pMeshes, procI)
            {
                pMeshes.set(procI, new pointMesh(procMeshes.meshes()[procI]));
            }

            pointFieldReconstructor pointReconstructor
            (
                pMesh,
                pMeshes,
                procMeshes.pointProcAddressing(),
                procMeshes.boundaryProcAddressing()
            );

            pointReconstructor.reconstructFields<scalar>(objects);
            pointReconstructor.reconstructFields<vector>(objects);
            pointReconstructor.reconstructFields<sphericalTensor>(objects);
            pointReconstructor.reconstructFields<symmTensor>(objects);
            pointReconstructor.reconstructFields<tensor>(objects);
        }
        else
        {
            Info << "No point fields" << nl << endl;
        }


        // If there are any tetFem fields, reconstruct them
        if
        (
            objects.lookupClass(tetPointScalarField::typeName).size()
         || objects.lookupClass(tetPointVectorField::typeName).size()
         || objects.lookupClass(tetPointSphericalTensorField::typeName).size()
         || objects.lookupClass(tetPointSymmTensorField::typeName).size()
         || objects.lookupClass(tetPointTensorField::typeName).size()
        )
        {
            Info << "Reconstructing tet point fields" << nl << endl;

            tetPolyMesh tetMesh(mesh);
            PtrList<tetPolyMesh> tetMeshes(procMeshes.meshes().size());

            forAll (tetMeshes, procI)
            {
                tetMeshes.set
                (
                    procI,
                    new tetPolyMesh(procMeshes.meshes()[procI])
                );
            }

            tetPointFieldReconstructor tetPointReconstructor
            (
                tetMesh,
                tetMeshes,
                procMeshes.pointProcAddressing(),
                procMeshes.faceProcAddressing(),
                procMeshes.cellProcAddressing(),
                procMeshes.boundaryProcAddressing()
            );

            // Reconstruct tet point fields
            tetPointReconstructor.reconstructFields<scalar>(objects);
            tetPointReconstructor.reconstructFields<vector>(objects);
            tetPointReconstructor.reconstructFields<sphericalTensor>(objects);
            tetPointReconstructor.reconstructFields<symmTensor>(objects);
            tetPointReconstructor.reconstructFields<tensor>(objects);
        }
        else
        {
            Info << "No tetFem fields" << nl << endl;
        }

        // If there is any proc with lagrangian, assume all have.        
        label lagrangianProcI = -1;

        forAll (databases, procI)
        {
            if 
            (
                IOobject
                (
                    "positions",
                    databases[procI].timeName(),
                    "lagrangian",
                    databases[procI],
                    IOobject::NO_READ
                ).headerOk()
            )
            {
                lagrangianProcI = procI;
                break;
            }
        }

        if (lagrangianProcI != -1)
        {
            Info << "Reconstructing lagrangian fields" << nl << endl;

            // Get list of objects from processor0 database
            IOobjectList objects
            (
                databases[lagrangianProcI],
                databases[lagrangianProcI].timeName(),
                "lagrangian"
            );

            PtrList<polyMesh>& meshes = 
                reinterpret_cast<PtrList<polyMesh>&>(procMeshes.meshes());

            reconstructLagrangianPositions
            (
                mesh,
                meshes,
                procMeshes.faceProcAddressing(),
                procMeshes.cellProcAddressing()
            );
            reconstructLagrangianFields<label>(mesh, meshes, objects);
            reconstructLagrangianFields<scalar>(mesh, meshes, objects);
            reconstructLagrangianFields<vector>(mesh, meshes, objects);
            reconstructLagrangianFields<sphericalTensor>(mesh, meshes, objects);
            reconstructLagrangianFields<symmTensor>(mesh, meshes, objects);
            reconstructLagrangianFields<tensor>(mesh, meshes, objects);
        }
        else
        {
            Info << "No lagrangian fields" << nl << endl;
        }


        // If there are any FA fields, reconstruct them

        if
        (
            objects.lookupClass(areaScalarField::typeName).size()
         || objects.lookupClass(areaVectorField::typeName).size()
         || objects.lookupClass(areaSphericalTensorField::typeName).size()
         || objects.lookupClass(areaSymmTensorField::typeName).size()
         || objects.lookupClass(areaTensorField::typeName).size()
         || objects.lookupClass(edgeScalarField::typeName).size()
        )
        {
            Info << "Reconstructing FA fields" << nl << endl;

            faMesh aMesh(mesh);

            processorFaMeshes procFaMeshes(procMeshes.meshes());
            
            faFieldReconstructor faReconstructor
            (
                aMesh,
                procFaMeshes.meshes(),
                procFaMeshes.edgeProcAddressing(),
                procFaMeshes.faceProcAddressing(),
                procFaMeshes.boundaryProcAddressing()
            );

            faReconstructor.reconstructFaAreaFields<scalar>(objects);
            faReconstructor.reconstructFaAreaFields<vector>(objects);
            faReconstructor
               .reconstructFaAreaFields<sphericalTensor>(objects);
            faReconstructor.reconstructFaAreaFields<symmTensor>(objects);
            faReconstructor.reconstructFaAreaFields<tensor>(objects);

            faReconstructor.reconstructFaEdgeFields<scalar>(objects);
        }
        else
        {
            Info << "No FA fields" << nl << endl;
        }

        // If there are any "uniform" directories copy them from
        // the master processor.

        fileName uniformDir0 = databases[0].timePath()/"uniform";
        if (dir(uniformDir0))
        {
            cp(uniformDir0, runTime.timePath());
        }
    }

    Info<< "End.\n" << endl;

    return 0;
}


// ************************************************************************* //
