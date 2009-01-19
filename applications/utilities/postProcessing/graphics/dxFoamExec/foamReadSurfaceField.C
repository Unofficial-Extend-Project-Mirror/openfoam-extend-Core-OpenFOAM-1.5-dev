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
#include "surfaceFields.H"
#include "foamValid.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
Error foamReadSurfaceField
(
    const IOobject& fieldObject,
    const dxFvMesh& mesh,
    DXField& dxPointField
)
{
    try
    {
        GeometricField<Type, fvsPatchField, surfaceMesh> stf(fieldObject, mesh);

        label nCells = mesh.totalNCells();
        label nFaces = stf.size() + (nCells - mesh.nCells());

        const surfaceVectorField& faceCentres = mesh.Cf();

        float *faceCentreArray = new float[vector::nComponents*nFaces];
        float *surfaceTypeArray = new float[Type::nComponents*nFaces];

        label i=0;
        label j=0;
        forAll(stf, facei)
        {
            faceCentreArray[i++] = faceCentres[facei].x();
            faceCentreArray[i++] = faceCentres[facei].y();
            faceCentreArray[i++] = faceCentres[facei].z();

            for (direction d=0; d<Type::nComponents; d++)
            {
                surfaceTypeArray[j++] = stf[facei].component(d);
            }
        }

        const polyBoundaryMesh& patches = mesh.boundaryMesh();

        forAll (patches, patchi)
        {
            const polyPatch& faces = patches[patchi];

            const vectorField& pfc = faceCentres.boundaryField()[patchi];
            const Field<Type>& pf = stf.boundaryField()[patchi];

            if (valid(faces))
            {
                forAll(pf, facei)
                {
                    faceCentreArray[i++] = pfc[facei].x();
                    faceCentreArray[i++] = pfc[facei].y();
                    faceCentreArray[i++] = pfc[facei].z();

                    for (direction d=0; d<Type::nComponents; d++)
                    {
                        surfaceTypeArray[j++] = pf[facei].component(d);
                    }
                }
            }
        }


        DXArray dxFacePositions =
            DXNewArray
            (
                TYPE_FLOAT,
                CATEGORY_REAL,
                vector::rank,
                vector::nComponents
            );

        if (!DXAddArrayData(dxFacePositions, 0, nFaces, faceCentreArray))
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamReadSurfaceField : Error in DXAddArrayData"
            );

            delete[] faceCentreArray;
            delete[] surfaceTypeArray;

            return DXERROR;
        }

        delete[] faceCentreArray;

        if
        (
            !DXSetStringAttribute
            (
                reinterpret_cast<Object>(dxFacePositions),
                "dep",
                "positions"
            )
        )
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamReadSurfaceField : "
                "Error in DXSetStringAttribute dep"
            );

            delete[] surfaceTypeArray;

            return DXERROR;
        }


        //Create an array of vector data
        DXArray dxSurfaceTypeArray =
            DXNewArrayV(TYPE_FLOAT, CATEGORY_REAL, Type::rank, shape<Type>());

        if (!DXAddArrayData(dxSurfaceTypeArray, 0, nFaces, surfaceTypeArray))
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamReadSurfaceField : Error in DXAddArrayData"
            );

            delete[] surfaceTypeArray;

            return DXERROR;
        }

        delete[] surfaceTypeArray;

        if 
        (
            !(dxPointField = createDxField
            (
                fieldObject.name(),
                dxSurfaceTypeArray,
                dxFacePositions,
                NULL,
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
