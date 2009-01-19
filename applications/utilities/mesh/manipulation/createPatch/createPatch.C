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
    Utility to create patches out of selected boundary faces. Faces come either
    from existing patches or from a faceSet.

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "polyMesh.H"
#include "Time.H"
#include "SortableList.H"
#include "OFstream.H"
#include "meshTools.H"
#include "faceSet.H"
#include "IOPtrList.H"
#include "repatchPolyTopoChanger.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    defineTemplateTypeNameAndDebug(IOPtrList<dictionary>, 0);
}


label getPatch(const polyBoundaryMesh& patches, const word& patchName)
{
    label patchI = patches.findPatchID(patchName);

    if (patchI == -1)
    {
        FatalErrorIn("createPatch")
            << "Cannot find source patch " << patchName
            << endl << "Valid patch names are " << patches.names()
            << exit(FatalError);
    }

    return patchI;
}


// Main program:

int main(int argc, char *argv[])
{
    argList::noParallel();

#   include "setRootCase.H"
#   include "createTime.H"
#   include "createPolyMesh.H"


    Info<< "Reading createPatchDict\n" << endl;

    PtrList<dictionary> patchSources
    (
        IOdictionary
        (
            IOobject
            (
                "createPatchDict",
                runTime.system(),
                runTime,
                IOobject::MUST_READ,
                IOobject::NO_WRITE,
                false
            )
        ).lookup("patches")
    );

    const polyBoundaryMesh& patches = mesh.boundaryMesh();

    // Old and new patches.
    DynamicList<polyPatch*> allPatches(patches.size() + patchSources.size());

    // Copy old patches.
    forAll(patches, patchI)
    {
        const polyPatch& pp = patches[patchI];

        Info<< "Copying patch " << pp.name() << " at position "
            << patchI << endl;

        allPatches.append
        (
            pp.clone
            (
                patches,
                patchI,
                pp.size(),
                pp.start()
            ).ptr()
        );
    }


    // For every boundary face the old patch.
    labelList oldPatchID(mesh.nFaces()-mesh.nInternalFaces());

    forAll(patches, patchI)
    {
        const polyPatch& pp = patches[patchI];

        label bFaceI = pp.start() - mesh.nInternalFaces();

        forAll(pp, i)
        {
            oldPatchID[bFaceI++] = patchI;
        }
    }

    // For every boundary face the new patch (or rather the index into
    // allPatches)
    labelList allPatchID(oldPatchID);


    //
    // Add new patches constructed according to dictionary.
    //


    forAll(patchSources, addedI)
    {
        const dictionary& dict = patchSources[addedI];

        word patchName(dict.lookup("name"));

        label destPatchI = patches.findPatchID(patchName);

        word patchType(dict.lookup("type"));

        if (destPatchI == -1)
        {
            destPatchI = allPatches.size();

            Info<< "Adding new patch " << patchName << " of type " << patchType
                << " as patch " << destPatchI << endl;

            // Add an empty patch.
            allPatches.append
            (
                polyPatch::New
                (
                    patchType,
                    patchName,
                    0,
                    mesh.nFaces(),
                    destPatchI,
                    patches
                ).ptr()
            );
        }

        word sourceType(dict.lookup("constructFrom"));

        if (sourceType == "patches")
        {
            wordList patchSources(dict.lookup("patches"));

            // Repatch faces of the patches.
            forAll(patchSources, sourceI)
            {
                label patchI = getPatch(patches, patchSources[sourceI]);

                const polyPatch& pp = patches[patchI];

                Info<< "Moving faces from patch " << pp.name()
                    << " to patch " << destPatchI << endl;

                forAll(pp, i)
                {
                    label faceI = pp.start() + i;

                    allPatchID[faceI - mesh.nInternalFaces()] = destPatchI;
                }
            }
        }
        else if (sourceType == "set")
        {
            word setName(dict.lookup("set"));

            faceSet faces(mesh, setName);

            Info<< "Read " << faces.size() << " faces from faceSet "
                << faces.name() << endl;

            // Sort (since faceSet contains faces in arbitrary order)
            labelList faceLabels(faces.toc());

            SortableList<label> patchFaces(faceLabels);

            forAll(patchFaces, i)
            {
                label faceI = patchFaces[i];

                if (mesh.isInternalFace(faceI))
                {
                    FatalErrorIn(args.executable())
                        << "Face " << faceI << " specified in set "
                        << faces.name()
                        << " is not an external face of the mesh." << endl
                        << "This application can only repatch existing boundary"
                        << " faces." << exit(FatalError);
                }

                if
                (
                    allPatchID[faceI - mesh.nInternalFaces()]
                 != oldPatchID[faceI - mesh.nInternalFaces()]
                )
                {
                    FatalErrorIn(args.executable())
                        << "Face " << faceI << " on new patch " << patchName
                        << " has already been marked for repatching to"
                        << " patch "
                        << allPatchID[faceI - mesh.nInternalFaces()]
                        << exit(FatalError);
                }
                allPatchID[faceI - mesh.nInternalFaces()] = destPatchI;
            }
        }
        else
        {
            FatalErrorIn(args.executable())
                << "Invalid source type " << sourceType << endl
                << "Valid source types are 'patches' 'set'" << exit(FatalError);
        }
    }

    allPatches.shrink();

    Pout<< endl;

    // Count sizes
    labelList patchSizes(allPatches.size(), 0);

    forAll(allPatchID, i)
    {
        patchSizes[allPatchID[i]]++;
    }
     

    // Create compacted patches.

    labelList compactPatchMap(allPatches.size(), -1);

    List<polyPatch*> compactPatches(allPatches.size());

    // Copy old and new patches.

    label compactPatchI = 0;

    label meshFaceI = mesh.nInternalFaces();

    forAll(allPatches, patchI)
    {
        const polyPatch& pp = *allPatches[patchI];

        if (patchSizes[patchI] == 0)
        {
            Pout<< "Removing empty patch " << pp.name() << endl;
        }
        else
        {
            compactPatchMap[patchI] = compactPatchI;

            compactPatches[compactPatchI] =
                pp.clone
                (
                    patches,
                    compactPatchI,
                    pp.size(),
                    meshFaceI
                ).ptr();

            meshFaceI += pp.size();

            compactPatchI++;
        }

        delete allPatches[patchI];
    }
    compactPatches.setSize(compactPatchI);


    Pout<< "Compacted patches:" << nl;

    forAll(compactPatches, i)
    {
        const polyPatch& pp = *compactPatches[i];

        Pout<< "    " << pp.name()
            << '\t' << "size:" << pp.size()
            << '\t' << "start:" << pp.start()
            << endl;
    }



    // Now we have all information. Start changing the mesh.

    repatchPolyTopoChanger repatcher(mesh);


    // Add new list of patches
    repatcher.changePatches(compactPatches);

    // Change patch ids
    forAll(allPatchID, i)
    {
        label compactedPatchI = compactPatchMap[allPatchID[i]];

        label faceI = i + mesh.nInternalFaces();

        repatcher.changePatchID(faceI, compactedPatchI);
    }



    // Set the precision of the points data to 10
    IOstream::defaultPrecision(10);

    runTime++;

    // Do all topology changes in one go
    repatcher.repatch();

    // Write resulting mesh
    Info<< "Writing repatched mesh to " << runTime.timeName() << endl;
    mesh.write();

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
