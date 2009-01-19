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

template<>
Error foamReadPointField<scalar>
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
        pointScalarField psf(fieldObject, pMesh);

        float *pointScalarArray = new float[mesh.totalNPoints()];

        forAll(psf, i)
        {
            pointScalarArray[i] = psf[i];
        }

        label i = mesh.nPoints();

        const cellList& cellFaces = mesh.cells();
        const faceList& faces = mesh.faces();

        const labelList& addPointCellLabels = mesh.addPointCellLabels();
        forAll (addPointCellLabels, j)
        {
            labelList cellPoints =
                cellFaces[addPointCellLabels[j]].labels(faces);

            scalar av = 0.0;
            forAll (cellPoints, pointi)
            {
                av += psf[cellPoints[pointi]];
            }

            av /= cellPoints.size();

            pointScalarArray[i++] = av;
        }

        //Create an array of scalar data
        DXArray dxPointScalarArray =
            DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 0, 1);

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
                "dxFoamExec:foamReadPointScalarField : Error in DXAddArrayData"
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
    catch(...)
    {
        DXSetError(ERROR_BAD_PARAMETER, "could not read field");

        return DXERROR;
    }
}


// ************************************************************************* //
