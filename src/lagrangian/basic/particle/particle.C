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

#include "particle.H"
#include "Cloud.H"
#include "wedgePolyPatch.H"
#include "symmetryPolyPatch.H"
#include "cyclicPolyPatch.H"
#include "processorPolyPatch.H"
#include "wallPolyPatch.H"
#include "transform.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class particleType>
labelList particle<particleType>::findFaces
(
    const vector& position
)
{
    const polyMesh& mesh = cloud_.polyMesh_;
    const labelList& faces = mesh.cells()[celli_];
    const vector& C = mesh.cellCentres()[celli_];

    labelList faceList(0);
    forAll(faces, i)
    {
        label facei = faces[i];
        scalar lam = lambda(C, position, facei);

        if ((lam > 0) && (lam < 1.0))
        {
            label n = faceList.size();
            faceList.setSize(n+1);
            faceList[n] = facei;
        }
    }
    
    return faceList;
}


template<class particleType>
labelList particle<particleType>::findFaces
(
    const vector& position,
    const label celli,
    const scalar stepFraction
)
{
    const polyMesh& mesh = cloud_.polyMesh_;
    const labelList& faces = mesh.cells()[celli];
    const vector& C = mesh.cellCentres()[celli];

    labelList faceList(0);
    forAll(faces, i)
    {
        label facei = faces[i];
        scalar lam = lambda(C, position, facei, stepFraction);

        if ((lam > 0) && (lam < 1.0))
        {
            label n = faceList.size();
            faceList.setSize(n+1);
            faceList[n] = facei;
        }
    }
    
    return faceList;
}


template<class particleType>
template<class TrackingData>
void particle<particleType>::prepareForParallelTransfer
(
    const label patchi,
    TrackingData& td
)
{
    // Convert the face index to be local to the processor patch
    facei_ = patchFace(patchi, facei_);
}


