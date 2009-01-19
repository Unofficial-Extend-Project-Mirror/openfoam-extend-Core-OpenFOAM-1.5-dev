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
#include "createLagrangianField.H"
#include "Cloud.H"
#include "passiveParticle.H"
#include "long.H"
#include "IOobjectList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

extern "C"
{


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Error m_foamChangeTime(Object *in, Object *out)
{
    Info<< "dxFoamExec:foamChangeTime : setting selected time" << endl;

    float newTimeValue = 0.0;

    if (!in[0])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamChangeTime : missing timeValue"
        );
        return DXERROR;
    }
    else if (!DXExtractFloat(in[0], &newTimeValue))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamChangeTime : timeValue must be a float"
        );
        return DXERROR;
    }


    dxDatabase* runTimePtr = NULL;

    if (!in[1])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamChangeTime : missing mesh"
        );
        return DXERROR;
    }
    else if (!(runTimePtr = (dxDatabase*)DXGetPrivateData((Private)in[1])))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamChangeTime : cannot get mesh from input"
        );
        return DXERROR;
    }

//-is not needed anymore. Stored on database.
//    if (!in[2])
//    {
//        DXSetError
//        (
//            ERROR_BAD_PARAMETER,
//            "dxFoamExec:foamChangeTime : missing mesh"
//        );
//        return DXERROR;
//    }
    dxDatabase& runTime = *runTimePtr;

    dxFvMesh& mesh = runTime.mesh();

    dxFvMesh* meshPtr = &mesh;


    // output dxDatabase is same as input one.
    out[1] = in[1];

    scalar oldTimeValue = runTime.value();

    // Set database time
    runTime.setTime
    (
        runTime.findClosestTime(newTimeValue), runTime.timeIndex() + 1
    );

    if
    (
        newTimeValue < oldTimeValue - SMALL
     || newTimeValue > oldTimeValue + SMALL
    )
    {
        Info<< "dxFoamExec:foamChangeTime : changing time from "
            << oldTimeValue << " to " << newTimeValue << endl;
        //
        // Topology or geometry-only changes
        //

        fileName facesDir(runTime.findInstance(meshPtr->meshDir(), "faces"));

        if (facesDir != meshPtr->facesDir())
        {
            // cells exists for time step

            // Delete old mesh
            if (runTime.validMesh())
            {
                dxFvMesh& mesh = runTime.mesh();

                Info<< "dxFoamExec:foamChangeTime : deleting old mesh "
                    << long(&mesh) << endl;

                delete &mesh;

                runTime.clearMesh();
            }

            try
            {
                // Create mesh
                meshPtr = new dxFvMesh
                (
                    runTime,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                );
            }
            catch (IOerror& ioErr) 
            {
                Info << endl << ioErr << endl << endl;

                DXSetError(ERROR_BAD_PARAMETER, "could not read mesh");

                return DXERROR;
            }
            catch (error& fErr) 
            {
                Info << endl << fErr << endl << endl;

                DXSetError(ERROR_BAD_PARAMETER, "could not read mesh");

                return DXERROR;
            }
            catch (...) 
            {
                DXSetError(ERROR_BAD_PARAMETER, "could not read mesh");

                return DXERROR;
            }

            meshPtr->writeStats(Info);

            // Add to dxDatabase
            runTime.setMesh(*meshPtr);

            // Construct new Time wrapper so as to evaluate for new mesh
            runTime.incCount();
        
            out[1] = (Object)DXNewPrivate(Pointer(runTimePtr), deleteDatabase);

            //Note: DXReference is needed. Don't know exactly why.
            //      Is since gets reevaluated for first rotation so you want
            //      to be able to return old mesh - which has to still have
            //      a valid DXreference count.
            out[2] = DXReference((Object)meshPtr->DXMesh());
        }
        else 
        {
            // Since we reuse old DX object make sure it does not get deleted.
            out[2] = DXReference((Object)meshPtr->DXMesh());

            bool hasMoved = false;

            if (!meshPtr->move(hasMoved))
            {
                return DXERROR;
            }
        }


        //
        // Lagrangian
        //

        // Delete old spray
        if (runTime.validField())
        {
            DXField field = runTime.field();

            Info<< "dxFoamExec:foamChangeTime : deleting old spray "
                << long(field) << endl;

            DXDelete((Object)field);

            runTime.clearField();
        }

        IOobjectList sprayObjects
        (
            *meshPtr,
            runTime.timeName(),
            Cloud<passiveParticle>::lagrangianSubDir
        );

        if (sprayObjects.size())
        {
            DXField field = createLagrangianField(runTime);
            runTime.setField(field);

            out[3] = DXReference((Object)field);

            Info<< "dxFoamExec:foamChangeTime : read spray " << long(field)
                << endl;
        }
        else
        {
            out[3] = NULL;
        }
    }
    else
    {
        // Since we reuse old DX object make sure it does not get deleted.
        out[2] = DXReference((Object)meshPtr->DXMesh());

        if (runTime.validField())
        {
            DXField field = runTime.field();

            // Since we reuse old DX object make sure it does not get deleted.
            out[3] = DXReference((Object)field);

            Info<< "dxFoamExec:foamChangeTime : reusing spray " << long(field)
                << endl;
        }
        else
        {
            out[3] = NULL;
        }
    }

    out[0] = in[0];

    return OK;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // end extern "C"

// ************************************************************************* //
