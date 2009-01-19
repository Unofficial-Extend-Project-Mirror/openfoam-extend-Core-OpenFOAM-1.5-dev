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

#include "dxFoamExec.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

extern "C"
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Error m_foamReadSTL(Object *in, Object *out)
{
    /*
    Info<< "dxFoamExec:foamReadSTL : reading STL surface description" << endl;

    dxDatabase* runTimePtr = NULL;

    if (!in[0])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetFieldNames : missing fieldType"
        );
        return DXERROR;
    }
    else if (!(runTimePtr = (dxDatabase*)DXGetPrivateData((Private)in[0])))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetFieldNames : fieldType must be a string"
        );
        return DXERROR;
    }

    dxDatabase& runTime = *runTimePtr;

    fileName STLfileName(runTime.constantPath() / "polyMesh" / "surface.stl");

    triangulatedSurface boundary(STLfileName);

    //
    // Positions
    //
    const pointField& pts = boundary.points();



    // Create vertex array
    float *pointVector = new float[3*pts.size()];

    // Set vertices
    label i = 0;
    forAll(pts, pointi)
    {
        pointVector[i++] = pts[pointi].x();
        pointVector[i++] = pts[pointi].y();
        pointVector[i++] = pts[pointi].z();
    }

    // Create array objects for points (positions)
    // -------------------------------------------

    DXArray dxPositions = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 1, 3);

    if (!DXAddArrayData(dxPositions, 0, pts.size(), pointVector))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamReadSTL : Error in DXAddArrayData"
        );

        delete[] pointVector;

        return DXERROR;
    }

    delete[] pointVector;

    if (!DXSetStringAttribute((Object)dxPositions, "dep", "positions"))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamReadSTL : "
            "Error in DXSetStringAttribute dep"
        );
        return DXERROR;
    }

    //
    // Connections
    //

    // Create temporary storage for DX connections array vector
    int *cellsArray = new int[3*boundary.size()];

    i = 0;
    forAll(boundary, celli)
    {
        cellsArray[i++] = boundary[celli].a();
        cellsArray[i++] = boundary[celli].b();
        cellsArray[i++] = boundary[celli].c();
    }

    
    // Create array objects for cell connections
    // -----------------------------------------
    DXArray dxCellCons = DXNewArray(TYPE_INT, CATEGORY_REAL, 1, 3);

    if (!DXAddArrayData(dxCellCons, 0, boundary.size(), cellsArray))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamReadSTL : Error in DXAddArrayData"
        );

        delete[] cellsArray;

        return DXERROR;
    }

    delete[] cellsArray;

    if (!DXSetStringAttribute((Object)dxCellCons, "element type", "triangles"))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamReadSTL : "
            "Error in DXSetStringAttribute element type"
        );
        return DXERROR;
    }

    if (!DXSetStringAttribute((Object)dxCellCons, "ref", "positions"))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamReadSTL : "
            "Error in DXSetStringAttribute ref"
        );
        return DXERROR;
    }

    if (!DXSetStringAttribute((Object)dxCellCons, "dep", "connections"))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamReadSTL : "
            "Error in DXSetStringAttribute dep"
        );
        return DXERROR;
    }


    //
    // Create DX mesh from positions and connections.
    //
    DXField dxMesh = DXNewField();

    if (!dxMesh)
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamReadSTL : Error creating dxMesh"
        );
        return DXERROR;
    }

    if (!DXSetStringAttribute((Object)dxMesh, "name","STLmesh"))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamReadSTL : Error in DXSetStringAttribute name"
        );
        return DXERROR;
    }

    if (!DXSetComponentValue(dxMesh, "positions", (Object)dxPositions))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamReadSTL : "
            "Error in DXSetComponentValue positions"
        );
        return DXERROR;
    }
    
    if (!DXSetComponentValue(dxMesh, "connections", (Object)dxCellCons))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamReadSTL : "
            "Error in DXSetComponentValue connections"
        );
        return DXERROR;
    }

//    if (!DXEndField(dxMesh))
//    {
//        DXSetError
//        (
//            ERROR_INTERNAL,
//            "dxFoamExec:foamReadSTL : Error in DXEndField"
//        );
//        return DXERROR;
//    }

    out[0] = (Object)dxMesh;
        */

    return OK;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // end extern "C"

// ************************************************************************* //

