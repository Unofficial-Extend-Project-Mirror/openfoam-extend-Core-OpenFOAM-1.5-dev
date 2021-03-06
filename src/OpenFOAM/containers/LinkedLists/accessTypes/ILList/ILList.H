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

Class
    Foam::ILList

Description
    Template class for intrusive linked lists.

SourceFiles
    ILList.C
    ILListIO.C

\*---------------------------------------------------------------------------*/

#ifndef ILList_H
#define ILList_H

#include "UILList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Istream;
class Ostream;

// Forward declaration of friend functions and operators

template<class LListBase, class T> class ILList;

template<class LListBase, class T> Istream& operator>>
(
    Istream&,
    ILList<LListBase, T>&
);


/*---------------------------------------------------------------------------*\
                           Class ILList Declaration
\*---------------------------------------------------------------------------*/

template<class LListBase, class T>
class ILList
:
    public UILList<LListBase, T>
{
    // Private member functions

        //- Read from Istream using given Istream constructor class
        template<class INew>
        void read(Istream&, const INew& inewt);


public:

    // Constructors

        //- Null construct
        ILList()
        {}

        //- Construct given initial T
        ILList(T* a)
        :
            UILList<LListBase, T>(a)
        {}

        //- Construct from Istream
        ILList(Istream&);

        //- Construct as copy
        ILList(const ILList<LListBase, T>&);

        //- Copy constructor with additional argument for clone
        template<class CloneArg>
        ILList(const ILList<LListBase, T>& slpl, const CloneArg& cloneArg)
        #ifdef __INTEL_COMPILER
        :
            UILList<LListBase, T>()
        {
            for
            (
                typename UILList<LListBase, T>::const_iterator iter =
                    slpl.begin();
                iter != slpl.end();
                ++iter
            )
            {
                append(iter().clone(cloneArg).ptr());
            }
        }
        #else
        ;
        #endif

        //- Construct from Istream using given Istream constructor class
        template<class INew>
        ILList(Istream&, const INew& inewt);


    // Destructor

        ~ILList();


    // Member Functions

        // Edit

            //- Remove the head element specified from the list and delete it
            bool eraseHead();

            //- Remove the specified element from the list and delete it
            bool erase(T* p);

            //- Clear the contents of the list
            void clear();


    // Member operators

        void operator=(const ILList<LListBase, T>&);


    // Istream operator

        //- Read List from Istream, discarding contents of existing List.
        friend Istream& operator>>
        #ifndef __CINT__
        <LListBase, T>
        #endif
        (
            Istream&,
            ILList<LListBase, T>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "ILList.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
