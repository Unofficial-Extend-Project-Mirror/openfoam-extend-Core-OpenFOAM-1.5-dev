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

    returns field with 0 (cell not in cellSet) or 1 (cell in cellSet).
    Used in combination with cell set handling.

\*---------------------------------------------------------------------------*/

#include "foamReadField.H"
#include "dxDatabase.H"
#include "foamValid.H"
#include "cellSet.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

extern "C"
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Error m_foamGetSet(Object *in, Object *out)
{
    Info<< "dxFoamExec:foamGetSet : constructing cell inclusion array" << endl;

    dxDatabase* runTimePtr = NULL;
    if (!in[0])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetSet : missing foamMesh"
        );
        return DXERROR;
    }
    else if (!(runTimePtr = (dxDatabase*)DXGetPrivateData((Private)in[0])))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetSet : cannot get foamMesh from input"
        );
        return DXERROR;
    }

    dxDatabase& runTime = *runTimePtr;

    dxFvMesh& mesh = runTime.mesh();

    DXArray dxPositions;
    DXArray dxConnections;

    if (!in[1])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetSet : missing mesh"
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
            "dxFoamExec:foamGetSet : "
            "missing positions or connections in mesh"
        );
        return DXERROR;
    }

    char *setNameChars;

    if (!in[2])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetSet : missing setName"
        );
        return DXERROR;
    }
    else if (!DXExtractString(in[2], &setNameChars))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetSet : setName must be a string"
        );
        return DXERROR;
    }

    word setName = setNameChars;


    // Load cellSet
    cellSet set(runTime, setName, IOobject::READ_IF_PRESENT);

    Info<< "dxFoamExec:foamGetSet : loaded " << set.size()
        << " cells from cellSet " << setName << endl;

    //
    // Create bool array with 1 for all cells in cellSet
    //


    label nCells = mesh.nCells();
    label totalNCells = mesh.totalNCells();

    int *validArray = new int[totalNCells];

    // Initialize to 0
    for (label celli=0; celli<totalNCells; celli++)
    {
        validArray[celli] = 0;
    }

    // Set undecomposed cells
    for
    (
        cellSet::const_iterator iter = set.begin();
        iter != set.end();
        ++iter
    )
    {
        validArray[iter.key()] = 1;
    }

    // Set decomposed cells
    label i = nCells;

    const labelList& superCells = mesh.superCells();
    forAll (superCells, j)
    {
        if (set.found(superCells[j]))
        {
            validArray[i] = 1;
        }
        i++;
    }

    // Create an array of int data
    DXArray dxValidArray = DXNewArray(TYPE_INT, CATEGORY_REAL, 0, 1);

    if (!DXAddArrayData(dxValidArray, 0, totalNCells, validArray))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamGetSet : Error in DXAddArrayData"
        );

        delete[] validArray;

        return DXERROR;
    }


    delete[] validArray;


    DXField dxIndexField = NULL;
    
    if 
    (
       !(dxIndexField = createDxField
        (
            "foamGetSet",
            dxValidArray,
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
            "dxFoamExec:foamGetSet : Error in DXEndField"
        );
        return DXERROR;
    }

    out[0] = (Object)dxIndexField;

    return OK;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // end extern "C"

// ************************************************************************* //
