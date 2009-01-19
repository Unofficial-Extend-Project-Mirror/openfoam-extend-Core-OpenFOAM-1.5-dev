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

Error m_foamNearestTime(Object *in, Object *out)
{
    Info<< "dxFoamExec:foamNearestTime : selecting nearest time" << endl;

    if (!in[0])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamNearestTime : missing timeValues"
        );
        return DXERROR;
    }

    int nItems = 0;
    if 
    (
        !DXGetArrayInfo((DXArray)in[0], &nItems, NULL, NULL, NULL, NULL)
     || (nItems == 0)
    )
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamNearestTime : "
            "timeValues is not array or empty array"
        );
        return DXERROR;
    }

    float *timeValues = (float *)DXGetArrayData((DXArray)in[0]);
    if (!timeValues)
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamNearestTime : timeValues must be an array"
        );
        return DXERROR;
    }

    float timeEstimate = 0.0;
    if (!DXExtractFloat(in[1], &timeEstimate))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamNearestTime : timeEstimate must be a float"
        );
        return DXERROR;
    }

    int i;
    for (i=0; i<nItems; i++)
    {
         if (timeValues[i] > timeEstimate)
         {
             break;
         }
    }
    int nearestIndex = 0;
    if (i == 0)
    {
        nearestIndex = i;
    }
    else if
    (
        mag(timeValues[i] - timeEstimate) < (timeEstimate - timeValues[i-1])
    )
    {
        nearestIndex = i;
    }
    else
    {
        nearestIndex = i-1;
    }


    DXArray dxNearestIndex = DXNewArray(TYPE_INT, CATEGORY_REAL, 0, 1);
    if
    (
        !DXAddArrayData
        (
            dxNearestIndex,
            0, 
            1,
            &nearestIndex
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamNearestTime : "
            "Error in DXAddArrayData to dxNearestIndex"
        );         
        return DXERROR;
    }

    DXArray dxNearestTime = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 0, 1);
    if
    (
        !DXAddArrayData
        (
            dxNearestTime,
            0, 
            1,
            &timeValues[nearestIndex]
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:foamNearestTime : "
            "Error in DXAddArrayData to dxNearestTime"
        );
        return DXERROR;
    }

    out[0] = (Object)dxNearestTime;
    out[1] = (Object)dxNearestIndex;

    Info<< "dxFoamExec:foamNearestTime : "
           "found nearest time " << timeValues[nearestIndex]
        << endl;

    return OK;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // end extern "C"

// ************************************************************************* //
