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

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*---------------------------------------------------------------------------*/

#include "ggiPolyPatch.H"
#include "polyBoundaryMesh.H"
#include "addToRunTimeSelectionTable.H"
#include "demandDrivenData.H"
#include "polyPatchID.H"
#include "SubField.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(ggiPolyPatch, 0);

    addToRunTimeSelectionTable(polyPatch, ggiPolyPatch, Istream);
    addToRunTimeSelectionTable(polyPatch, ggiPolyPatch, dictionary);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::ggiPolyPatch::calcInterpolation() const
{
    // Create patch-to-patch interpolation
    if (patchToPatchPtr_)
    {
        FatalErrorIn("void ggiPolyPatch::calcInterpolation() const")
            << "Patch to patch interpolation already calculated"
            << abort(FatalError);
    }

#ifdef TestGGI
    if (master())
    {
        patchToPatchPtr_ =
            new ggiInterpolation
            (
                *this,
                boundaryMesh()[shadowIndex()],
                intersection::VISIBLE,
                intersection::VECTOR
            );
    }
    else
    {
        FatalErrorIn("void ggiPolyPatch::calcInterpolation() const")
            << "Attempting to create GGIinterpolation on a shadow"
            << abort(FatalError);
    }
#else
    patchToPatchPtr_ =
        new patchToPatchInterpolation
        (
            boundaryMesh()[shadowIndex()],
            *this,
            intersection::VISIBLE,
            intersection::VECTOR
        );
#endif
}


#ifdef TestGGI
const Foam::ggiInterpolation& Foam::ggiPolyPatch::patchToPatch() const
{
    if (master())
    {
        if (!patchToPatchPtr_)
        {
            calcInterpolation();
        }

        return *patchToPatchPtr_;
    }
    else
    {
        return shadow().patchToPatch();
    }
}
#else
const Foam::patchToPatchInterpolation& Foam::ggiPolyPatch::patchToPatch() const
{
    if (!patchToPatchPtr_)
    {
        calcInterpolation();
    }

    return *patchToPatchPtr_;
}
#endif


void Foam::ggiPolyPatch::calcReconFaceCellCentres() const
{
    // Create neighbouring face centres using interpolation

    if (master())
    {
        const label shadowID = shadowIndex();

        // Reconstruct the shadow cell face centres
        vectorField localCtrs = faceCellCentres();

        vectorField reconCtrs =
            interpolate
            (
                boundaryMesh()[shadowID].faceCellCentres()
            );

        // Calculate reconstructed centres by eliminating non-orthogonality
        const vectorField& n = faceNormals();

        reconFaceCellCentresPtr_ =
            new vectorField(localCtrs + n*(n & (reconCtrs - localCtrs)));
    }
    else
    {
        FatalErrorIn("void ggiPolyPatch::calcReconFaceCellCentres() const")
            << "Attempting to create reconFaceCellCentres on a shadow"
            << abort(FatalError);
    }
}


