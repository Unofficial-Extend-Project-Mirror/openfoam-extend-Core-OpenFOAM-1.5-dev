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

#include "processorFaePatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * //

template<class Type>
processorFaePatchField<Type>::processorFaePatchField
(
    const faPatch& p,
    const DimensionedField<Type, edgeMesh>& iF
)
:
    coupledFaePatchField<Type>(p, iF),
    procPatch_(refCast<const processorFaPatch>(p))
{}


template<class Type>
processorFaePatchField<Type>::processorFaePatchField
(
    const faPatch& p,
    const DimensionedField<Type, edgeMesh>& iF,
    const Field<Type>& f
)
:
    coupledFaePatchField<Type>(p, iF, f),
    procPatch_(refCast<const processorFaPatch>(p))
{}


// Construct by mapping given processorFaePatchField<Type>
template<class Type>
processorFaePatchField<Type>::processorFaePatchField
(
    const processorFaePatchField<Type>& ptf,
    const faPatch& p,
    const DimensionedField<Type, edgeMesh>& iF,
    const faPatchFieldMapper& mapper
)
:
    coupledFaePatchField<Type>(ptf, p, iF, mapper),
    procPatch_(refCast<const processorFaPatch>(p))
{
    if (!isType<processorFaPatch>(this->patch()))
    {
        FatalErrorIn
        (
            "processorFaePatchField<Type>::processorFaePatchField\n"
            "(\n"
            "    const processorFaePatchField<Type>& ptf,\n"
            "    const faPatch& p,\n"
            "    const DimensionedField<Type, edgeMesh>& iF,\n"
            "    const faPatchFieldMapper& mapper\n"
            ")\n"
        )   << "Field type does not correspond to patch type for patch "
            << this->patch().index() << "." << endl
            << "Field type: " << typeName << endl
            << "Patch type: " << this->patch().type()
            << exit(FatalError);
    }
}


template<class Type>
processorFaePatchField<Type>::processorFaePatchField
(
    const faPatch& p,
    const DimensionedField<Type, edgeMesh>& iF,
    const dictionary& dict
)
:
    coupledFaePatchField<Type>(p, iF, dict),
    procPatch_(refCast<const processorFaPatch>(p))
{
    if (!isType<processorFaPatch>(p))
    {
        FatalIOErrorIn
        (
            "processorFaePatchField<Type>::processorFaePatchField\n"
            "(\n"
            "    const faPatch& p,\n"
            "    const Field<Type>& field,\n"
            "    const dictionary& dict\n"
            ")\n",
            dict
        )   << "patch " << this->patch().index() << " not processor type. "
            << "Patch type = " << p.type()
            << exit(FatalIOError);
    }
}


template<class Type>
processorFaePatchField<Type>::processorFaePatchField
(
    const processorFaePatchField<Type>& ptf
)
:
    coupledFaePatchField<Type>(ptf),
    procPatch_(refCast<const processorFaPatch>(ptf.patch()))
{}


template<class Type>
processorFaePatchField<Type>::processorFaePatchField
(
    const processorFaePatchField<Type>& ptf,
    const DimensionedField<Type, edgeMesh>& iF
)
:
    coupledFaePatchField<Type>(ptf, iF),
    procPatch_(refCast<const processorFaPatch>(ptf.patch()))
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class Type>
processorFaePatchField<Type>::~processorFaePatchField()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
