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

template<>
Error foamReadSurfaceField<scalar>
(
    const IOobject& fieldObject,
    const dxFvMesh& mesh,
    DXField& dxPointField
)
{
    try
    {
        surfaceScalarField ssf(fieldObject, mesh);

        label nCells = mesh.totalNCells();
        label nFaces = ssf.size() + (nCells - mesh.nCells());

        const surfaceVectorField& faceCentres = mesh.Cf();
        const surfaceVectorField& Sf = mesh.Sf();
        const surfaceScalarField& magSf = mesh.magSf();

        float *faceCentreArray = new float[3*nFaces];
        float *surfaceVectorArray = new float[3*nFaces];

        label i=0;
        forAll(ssf, facei)
        {
            vector fluxVector = ssf[facei]*Sf[facei]/magSf[facei];

            faceCentreArray[i] = faceCentres[facei].x();
            surfaceVectorArray[i++] = fluxVector.x();

            faceCentreArray[i] = faceCentres[facei].y();
            surfaceVectorArray[i++] = fluxVector.y();

            faceCentreArray[i] = faceCentres[facei].z();
            surfaceVectorArray[i++] = fluxVector.z();
        }

        const polyBoundaryMesh& patches = mesh.boundaryMesh();

        forAll (patches, patchi)
        {
            const polyPatch& faces = patches[patchi];

            const vectorField& pfc = faceCentres.boundaryField()[patchi];
            const vectorField& pSf = Sf.boundaryField()[patchi];
            const scalarField& pMagSf = magSf.boundaryField()[patchi];

            const scalarField& pf = ssf.boundaryField()[patchi];

            if (valid(faces))
            {
                forAll(pf, facei)
                {
                    vector fluxVector = pf[facei]*pSf[facei]/pMagSf[facei];

                    faceCentreArray[i] = pfc[facei].x();
                    surfaceVectorArray[i++] = fluxVector.x();

                    faceCentreArray[i] = pfc[facei].y();
                    surfaceVectorArray[i++] = fluxVector.y();

                    faceCentreArray[i] = pfc[facei].z();
                    surfaceVectorArray[i++] = fluxVector.z();
                }
            }
        }


        DXArray dxFacePositions = DXNewArray
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
                "dxFoamExec:foamReadSurfaceScalarField : "
                "Error in DXAddArrayData"
            );

            delete[] faceCentreArray;
            delete[] surfaceVectorArray;

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
                "dxFoamExec:foamReadSurfaceScalarField : "
                "Error in DXSetStringAttribute dep"
            );

            delete[] surfaceVectorArray;

            return DXERROR;
        }


        //Create an array of vector data
        DXArray dxSurfaceVectorArray = DXNewArray
        (
            TYPE_FLOAT,
            CATEGORY_REAL,
            vector::rank,
            vector::nComponents
        );

        if
        (
            !DXAddArrayData
            (
                dxSurfaceVectorArray,
                0,
                nFaces,
                surfaceVectorArray
            )
        )
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:foamReadSurfaceScalarField : "
                "Error in DXAddArrayData"
            );

            delete[] surfaceVectorArray;

            return DXERROR;
        }

        delete[] surfaceVectorArray;

        if 
        (
            !(dxPointField = createDxField
            (
                fieldObject.name(),
                dxSurfaceVectorArray,
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
