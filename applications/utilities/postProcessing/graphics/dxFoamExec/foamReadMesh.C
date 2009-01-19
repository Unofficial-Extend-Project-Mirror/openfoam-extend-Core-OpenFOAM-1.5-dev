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

Error m_foamReadMesh(Object *in, Object *out)
{
    Info<< "dxFoamExec:foamReadMesh : reading mesh" << endl;

    dxDatabase* runTimePtr = NULL;

    if (!in[0])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamReadMesh : missing database"
        );
        return DXERROR;
    }
    else if (!(runTimePtr = (dxDatabase*)DXGetPrivateData((Private)in[0])))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamReadMesh : cannot get database from input"
        );
        return DXERROR;
    }

    dxDatabase& runTime = *runTimePtr;


    dxFvMesh* meshPtr = NULL;

    if (runTime.validMesh())
    {
        dxFvMesh& mesh = runTime.mesh();

        meshPtr = &mesh;

        Info<< "dxFoamExec:foamReadMesh : reusing old mesh "
            << long(meshPtr) << endl;

        out[0] = in[0];
    }
    else
    {
        try
        {
            // Create mesh
            meshPtr = new dxFvMesh
            (
                runTime,
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            );

            Info<< "dxFoamExec:foamReadMesh : read new mesh ("
                << long(meshPtr) << ')' << endl;

            meshPtr->writeStats(Info);

            // Add to dxDatabase
            runTime.setMesh(*meshPtr);
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
        catch(...)
        {
            DXSetError(ERROR_BAD_PARAMETER, "could not read mesh");

            return DXERROR;
        }

        // Construct new Time wrapper so as to evaluate for new mesh
        runTime.incCount();

        Private dxFoamMesh = DXNewPrivate(Pointer(runTimePtr), deleteDatabase);
        out[0] = (Object)dxFoamMesh;
    }

    // Used to pass DXMesh but not anymore.
    out[1] = NULL;

    return OK;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // end extern "C"

// ************************************************************************* //