void Foam::ggiPolyPatch::clearOut()
{
    deleteDemandDrivenData(patchToPatchPtr_);
    deleteDemandDrivenData(reconFaceCellCentresPtr_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
Foam::ggiPolyPatch::ggiPolyPatch
(
    const word& name,
    const label size,
    const label start,
    const label index,
    const polyBoundaryMesh& bm,
    const word& shadowName
)
:
    coupledPolyPatch(name, size, start, index, bm),
    shadowName_(shadowName),
    shadowIndex_(-1),
    patchToPatchPtr_(NULL),
    reconFaceCellCentresPtr_(NULL)
{}


// Construct from Istream
Foam::ggiPolyPatch::ggiPolyPatch
(
    Istream& is,
    const label index,
    const polyBoundaryMesh& bm
)
:
    coupledPolyPatch(is, index, bm),
    shadowName_(is),
    shadowIndex_(-1),
    patchToPatchPtr_(NULL),
    reconFaceCellCentresPtr_(NULL)
{}


// Construct from dictionary
Foam::ggiPolyPatch::ggiPolyPatch
(
    const word& name,
    const dictionary& dict,
    const label index,
    const polyBoundaryMesh& bm
)
:
    coupledPolyPatch(name, dict, index, bm),
    shadowName_(dict.lookup("shadowPatch")),
    shadowIndex_(-1),
    patchToPatchPtr_(NULL),
    reconFaceCellCentresPtr_(NULL)
{}


//- Construct as copy, resetting the boundary mesh
Foam::ggiPolyPatch::ggiPolyPatch
(
    const ggiPolyPatch& pp,
    const polyBoundaryMesh& bm
)
:
    coupledPolyPatch(pp, bm),
    shadowName_(pp.shadowName_),
    shadowIndex_(-1),
    patchToPatchPtr_(NULL),
    reconFaceCellCentresPtr_(NULL)
{}


//- Construct as copy, resetting the face list and boundary mesh data
Foam::ggiPolyPatch::ggiPolyPatch
(
    const ggiPolyPatch& pp,
    const polyBoundaryMesh& bm,
    const label index,
    const label newSize,
    const label newStart
)
:
    coupledPolyPatch(pp, bm, index, newSize, newStart),
    shadowName_(pp.shadowName_),
    shadowIndex_(-1),
    patchToPatchPtr_(NULL),
    reconFaceCellCentresPtr_(NULL)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::ggiPolyPatch::~ggiPolyPatch()
{
    clearOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::label Foam::ggiPolyPatch::shadowIndex() const
{
    if (shadowIndex_ == -1)
    {
        // Grab shadow patch index
        polyPatchID shadow(shadowName_, boundaryMesh());

        if (!shadow.active())
        {
            FatalErrorIn("label ggiPolyPatch::shadowIndex() const")
                << "Shadow patch name " << shadowName_
                << " not found.  Please check your GGI interface definition."
                << abort(FatalError);
        }

        shadowIndex_ = shadow.index();

        // Check the other side is a ggi
        if (!isType<ggiPolyPatch>(boundaryMesh()[shadowIndex_]))
        {
            FatalErrorIn("label ggiPolyPatch::shadowIndex() const")
                << "Shadow of ggi patch " << name()
                << " named " << shadowName() << " is not a ggi." << nl
                << "This is not allowed.  Please check your mesh definition."
                << abort(FatalError);
        }
    }

    return shadowIndex_;
}


const Foam::ggiPolyPatch& Foam::ggiPolyPatch::shadow() const
{
    return refCast<const ggiPolyPatch>(boundaryMesh()[shadowIndex()]);
}


const Foam::vectorField&
Foam::ggiPolyPatch::reconFaceCellCentres() const
{
    if (!reconFaceCellCentresPtr_)
    {
        calcReconFaceCellCentres();
    }

    return *reconFaceCellCentresPtr_;
}


void Foam::ggiPolyPatch::initGeometry()
{
    polyPatch::initGeometry();
}


void Foam::ggiPolyPatch::calcGeometry()
{
    // Reconstruct the cell face centres
    if (master())
    {
        reconFaceCellCentres();
    }
}


void Foam::ggiPolyPatch::initMovePoints(const pointField& p)
{
    polyPatch::initMovePoints(p);
}


void Foam::ggiPolyPatch::movePoints(const pointField& p)
{
    polyPatch::movePoints(p);

    // Clear reconstructed face centres
    deleteDemandDrivenData(reconFaceCellCentresPtr_);

    if (patchToPatchPtr_)
    {
        patchToPatchPtr_->movePoints();
    }
}


void Foam::ggiPolyPatch::initUpdateMesh()
{
    polyPatch::initUpdateMesh();
}


void Foam::ggiPolyPatch::updateMesh()
{
    polyPatch::updateMesh();
    clearOut();
}


void Foam::ggiPolyPatch::calcTransformTensors
(
    const vectorField& Cf,
    const vectorField& Cr,
    const vectorField& nf,
    const vectorField& nry
) const
{
    FatalErrorIn("void ggiPolyPatch::calcTransformTensors")
        << "Not ready"
        << abort(FatalError);
}


//- Initialize ordering (on new mesh)
void Foam::ggiPolyPatch::initOrder(const primitivePatch& pp) const
{}


//- Return new ordering. Ordering is -faceMap: for every face index
//  the new face -rotation:for every new face the clockwise shift
//  of the original face. Return false if nothing changes (faceMap
//  is identity, rotation is 0)
bool Foam::ggiPolyPatch::order
(
    const primitivePatch& pp,
    labelList& faceMap,
    labelList& rotation
) const
{
    faceMap.setSize(pp.size());
    faceMap = -1;

    rotation.setSize(pp.size());
    rotation = 0;

    // Nothing changes
    return false;
}


// Write
void Foam::ggiPolyPatch::write(Ostream& os) const
{
    polyPatch::write(os);

    os  << nl << shadowName_ << endl;
}


void Foam::ggiPolyPatch::writeDict(Ostream& os) const
{
    os  << nl << name() << nl << token::BEGIN_BLOCK << nl
        << "    type " << type() << token::END_STATEMENT << nl;
    patchIdentifier::writeDict(os);
    os  << "    nFaces " << size() << token::END_STATEMENT << nl
        << "    startFace " << start() << token::END_STATEMENT << nl
        << "    shadowPatch " << shadowName_ << token::END_STATEMENT << nl
        << token::END_BLOCK << endl;
}


// ************************************************************************* //
