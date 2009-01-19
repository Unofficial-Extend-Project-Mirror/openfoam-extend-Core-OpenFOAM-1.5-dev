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
#include "volFields.H"
#include "foamValid.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<>
Error foamReadVolField<scalar>
(
    const IOobject& fieldObject,
    const dxFvMesh& mesh,
    const DXArray dxPositions,
    const DXArray dxConnections,
    DXField& dxVolField,
    DXField& dxPointField
)
{
    try
    {
        volScalarField vsf(fieldObject, mesh);

        label totalNCells = mesh.totalNCells();

        // Create vertex array
        float *volScalarArray = new float[totalNCells];

        label i = 0;
        forAll(vsf, celli)
        {
            volScalarArray[i++] = vsf[celli];
        }

        const labelList& superCells = mesh.superCells();
        forAll (superCells, j)
        {
            volScalarArray[i++] = vsf[superCells[j]];
        }

        const polyBoundaryMesh& patches = mesh.boundaryMesh();

        forAll (patches, patchi)
        {
            const polyPatch& faces = patches[patchi];
            const scalarField& pf = vsf.boundaryField()[patchi];

            if (valid(faces))
            {
                forAll(pf, facei)
                {
                    volScalarArray[i++] = pf[facei];
                }
            }
        }

        const labelList& superFacePatches = mesh.superFacePatches();
        const labelList& superFaces = mesh.superFaces();
        forAll (superFacePatches, j)
        {
            volScalarArray[i++] =
                vsf.boundaryField()[superFacePatches[j]][superFaces[j]];
        }


        // Create an array of scalar data
        DXArray dxVolScalarArray = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 0, 1);

        if (!DXAddArrayData(dxVolScalarArray, 0, totalNCells, volScalarArray))
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamReadVolField<scalar> : Error in DXAddArrayData"
            );

            delete[] volScalarArray;

            return DXERROR;
        }

        delete[] volScalarArray;

        if 
        (
           !(dxVolField = createDxField
            (
                fieldObject.name(),
                dxVolScalarArray,
                dxPositions,
                dxConnections,
                "connections"
            ))
        )
        {
            return DXERROR;
        }

        if (!DXEndField(dxVolField))
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamReadVolField<scalar> : Error in DXEndField"
            );
            return DXERROR;
        }


        pointScalarField psf
        (
            mesh.interpolate(vsf)
        );

        float *pointScalarArray = new float[mesh.totalNPoints()];

        i=0;
        forAll(psf, verti)
        {
            pointScalarArray[i++] = psf[verti];
        }

        const labelList& addPointCellLabels = mesh.addPointCellLabels();
        forAll (addPointCellLabels, j)
        {
            pointScalarArray[i++] = vsf[addPointCellLabels[j]];
        }


        //Create an array of scalar data
        DXArray dxPointScalarArray = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 0, 1);

        if
        (
            !DXAddArrayData
            (
                dxPointScalarArray,
                0,
                mesh.totalNPoints(),
                pointScalarArray
            )
        )
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamReadVolField<scalar> : Error in DXAddArrayData"
            );

            delete[] pointScalarArray;

            return DXERROR;
        }

        delete[] pointScalarArray;

        if 
        (
            !(dxPointField = createDxField
            (
                fieldObject.name(),
                dxPointScalarArray,
                dxPositions,
                dxConnections,
                "positions"
            ))
        )
        {
            return DXERROR;
        }

        return OK;
    }
    catch (IOerror& ioErr) 
    {
        Info << endl << ioErr << endl << endl;

        DXSetError(ERROR_BAD_PARAMETER, "could not read field");

        return DXERROR;
    }
    catch (error& fErr) 
    {
        Info << endl << fErr << endl << endl;

        DXSetError(ERROR_BAD_PARAMETER, "could not read field");

        return DXERROR;
    }
    catch (...) 
    {
        DXSetError(ERROR_BAD_PARAMETER, "could not read field");

        return DXERROR;
    }
}


// ************************************************************************* //
