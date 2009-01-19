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
    decomposePar

Description
    Automatically decomposes a mesh and fields of a case for parallel execution
    of FOAM.

\*---------------------------------------------------------------------------*/

#include "OSspecific.H"
#include "fvCFD.H"
#include "IOobjectList.H"
#include "processorFvPatchFields.H"
#include "domainDecomposition.H"
#include "labelIOField.H"

#include "scalarIOField.H"
#include "vectorIOField.H"
#include "sphericalTensorIOField.H"
#include "symmTensorIOField.H"
#include "tensorIOField.H"

#include "tetPointFields.H"
#include "tetFemMatrices.H"
#include "tetPointFieldDecomposer.H"

#include "pointFields.H"

#include "readFields.H"

#include "fvFieldDecomposer.H"
#include "pointFieldDecomposer.H"
#include "lagrangianFieldDecomposer.H"

#include "faCFD.H"
#include "emptyFaPatch.H"
#include "faMeshDecomposition.H"
#include "faFieldDecomposer.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::noParallel();
    argList::validOptions.insert("fields", "");
    argList::validOptions.insert("cellDist", "");
    argList::validOptions.insert("filterPatches", "");
    argList::validOptions.insert("copyUniform", "");

#   include "setRootCase.H"

    bool decomposeFieldsOnly(args.options().found("fields"));

    bool writeCellDist(args.options().found("cellDist"));

    bool filterPatches(args.options().found("filterPatches"));

    bool copyUniform(args.options().found("copyUniform"));

