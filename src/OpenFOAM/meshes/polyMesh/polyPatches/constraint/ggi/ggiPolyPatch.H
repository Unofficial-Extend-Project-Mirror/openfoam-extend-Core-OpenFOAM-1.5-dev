/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2006-7 H. Jasak All rights reserved
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
    ggiPolyPatch

Description
    Generalised grid interface (GGI) patch.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

SourceFiles
    ggiPolyPatch.C

\*---------------------------------------------------------------------------*/

#ifndef ggiPolyPatch_H
#define ggiPolyPatch_H

#define TestGGI

#include "coupledPolyPatch.H"

#ifdef TestGGI
#include "ggiInterpolation.H"
#else
#include "patchToPatchInterpolation.H"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class ggiPolyPatch Declaration
\*---------------------------------------------------------------------------*/

class ggiPolyPatch
:
    public coupledPolyPatch
{
    // Private data

        //- Shadow patch name
        const word shadowName_;

        //- Shadow patch index.  Delayed evaluation for construction
        mutable label shadowIndex_;

        //- Patch-to-patch interpolation
#ifdef TestGGI
        mutable ggiInterpolation* patchToPatchPtr_;
#else
        mutable patchToPatchInterpolation* patchToPatchPtr_;
#endif

        //- Reconstructed patch neighbour cell centres
        mutable vectorField* reconFaceCellCentresPtr_;


    // Private member functions

        //- Calculate interpolation
        void calcInterpolation() const;


protected:

    // Protected Member functions

        //- Initialise the calculation of the patch geometry
        virtual void initGeometry();

        //- Calculate the patch geometry
        virtual void calcGeometry();

        //- Initialise the patches for moving points
        virtual void initMovePoints(const pointField&);

        //- Correct patches after moving points
        virtual void movePoints(const pointField&);

        //- Initialise the update of the patch topology
        virtual void initUpdateMesh();

        //- Update of the patch topology
        virtual void updateMesh();


        //- Return reference to patch-to-patch interpolation
#ifdef TestGGI
        const ggiInterpolation& patchToPatch() const;
#else
        const patchToPatchInterpolation& patchToPatch() const;
#endif

        //- Calculate reconstructed cell centres
        void calcReconFaceCellCentres() const;

        //- Clear out
        void clearOut();

public:

    //- Runtime type information
    TypeName("ggi");


    // Constructors

        //- Construct from components
        ggiPolyPatch
        (
            const word& name,
            const label size,
            const label start,
            const label index,
            const polyBoundaryMesh& bm,
            const word& shadowName
        );

        //- Construct from Istream
        ggiPolyPatch
        (
            Istream& is,
            const label index,
            const polyBoundaryMesh&
        );

        //- Construct from dictionary
        ggiPolyPatch
        (
            const word& name,
            const dictionary& dict,
            const label index,
            const polyBoundaryMesh&
        );

        //- Construct as copy, resetting the boundary mesh
        ggiPolyPatch
        (
            const ggiPolyPatch&,
            const polyBoundaryMesh&
        );

        //- Construct as given the original patch and resetting the
        //  face list and boundary mesh information
        ggiPolyPatch
        (
            const ggiPolyPatch& pp,
            const polyBoundaryMesh& bm,
            const label index,
            const label newSize,
            const label newStart
        );

        //- Construct and return a clone, resetting the boundary mesh
        virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
        {
            return autoPtr<polyPatch>(new ggiPolyPatch(*this, bm));
        }

        //- Construct and return a clone, resetting the face list
        //  and boundary mesh
        virtual autoPtr<polyPatch> clone
        (
            const polyBoundaryMesh& bm,
            const label index,
            const label newSize,
            const label newStart
        ) const
        {
            return autoPtr<polyPatch>
            (
                new ggiPolyPatch
                (
                    refCast<const ggiPolyPatch>(*this),
                    bm,
                    index,
                    newSize,
                    newStart
                )
            );
        }


    // Destructor

        virtual ~ggiPolyPatch();


    // Member functions

        //- Return shadow patch name
        const word& shadowName() const
        {
            return shadowName_;
        }

        //- Return shadow patch index
        int shadowIndex() const;

        //- Is this the master side?
        bool master() const
        {
            return index() < shadowIndex();
        }

        //- Is this the slave side?
        bool slave() const
        {
            return !master();
        }

        //- Return shadow patch
        const ggiPolyPatch& shadow() const;

        //- Interpolate face field: given field on a the shadow side,
        //  create an interpolated field on this side
#ifdef TestGGI
        template<class Type>
        tmp<Field<Type> > interpolate(const Field<Type>& pf) const
        {
            if (master())
            {
                return patchToPatch().slaveToMaster(pf);
            }
            else
            {
                return patchToPatch().masterToSlave(pf);
            }
        }

        template<class Type>
        tmp<Field<Type> > interpolate(const tmp<Field<Type> >& tpf) const
        {
            if (master())
            {
                return patchToPatch().slaveToMaster(tpf);
            }
            else
            {
                return patchToPatch().masterToSlave(tpf);
            }
        }
#else
        template<class Type>
        tmp<Field<Type> > interpolate(const Field<Type>& pf) const
        {
            return patchToPatch().faceInterpolate(pf);
        }

        template<class Type>
        tmp<Field<Type> > interpolate(const tmp<Field<Type> >& tpf) const
        {
            return patchToPatch().faceInterpolate(tpf);
        }
#endif

        //- Return reconstructed cell centres
        const vectorField& reconFaceCellCentres() const;

        //- Force calculation of transformation tensors
        void calcTransformTensors
        (
            const vectorField& Cf,
            const vectorField& Cr,
            const vectorField& nf,
            const vectorField& nr
        ) const;

        //- Initialize ordering for primitivePatch. Does not
        //  refer to *this (except for name() and type() etc.)
        virtual void initOrder(const primitivePatch&) const;

        //- Return new ordering for primitivePatch.
        //  Ordering is -faceMap: for every face
        //  index of the new face -rotation: for every new face the clockwise
        //  shift of the original face. Return false if nothing changes
        //  (faceMap is identity, rotation is 0), true otherwise.
        virtual bool order
        (
            const primitivePatch&,
            labelList& faceMap,
            labelList& rotation
        ) const;


        //- Write
        virtual void write(Ostream&) const;

        //- Write dictionary
        virtual void writeDict(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
