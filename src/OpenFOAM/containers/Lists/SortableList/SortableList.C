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

\*---------------------------------------------------------------------------*/

#include "OSspecific.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from List
template<class Type>
SortableList<Type>::SortableList(const List<Type>& values)
:
    List<Type>(values),
    indices_(values.size())
{
    // Fill indices
    forAll(indices_, elemI)
    {
        indices_[elemI] = elemI;
    }

    sort();
}


// Construct from List and indexList
template<class Type>
SortableList<Type>::SortableList
(
    const List<Type>& values,
    const labelList& indices
)
:
    List<Type>(values),
    indices_(indices)
{
    if (values.size() != indices.size())
    {
        FatalErrorIn
        (
            "SortableList<Type>::SortableList"
            "(const List<Type>&, const labelList&)"
        )   << "Length of values " << values.size()
            << " differs from length of indices " << indices.size()
            << abort(FatalError);
    }

    sort();
}


// Construct given size. Sort later on.
template<class Type>
SortableList<Type>::SortableList(const label size)
:
    List<Type>(size),
    indices_(size)
{
    // Fill indices
    forAll(indices_, elemI)
    {
        indices_[elemI] = elemI;
    }
}


// Construct given size and initial value. Sort later on.
template<class Type>
SortableList<Type>::SortableList(const label size, const Type& val)
:
    List<Type>(size, val),
    indices_(size)
{
    // Fill indices
    forAll(indices_, elemI)
    {
        indices_[elemI] = elemI;
    }
}


// Construct as copy.
template<class Type>
SortableList<Type>::SortableList(const SortableList<Type>& lst)
:
    List<Type>(lst),
    indices_(lst.indices())
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void SortableList<Type>::setSize(const label newSize)
{
    label oldSize = List<Type>::size();

    List<Type>::setSize(newSize);
    indices_.setSize(newSize);

    if (newSize > oldSize)
    {
        // Has grown. Adapt indices.
        for (label elemI = oldSize; elemI < newSize; elemI++)
        {
            indices_[elemI] = elemI;
        }
    }
}


template<class Type>
void SortableList<Type>::sort()
{
    Foam::sort(indices_, less(*this));

    List<Type> tmpValues(this->size());

    forAll(indices_, i)
    {
        tmpValues[i] = this->operator[](indices_[i]);
    }

    List<Type>::operator=(tmpValues);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void Foam::SortableList<Type>::operator=(const SortableList<Type>& rhs)
{
    List<Type>::operator=(rhs);
    indices_ = rhs.indices();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