#   include "createTime.H"

    Info<< "Time = " << runTime.timeName() << endl;

    Info<< "Create mesh\n" << endl;
    domainDecomposition mesh
    (
        IOobject
        (
            domainDecomposition::defaultRegion,
            runTime.timeName(),
            runTime
        )
    );

    // Decompose the mesh
    if (!decomposeFieldsOnly)
    {
        if (dir(runTime.path()/"processor1"))
        {
            FatalErrorIn(args.executable())
                << "Case is already decomposed." << endl
                << "    Please remove processor directories before "
                   "decomposing e.g. using:" << nl
                << "    rm -rf " << runTime.path().c_str() << "/processor*"
                << exit(FatalError);
        }

        mesh.decomposeMesh(filterPatches);

        mesh.writeDecomposition();

        if (writeCellDist)
        {
            // Write the decomposition as labelList for use with 'manual'
            // decomposition method.
            OFstream str
            (
                runTime.path()
              / mesh.facesInstance()
              / polyMesh::defaultRegion
              / "cellDecomposition"
            );

            str << mesh.cellToProc();

            Info<< nl << "Written decomposition to "
                << str.name() << " for use in manual decomposition."
                << nl << endl;

            // Write as volScalarField for postprocessing.
            volScalarField cellDist
            (
                IOobject
                (
                    "cellDist",
                    runTime.timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh,
                dimensionedScalar("cellDist", dimless, 0),
                zeroGradientFvPatchScalarField::typeName
            );

            const labelList& procIds = mesh.cellToProc();
            forAll(procIds, celli)
            {
               cellDist[celli] = procIds[celli]; 
            }

            cellDist.write();

            Info<< nl << "Written decomposition as volScalarField to "
                << cellDist.name() << " for use in postprocessing."
                << nl << endl;
        }
    }

    // Read lagrangian particles
    Cloud<indexedParticle> lagrangianPositions(mesh);

    if (lagrangianPositions.size())
    {
        Info<< nl << "Creating lagrangian particle to processor addressing"
            << endl;
    }

    // Sort particles according to cell they are in
    List<SLList<indexedParticle*>*> cellParticles
    (
        mesh.nCells(),
        static_cast<SLList<indexedParticle*>*>(NULL)
    );

    label i = 0;

    for
    (
        Cloud<indexedParticle>::iterator iter = lagrangianPositions.begin();
        iter != lagrangianPositions.end();
        ++iter
    )
    {
        iter().index() = i++;

        label celli = iter().cell();

        // Check
        if (celli < 0 || celli >= mesh.nCells())
        {
            FatalErrorIn(args.executable())
                << "Illegal cell number " << celli
                << " for particle with index " << iter().index()
                << " at position " << iter().position() << nl
                << "Cell number should be between 0 and " << mesh.nCells()-1
                << nl
                << "On this mesh the particle should be in cell "
                << mesh.findCell(iter().position())
                << exit(FatalError);
        }

        if (!cellParticles[celli])
        {
            cellParticles[celli] = new SLList<indexedParticle*>();
        }

        cellParticles[celli]->append(&iter());
    }


    // Search for list of objects for this time
    IOobjectList objects(mesh, runTime.timeName());

    // Construct the vol fields
    // ~~~~~~~~~~~~~~~~~~~~~~~~
    PtrList<volScalarField> volScalarFields;
    readFields(mesh, objects, volScalarFields);

    PtrList<volVectorField> volVectorFields;
    readFields(mesh, objects, volVectorFields);

    PtrList<volSphericalTensorField> volSphericalTensorFields;
    readFields(mesh, objects, volSphericalTensorFields);

    PtrList<volSymmTensorField> volSymmTensorFields;
    readFields(mesh, objects, volSymmTensorFields);

    PtrList<volTensorField> volTensorFields;
    readFields(mesh, objects, volTensorFields);


    // Construct the surface fields
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    PtrList<surfaceScalarField> surfaceScalarFields;
    readFields(mesh, objects, surfaceScalarFields);


    // Construct the point fields
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~
    pointMesh pMesh(mesh);

    PtrList<pointScalarField> pointScalarFields;
    readFields(pMesh, objects, pointScalarFields);

    PtrList<pointVectorField> pointVectorFields;
    readFields(pMesh, objects, pointVectorFields);

    PtrList<pointSphericalTensorField> pointSphericalTensorFields;
    readFields(pMesh, objects, pointSphericalTensorFields);

    PtrList<pointSymmTensorField> pointSymmTensorFields;
    readFields(pMesh, objects, pointSymmTensorFields);

    PtrList<pointTensorField> pointTensorFields;
    readFields(pMesh, objects, pointTensorFields);


    // Construct the tetPoint fields
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~
    tetPolyMesh* tetMeshPtr = NULL;

    PtrList<tetPointScalarField> tetPointScalarFields;
    PtrList<tetPointVectorField> tetPointVectorFields;
    PtrList<tetPointSphericalTensorField> tetPointSphericalTensorFields;
    PtrList<tetPointSymmTensorField> tetPointSymmTensorFields;
    PtrList<tetPointTensorField> tetPointTensorFields;

    if
    (
        objects.lookupClass("tetPointScalarField").size() > 0
     || objects.lookupClass("tetPointVectorField").size() > 0
     || objects.lookupClass("tetPointSphericalTensorField").size() > 0
     || objects.lookupClass("tetPointSymmTensorField").size() > 0
     || objects.lookupClass("tetPointTensorField").size() > 0
    )
    {
        tetMeshPtr = new tetPolyMesh(mesh);
        tetPolyMesh& tetMesh = *tetMeshPtr;

        readFields(tetMesh, objects, tetPointScalarFields);
        readFields(tetMesh, objects, tetPointVectorFields);
        readFields(tetMesh, objects, tetPointSphericalTensorFields);
        readFields(tetMesh, objects, tetPointSymmTensorFields);
        readFields(tetMesh, objects, tetPointTensorFields);
    }


    // Construct the Lagrangian fields
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    IOobjectList lagrangianObjects(mesh, runTime.timeName(), "lagrangian");

    PtrList<labelIOField> lagrangianLabelFields;
    lagrangianFieldDecomposer::readFields
    (
        lagrangianObjects,
        lagrangianLabelFields
    );

    PtrList<scalarIOField> lagrangianScalarFields;
    lagrangianFieldDecomposer::readFields
    (
        lagrangianObjects,
        lagrangianScalarFields
    );

    PtrList<vectorIOField> lagrangianVectorFields;
    lagrangianFieldDecomposer::readFields
    (
        lagrangianObjects,
        lagrangianVectorFields
    );

    PtrList<sphericalTensorIOField> lagrangianSphericalTensorFields;
    lagrangianFieldDecomposer::readFields
    (
        lagrangianObjects,
        lagrangianSphericalTensorFields
    );

    PtrList<symmTensorIOField> lagrangianSymmTensorFields;
    lagrangianFieldDecomposer::readFields
    (
        lagrangianObjects,
        lagrangianSymmTensorFields
    );

    PtrList<tensorIOField> lagrangianTensorFields;
    lagrangianFieldDecomposer::readFields
    (
        lagrangianObjects,
        lagrangianTensorFields
    );


    // Any uniform data to copy/link?

    fileName uniformDir;

    if (dir(runTime.timePath()/"uniform"))
    {
        uniformDir = runTime.timePath()/"uniform";

        Info<< "Detected additional non-decomposed files in " << uniformDir
            << endl;
    }


    Info<< endl;


    // split the fields over processors
    for (label procI = 0; procI < mesh.nProcs(); procI++)
    {
        Info<< "Processor " << procI << ": field transfer" << endl;

        // open the database
        Time processorDb
        (
            Time::controlDictName,
            args.rootPath(),
            args.caseName()/fileName(word("processor") + name(procI))
        );

        processorDb.setTime(runTime);

        // read the mesh
        fvMesh procMesh
        (
            IOobject
            (
                fvMesh::defaultRegion,
                processorDb.timeName(),
                processorDb
            )
        );

        labelIOList cellProcAddressing
        (
            IOobject
            (
                "cellProcAddressing",
                "constant",
                procMesh.meshSubDir,
                procMesh,
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            )
        );

        labelIOList boundaryProcAddressing
        (
            IOobject
            (
                "boundaryProcAddressing",
                "constant",
                procMesh.meshSubDir,
                procMesh,
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            )
        );

        // FV fields
        if
        (
            volScalarFields.size()
         || volVectorFields.size()
         || volSphericalTensorFields.size()
         || volSymmTensorFields.size()
         || volTensorFields.size()
         || surfaceScalarFields.size()
        )
        {
            labelIOList faceProcAddressing
            (
                IOobject
                (
                    "faceProcAddressing",
                    "constant",
                    procMesh.meshSubDir,
                    procMesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            );

            fvFieldDecomposer fieldDecomposer
            (
                mesh,
                procMesh,
                faceProcAddressing,
                cellProcAddressing,
                boundaryProcAddressing
            );

            fieldDecomposer.decomposeFields(volScalarFields);
            fieldDecomposer.decomposeFields(volVectorFields);
            fieldDecomposer.decomposeFields(volSphericalTensorFields);
            fieldDecomposer.decomposeFields(volSymmTensorFields);
            fieldDecomposer.decomposeFields(volTensorFields);

            fieldDecomposer.decomposeFields(surfaceScalarFields);
        }

        // Point fields
        if
        (
            pointScalarFields.size()
         || pointVectorFields.size()
         || pointSphericalTensorFields.size()
         || pointSymmTensorFields.size()
         || pointTensorFields.size()
        )
        {
            labelIOList pointProcAddressing
            (
                IOobject
                (
                    "pointProcAddressing",
                    "constant",
                    procMesh.meshSubDir,
                    procMesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            );

            pointMesh procPMesh(procMesh, true);

            pointFieldDecomposer fieldDecomposer
            (
                pMesh,
                procPMesh,
                pointProcAddressing,
                boundaryProcAddressing
            );

            fieldDecomposer.decomposeFields(pointScalarFields);
            fieldDecomposer.decomposeFields(pointVectorFields);
            fieldDecomposer.decomposeFields(pointSphericalTensorFields);
            fieldDecomposer.decomposeFields(pointSymmTensorFields);
            fieldDecomposer.decomposeFields(pointTensorFields);
        }


        // tetPoint fields
        if (tetMeshPtr)
        {
            const tetPolyMesh& tetMesh = *tetMeshPtr;
            tetPolyMesh procTetMesh(procMesh);

            // Read the point addressing information
            labelIOList pointProcAddressing
            (
                IOobject
                (
                    "pointProcAddressing",
                    "constant",
                    procMesh.meshSubDir,
                    procMesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            );

            // Read the point addressing information
            labelIOList faceProcAddressing
            (
                IOobject
                (
                    "faceProcAddressing",
                    "constant",
                    procMesh.meshSubDir,
                    procMesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            );

            tetPointFieldDecomposer fieldDecomposer
            (
                tetMesh,
                procTetMesh,
                pointProcAddressing,
                faceProcAddressing,
                cellProcAddressing,
                boundaryProcAddressing
            );

            fieldDecomposer.decomposeFields(tetPointScalarFields);
            fieldDecomposer.decomposeFields(tetPointVectorFields);
            fieldDecomposer.decomposeFields(tetPointSphericalTensorFields);
            fieldDecomposer.decomposeFields(tetPointSymmTensorFields);
            fieldDecomposer.decomposeFields(tetPointTensorFields);
        }


        // If there is lagrangian data write it out
        if (lagrangianPositions.size())
        {
            lagrangianFieldDecomposer fieldDecomposer
            (
                mesh,
                procMesh,
                cellProcAddressing,
                lagrangianPositions,
                cellParticles
            );

            // Lagrangian fields
            if
            (
                lagrangianLabelFields.size()
             || lagrangianScalarFields.size()
             || lagrangianVectorFields.size()
             || lagrangianSphericalTensorFields.size()
             || lagrangianSymmTensorFields.size()
             || lagrangianTensorFields.size()
            )
            {
                fieldDecomposer.decomposeFields(lagrangianLabelFields);
                fieldDecomposer.decomposeFields(lagrangianScalarFields);
                fieldDecomposer.decomposeFields(lagrangianVectorFields);
                fieldDecomposer.decomposeFields
                (
                    lagrangianSphericalTensorFields
                );
                fieldDecomposer.decomposeFields(lagrangianSymmTensorFields);
                fieldDecomposer.decomposeFields(lagrangianTensorFields);
            }
        }


        // Any non-decomposed data to copy?
        if (uniformDir.size() > 0)
        {
            if (copyUniform || mesh.distributed())
            {
                cp(uniformDir, processorDb.timePath()/"uniform");
            }
            else
            {
                fileName timePath = processorDb.timePath();

                if (timePath[0] != '/')
                {
                    // Adapt uniformDir and timePath to be relative paths.
                    string parentPath(string("..")/"..");
                    fileName currentDir(cwd());
                    chDir(timePath);
                    ln(parentPath/uniformDir, parentPath/timePath/"uniform");
                    chDir(currentDir);
                }
                else
                {
                    ln(uniformDir, timePath/"uniform");
                }
            }
        }
    }

    if (tetMeshPtr)
    {
        delete tetMeshPtr;
        tetMeshPtr = NULL;
    }


    // Finite area mesh and filed decomposition

    IOobject faMeshBoundaryIOobj
    (
        "boundary",
        mesh.time().findInstance(mesh.dbDir()/fvMesh::meshSubDir, "boundary"),
        faMesh::meshSubDir,
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    );


    if(faMeshBoundaryIOobj.headerOk())
    {
        Info << "\nFinite area mesh decomposition" << endl;

        faMeshDecomposition aMesh(mesh);

        aMesh.decomposeMesh(filterPatches);

        aMesh.writeDecomposition();


        // Construct the area fields
        // ~~~~~~~~~~~~~~~~~~~~~~~~
        PtrList<areaScalarField> areaScalarFields;
        readFields(mesh, objects, areaScalarFields);

        PtrList<areaVectorField> areaVectorFields;
        readFields(mesh, objects, areaVectorFields);

        PtrList<areaSphericalTensorField> areaSphericalTensorFields;
        readFields(mesh, objects, areaSphericalTensorFields);

        PtrList<areaSymmTensorField> areaSymmTensorFields;
        readFields(mesh, objects, areaSymmTensorFields);

        PtrList<areaTensorField> areaTensorFields;
        readFields(mesh, objects, areaTensorFields);


        // Construct the edge fields
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        PtrList<edgeScalarField> edgeScalarFields;
        readFields(mesh, objects, edgeScalarFields);

        Info << endl;

        // Split the fields over processors
        for (label procI = 0; procI < mesh.nProcs(); procI++)
        {
            Info<< "Processor " << procI
                << ": finite area field transfer" << endl;

            // open the database
            Time processorDb
            (
                Time::controlDictName,
                args.rootPath(),
                args.caseName()/fileName(word("processor") + name(procI))
            );

            processorDb.setTime(runTime);

            // Read the mesh
            fvMesh procFvMesh
            (
                IOobject
                (
                    fvMesh::defaultRegion,
                    processorDb.timeName(),
                    processorDb
                )
            );

            faMesh procMesh(procFvMesh);

            labelIOList faceProcAddressing
            (
                IOobject
                (
                    "faceProcAddressing",
                    "constant",
                    procMesh.meshSubDir,
                    procFvMesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            );

            labelIOList boundaryProcAddressing
            (
                IOobject
                (
                    "boundaryProcAddressing",
                    "constant",
                    procMesh.meshSubDir,
                    procFvMesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            );

            // FA fields
            if
            (
                areaScalarFields.size()
             || areaVectorFields.size()
             || areaSphericalTensorFields.size()
             || areaSymmTensorFields.size()
             || areaTensorFields.size()
             || edgeScalarFields.size()
            )
            {
                labelIOList edgeProcAddressing
                (
                    IOobject
                    (
                        "edgeProcAddressing",
                        "constant",
                        procMesh.meshSubDir,
                        procFvMesh,
                        IOobject::MUST_READ,
                        IOobject::NO_WRITE
                    )
                );

                faFieldDecomposer fieldDecomposer
                (
                    aMesh,
                    procMesh,
                    edgeProcAddressing,
                    faceProcAddressing,
                    boundaryProcAddressing
                );

                fieldDecomposer.decomposeFields(areaScalarFields);
                fieldDecomposer.decomposeFields(areaVectorFields);
                fieldDecomposer.decomposeFields(areaSphericalTensorFields);
                fieldDecomposer.decomposeFields(areaSymmTensorFields);
                fieldDecomposer.decomposeFields(areaTensorFields);

                fieldDecomposer.decomposeFields(edgeScalarFields);
            }
        }
    }


    Info<< "\nEnd.\n" << endl;

    return(0);
}


// ************************************************************************* //
