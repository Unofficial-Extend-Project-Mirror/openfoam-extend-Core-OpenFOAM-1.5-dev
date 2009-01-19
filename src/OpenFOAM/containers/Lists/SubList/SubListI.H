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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
inline SubList<T>::SubList
(
    const UList<T>& list,
    const label subSize
)
:
    UList<T>(list.v_, subSize)
{
#   ifdef FULLDEBUG
    list.checkSize(subSize);
#   endif
}


template<class T>
inline SubList<T>::SubList
(
    const UList<T>& list,
    const label subSize,
    const label startIndex
)
:
    UList<T>(&(list.v_[startIndex]), subSize)
{
#   ifdef FULLDEBUG

    // Artificially allowing the start of a zero-sized subList to be
    // one past the end of the original list.  
    if (subSize > 0)
    {
        list.checkStart(startIndex);
        list.checkSize(startIndex + subSize);
    }
    else
    {
        // Start index needs to fall between 0 and size.  One position
        // behind the last element is allowed
        list.checkSize(startIndex);
    }
#   endif
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Return a null SubList
template<class T>
SubList<T>& SubList<T>::null()
{
    SubList<T>* nullPtr = reinterpret_cast<SubList<T>*>(NULL);
    return *nullPtr;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

// Allow cast to a const List<T>&
template<class T>
inline SubList<T>::operator const List<T>&() const
{
    return *reinterpret_cast<const List<T>*>(this);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
