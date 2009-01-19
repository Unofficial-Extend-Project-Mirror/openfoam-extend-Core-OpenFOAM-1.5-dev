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

#include "FixedList.H"
#include "ListLoopM.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Return a null FixedList
template<class T, label Size>
FixedList<T, Size>& FixedList<T, Size>::null()
{
    FixedList<T, Size>* nullPtr = reinterpret_cast<FixedList<T, Size>*>(NULL);
    return *nullPtr;
}


// * * * * * * * * * * * * * * STL Member Functions  * * * * * * * * * * * * //

template<class T, label Size>
void FixedList<T, Size>::swap(FixedList<T, Size>& a)
{
    List_ACCESS(T, (*this), vp);
    List_ACCESS(T, a, ap);
    T tmp;
    List_FOR_ALL((*this), i)
        tmp = List_ELEM((*this), vp, i);
        List_ELEM((*this), vp, i) = List_ELEM(a, ap, i);
        List_ELEM(a, ap, i) = tmp;
    List_END_FOR_ALL
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

// Comparison for equality
template<class T, label Size>
bool FixedList<T, Size>::operator==(const FixedList<T, Size>& a) const
{
    bool equal = true;

    List_CONST_ACCESS(T, (*this), vp);
    List_CONST_ACCESS(T, (a), ap);

    List_FOR_ALL((*this), i)
        equal = equal && (List_ELEM((*this), vp, i) == List_ELEM((a), ap, i));
    List_END_FOR_ALL

    return equal;
}


// Comparison for inequality
template<class T, label Size>
bool FixedList<T, Size>::operator!=(const FixedList<T, Size>& a) const
{
    return !operator==(a);
}


// Compare FixedLists lexicographically
template<class T, label Size>
bool FixedList<T, Size>::operator<(const FixedList<T, Size>& a) const
{
    for
    (
        const_iterator vi = begin(), ai = a.begin();
        vi < end() && ai < a.end();
        vi++, ai++
    )
    {
        if (*vi < *ai)
        {
            return true;
        }
        else if (*vi > *ai)
        {
            return false;
        }
    }

    if (Size < a.Size)
    {
        return true;
    }
    else
    {
        return false;
    }
}


// Compare FixedLists lexicographically
template<class T, label Size>
bool FixedList<T, Size>::operator>(const FixedList<T, Size>& a) const
{
    return a.operator<(*this);
}


//- Return true if !(a > b). Takes linear time.
template<class T, label Size>
bool FixedList<T, Size>::operator<=(const FixedList<T, Size>& a) const
{
    return !operator>(a);
}


//- Return true if !(a < b). Takes linear time.
template<class T, label Size>
bool FixedList<T, Size>::operator>=(const FixedList<T, Size>& a) const
{
    return !operator<(a);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * //

#include "FixedListIO.C"

// ************************************************************************* //
