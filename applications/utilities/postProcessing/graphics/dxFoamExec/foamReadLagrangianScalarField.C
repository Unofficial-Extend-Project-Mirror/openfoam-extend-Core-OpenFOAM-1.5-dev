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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<>
Error foamReadLagrangianField<scalar>
(
    const IOobject& fieldObject,
    const DXArray dxLagrangianPositions,
    DXField& dxPointField
)
{
    try
    {
        IOField<scalar> pf(fieldObject);

        float *lagrangianScalarArray = new float[pf.size()];

        forAll(pf, pi)
        {
            lagrangianScalarArray[pi] = pf[pi];
        }

        //Create an array of scalar data
        DXArray dxLagrangianScalarArray =
            DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 0, 1);

        if
        (
            !DXAddArrayData
            (
                dxLagrangianScalarArray,
                0,
                pf.size(),
                lagrangianScalarArray
            )
        )
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamReadLagrangianScalarField : Error in DXAddArrayData"
            );

            delete[] lagrangianScalarArray;

            return DXERROR;
        }

        delete[] lagrangianScalarArray;

        if 
        (
            !(dxPointField = createDxField
            (
                fieldObject.name(),
                dxLagrangianScalarArray,
                dxLagrangianPositions,
                NULL,
                "positions"
            ))
        )
        {
            return DXERROR;
        }

        Info<< "dxFoamExec:foamReadLagrangianField<scalar> : read field "
            << fieldObject.name() << endl;

        return OK;
    }
    catch (IOerror& ioErr) 
    {
        Info << endl << ioErr << endl << endl;

        DXSetError(ERROR_BAD_PARAMETER, "could not read field\n");

        return DXERROR;
    }
    catch (error& fErr) 
    {
        Info << endl << fErr << endl << endl;

        DXSetError(ERROR_BAD_PARAMETER, "could not read field\n");

        return DXERROR;
    }
    catch (...) 
    {
        DXSetError(ERROR_BAD_PARAMETER, "could not read field\n");

        return DXERROR;
    }
}


// ************************************************************************* //
