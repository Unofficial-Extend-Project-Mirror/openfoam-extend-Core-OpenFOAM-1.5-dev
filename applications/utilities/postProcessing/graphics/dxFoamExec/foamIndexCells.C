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

Error m_foamIndexCells(Object *in, Object *out)
{
    Info<< "dxFoamExec:foamIndexCells : constructing cell index array" << endl;

    dxDatabase* runTimePtr = NULL;
    if (!in[0])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamIndexCells : missing foamMesh"
        );
        return DXERROR;
    }
    else if (!(runTimePtr = (dxDatabase*)DXGetPrivateData((Private)in[0])))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamIndexCells : cannot get foamMesh from input"
        );
        return DXERROR;
    }
    dxFvMesh& mesh = runTimePtr->mesh();

    DXArray dxPositions;
    DXArray dxConnections;

    if (!in[1])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamIndexCells : missing mesh"
        );
        return DXERROR;
    }
    else if
    (
        !(dxPositions = (DXArray)DXExtract(in[1], "positions"))
     || !(dxConnections = (DXArray)DXExtract(in[1], "connections"))
    )
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamIndexCells : "
            "missing positions or connections in mesh"
        );
        return DXERROR;
    }

    label nCells = mesh.nCells();
    label totalNCells = mesh.totalNCells();

    int *indexArray = new int[totalNCells];

    label i = 0;
    for (label celli=0; celli<nCells; celli++)
    {
        indexArray[i++] = celli;
    }

    const labelList& superCells = mesh.superCells();
    forAll (superCells, j)
    {
        indexArray[i++] = superCells[j];
    }

    const polyBoundaryMesh& patches = mesh.boundaryMesh();
    forAll (patches, patchi)
    {
        const polyPatch& faces = patches[patchi];

        if (valid(faces))
        {
            forAll (faces, facei)
            {
               indexArray[i++] = -patchi - 1;
            }
        }
    }

    const labelList& superFacePatches = mesh.superFacePatches();
    forAll (superFacePatches, j)
    {
        indexArray[i++] = -superFacePatches[j] - 1;
    }

    // Check
    if (i != totalNCells)
    {
        SeriousError
            << "dxFoamExec:foamIndexCells : "
            << "indexArray over/underflow:" << endl
            << "    i:" << i << endl
            << "    allocated:" << totalNCells
            << endl;

        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamIndexCells : "
            "missing positions or connections in mesh"
        );

        delete[] indexArray;

        return DXERROR;
    }

    // Create an array of scalar data
    DXArray dxIndexArray = DXNewArray(TYPE_INT, CATEGORY_REAL, 0, 1);

    if (!DXAddArrayData(dxIndexArray, 0, totalNCells, indexArray))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamIndexCells : Error in DXAddArrayData"
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
            "foamIndexCells",
            dxIndexArray,
            dxPositions,
            dxConnections,
            "connections"
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
            "dxFoamExec:foamIndexCells : Error in DXEndField"
        );
        return DXERROR;
    }

    out[0] = (Object)dxIndexField;

    return OK;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // end extern "C"

// ************************************************************************* //
