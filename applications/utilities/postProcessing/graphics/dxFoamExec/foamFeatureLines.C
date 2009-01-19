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
#include "dxDatabase.H"
#include "dxFvMesh.H"
#include "featureLines.H"
#include "foamValid.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

extern "C"
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Error m_foamFeatureLines(Object *in, Object *out)
{
    Info<< "dxFoamExec:foamFeatureLines : extracting edges" << endl;

    dxDatabase* runTimePtr = NULL;

    if (!in[0])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamFeatureLines : missing mesh"
        );
        return DXERROR;
    }
    if (!(runTimePtr = (dxDatabase*)DXGetPrivateData((Private)in[0])))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamFeatureLines : cannot get mesh from input"
        );
        return DXERROR;
    }

    dxFvMesh& mesh = runTimePtr->mesh();


    DXArray dxPositions;
    if (!in[1])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamFeatureLines : missing mesh"
        );
        return DXERROR;
    }
    else if
    (
        !(dxPositions = (DXArray)DXExtract(in[1], "positions"))
    )
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamFeatureLines : "
            "missing positions in mesh"
        );
        return DXERROR;
    }




    // Get list of patch numbers to suppress

    int nItems = 0;

    int *dxPatchNumbers = NULL;

    if (in[2])
    {

        Type arrType;

        if
        (
            !DXGetArrayInfo
            (
                (DXArray)in[2],
                &nItems,
                &arrType,
                NULL,
                NULL,
                NULL
            )
         || (nItems == 0) || (arrType != TYPE_INT)
        )
        {
            DXSetError
            (
                ERROR_BAD_PARAMETER,
                "dxFoamExec:foamFeatureLines : "
                "patch array is not array or empty array or not integer array"
            );
            return DXERROR;
        }

        if (!(dxPatchNumbers = (int *)DXGetArrayData((DXArray)in[2])))
        {
            DXSetError
            (
                ERROR_BAD_PARAMETER,
                "dxFoamExec:foamFeatureLines : "
                "cannot get patch array from input"
            );
            return DXERROR;
        }
    }


    // Get feature angle

    float featureAngle = 0.0;

    if (!in[3])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamChangeTime : missing feature angle"
        );
        return DXERROR;
    }
    else if (!DXExtractFloat(in[3], &featureAngle))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamFeatureLines : featureAngle must be a float"
        );
        return DXERROR;
    }


    // Combine feature lines of all patches into single Dx array

    const polyBoundaryMesh& patches = mesh.boundaryMesh();

    // Create temporary array big enough to hold sum of all patch edges
    label nTotalEdges = mesh.nEdges()*2;

    int *linesArray = new int[2*nTotalEdges];
    label verti = 0;

    float *dataArray = new float[nTotalEdges];
    label linei = 0;


    label suppressi = 0;

    // numbers in dxPatchNumbers are dx patch numbers
    // (so with empty patches filtered out)
    // so keep dxPatch number corresponding to foam patch number.
    label dxPatchi = 0;

    forAll (patches, patchi)
    {
        const polyPatch& faces = patches[patchi];

        if (!valid(faces))
        {
            // Skip empty patch
            continue;
        }

        // search remaining dxPatchNumbers to suppress
        bool suppressPatch = false;
        for (int j=suppressi; j<nItems; j++)
        {
            if (dxPatchi == dxPatchNumbers[j])
            {
                suppressi = j;
                suppressPatch = true;
                break;
            }
        }

        dxPatchi++;

        if (suppressPatch)
        {
            continue;
        }


        try
        {
            featureLines fLines(faces);

            edgeList subEdges = fLines.extractEdges(featureAngle);

            const labelList& mPoints =  faces.meshPoints();

            forAll(subEdges, edgei)
            {
                label startVert = mPoints[subEdges[edgei].start()];
                label endVert = mPoints[subEdges[edgei].end()];

                //Info<< "start vertex=" << startVert
                //    << "    end vertex=" << endVert
                //    << endl;

                linesArray[verti++] = startVert;
                linesArray[verti++] = endVert;

                dataArray[linei++] = patchi;
            }
        }
        catch (IOerror& ioErr) 
        {
            Info << endl << ioErr << endl << endl;

            DXSetError(ERROR_BAD_PARAMETER, "could not extract featureLines");

            delete[] linesArray;
            delete[] dataArray;

            return DXERROR;
        }
        catch (error& fErr) 
        {
            Info << endl << fErr << endl << endl;

            DXSetError(ERROR_BAD_PARAMETER, "could not extract featureLines");

            delete[] linesArray;
            delete[] dataArray;

            return DXERROR;
        }
        catch (...) 
        {
            DXSetError(ERROR_BAD_PARAMETER, "could not extract featureLines");

            delete[] linesArray;
            delete[] dataArray;

            return DXERROR;
        }
    }


    // Create array objects for cell connections
    // -----------------------------------------
    DXArray dxCellCons = DXNewArray(TYPE_INT, CATEGORY_REAL, 1, 2);

    if (!DXAddArrayData(dxCellCons, 0, linei, linesArray))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamFeatureLines : Error in DXAddArrayData"
        );

        delete[] linesArray;
        delete[] dataArray;

        return DXERROR;
    }


    delete[] linesArray;

    if (!DXSetStringAttribute((Object)dxCellCons, "element type", "lines"))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamFeatureLines : "
            "Error in DXSetStringAttribute element type"
        );

        delete[] dataArray;

        return DXERROR;
    }

    if (!DXSetStringAttribute((Object)dxCellCons, "ref", "positions"))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamFeatureLines : "
            "Error in DXSetStringAttribute ref"
        );

        delete[] dataArray;

        return DXERROR;
    }

    if (!DXSetStringAttribute((Object)dxCellCons, "dep", "connections"))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamFeatureLines : "
            "Error in DXSetStringAttribute dep"
        );
        delete[] dataArray;

        return DXERROR;
    }

    // Create array objects for data
    // -----------------------------
    DXArray dxCellData = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 0, 1);

    if (!DXAddArrayData(dxCellData, 0, linei, dataArray))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamFeatureLines : Error in DXAddArrayData"
        );

        delete[] dataArray;

        return DXERROR;
    }


    delete[] dataArray;

    if (!DXSetStringAttribute((Object)dxCellData, "dep", "connections"))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamFeatureLines : "
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
            "dxFoamExec:foamFeatureLines : Error creating dxMesh"
        );
        return DXERROR;
    }

    if (!DXSetStringAttribute((Object)dxMesh, "name","featureLines"))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamFeatureLines : Error in DXSetStringAttribute name"
        );
        return DXERROR;
    }

    if (!DXSetComponentValue(dxMesh, "positions", (Object)dxPositions))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamFeatureLines : "
            "Error in DXSetComponentValue positions"
        );
        return DXERROR;
    }

    if (!DXSetComponentValue(dxMesh, "connections", (Object)dxCellCons))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamFeatureLines : "
            "Error in DXSetComponentValue connections"
        );
        return DXERROR;
    }

    if (!DXSetComponentValue(dxMesh, "data", (Object)dxCellData))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamFeatureLines : "
            "Error in DXSetComponentValue data"
        );
        return DXERROR;
    }

    out[0] = (Object)dxMesh;

    return OK;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // end extern "C"

// ************************************************************************* //
