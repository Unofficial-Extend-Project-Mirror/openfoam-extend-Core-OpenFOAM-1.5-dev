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

#include "coupledPolyPatch.H"
#include "SortableList.H"
#include "ListOps.H"
#include "transform.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(coupledPolyPatch, 0);


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

void coupledPolyPatch::calcTransformTensors
(
    const vector& Cf,
    const vector& Cr,
    const vector& nf,
    const vector& nr
) const
{
    if (debug)
    {
        Pout<< "coupledPolyPatch::calcTransformTensors (with single vector) : "
            << name() << endl
            << "    nf:" << nf << nl
            << "    nr:" << nr << nl
            << "    mag(nf&nr):" << mag(nf & nr) << nl
            << "    Cf:" << Cf << nl
            << "    Cr:" << Cr << endl;
    }

    if (mag(nf & nr) < 1 - SMALL)
    {
        separation_.setSize(0);

        forwardT_ = tensorField(1, rotationTensor(-nr, nf));
        reverseT_ = tensorField(1, rotationTensor(nf, -nr));
    }
    else
    {
        forwardT_.setSize(0);
        reverseT_.setSize(0);

        vector separation = (nf & (Cr - Cf))*nf;

        if (mag(separation) > SMALL)
        {
            separation_ = vectorField(1, separation);
        }
        else
        {
            separation_.setSize(0);
        }
    }

    if (debug)
    {
        Pout<< "    separation_:" << separation_ << nl
            << "    forwardT size:" << forwardT_.size() << endl;
    }
}


void coupledPolyPatch::calcTransformTensors
(
    const vectorField& Cf,
    const vectorField& Cr,
    const vectorField& nf,
    const vectorField& nr
) const
{
    if (debug)
    {
        Pout<< "coupledPolyPatch::calcTransformTensors (with field): "
            << name() << endl
            << "    size:" << size() << nl
            << "    sum(mag(nf & nr)):" << sum(mag(nf & nr)) << endl;
    }

    if (size() == 0)
    {
        // Dummy geometry.
        separation_.setSize(0);
        forwardT_ = I;
        reverseT_ = I;
    }
    else if (sum(mag(nf & nr)) < size() - 1E-12)
    {
        separation_.setSize(0);

        forwardT_.setSize(size());
        reverseT_.setSize(size());

        forAll (forwardT_, facei)
        {
            forwardT_[facei] = rotationTensor(-nr[facei], nf[facei]);
            reverseT_[facei] = rotationTensor(nf[facei], -nr[facei]);
        }

        if (sum(mag(forwardT_ - forwardT_[0])) < 1E-12)
        {
            forwardT_.setSize(1);
            reverseT_.setSize(1);
        }
    }
    else
    {
        forwardT_.setSize(0);
        reverseT_.setSize(0);

        separation_ = (nf & (Cr - Cf))*nf;

        if (sum(mag(separation_))/size() < 1E-12)
        {
            separation_.setSize(0);
        }
        else if (sum(mag(separation_ - separation_[0]))/size() < 1E-12)
        {
            separation_.setSize(1);
        }
    }

    if (debug)
    {
        Pout<< "    separation_:" << separation_ << nl
            << "    forwardT size:" << forwardT_.size() << endl;
    }
}


// * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * * * * //

coupledPolyPatch::coupledPolyPatch
(
    const word& name,
    const label size,
    const label start,
    const label index,
    const polyBoundaryMesh& bm
)
:
    polyPatch(name, size, start, index, bm)
{}


coupledPolyPatch::coupledPolyPatch
(
    Istream& is,
    const label index,
    const polyBoundaryMesh& bm
)
:
    polyPatch(is, index, bm)
{}


coupledPolyPatch::coupledPolyPatch
(
    const word& name,
    const dictionary& dict,
    const label index,
    const polyBoundaryMesh& bm
)
:
    polyPatch(name, dict, index, bm)
{}


coupledPolyPatch::coupledPolyPatch
(
    const coupledPolyPatch& pp,
    const polyBoundaryMesh& bm
)
:
    polyPatch(pp, bm)
{}


coupledPolyPatch::coupledPolyPatch
(
    const coupledPolyPatch& pp,
    const polyBoundaryMesh& bm,
    const label index,
    const label newSize,
    const label newStart
)
:
    polyPatch(pp, bm, index, newSize, newStart)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

coupledPolyPatch::~coupledPolyPatch()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
