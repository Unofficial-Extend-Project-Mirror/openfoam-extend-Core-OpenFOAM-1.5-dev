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
    Generalized grid interface (GGI) patch, providing coupling
    between arbitrary patches which belong to the same fvMesh

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

Contributor
    Martin Beaudoin, Hydro-Quebec, (2008)

\*---------------------------------------------------------------------------*/

#include "ggiFvPatch.H"
#include "addToRunTimeSelectionTable.H"
#include "fvBoundaryMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(ggiFvPatch, 0);
    addToRunTimeSelectionTable(fvPatch, ggiFvPatch, polyPatch);
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

// Make patch weighting factors
void Foam::ggiFvPatch::makeWeights(scalarField& w) const
{
    // Calculation of weighting factors is performed from the master
    // position, using reconstructed shadow cell centres
    // HJ, 2/Aug/2007
    if (ggiPolyPatch_.master())
    {
        vectorField n = nf();
        scalarField nfc = n & (ggiPolyPatch_.reconFaceCellCentres() - Cf());

        w = nfc/((n & (Cf() - Cn())) + nfc);
    }
    else
    {
        // Pick up weights from the master side
        scalarField masterWeights(shadow().size());
        shadow().makeWeights(masterWeights);

        //HJ, change 1: partial coverage in cyclic GGI. HJ, 18/Jan/2009
        //    Experimental: avoiding zeros in uncovered faces
//         w = interpolate(1 - masterWeights);
        w = 1 - interpolate(masterWeights);
    }
}


// Make patch face - neighbour cell distances
void Foam::ggiFvPatch::makeDeltaCoeffs(scalarField& dc) const
{
    if (ggiPolyPatch_.master())
    {
        dc = 1.0/max(nf() & fvPatch::delta(), 0.05*mag(fvPatch::delta()));
    }
    else
    {
        scalarField masterDeltas(shadow().size());
        shadow().makeDeltaCoeffs(masterDeltas);
        dc = interpolate(masterDeltas);
    }
}


void Foam::ggiFvPatch::makeCorrVecs(vectorField& cv) const
{
    // No non-orthogonality correction on a ggi interface
    // HJ, 2/Aug/2007
    cv = vector::zero;

    // Full non-orthogonality treatment

    // Calculate correction vectors on coupled patches
//     const scalarField& patchDeltaCoeffs = deltaCoeffs();

//     vectorField patchDeltas = delta();
//     vectorField n = nf();
//     cv = n - patchDeltas*patchDeltaCoeffs;
}


const Foam::ggiFvPatch& Foam::ggiFvPatch::shadow() const
{
    const fvPatch& p = this->boundaryMesh()[ggiPolyPatch_.shadowIndex()];

    return refCast<const ggiFvPatch>(p);
}


// Return delta (P to N) vectors across coupled patch
Foam::tmp<Foam::vectorField> Foam::ggiFvPatch::delta() const
{
    if (ggiPolyPatch_.master())
    {
        return ggiPolyPatch_.reconFaceCellCentres() - Cn();
    }
    else
    {
        return interpolate
        (
            shadow().Cn() - ggiPolyPatch_.shadow().reconFaceCellCentres()
        );
    }
}


Foam::tmp<Foam::labelField> Foam::ggiFvPatch::interfaceInternalField
(
    const unallocLabelList& internalData
) const
{
    return patchInternalField(internalData);
}


Foam::tmp<Foam::labelField> Foam::ggiFvPatch::transfer
(
    const Pstream::commsTypes,
    const unallocLabelList& interfaceData
) const
{
    notImplemented
    (
        "ggiFvPatchField<Type>::"
        "transfer(const unallocLabelList& interfaceData) const"
    );

    return labelField::null();
}


Foam::tmp<Foam::labelField> Foam::ggiFvPatch::internalFieldTransfer
(
    const Pstream::commsTypes,
    const unallocLabelList& iF
) const
{
    return shadow().patchInternalField(iF);
}


// ************************************************************************* //
