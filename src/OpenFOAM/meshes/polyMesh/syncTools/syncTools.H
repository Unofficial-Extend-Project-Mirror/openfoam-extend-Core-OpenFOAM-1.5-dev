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
    syncTools

Description
    Various tools to aid synchronizing lists across coupled patches.

    Require 
    - combineOperator (e.g. sumEqOp - not sumOp!) that is defined for the
      type and combineReduce(UList<T>, combineOperator) should be defined.
    - null value which gets overridden by any valid value.
    - transform function

    Can apply coordinate rotation/separation on cyclics
    This is not tested!!!

SourceFiles
    syncTools.C
    syncToolsTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef syncTools_H
#define syncTools_H

#include "UList.H"
#include "Pstream.H"
#include "tensorField.H"
#include "PackedList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Define dummy transform for labels.
label transform(const tensor&, const label val);

class polyBoundaryMesh;
class polyMesh;
class coupledPolyPatch;

/*---------------------------------------------------------------------------*\
                           Class syncTools Declaration
\*---------------------------------------------------------------------------*/

class syncTools
{
    // Private Member Functions

        //- Check whether uses couples.
        static bool hasCouples(const polyBoundaryMesh&);

        //- Check for single transformation tensor only.
        static void checkTransform(const coupledPolyPatch&, const bool);

        //- Dummy transform for labels
        static label transform(const tensor&, const label i)
        {
            return i;
        }

        //- Apply transformation to list. Either single transformation tensor
        //  or one tensor per element.
        template <class T>
        static void transformList(const tensorField&, UList<T>&);

        //- Apply separation to list. Either single vector or one vector
        //  per element.
        template <class T>
        static void separateList(const vectorField&, UList<T>&);


public:

    // Static data members

        //- Synchronize values on all mesh points.
        //  Applies rotation and optionally separation for parallel cyclics
        template <class T, class CombineOp>
        static void syncPointList
        (
            const polyMesh&,
            UList<T>&,
            const CombineOp& bop,
            const T& nullValue,
            const bool applySeparation
        );

        //- Synchronize values on selected mesh points.
        //  Applies rotation and optionally separation for parallel cyclics
        template <class T, class CombineOp>
        static void syncPointList
        (
            const polyMesh&,
            const labelList& meshPoints,
            UList<T>&,
            const CombineOp& bop,
            const T& nullValue,
            const bool applySeparation
        );

        //- Synchronize values on all mesh edges.
        //  Applies rotation and optionally separation for parallel cyclics
        template <class T, class CombineOp>
        static void syncEdgeList
        (
            const polyMesh&,
            UList<T>&,
            const CombineOp& bop,
            const T& nullValue,
            const bool applySeparation
        );

        //- Synchronize values on boundary faces only.
        //  Optionally applies rotation tensor for non-parallel cyclics
        //  (but not separation!)
        template <class T, class CombineOp>
        static void syncBoundaryFaceList
        (
            const polyMesh&,
            UList<T>&,
            const CombineOp& bop,
            const bool applySeparation
        );

        //- Synchronize values on all mesh faces.
        //  Optionally applies rotation tensor for non-parallel cyclics
        //  (but not separation!)
        template <class T, class CombineOp>
        static void syncFaceList
        (
            const polyMesh&,
            UList<T>&,
            const CombineOp& bop,
            const bool applySeparation
        );

        //- Swap coupled face values.
        //  Applies rotation and optionally separation for parallel cyclics
        template <class T>
        static void swapBoundaryFaceList
        (
            const polyMesh&,
            UList<T>&,
            const bool applySeparation
        );

        //- Swap coupled face values.
        //  Applies rotation and optionally separation for parallel cyclics
        template <class T>
        static void swapFaceList
        (
            const polyMesh&,
            UList<T>&,
            const bool applySeparation
        );

        //- Get per point whether is it master (of a coupled set of points)
        static PackedList<1> getMasterPoints(const polyMesh&);

        //- Get per edge whether is it master (of a coupled set of edge)
        static PackedList<1> getMasterEdges(const polyMesh&);


};


template <>
inline void syncTools::transformList(const tensorField&, UList<label>&)
{}

template <>
inline void syncTools::transformList(const tensorField&, UList<scalar>&)
{}

template <>
void syncTools::separateList(const vectorField&, UList<vector>&);

template <class T>
List<T> transform(const tensor& rotTensor, UList<T>& field);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "syncToolsTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
