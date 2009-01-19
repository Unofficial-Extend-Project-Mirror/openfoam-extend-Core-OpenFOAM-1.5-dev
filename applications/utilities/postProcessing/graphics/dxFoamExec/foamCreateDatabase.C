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
#include "dxFvMesh.H"
#include "dxDatabase.H"
#include "long.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

extern "C"
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Error deleteDatabase(Pointer dxFoamDatabasePtr)
{
    dxDatabase* runTimePtr = (dxDatabase*)dxFoamDatabasePtr;

    Error dxError = OK;

    if (runTimePtr)
    {
        label refCount = runTimePtr->decCount();

        if (refCount == 0)
        {
            // delete database only now when no DXPrivate refers to it.
            delete runTimePtr;
        }
        else if (refCount < 0)
        {
            string msg("dxFoamExec:deleteDatabase : refCount on dxDatabase ");
            msg += name(long(runTimePtr));
            msg += " less than 0 : ";
            msg += name(refCount);

            DXSetError(ERROR_UNEXPECTED, (char *)msg.c_str());

            dxError = DXERROR;
        }
        else
        {
            //Info<< "dxFoamExec:deleteDatabase : "
            //    << "Not yet deleting database " << long(runTimePtr)
            //    << " since refCount = "
            //    << refCount << endl;
        }
    }
    else
    {
        DXSetError
        (
            ERROR_UNEXPECTED,
            "dxFoamExec:deleteDatabase : database not allocated"
        );
        
        dxError = DXERROR;
    }

    return dxError;
}


Error m_foamCreateDatabase(Object *in, Object *out)
{
    Info<< "dxFoamExec:foamCreateDatabase : reading database" << endl;

    char *rootDirChars = NULL;

    if (!in[0])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamCreateDatabase : missing root"
        );
        return DXERROR;
    }
    else if (!DXExtractString(in[0], &rootDirChars))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamCreateDatabase : root must be a string"
        );
        return DXERROR;
    }

    fileName rootDir = fileName(rootDirChars).expand();

    char *caseName = NULL;

    if (!in[1])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamCreateDatabase : missing caseName"
        );
        return DXERROR;
    }
    else if (!DXExtractString(in[1], &caseName))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamCreateDatabase : caseName must be a string"
        );
        return DXERROR;
    }


    dxDatabase* runTimePtr = NULL;
    try
    {
        runTimePtr = new dxDatabase
        (
            Time::controlDictName,
            rootDir,
            caseName
        );
    }
    catch (IOerror& ioErr) 
    {
        Info << endl << ioErr << endl << endl;

        DXSetError(ERROR_BAD_PARAMETER, "could not read database");

        return DXERROR;
    }
    catch (error& fErr) 
    {
        Info << endl << fErr << endl << endl;

        DXSetError(ERROR_BAD_PARAMETER, "could not read database");

        return DXERROR;
    }
    catch (...) 
    {
        DXSetError(ERROR_BAD_PARAMETER, "could not read database");

        return DXERROR;
    }

    if (runTimePtr)
    {
        dxDatabase& runTime = *runTimePtr;


        Info<< "dxFoamExec:foamCreateDatabase : created database "
            << long(runTimePtr) << " for " << caseName
            << endl;

        instantList times = Foam::Time::findTimes(runTime.path());

        if (times.size())
        {
            runTime.setTime(times[0], 0);
        }

        out[0] = (Object)DXNewPrivate(Pointer(runTimePtr), deleteDatabase);

        return OK;
    }
    else
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamCreateDatabase : Could not read database. "
            "Is case Foam2.2 compatible?"
        );

        out[0] = NULL;

        return DXERROR;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // end extern "C"

// ************************************************************************* //
