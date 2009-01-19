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

#include "cyclicAmgInterfaceField.H"
#include "addToRunTimeSelectionTable.H"
#include "lduMatrix.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(cyclicAmgInterfaceField, 0);
addToRunTimeSelectionTable
(
    amgInterfaceField,
    cyclicAmgInterfaceField,
    lduInterface
);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
tmp<Field<Type> > cyclicAmgInterfaceField::interfaceInternalField
(
    const UList<Type>& iF
) const
{
    const unallocLabelList& addr = faceCells();

    tmp<Field<Type> > tresult(new Field<Type>(size()));
    Field<Type>& result = tresult();

    forAll (result, elemI)
    {
        result[elemI] = iF[addr[elemI]];
    }

    return tresult;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

cyclicAmgInterfaceField::cyclicAmgInterfaceField
(
    const lduInterfaceField* fineInterfacePtr
)
:
    amgInterfaceField(fineInterfacePtr),
    doTransform_(false),
    forwardT_(1, tensor::zero),
    reverseT_(1, tensor::zero),
    rank_(0),
    addrPtr_(NULL)
{
    const cyclicLduInterfaceField& p =
        refCast<const cyclicLduInterfaceField>(*fineInterfacePtr);

    doTransform_ = p.doTransform();

    if (doTransform())
    {
        forwardT_ = p.forwardT();
        reverseT_ = p.reverseT();
    }

    rank_ = p.rank();

    // Size remains unknown until the addressing is calculated
}


// * * * * * * * * * * * * * * * * Desstructor * * * * * * * * * * * * * * * //

cyclicAmgInterfaceField::~cyclicAmgInterfaceField()
{
    deleteDemandDrivenData(addrPtr_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

label cyclicAmgInterfaceField::size() const
{
    return faceCells().size();
}


const unallocLabelList& cyclicAmgInterfaceField::faceCells() const
{
    if (!addrPtr_)
    {
        FatalErrorIn
        (
            "const labelField& cyclicAmgInterfaceField::faceCells() const"
        )   << "addressing not available"
            << abort(FatalError);
    }

    return *addrPtr_;
}


tmp<labelField> cyclicAmgInterfaceField::interfaceInternalField
(
    const unallocLabelList& internalData
) const
{
    return interfaceInternalField<label>(internalData);
}


tmp<labelField> cyclicAmgInterfaceField::transfer
(
    const unallocLabelList& interfaceData
) const
{
    tmp<labelField> tpnf(new labelField(size()));
    labelField& pnf = tpnf();

    label sizeby2 = size()/2;

    for (label facei=0; facei<sizeby2; facei++)
    {
        pnf[facei] = interfaceData[facei + sizeby2];
        pnf[facei + sizeby2] = interfaceData[facei];
    }

    return tpnf;
}


tmp<labelField> cyclicAmgInterfaceField::internalFieldTransfer
(
    const unallocLabelList& iF
) const
{
    const unallocLabelList& addr = faceCells();

    tmp<labelField> tpnf(new labelField(size()));
    labelField& pnf = tpnf();

    label sizeby2 = size()/2;

    for (label facei=0; facei<sizeby2; facei++)
    {
        pnf[facei] = iF[addr[facei + sizeby2]];
        pnf[facei + sizeby2] = iF[addr[facei]];
    }

    return tpnf;
}



tmp<scalarField> cyclicAmgInterfaceField::coeffs
(
    const labelField& localRestrictAddressing,
    const labelField&,
    const scalarField& fineCoeffs
) const
{
    // Make a lookup table of entries for owner/neighbour
    HashTable<SLList<label>, label, Hash<label> > neighbourLookup
        (localRestrictAddressing.size());

    HashTable<SLList<scalar>, label, Hash<label> > neiCoeffs
        (localRestrictAddressing.size());

    label nCoarseFaces = 0;

    label sizeBy2 = localRestrictAddressing.size()/2;

    for (label fineFaceI = 0; fineFaceI < sizeBy2; fineFaceI++)
    {
        label curMaster = localRestrictAddressing[fineFaceI];
        label curSlave = localRestrictAddressing[fineFaceI + sizeBy2];

        // Look for the master cell.  If it has already got a face,
        // add the coefficient to the face.  If not, create a new
        // face.
        if (neighbourLookup.found(curMaster))
        {
            // Check all current neighbours to see if the current
            // slave already exists.  If so, add the coefficient.

            SLList<label>& curNbrs = neighbourLookup.find(curMaster)();

            SLList<scalar>& curNeiCoeffs = neiCoeffs.find(curMaster)();

            bool nbrFound = false;

            // Warning. Synchronous iterators
            SLList<label>::iterator curNbrsIter = curNbrs.begin();

            SLList<scalar>::iterator curNeiCoeffsIter =
                curNeiCoeffs.begin();

            for
            (
                ;

                curNbrsIter != curNbrs.end(),
                curNeiCoeffsIter != curNeiCoeffs.end();

                ++curNbrsIter,
                ++curNeiCoeffsIter
            )
            {
                if (curNbrsIter() == curSlave)
                {
                    nbrFound = true;

                    curNeiCoeffsIter() += fineCoeffs[fineFaceI];

                    break;
                }
            }

            if (!nbrFound)
            {
                curNbrs.append(curSlave);

                curNeiCoeffs.append(fineCoeffs[fineFaceI]);

                // New coarse face created
                nCoarseFaces++;
            }
        }
        else
        {
            // This master has got no neighbours yet.  Add a neighbour
            // and a coefficient, thus creating a new face
            neighbourLookup.insert(curMaster, SLList<label>(curSlave));
            neiCoeffs.insert(curMaster, SLList<scalar>(fineCoeffs[fineFaceI]));

            // New coarse face created
            nCoarseFaces++;
        }
    } // end for all fine faces

    // Create the coarse interface and coarse interface coefficients
    if (addrPtr_)
    {
        FatalErrorIn
       (
           "tmp<scalarField> cyclicAmgInterfaceField::coeffsAddressing\n"
           "(\n"
           "    const labelField& localRestrictAddressing,\n"
           "    const labelField& neighbourRestrictAddressing,\n"
           "    const scalarField& fineCoeffs\n"
           ") const"
       )   << "coefficients and addressing already calculated."
           << abort(FatalError);
    }

    // Allocate addressing and coefficients

    addrPtr_ = new labelField(2*nCoarseFaces, -1);
    labelField& coarseAddr = *addrPtr_;

    tmp<scalarField> tcoarseCoeffs(new scalarField(2*nCoarseFaces));
    scalarField& coarseCoeffs = tcoarseCoeffs();

    labelList contents = neighbourLookup.toc();

    // Reset face counter for re-use
    nCoarseFaces = 0;

    // On master side, the owner addressing is stored in table of contents
    forAll (contents, masterI)
    {
        SLList<label>& curNbrs = neighbourLookup.find(contents[masterI])();
        SLList<scalar>& curNeiCoeffs = neiCoeffs.find(contents[masterI])();

        // Warning. Synchronous iterators
        SLList<label>::iterator curNbrsIter = curNbrs.begin();

        SLList<scalar>::iterator curNeiCoeffsIter = curNeiCoeffs.begin();

        for
        (
            ;

            curNbrsIter != curNbrs.end(),
            curNeiCoeffsIter != curNeiCoeffs.end();

            ++curNbrsIter,
            ++curNeiCoeffsIter
        )
        {
            coarseAddr[nCoarseFaces] = contents[masterI];
            coarseCoeffs[nCoarseFaces] = curNeiCoeffsIter();
            nCoarseFaces++;
        }
    }

    // On slave side, the owner addressing is stored in linked lists
    forAll (contents, masterI)
    {
        SLList<label>& curNbrs = neighbourLookup.find(contents[masterI])();
        SLList<scalar>& curNeiCoeffs = neiCoeffs.find(contents[masterI])();

        // Warning. Synchronous iterators
        SLList<label>::iterator curNbrsIter = curNbrs.begin();

        SLList<scalar>::iterator curNeiCoeffsIter = curNeiCoeffs.begin();

        for
        (
            ;

            curNbrsIter != curNbrs.end(),
            curNeiCoeffsIter != curNeiCoeffs.end();

            ++curNbrsIter,
            ++curNeiCoeffsIter
        )
        {
            coarseAddr[nCoarseFaces] = curNbrsIter();
            coarseCoeffs[nCoarseFaces] = curNeiCoeffsIter();
            nCoarseFaces++;
        }
    }

    return tcoarseCoeffs;
}


// Transfer internal cell data to neighbour cyclic
// and receive and return neighbour cyclic internal cell data
void cyclicAmgInterfaceField::updateInterfaceMatrix
(
    const scalarField& psiInternal,
    scalarField& result,
    const lduMatrix&,
    const scalarField& coeffs,
    const direction cmpt
) const
{
    const unallocLabelList& addr = faceCells();

    scalarField pnf(size());

    label sizeby2 = size()/2;

    for (label facei=0; facei<sizeby2; facei++)
    {
        pnf[facei] = psiInternal[addr[facei + sizeby2]];
        pnf[facei + sizeby2] = psiInternal[addr[facei]];
    }

    transformCoupleField(pnf, cmpt);

    // Multiply the field by coefficients and add into the result

    forAll(addr, elemI)
    {
        result[addr[elemI]] -= coeffs[elemI]*pnf[elemI];
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