template<class particleType>
template<class TrackingData>
void particle<particleType>::correctAfterParallelTransfer
(
    const label patchi,
    TrackingData& td
)
{
    const processorPolyPatch& ppp = 
        refCast<const processorPolyPatch>
        (cloud_.pMesh().boundaryMesh()[patchi]);

    celli_ = ppp.faceCells()[facei_];

    if (!ppp.parallel())
    {
        if (ppp.forwardT().size() == 1)
        {
            const tensor& T = ppp.forwardT()[0];
            transformPosition(T);
            static_cast<particleType&>(*this).transformProperties(T);
        }
        else
        {
            const tensor& T = ppp.forwardT()[facei_];
            transformPosition(T);
            static_cast<particleType&>(*this).transformProperties(T);
        }
    }
    else if (ppp.separated())
    {
        if (ppp.separation().size() == 1)
        {
            position_ -= ppp.separation()[0];
            static_cast<particleType&>(*this).transformProperties
            (
                -ppp.separation()[0]
            );
        }
        else
        {
            position_ -= ppp.separation()[facei_];
            static_cast<particleType&>(*this).transformProperties
            (
                -ppp.separation()[facei_]
            );
        }
    }

    // Convert the face index back into a global index
    facei_ += ppp.start();

    // Reset the face index for the next tracking operation
    // facei_ = -1;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class particleType>
particle<particleType>::particle
(
    const Cloud<particleType>& cloud,
    const vector& position,
    const label celli
)
:
    cloud_(cloud),
    position_(position),
    celli_(celli),
    facei_(-1),
    stepFraction_(0.0)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class particleType>
template<class TrackingData>
label particle<particleType>::track
(
    const vector& endPosition,
    TrackingData& td
)
{
    facei_ = -1;

    // Tracks to endPosition or stop on boundary
    while(!onBoundary() && stepFraction_ < 1.0 - SMALL)
    {
        stepFraction_ += trackToFace(endPosition, td)*(1.0 - stepFraction_);
    }

    return facei_;
}


template<class particleType>
label particle<particleType>::track(const vector& endPosition)
{
    int dummyTd;
    return track(endPosition, dummyTd);
}


template<class particleType>
template<class TrackingData>
scalar particle<particleType>::trackToFace
(
    const vector& endPosition,
    TrackingData& td
)
{
    const polyMesh& mesh = cloud_.polyMesh_;

    labelList faces = findFaces(endPosition);

    facei_ = -1;
    scalar trackFraction = 0.0;

    if (faces.size() == 0) // inside cell
    {
        trackFraction = 1.0;
        position_ = endPosition;
    }
    else // hit face
    {
        scalar lambdaMin = GREAT;

        if (faces.size() == 1)
        {
            lambdaMin = lambda(position_, endPosition, faces[0], stepFraction_);
            facei_ = faces[0];
        }
        else
        {
            // if the particle have to cross more than one cell to reach the
            // endPosition, we check which way to go.
            // if one of the faces is a boundary face and the particle is
            // outside we choose the boundary face.
            // the particle is outside if one of the lambda's is > 1 or < 0
            forAll(faces, i)
            {
                scalar lam = 
                    lambda(position_, endPosition, faces[i], stepFraction_);

                if (lam < lambdaMin)
                {
                    lambdaMin = lam;
                    facei_ = faces[i];
                }
            }
        }

        bool internalFace = cloud_.internalFace(facei_);

        // For warped faces the particle can be 'outside' the cell
        // this will yield a lambda larger than 1, or smaller than 0
        // for values < 0, the particle travels away from the cell
        // and we don't move the particle, only change cell.
        // for values larger than 1, we move the particle to endPosition only.
        if (lambdaMin > 0.0)
        {
            if (lambdaMin <= 1.0)
            {
                trackFraction = lambdaMin;
                position_ += trackFraction*(endPosition - position_);
            }
            else
            {
                trackFraction = 1.0;
                position_ = endPosition;
            }
        }
        else if (static_cast<particleType&>(*this).softImpact())
        {
            // soft-sphere particles can travel outside the domain
            // but we don't use lambda since this the particle
            // is going away from face
            trackFraction = 1.0;
            position_ = endPosition;
        }

        // change cell
        if (internalFace) // Internal face
        {
            if (celli_ == cloud_.owner_[facei_])
            {
                celli_ = cloud_.neighbour_[facei_];
            }
            else if (celli_ == cloud_.neighbour_[facei_])
            {
                celli_ = cloud_.owner_[facei_];
            }
            else
            {
                FatalErrorIn
                (
                    "particle::trackToFace"
                    "(const vector& endPosition, scalar& trackFraction)"
                )   << "addressing failure"
                    << abort(FatalError);
            }
        }
        else
        {
            particleType& p = static_cast<particleType&>(*this);

            // soft-sphere algorithm ignores the boundary
            if (p.softImpact())
            {
                trackFraction = 1.0;
                position_ = endPosition;
            }

            label patchi = patch(facei_);
            const polyPatch& patch = mesh.boundaryMesh()[patchi];

            if (isA<wedgePolyPatch>(patch))
            {
                p.hitWedgePatch
                (
                    static_cast<const wedgePolyPatch&>(patch), td
                );
            }
            else if (isA<symmetryPolyPatch>(patch))
            {
                p.hitSymmetryPatch
                (
                    static_cast<const symmetryPolyPatch&>(patch), td
                );
            }
            else if (isA<cyclicPolyPatch>(patch))
            {
                p.hitCyclicPatch
                (
                    static_cast<const cyclicPolyPatch&>(patch), td
                );
            }
            else if (isA<processorPolyPatch>(patch))
            {
                p.hitProcessorPatch
                (
                    static_cast<const processorPolyPatch&>(patch), td
                );
            }
            else if (isA<wallPolyPatch>(patch))
            {
                p.hitWallPatch
                (
                    static_cast<const wallPolyPatch&>(patch), td
                );
            }
            else if (isA<polyPatch>(patch))
            {
                p.hitPatch
                (
                    static_cast<const polyPatch&>(patch), td
                );
            }
            else
            {
                FatalErrorIn
                (
                    "particle::trackToFace"
                    "(const vector& endPosition, scalar& trackFraction)"
                )   << "patch type " << patch.type() << " not suported"
                    << abort(FatalError);
            }
        }        
    }

    // If the trackFraction = 0 something went wrong.
    // Either the particle is flipping back and forth across a face perhaps
    // due to velocity interpolation errors or it is in a "hole" in the mesh
    // caused by face warpage.
    // In both cases resolve the positional ambiguity by moving the particle
    // slightly towards the cell-centre.
    if (trackFraction < SMALL)
    {
        position_ += 1.0e-6*(mesh.cellCentres()[celli_] - position_);
    }

    return trackFraction;
}


template<class particleType>
scalar particle<particleType>::trackToFace
(
    const vector& endPosition
)
{
    int dummyTd;
    return trackToFace(endPosition, dummyTd);
}


template<class particleType>
void particle<particleType>::transformPosition(const tensor& T)
{
    position_ = transform(T, position_);
}


template<class particleType>
void particle<particleType>::transformProperties(const tensor&)
{}


template<class particleType>
void particle<particleType>::transformProperties(const vector&)
{}


template<class particleType>
template<class TrackingData>
void particle<particleType>::hitWedgePatch
(
    const wedgePolyPatch& wpp,
    TrackingData&
)
{
    vector nf = wpp.faceAreas()[wpp.whichFace(facei_)];
    nf /= mag(nf);

    static_cast<particleType&>(*this).transformProperties(I - 2.0*nf*nf);
}


template<class particleType>
template<class TrackingData>
void particle<particleType>::hitSymmetryPatch
(
    const symmetryPolyPatch& spp,
    TrackingData&
)
{
    vector nf = spp.faceAreas()[spp.whichFace(facei_)];
    nf /= mag(nf);

    static_cast<particleType&>(*this).transformProperties(I - 2.0*nf*nf);
}


template<class particleType>
template<class TrackingData>
void particle<particleType>::hitCyclicPatch
(
    const cyclicPolyPatch& cpp,
    TrackingData&
)
{
    label patchFacei_ = cpp.whichFace(facei_);

    facei_ = cpp.transformGlobalFace(facei_);

    celli_ = cloud_.polyMesh_.faceOwner()[facei_];

    if (!cpp.parallel())
    {
        const tensor& T = cpp.transformT(patchFacei_);

        transformPosition(T);
        static_cast<particleType&>(*this).transformProperties(T);
    }
    else if (cpp.separated())
    {
        position_ += cpp.separation(patchFacei_);
        static_cast<particleType&>(*this).transformProperties
        (
            cpp.separation(patchFacei_)
        );
    }
}


template<class particleType>
template<class TrackingData>
void particle<particleType>::hitProcessorPatch
(
    const processorPolyPatch& spp,
    TrackingData&
)
{}


template<class particleType>
template<class TrackingData>
void particle<particleType>::hitWallPatch
(
    const wallPolyPatch& spp,
    TrackingData&
)
{}


template<class particleType>
template<class TrackingData>
void particle<particleType>::hitPatch
(
    const polyPatch& spp,
    TrackingData&
)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "particleIO.C"

// ************************************************************************* //
