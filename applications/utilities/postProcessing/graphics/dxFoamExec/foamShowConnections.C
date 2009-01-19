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

Error m_foamShowConnections(Object *in, Object *out)
{
    Info<< "dxFoamExec:foamShowConnections : constructing mesh edge lines"
        << endl;

    dxDatabase* runTimePtr = NULL;
    if (!in[0])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamShowConnections : missing foamMesh"
        );
        return DXERROR;
    }
    else if (!(runTimePtr = (dxDatabase*)DXGetPrivateData((Private)in[0])))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamShowConnections : cannot get foamMesh from input"
        );
        return DXERROR;
    }
    dxFvMesh& mesh = runTimePtr->mesh();


    //
    // Reuse existing positions (mesh points + cell centres)
    //

    DXArray dxPositions;

    if (!in[1])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamIndexCells : missing mesh"
        );
        return DXERROR;
    }
    else if (!(dxPositions = (DXArray)DXExtract(in[1], "positions")))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamIndexCells : "
            "missing positions in mesh"
        );
        return DXERROR;
    }

    //
    // Create connections from mesh edges.
    //

    int *linesArray = new int[2*mesh.nEdges()];

    label vertI = 0;

    forAll(mesh.edges(), edgeI)
    {
        const edge& e = mesh.edges()[edgeI];

        linesArray[vertI++] = e.start();
        linesArray[vertI++] = e.end();
    }

    DXArray dxConnections = DXNewArray(TYPE_INT, CATEGORY_REAL, 1, 2);

    if (!DXAddArrayData(dxConnections, 0, mesh.nEdges(), linesArray))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamShowConnections : Error in DXAddArrayData"
        );

        delete[] linesArray;

        return DXERROR;
    }

    delete[] linesArray;


    if (!DXSetStringAttribute((Object)dxConnections, "element type", "lines"))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamShowConnections : "
            "Error in DXSetStringAttribute element type"
        );

        return DXERROR;
    }

    if (!DXSetStringAttribute((Object)dxConnections, "ref", "positions"))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamShowConnections : "
            "Error in DXSetStringAttribute ref"
        );

        return DXERROR;
    }

    if (!DXSetStringAttribute((Object)dxConnections, "dep", "connections"))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamShowConnections : "
            "Error in DXSetStringAttribute dep"
        );
        return DXERROR;
    }

    //
    // Create field from positions and connections
    //

    DXField dxField = DXNewField();

    if (!dxField)
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamShowConnections : Error creating dxField"
        );
        return DXERROR;
    }

    
    if (!DXSetComponentValue(dxField, "positions", (Object)dxPositions))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamShowConnections : "
            "Error in DXSetComponentValue positions"
        );
        return DXERROR;
    }
    
    if (!DXSetComponentValue(dxField, "connections", (Object)dxConnections))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamShowConnections : "
            "Error in DXSetComponentValue connections"
        );
        return DXERROR;
    }

    if (!DXEndField(dxField))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamShowConnections : Error in DXEndField"
        );
        return DXERROR;
    }

    out[0] = (Object)dxField;

    return OK;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // end extern "C"

// ************************************************************************* //
