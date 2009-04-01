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

#include "ggiGAMGInterfaceField.H"
#include "addToRunTimeSelectionTable.H"
#include "lduMatrix.H"

#include "OSspecific.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(ggiGAMGInterfaceField, 0);
    addToRunTimeSelectionTable
    (
        GAMGInterfaceField,
        ggiGAMGInterfaceField,
        lduInterface
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::ggiGAMGInterfaceField::ggiGAMGInterfaceField
(
    const GAMGInterface& GAMGCp,
    const lduInterfaceField& fineInterface
)
:
    GAMGInterfaceField(GAMGCp, fineInterface),
    ggiInterface_(refCast<const ggiGAMGInterface>(GAMGCp)),
    doTransform_(false),
    rank_(0),
    transferBuffer_()
{
    const ggiLduInterfaceField& p =
        refCast<const ggiLduInterfaceField>(fineInterface);

    doTransform_ = p.doTransform();
    rank_ = p.rank();
}


// * * * * * * * * * * * * * * * * Desstructor * * * * * * * * * * * * * * * //

Foam::ggiGAMGInterfaceField::~ggiGAMGInterfaceField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::ggiGAMGInterfaceField::initInterfaceMatrixUpdate
(
    const scalarField& psiInternal,
    scalarField&,
    const lduMatrix&,
    const scalarField&,
    const direction,
    const Pstream::commsTypes commsType
) const
{
    ggiInterface_.initInternalFieldTransfer(commsType, psiInternal);
}


void Foam::ggiGAMGInterfaceField::updateInterfaceMatrix
(
    const scalarField& psiInternal,
    scalarField& result,
    const lduMatrix&,
    const scalarField& coeffs,
    const direction cmpt,
    const Pstream::commsTypes commsType
) const
{
    scalarField pnf =
        ggiInterface_.internalFieldTransfer(commsType, psiInternal);
    transformCoupleField(pnf, cmpt);

    // Expand data to zone size
    scalarField zonePnf(ggiInterface_.shadowInterface().zoneSize(), 0);

    const labelList& shadowZa =
        ggiInterface_.shadowInterface().zoneAddressing();

    forAll (shadowZa, i)
    {
        zonePnf[shadowZa[i]] = pnf[i];
    }

    // Reduce zone data
    reduce(zonePnf, sumOp<scalarField>());

    const unallocLabelList& faceCells = ggiInterface_.faceCells();

    const labelList& za = ggiInterface_.zoneAddressing();

    forAll(faceCells, elemI)
    {
        result[faceCells[elemI]] -= coeffs[elemI]*zonePnf[za[elemI]];
    }
}


// ************************************************************************* //
