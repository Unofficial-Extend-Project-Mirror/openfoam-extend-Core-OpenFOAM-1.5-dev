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
    List\<T\> is a 1D vector of objects of type T, where the size of the
    vector is known and used for subscript bounds checking, etc.

\*---------------------------------------------------------------------------*/

#ifndef ListLoop_H
#define ListLoop_H

#include "undefListLoopM.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef vectorMachine

// Element access looping using [] for vector machines

#define List_FOR_ALL(f, i)                      \
        register const label _n##i = (f).size();\
        for (register label i=0; i<_n##i; i++)  \
        {

#define List_END_FOR_ALL  }

#define List_ELEM(f, fp, i)  (fp[i])

#define List_ACCESS(type, f, fp) type* __restrict__ fp = (f).begin()
#define List_CONST_ACCESS(type, f, fp) const type* __restrict__ fp = (f).begin()

#else

// Pointer looping for scalar machines

#define List_FOR_ALL(f, i)                      \
        register label i = (f).size();          \
        while (i--)                             \
        {                                       \

#define List_END_FOR_ALL  }

#define List_ELEM(f, fp, i)  (*fp++)

#define List_ACCESS(type, f, fp)  register type* __restrict__ fp = (f).begin()
#define List_CONST_ACCESS(type, f, fp)  register const type* __restrict__ fp = (f).begin()

#endif


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
