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
#include "pointFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
Error foamReadPointField
(
    const IOobject& fieldObject,
    const dxFvMesh& mesh,
    const DXArray dxPositions,
    const DXArray dxConnections,
    DXField& dxPointField
)
{
    try
    {
        pointMesh pMesh(mesh);
        GeometricField<Type, pointPatchField, pointMesh> ptf
        (
            fieldObject,
            pMesh
        );

        float *pointTypeArray =
            new float[Type::nComponents*mesh.totalNPoints()];

        label i=0;
        forAll(ptf, verti)
        {
            for (direction d=0; d<Type::nComponents; d++)
            {
                pointTypeArray[i++] = ptf[verti].component(d);
            }
        }

        const cellList& cellFaces = mesh.cells();
        const faceList& faces = mesh.faces();

        const labelList& addPointCellLabels = mesh.addPointCellLabels();
        forAll (addPointCellLabels, j)
        {
            labelList cellPoints =
                cellFaces[addPointCellLabels[j]].labels(faces);

            Type av = pTraits<Type>::zero;
            forAll (cellPoints, pointi)
            {
                av += ptf[cellPoints[pointi]];
            }

            av /= cellPoints.size();

            for (direction d=0; d<Type::nComponents; d++)
            {
                pointTypeArray[i++] = av.component(d);
            }
        }


        //Create an array of vector data
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
                "dxFoamExec:foamReadPointField : Error in DXAddArrayData"
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
    catch(...)
    {
        DXSetError(ERROR_BAD_PARAMETER, "could not read field");

        return DXERROR;
    }
}


// ************************************************************************* //
