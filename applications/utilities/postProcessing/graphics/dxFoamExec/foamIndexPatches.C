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

    See README.

\*---------------------------------------------------------------------------*/

#include "foamReadField.H"
#include "dxDatabase.H"
#include "foamValid.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

extern "C"
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Error m_foamIndexPatches(Object *in, Object *out)
{
    Info<< "dxFoamExec:foamIndexPatches : constructing patch index array"
        << endl;

    dxDatabase* runTimePtr = NULL;
    if (!in[0])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamIndexPatches : missing foamMesh"
        );
        return DXERROR;
    }
    else if (!(runTimePtr = (dxDatabase*)DXGetPrivateData((Private)in[0])))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamIndexPatches : cannot get mesh from input"
        );
        return DXERROR;
    }
    dxFvMesh& mesh = runTimePtr->mesh();


    DXArray dxPositions;
    DXArray dxBFconnections;

    if (!in[1])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamIndexPatches : missing mesh"
        );
        return DXERROR;
    }
    else if
    (
        !(dxPositions = (DXArray)DXExtract(in[1], "positions"))
     || !(dxBFconnections = (DXArray)DXExtract(in[1], "BFconnections"))
    )
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamIndexPatches : "
            "missing positions or connections in mesh"
        );
        return DXERROR;
    }

    // Label faces with valid patch number

    int *indexArray = new int[mesh.totalBfaces() + mesh.totalZFaces()];

    label i = 0;

    const polyBoundaryMesh& patches = mesh.boundaryMesh();

    labelList patchLabels(patches.size(), -1);

    label displayLabel = 0;

    // Determine display label for all patches
    forAll (patches, patchi)
    {
        const polyPatch& faces = patches[patchi];

        if (valid(faces))
        {
            patchLabels[patchi] = displayLabel++;
        }
    }

    // Set indexArray
    forAll (patches, patchi)
    {
        const polyPatch& faces = patches[patchi];

        if (valid(faces))
        {
            forAll (faces, facei)
            {
               indexArray[i++] = patchLabels[patchi];
            }
        }
    }

    const labelList& superFacePatches = mesh.superFacePatches();
    forAll (superFacePatches, j)
    {
        indexArray[i++] = patchLabels[superFacePatches[j]];
    }


    // Label faceZones after patches

    labelList zoneLabels(mesh.faceZones().size(), -1);

    // Determine zone numbering
    forAll(mesh.faceZones(), zonei)
    {
        zoneLabels[zonei] = displayLabel++;
    }

    forAll(mesh.faceZones(), zonei)
    {
        const labelList& indices = mesh.faceZones()[zonei];

        forAll (indices, indexi)
        {
            if (indices[indexi] < mesh.nFaces())
            {
                indexArray[i++] = zoneLabels[zonei];
            }
        }
    }


    const labelList& superZFaceZones = mesh.superZFaceZones();
    forAll (superZFaceZones, j)
    {
        indexArray[i++] = zoneLabels[superZFaceZones[j]];
    }


    // Check
    if (i != mesh.totalBfaces() + mesh.totalZFaces())
    {
        SeriousError
            << "dxFoamExec:foamIndexPatches : "
            << "indexArray under/overflow:" << endl
            << "    i:" << i << endl
            << "    allocated:" << mesh.totalBfaces() + mesh.totalZFaces()
            << endl;

        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamIndexPatches : Error in DXAddArrayData"
        );

        delete[] indexArray;

        return DXERROR;
    }


    // Create an array of scalar data
    DXArray dxIndexArray = DXNewArray(TYPE_INT, CATEGORY_REAL, 0, 1);

    if
    (
        !DXAddArrayData
        (
            dxIndexArray,
            0,
            mesh.totalBfaces() + mesh.totalZFaces(),
            indexArray
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamIndexPatches : Error in DXAddArrayData"
        );

        delete[] indexArray;

        return DXERROR;
    }

    delete[] indexArray;


    DXField dxIndexField = NULL;
    
    if 
    (
       !(dxIndexField = createDxField
        (
            "foamIndexPatches",
            dxIndexArray,
            dxPositions,
            dxBFconnections,
            "BFconnections"
        ))
    )
    {
        return DXERROR;
    }

    if (!DXEndField(dxIndexField))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamIndexPatches : Error in DXEndField"
        );
        return DXERROR;
    }

    out[0] = (Object)dxIndexField;

    return OK;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // end extern "C"

// ************************************************************************* //
