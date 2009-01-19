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

#include "error.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Store object pointer
template<class T>
inline autoPtr<T>::autoPtr(T* tPtr)
:
    ptr_(tPtr)
{}


// Construct copy (increment reference count if object is temporary)
template<class T>
inline autoPtr<T>::autoPtr(const autoPtr<T>& ap)
:
    ptr_(ap.ptr_)
{
    ap.ptr_ = NULL;
}


// Delete object
template<class T>
inline autoPtr<T>::~autoPtr()
{
    clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Is the autoPtr valid, i.e. is the pointer set
template<class T>
inline bool autoPtr<T>::valid() const
{
    return ptr_;
}

// Return object pointer for reuse
template<class T>
inline T* autoPtr<T>::ptr()
{
    T* ptr = ptr_;
    ptr_ = NULL;
    return ptr;
}


// Set pointer to that given.
// If object pointer already set issue a FatalError.
template<class T>
inline void autoPtr<T>::set(T* p)
{
    if (ptr_)
    {
        FatalErrorIn("void autoPtr<T>::set(T* p)")
            << "object already allocated"
            << abort(FatalError);
    }

    ptr_ = p;
}


// If object pointer already set delete object and
// set pointer to that given
template<class T>
inline void autoPtr<T>::reset(T* p)
{
    if (ptr_)
    {
        delete ptr_;
    }

    ptr_ = p;
}


// If object pointer points to valid object:
// delete object and set pointer to NULL
template<class T>
inline void autoPtr<T>::clear()
{
    reset(NULL);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline T& autoPtr<T>::operator()()
{
    if (!ptr_)
    {
        FatalErrorIn("T& autoPtr<T>::operator()()")
            << "object is not allocated"
            << abort(FatalError);
    }

    return *ptr_;
}


template<class T>
inline const T& autoPtr<T>::operator()() const
{
    if (!ptr_)
    {
        FatalErrorIn("const T& autoPtr<T>::operator()() const")
            << "object is not allocated"
            << abort(FatalError);
    }

    return *ptr_;
}


/*
template<class T>
inline T& autoPtr<T>::operator*()
{
    return operator()();
}


template<class T>
inline const T& autoPtr<T>::operator*() const
{
    return operator()();
}
*/


template<class T>
inline autoPtr<T>::operator const T&() const
{
    return operator()();
}


// Return object pointer
template<class T>
inline T* autoPtr<T>::operator->()
{
    if (!ptr_)
    {
        FatalErrorIn("autoPtr<T>::operator->()")
            << "object is not allocated"
            << abort(FatalError);
    }

    return ptr_;
}


// Return object pointer
template<class T>
inline const T* autoPtr<T>::operator->() const
{
    return const_cast<autoPtr<T>&>(*this).operator->();
}


template<class T>
inline void autoPtr<T>::operator=(const autoPtr<T>& ap)
{
    if (this != &ap)
    {
        const_cast<autoPtr<T>&>(*this).reset
        (
            const_cast<autoPtr<T>&>(ap).ptr()
        );
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
