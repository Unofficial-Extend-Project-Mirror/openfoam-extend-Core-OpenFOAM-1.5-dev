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

#include "dxDatabase.H"
#include "dxFvMesh.H"
#include "Cloud.H"
#include "passiveParticle.H"
#include "error.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

DXField createLagrangianField
(
    const dxDatabase& runTime
)
{
    Info<< "dxFoamExec:foamChangeTime : reading spray for time "
        << runTime.timeName() << endl;

    // Create new FOAM Spray

    try
    {
        Cloud<passiveParticle> foamSpray(runTime.mesh());


        Info<< "dxFoamExec:foamChangeTime : read " << foamSpray.size()
            << " particles" << endl;

        float *positionVector = new float[3*foamSpray.size()];

        label i = 0;
        for
        (
            Cloud<passiveParticle>::iterator parcel = foamSpray.begin();
            parcel != foamSpray.end();
            ++parcel
        )
        {
            positionVector[i++] = parcel().position().x();
            positionVector[i++] = parcel().position().y();
            positionVector[i++] = parcel().position().z();
        }

        DXArray dxLagrangianPositions = DXNewArray
        (
            TYPE_FLOAT,
            CATEGORY_REAL,
            1,
            3
        );

        if
        (
            !DXAddArrayData
            (
                dxLagrangianPositions,
                0,
                foamSpray.size(),
                positionVector
            )
        )
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamChangeTime : Error in DXAddArrayData"
            );

            delete[] positionVector;

            return DXERROR;
        }

        delete[] positionVector;

        if 
        (
            !DXSetStringAttribute
            (
                reinterpret_cast<Object>(dxLagrangianPositions),
                "dep",
                "positions"
            )
        )
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamChangeTime : "
                "Error in DXSetStringAttribute dep"
            );
            return DXERROR;
        }

        DXField dxLagrangianPositionField = DXNewField();

        if (!dxLagrangianPositionField)
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamChangeTime : "
                "Error creating dxLagrangianPositionField"
            );
            return DXERROR;
        }

        if
        (
            !DXSetStringAttribute
            (
                reinterpret_cast<Object>(dxLagrangianPositionField),
                "name",
                "sprayData"
            )
        )
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamChangeTime : Error in DXSetStringAttribute name"
            );
            return DXERROR;
        }

        if
        (
            !DXSetComponentValue
            (
                dxLagrangianPositionField,
                "positions",
                reinterpret_cast<Object>(dxLagrangianPositions)
            )
        )
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamChangeTime : "
                "Error in DXSetComponentValue positions"
            );
            return DXERROR;
        }

        return dxLagrangianPositionField;
    }
    catch (IOerror& ioErr) 
    {
        Info << endl << ioErr << endl << endl;

        DXSetError(ERROR_BAD_PARAMETER, "could not read spray");

        return DXERROR;
    }
    catch (error& fErr) 
    {
        Info << endl << fErr << endl << endl;

        DXSetError(ERROR_BAD_PARAMETER, "could not read spray");

        return DXERROR;
    }
    catch(...)
    {
        DXSetError(ERROR_BAD_PARAMETER, "could not read spray");

        return DXERROR;
    }
}


// ************************************************************************* //
