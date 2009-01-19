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

template<class Type>
Error foamReadLagrangianField
(
    const IOobject& fieldObject,
    const DXArray dxLagrangianPositions,
    DXField& dxPointField
)
{
    try
    {
        IOField<Type> pf(fieldObject);

        float *lagrangianTypeArray = new float[Type::nComponents*pf.size()];

        label i = 0;
        forAll(pf, pi)
        {
            for (direction d=0; d<Type::nComponents; d++)
            {
                lagrangianTypeArray[i++] = pf[pi].component(d);
            }
        }

        //Create an array of type data
        DXArray dxLagrangianTypeArray =
            DXNewArrayV(TYPE_FLOAT, CATEGORY_REAL, Type::rank, shape<Type>());

        if
        (
            !DXAddArrayData
            (
                dxLagrangianTypeArray,
                0,
                pf.size(),
                lagrangianTypeArray
            )
        )
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamReadLagrangianField : Error in DXAddArrayData"
            );

            delete[] lagrangianTypeArray;

            return DXERROR;
        }

        delete[] lagrangianTypeArray;

        if 
        (
            !(dxPointField = createDxField
            (
                fieldObject.name(),
                dxLagrangianTypeArray,
                dxLagrangianPositions,
                NULL,
                "positions"
            ))
        )
        {
            return DXERROR;
        }

        Info<< "dxFoamExec:foamReadLagrangianField : read field "
            << fieldObject.name() << endl;

        return OK;
    }
    catch (IOerror& ioErr) 
    {
        Info << endl << ioErr << endl << endl;

        DXSetError(ERROR_BAD_PARAMETER, "could not read lagrangian field");

        return DXERROR;
    }
    catch (error& fErr) 
    {
        Info << endl << fErr << endl << endl;

        DXSetError(ERROR_BAD_PARAMETER, "could not read lagrangian field");

        return DXERROR;
    }
    catch (...) 
    {
        DXSetError(ERROR_BAD_PARAMETER, "could not read lagrangian field");

        return DXERROR;
    }
}


// ************************************************************************* //
