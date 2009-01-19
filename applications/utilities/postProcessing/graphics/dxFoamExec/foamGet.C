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
#include "fileNameList.H"
#include "IOobjectList.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "pointFields.H"
#include "tetPointFields.H"
#include "IFstream.H"
#include "dxDatabase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

extern "C"
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Error m_foamGetRoots(Object *in, Object *out)
{
    fileName rootsFileName = dotFoam(Time::controlDictName);

    Info<< "dxFoamExec:foamGetRoots : reading roots from file " << rootsFileName
        << endl;

    IFstream rootsFile(rootsFileName);
    fileNameList rootsList(dictionary(rootsFile).lookup("caseRoots"));

    DXArray dxRootsArray = NULL;

    if (rootsList.size())
    {
        const char** rootsStrings = new const char*[rootsList.size()];

        for (int i=0; i<rootsList.size(); i++)
        {
            rootsStrings[i] = rootsList[i].c_str();
        }

        dxRootsArray = DXMakeStringListV(rootsList.size(),(char**)rootsStrings);
        delete[] rootsStrings;

        if (!dxRootsArray)
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamGetRoots : Error in DXMakeStringList"
            );
            return DXERROR;
        }
    }

    out[0] = (Object)dxRootsArray;

    return OK;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int fileNameCompare(const void* one, const void* two)
{
    return strcmp
    (
        ((const fileName*) one)->c_str(),
        ((const fileName*) two)->c_str()
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Error m_foamGetCases(Object *in, Object *out)
{
    Info<< "dxFoamExec:foamGetCases : reading cases for root" << endl;

    char *rootDirChars = NULL;

    if (!in[0])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetCases : missing root"
        );
        return DXERROR;
    }
    else if (!DXExtractString(in[0], &rootDirChars))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetCases : root must be a string"
        );
        return DXERROR;
    }

    fileName rootDir = fileName(rootDirChars).expand();
    fileNameList caseList = readDir(rootDir, fileName::DIRECTORY);

    DXArray dxCaseArray = NULL;

    if (caseList.size())
    {
        // Sort the times values in accending order
        sort(caseList);

        const char** caseStrings = new const char*[caseList.size()];

        label goodCases = 0;
        forAll(caseList, i)
        {
            if 
            (
                IFstream(rootDir/caseList[i]/"system/controlDict").good()
             || (
                    caseList[i](0,9) == "processor"
                 && IFstream(rootDir/caseList[i]/"../system/controlDict").good()
                )
            )
            {
                caseStrings[goodCases++] = caseList[i].c_str();
            }
        }

        if (goodCases)
        {
            dxCaseArray = DXMakeStringListV(goodCases, (char**)caseStrings);

            if (!dxCaseArray)
            {
                DXSetError
                (
                    ERROR_INTERNAL,
                    "dxFoamExec:foamGetCases : Error in DXMakeStringList"
                );

                delete[] caseStrings;

                return DXERROR;
            }
        }

        delete[] caseStrings;
    }


    out[0] = (Object)dxCaseArray;

    return OK;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Error m_foamGetTimes(Object *in, Object *out)
{
    Info<< "dxFoamExec:foamGetTimes : reading timesteps" << endl;

    dxDatabase* runTimePtr = NULL;

    if (!in[0])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetTimes : missing database"
        );
        return DXERROR;
    }
    else if (!(runTimePtr = (dxDatabase*)DXGetPrivateData((Private)in[0])))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetTimes : cannot get database from input"
        );
        return DXERROR;
    }

    dxDatabase& runTime = *runTimePtr;


    instantList times = Foam::Time::findTimes(runTime.path());

    DXArray dxTimeNames = NULL;
    DXArray dxTimeValues = NULL;

    if (times.size() > 0)
    {
        label nTimes = times.size();
        float* timeValues = new float[nTimes];
        const char** timeNameStringsConst = new const char*[nTimes];

        forAll(times, i)
        {
            timeNameStringsConst[i] = times[i].name().c_str();
            timeValues[i] = times[i].value();
        }

        char** timeNameStrings = const_cast<char**>(timeNameStringsConst);

        dxTimeNames =
            DXMakeStringListV
            (
                nTimes,
                timeNameStrings
            );

        delete[] timeNameStrings;

        if (!dxTimeNames)
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamGetTimes : Error in DXMakeStringList"
            );

            delete[] timeValues;

            return DXERROR;
        }

        dxTimeValues = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 0, 1);

        if
        (
            !DXAddArrayData
            (
                dxTimeValues,
                0, 
                nTimes,
                timeValues
            )
        )
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamGetTimes : Error in DXAddArrayData"
            );

            delete[] timeValues;
             
            return DXERROR;
        }

        delete[] timeValues;
    }


    out[0] = (Object)dxTimeValues;
    out[1] = (Object)dxTimeNames;

    return OK;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Error m_foamGetFieldNames(Object *in, Object *out)
{
    Info<< "dxFoamExec:foamGetFieldNames : reading field names" << endl;

    dxDatabase* runTimePtr = NULL;

    if (!in[0])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetFieldNames : missing database"
        );
        return DXERROR;
    }
    else if (!(runTimePtr = (dxDatabase*)DXGetPrivateData((Private)in[0])))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetFieldNames : cannot get database from input"
        );
        return DXERROR;
    }

    dxDatabase& runTime = *runTimePtr;


    float timeValue = 0.0;

    if (!in[1])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetFieldNames : missing timeValue"
        );
        return DXERROR;
    }
    else if (!DXExtractFloat(in[1], &timeValue))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamNearestTime : timeValue must be a float"
        );
        return DXERROR;
    }

    char *fieldTypeString = NULL;

    if (!in[2])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetFieldNames : missing fieldType"
        );
        return DXERROR;
    }
    else if (!DXExtractString(in[2], &fieldTypeString))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamGetFieldNames : fieldType must be a string"
        );
        return DXERROR;
    }

    word fieldType(fieldTypeString);


    DXArray dxFieldNameArray = NULL;

    if
    (
        fieldType == volScalarField::typeName
     || fieldType == volVectorField::typeName
     || fieldType == volTensorField::typeName
     || fieldType == surfaceScalarField::typeName
     || fieldType == surfaceVectorField::typeName
     || fieldType == surfaceTensorField::typeName
     || fieldType == pointScalarField::typeName
     || fieldType == pointVectorField::typeName
     || fieldType == pointTensorField::typeName
     || fieldType == tetPointScalarField::typeName
     || fieldType == tetPointVectorField::typeName
     || fieldType == tetPointTensorField::typeName
    )
    {
        IOobjectList objects(runTime, runTime.timeName());

        wordList fieldNames = objects.names(fieldType);
        label nFields = fieldNames.size();

        if (nFields)
        {
            const char** fieldNameStringsConst = new const char*[nFields];

            forAll(fieldNames, i)
            {
                fieldNameStringsConst[i] = fieldNames[i].c_str();
            }

            char** fieldNameStrings =
                const_cast<char**>(fieldNameStringsConst);

            dxFieldNameArray = DXMakeStringListV
            (
                nFields,
                fieldNameStrings
            );

            delete[] fieldNameStrings;

            if (!dxFieldNameArray)
            {
                DXSetError
                (
                    ERROR_INTERNAL,
                    "dxFoamExec:foamGetFieldNames : Error in DXMakeStringList"
                );
                return DXERROR;
            }
        }
    }
    else if 
    (
        fieldType == "scalarField"
     || fieldType == "vectorField"
     || fieldType == "tensorField"
    )
    {
        IOobjectList objects(runTime, runTime.timeName(), "lagrangian");

        wordList fieldNames = objects.names(fieldType);

        label nFields = fieldNames.size();

        if (nFields)
        {
            const char** fieldNameStringsConst = new const char*[nFields];

            forAll(fieldNames, i)
            {
                fieldNameStringsConst[i] = fieldNames[i].c_str();
            }

            char** fieldNameStrings =
                const_cast<char**>(fieldNameStringsConst);

            dxFieldNameArray = DXMakeStringListV
            (
                nFields,
                fieldNameStrings
            );

            delete[] fieldNameStrings;

            if (!dxFieldNameArray)
            {
                DXSetError
                (
                    ERROR_INTERNAL,
                    "dxFoamExec:foamGetFieldNames : Error in DXMakeStringList"
                );
                return DXERROR;
            }
        }
    }
    else
    {
        DXSetError
        (
            ERROR_NOT_IMPLEMENTED,
            (char*)
            (
                "dxFoamExec:foamGetFieldNames : fieldType " + fieldType
              + " not supported"
            ).c_str()
        );
        return DXERROR;
    }

    out[0] = (Object)dxFieldNameArray;

    return OK;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // end extern "C"

// ************************************************************************* //
