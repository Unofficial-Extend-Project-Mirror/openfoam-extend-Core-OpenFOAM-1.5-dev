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
    GGI interpolation functions

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //
template<class MasterPatch, class SlavePatch>
template<class Type>
void GGIinterpolation<MasterPatch, SlavePatch>::interpolate
(
    const Field<Type>& ff,
    Field<Type>& result,
    const labelListList& addr,
    const scalarListList& weights
)
{
    forAll (result, faceI)
    {
        const labelList& curAddr = addr[faceI];
        const scalarList& curWeights = weights[faceI];

        result[faceI] = pTraits<Type>::zero;

        forAll (curAddr, i)
        {
            result[faceI] += ff[curAddr[i]]*curWeights[i];
        }
    }
}



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class MasterPatch, class SlavePatch>
template<class Type>
tmp<Field<Type> >
GGIinterpolation<MasterPatch, SlavePatch>::masterToSlave
(
    const Field<Type>& ff
) const
{
    if (ff.size() != masterPatch_.size())
    {
        FatalErrorIn
        (
            "GGIinterpolation::masterToSlave"
            "(const Field<Type> ff)"
        )   << "given field does not correspond to patch. Patch size: "
            << masterPatch_.size() << " field size: " << ff.size()
            << abort(FatalError);
    }

    tmp<Field<Type> > tresult
    (
        new Field<Type>
        (
            slavePatch_.size(),
            pTraits<Type>::zero
        )
    );

    // Do interpolation
    Field<Type>& result = tresult();

    GGIinterpolation<MasterPatch, SlavePatch>::interpolate
    (
        ff,
        result,
        this->slaveAddr(),
        this->slaveWeights()
    );

    return tresult;
}


template<class MasterPatch, class SlavePatch>
template<class Type>
tmp<Field<Type> >
GGIinterpolation<MasterPatch, SlavePatch>::masterToSlave
(
    const tmp<Field<Type> >& tff
) const
{
    tmp<Field<Type> > tint = masterToSlave(tff());
    tff.clear();
    return tint;
}


template<class MasterPatch, class SlavePatch>
template<class Type>
tmp<Field<Type> >
GGIinterpolation<MasterPatch, SlavePatch>::slaveToMaster
(
    const Field<Type>& ff
) const
{
    if (ff.size() != slavePatch_.size())
    {
        FatalErrorIn
        (
            "GGIinterpolation::slaveToMaster"
            "(const Field<Type> ff)"
        )   << "given field does not correspond to patch. Patch size: "
            << slavePatch_.size() << " field size: " << ff.size()
            << abort(FatalError);
    }

    tmp<Field<Type> > tresult
    (
        new Field<Type>
        (
            masterPatch_.size(),
            pTraits<Type>::zero
        )
    );

    // Do interpolation
    Field<Type>& result = tresult();

    GGIinterpolation<MasterPatch, SlavePatch>::interpolate
    (
        ff,
        result,
        this->masterAddr(),
        this->masterWeights()
    );

    return tresult;
}


template<class MasterPatch, class SlavePatch>
template<class Type>
tmp<Field<Type> >
GGIinterpolation<MasterPatch, SlavePatch>::slaveToMaster
(
    const tmp<Field<Type> >& tff
) const
{
    tmp<Field<Type> > tint = slaveToMaster(tff());
    tff.clear();
    return tint;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
