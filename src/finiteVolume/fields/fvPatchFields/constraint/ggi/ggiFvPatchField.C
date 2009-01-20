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

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

Contributor:
    Martin Beaudoin, Hydro-Quebec, (2008)

\*---------------------------------------------------------------------------*/

#include "ggiFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
ggiFvPatchField<Type>::ggiFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    coupledFvPatchField<Type>(p, iF),
    ggiPatch_(refCast<const ggiFvPatch>(p))
{}


template<class Type>
ggiFvPatchField<Type>::ggiFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    coupledFvPatchField<Type>(p, iF, dict),
    ggiPatch_(refCast<const ggiFvPatch>(p))
{
    if (!isType<ggiFvPatch>(p))
    {
        FatalIOErrorIn
        (
            "ggiFvPatchField<Type>::ggiFvPatchField\n"
            "(\n"
            "    const fvPatch& p,\n"
            "    const DimensionedField<Type, volMesh>& iF,\n"
            "    const dictionary& dict\n"
            ")\n",
            dict
        )   << "patch " << this->patch().index() << " not ggi type. "
            << "Patch type = " << p.type()
            << exit(FatalIOError);
    }
}


template<class Type>
ggiFvPatchField<Type>::ggiFvPatchField
(
    const ggiFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    coupledFvPatchField<Type>(ptf, p, iF, mapper),
    ggiPatch_(refCast<const ggiFvPatch>(p))
{
    if (!isType<ggiFvPatch>(this->patch()))
    {
        FatalErrorIn
        (
            "ggiFvPatchField<Type>::ggiFvPatchField\n"
            "(\n"
            "    const ggiFvPatchField<Type>& ptf,\n"
            "    const fvPatch& p,\n"
            "    const DimensionedField<Type, volMesh>& iF,\n"
            "    const fvPatchFieldMapper& mapper\n"
            ")\n"
        )   << "Field type does not correspond to patch type for patch "
            << this->patch().index() << "." << endl
            << "Field type: " << typeName << endl
            << "Patch type: " << this->patch().type()
            << exit(FatalError);
    }
}


template<class Type>
ggiFvPatchField<Type>::ggiFvPatchField
(
    const ggiFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    ggiLduInterfaceField(),
    coupledFvPatchField<Type>(ptf, iF),
    ggiPatch_(refCast<const ggiFvPatch>(ptf.patch()))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Return neighbour field
template<class Type>
tmp<Field<Type> > ggiFvPatchField<Type>::patchNeighbourField() const
{
    const Field<Type>& iField = this->internalField();
    // Get shadow face-cells and assemble shadow field
    const unallocLabelList& sfc = ggiPatch_.shadow().faceCells();

    Field<Type> sField(sfc.size());

    forAll (sField, i)
    {
        sField[i] = iField[sfc[i]];
    }

    //HJ, change 1: partial coverage in cyclic GGI. HJ, 18/Jan/2009
    //    Experimental: avoiding zeros in uncovered faces
//     const Field<Type>& pField = *this;
//     return pField + ggiPatch_.interpolate(sField - pField);

    return ggiPatch_.interpolate(sField);
}


template<class Type>
void ggiFvPatchField<Type>::evaluate
(
    const Pstream::commsTypes
)
{
    Field<Type>::operator=
    (
        this->patch().weights()*this->patchInternalField()
      + (1.0 - this->patch().weights())*this->patchNeighbourField()
    );
}


// Return matrix product for coupled boundary
template<class Type>
void ggiFvPatchField<Type>::updateInterfaceMatrix
(
    const scalarField& psiInternal,
    scalarField& result,
    const lduMatrix&,
    const scalarField& coeffs,
    const direction cmpt,
    const Pstream::commsTypes
) const
{
    // Get shadow face-cells and assemble shadow field
    const unallocLabelList& sfc = ggiPatch_.shadow().faceCells();

    scalarField sField(sfc.size());

    forAll (sField, i)
    {
        sField[i] = psiInternal[sfc[i]];
    }

    scalarField pnf = ggiPatch_.interpolate(sField);

    // Multiply the field by coefficients and add into the result
    const unallocLabelList& fc = ggiPatch_.faceCells();

    forAll(fc, elemI)
    {
        result[fc[elemI]] -= coeffs[elemI]*pnf[elemI];
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
