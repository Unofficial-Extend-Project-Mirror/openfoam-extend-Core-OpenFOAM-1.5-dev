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

template<class Type>
Error foamReadVolField
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
        GeometricField<Type, fvPatchField, volMesh> vtf(fieldObject, mesh);

        label totalNCells = mesh.totalNCells();

        // Create vertex array
        float *volTypeArray = new float[Type::nComponents*totalNCells];

        label i = 0;
        forAll(vtf, celli)
        {
            for (direction d=0; d<Type::nComponents; d++)
            {
                volTypeArray[i++] = vtf[celli].component(d);
            }
        }

        const labelList& superCells = mesh.superCells();
        forAll (superCells, j)
        {
            for (direction d=0; d<Type::nComponents; d++)
            {
                volTypeArray[i++] = vtf[superCells[j]].component(d);
            }
        }

        const polyBoundaryMesh& patches = mesh.boundaryMesh();

        forAll (patches, patchi)
        {
            const polyPatch& faces = patches[patchi];
            const Field<Type>& pf = vtf.boundaryField()[patchi];

            if (valid(faces))
            {
                forAll(pf, facei)
                {
                    for (direction d=0; d<Type::nComponents; d++)
                    {
                        volTypeArray[i++] = pf[facei].component(d);
                    }
                }
            }
        }

        const labelList& superFacePatches = mesh.superFacePatches();
        const labelList& superFaces = mesh.superFaces();
        forAll (superFacePatches, j)
        {
            for (direction d=0; d<Type::nComponents; d++)
            {
                volTypeArray[i++] =
                    vtf.boundaryField()[superFacePatches[j]][superFaces[j]]
                   .component(d);
            }
        }

        //Create an array of Type data
        DXArray dxVolTypeArray =
            DXNewArrayV(TYPE_FLOAT, CATEGORY_REAL, Type::rank, shape<Type>());

        if (!DXAddArrayData(dxVolTypeArray, 0, totalNCells, volTypeArray))
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamReadVolField : Error in DXAddArrayData"
            );

            delete[] volTypeArray;

            return DXERROR;
        }

        delete[] volTypeArray;

        if 
        (
            !(dxVolField = createDxField
            (
                fieldObject.name(),
                dxVolTypeArray,
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
                "dxFoamExec:foamReadVolField : Error in DXEndField"
            );
            return DXERROR;
        }


        GeometricField<Type, pointPatchField, pointMesh>  ptf
        (
            mesh.interpolate(vtf)
        );


        float *pointTypeArray = new float[Type::nComponents*mesh.totalNPoints()];

        i=0;
        forAll(ptf, verti)
        {
            for (direction d=0; d<Type::nComponents; d++)
            {
                pointTypeArray[i++] = ptf[verti].component(d);
            }
        }

        const labelList& addPointCellLabels = mesh.addPointCellLabels();
        forAll (addPointCellLabels, j)
        {
            for (direction d=0; d<Type::nComponents; d++)
            {
                pointTypeArray[i++] = vtf[addPointCellLabels[j]].component(d);
            }
        }

        //Create an array of Type data
        DXArray dxPointTypeArray =
            DXNewArrayV(TYPE_FLOAT, CATEGORY_REAL, Type::rank, shape<Type>());

        if
        (
            !DXAddArrayData
            (
                dxPointTypeArray,
                0,
                mesh.totalNPoints(),
                pointTypeArray
            )
        )
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamReadVolField : Error in DXAddArrayData"
            );

            delete[] pointTypeArray;

            return DXERROR;
        }

        delete[] pointTypeArray;

        if 
        (
            !(dxPointField = createDxField
            (
                fieldObject.name(),
                dxPointTypeArray,
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
