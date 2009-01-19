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

\*---------------------------------------------------------------------------*/

#include "fvMeshSubset.H"
#include "emptyFvsPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > fvMeshSubset::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    // Get reference to the subset mesh
    const fvMesh& sMesh = subMesh();

    // Create and map the internal-field values
    Field<Type> internalField(vf.internalField(), cellMap());

    // Create and map the patch field values
    const labelList& pm = patchMap();

    PtrList<fvPatchField<Type> > patchFields(pm.size());

    label internalFacesPatchIndex = -1;

    forAll (patchFields, patchI)
    {
        // Set the first one by hand as it corresponds to the
        // internal field.  Additional interpolation can be put here
        // as necessary.  
        if (pm[patchI] == -1)
        {
            internalFacesPatchIndex = patchI;

            patchFields.set
            (
                patchI,
                new calculatedFvPatchField<Type>
                (
                    sMesh.boundary()[patchI],
                    DimensionedField<Type, volMesh>::null()
                )
            );
        }
        else
        {
            patchFields.set
            (
                patchI,
                fvPatchField<Type>::New
                (
                    vf.boundaryField()[pm[patchI]],
                    sMesh.boundary()[patchI],
                    DimensionedField<Type, volMesh>::null(),
                    patchFieldSubset(*this, patchI)
                )
            );
        }
    }

    // Linear interpolation for last patch
    if (internalFacesPatchIndex > -1)
    {
        const Field<Type>& vfI = vf.internalField();
        const scalarField& w = baseMesh().weights().internalField();
        const labelList& own = baseMesh().allOwner();
        const labelList& ngb = baseMesh().allNeighbour();

        Field<Type>& lastPatchField = patchFields[internalFacesPatchIndex];

        label lastPatchStart =
            sMesh.boundaryMesh()[internalFacesPatchIndex].start();

        const labelList& fm = faceMap();

        forAll(lastPatchField, faceI)
        {
            lastPatchField[faceI] = 
                w[fm[lastPatchStart + faceI]]*
                vfI[own[fm[lastPatchStart + faceI]]]
              + (1.0 - w[fm[lastPatchStart + faceI]])*
                vfI[ngb[fm[lastPatchStart + faceI]]];
        }
    }

    // Create the complete field from the pieces
    tmp<GeometricField<Type, fvPatchField, volMesh> > tresF
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                "subset"+vf.name(),
                sMesh.time().timeName(),
                sMesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            sMesh,
            vf.dimensions(),
            internalField,
            patchFields
        )
    );

    return tresF;
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > fvMeshSubset::interpolate
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& vf
) const
{
    // Get reference to the subset mesh
    const fvMesh& sMesh = subMesh();

    // Create and map the internal-field values
    Field<Type> internalField
    (
        vf.internalField(),
        SubList<label>
        (
            faceMap(),
            sMesh.nInternalFaces()
        )
    );

    // Create and map the patch field values
    const labelList& pm = patchMap();

    PtrList<fvsPatchField<Type> > patchFields(pm.size());

    forAll (patchFields, patchI)
    {
        // Set the first one by hand as it corresponds to the
        // internal field.  Additional interpolation can be put here
        // as necessary.  
        if (pm[patchI] == -1)
        {
            patchFields.set
            (
                patchI,
                new emptyFvsPatchField<Type>
                (
                    sMesh.boundary()[patchI],
                    DimensionedField<Type, surfaceMesh>::null()
                )
            );
        }
        else
        {
            patchFields.set
            (
                patchI,
                fvsPatchField<Type>::New
                (
                    vf.boundaryField()[pm[patchI]],
                    sMesh.boundary()[patchI],
                    DimensionedField<Type, surfaceMesh>::null(),
                    patchFieldSubset(*this, patchI)
                )
            );
        }
    }


    // Map exposed internal faces. Note: Only nessecary if exposed faces added
    // into existing patch but since we don't know that at this point...
    forAll(patchFields, patchI)
    {
        fvsPatchField<Type>& pfld = patchFields[patchI];

        label meshFaceI = pfld.patch().patch().start();

        forAll(pfld, i)
        {
            label oldFaceI = faceMap_[meshFaceI++];

            if (oldFaceI < vf.internalField().size())
            {
                pfld[i] = vf.internalField()[oldFaceI];
            }
        }
    }


    // Create the complete field from the pieces
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tresF
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                "subset"+vf.name(),
                sMesh.time().timeName(),
                sMesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            sMesh,
            vf.dimensions(),
            internalField,
            patchFields
        )
    );

    return tresF;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
