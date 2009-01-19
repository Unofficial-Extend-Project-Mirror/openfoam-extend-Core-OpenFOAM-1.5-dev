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

#include "IOobject.H"
#include "dictionary.H"
#include "fvMesh.H"
#include "fvPatchFieldMapper.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
fvPatchField<Type>::fvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    Field<Type>(p.size()),
    patch_(p),
    internalField_(iF),
    updated_(false)
{}


template<class Type>
fvPatchField<Type>::fvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const Field<Type>& f
)
:
    Field<Type>(f),
    patch_(p),
    internalField_(iF),
    updated_(false)
{}


template<class Type>
fvPatchField<Type>::fvPatchField
(
    const fvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    Field<Type>(ptf, mapper),
    patch_(p),
    internalField_(iF),
    updated_(false)
{}


template<class Type>
fvPatchField<Type>::fvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    Field<Type>(p.size()),
    patch_(p),
    internalField_(iF),
    updated_(false)
{
    if (dict.found("value"))
    {
        fvPatchField<Type>::operator=
        (
            Field<Type>("value", dict, p.size())
        );
    }
    else
    {
        fvPatchField<Type>::operator=(pTraits<Type>::zero);
    }
}


template<class Type>
fvPatchField<Type>::fvPatchField
(
    const fvPatchField<Type>& ptf
)
:
    Field<Type>(ptf),
    patch_(ptf.patch_),
    internalField_(ptf.internalField_),
    updated_(false)
{}


template<class Type>
fvPatchField<Type>::fvPatchField
(
    const fvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    Field<Type>(ptf),
    patch_(ptf.patch_),
    internalField_(iF),
    updated_(false)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
const objectRegistry& fvPatchField<Type>::db() const
{
    return patch_.boundaryMesh().mesh();
}


template<class Type>
void fvPatchField<Type>::check(const fvPatchField<Type>& ptf) const
{
    if (&patch_ != &(ptf.patch_))
    {
        FatalErrorIn("PatchField<Type>::check(const fvPatchField<Type>&)")
            << "different patches for fvPatchField<Type>s"
            << abort(FatalError);
    }
}


// Return gradient at boundary
template<class Type>
tmp<Field<Type> > fvPatchField<Type>::snGrad() const
{
    return (*this - patchInternalField())*patch_.deltaCoeffs();
}


// Return internal field next to patch as patch field
template<class Type>
tmp<Field<Type> > fvPatchField<Type>::patchInternalField() const
{
    return patch_.patchInternalField(internalField_);
}


template<class Type>
void fvPatchField<Type>::autoMap
(
    const fvPatchFieldMapper& m
)
{
    Field<Type>::autoMap(m);
}


template<class Type>
void fvPatchField<Type>::rmap
(
    const fvPatchField<Type>& ptf,
    const labelList& addr
)
{
    Field<Type>::rmap(ptf, addr);
}


template<class Type>
void fvPatchField<Type>::evaluate()
{
    if (!updated_)
    {
        updateCoeffs();
    }
    
    updated_ = false;
}


template<class Type>
void fvPatchField<Type>::write(Ostream& os) const
{
    os.writeKeyword("type") << type() << token::END_STATEMENT << nl;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void fvPatchField<Type>::operator=
(
    const UList<Type>& ul
)
{
    Field<Type>::operator=(ul);
}


template<class Type>
void fvPatchField<Type>::operator=
(
    const fvPatchField<Type>& ptf
)
{
    check(ptf);
    Field<Type>::operator=(ptf);
}


template<class Type>
void fvPatchField<Type>::operator+=
(
    const fvPatchField<Type>& ptf
)
{
    check(ptf);
    Field<Type>::operator+=(ptf);
}


template<class Type>
void fvPatchField<Type>::operator-=
(
    const fvPatchField<Type>& ptf
)
{
    check(ptf);
    Field<Type>::operator-=(ptf);
}


template<class Type>
void fvPatchField<Type>::operator*=
(
    const fvPatchField<scalar>& ptf
)
{
    if (&patch_ != &ptf.patch())
    {
        FatalErrorIn
        (
            "PatchField<Type>::operator*=(const fvPatchField<scalar>& ptf)"
        )   << "incompatible patches for patch fields"
            << abort(FatalError);
    }

    Field<Type>::operator*=(ptf);
}


template<class Type>
void fvPatchField<Type>::operator/=
(
    const fvPatchField<scalar>& ptf
)
{
    if (&patch_ != &ptf.patch())
    {
        FatalErrorIn
        (
            "PatchField<Type>::operator/=(const fvPatchField<scalar>& ptf)"
        )   << "    incompatible patches for patch fields"
            << abort(FatalError);
    }

    Field<Type>::operator/=(ptf);
}


template<class Type>
void fvPatchField<Type>::operator+=
(
    const Field<Type>& tf
)
{
    Field<Type>::operator+=(tf);
}


template<class Type>
void fvPatchField<Type>::operator-=
(
    const Field<Type>& tf
)
{
    Field<Type>::operator-=(tf);
}


template<class Type>
void fvPatchField<Type>::operator*=
(
    const scalarField& tf
)
{
    Field<Type>::operator*=(tf);
}


template<class Type>
void fvPatchField<Type>::operator/=
(
    const scalarField& tf
)
{
    Field<Type>::operator/=(tf);
}


template<class Type>
void fvPatchField<Type>::operator=
(
    const Type& t
)
{
    Field<Type>::operator=(t);
}


template<class Type>
void fvPatchField<Type>::operator+=
(
    const Type& t
)
{
    Field<Type>::operator+=(t);
}


template<class Type>
void fvPatchField<Type>::operator-=
(
    const Type& t
)
{
    Field<Type>::operator-=(t);
}


template<class Type>
void fvPatchField<Type>::operator*=
(
    const scalar s
)
{
    Field<Type>::operator*=(s);
}


template<class Type>
void fvPatchField<Type>::operator/=
(
    const scalar s
)
{
    Field<Type>::operator/=(s);
}


// Force an assignment, overriding fixedValue status
template<class Type>
void fvPatchField<Type>::operator==
(
    const fvPatchField<Type>& ptf
)
{
    Field<Type>::operator=(ptf);
}


template<class Type>
void fvPatchField<Type>::operator==
(
    const Field<Type>& tf
)
{
    Field<Type>::operator=(tf);
}


template<class Type>
void fvPatchField<Type>::operator==
(
    const Type& t
)
{
    Field<Type>::operator=(t);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
Ostream& operator<<(Ostream& os, const fvPatchField<Type>& ptf)
{
    ptf.write(os);

    os.check("Ostream& operator<<(Ostream&, const fvPatchField<Type>&");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#   include "newFvPatchField.C"

// ************************************************************************* //
