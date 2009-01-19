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

#include "processorAmgInterfaceField.H"
#include "addToRunTimeSelectionTable.H"
#include "lduMatrix.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(processorAmgInterfaceField, 0);
addToRunTimeSelectionTable
(
    amgInterfaceField,
    processorAmgInterfaceField,
    lduInterface
);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
tmp<Field<Type> > processorAmgInterfaceField::interfaceInternalField
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

processorAmgInterfaceField::processorAmgInterfaceField
(
    const lduInterfaceField* fineInterfacePtr
)
:
    amgInterfaceField(fineInterfacePtr),
    myProcNo_(-1),
    neighbProcNo_(-1),
    doTransform_(false),
    forwardT_(1, tensor::zero),
    rank_(0),
    addrPtr_(NULL)
{
    const processorLduInterfaceField& p =
        refCast<const processorLduInterfaceField>(*fineInterfacePtr);

    myProcNo_ = p.myProcNo();
    neighbProcNo_ = p.neighbProcNo();
    doTransform_ = p.doTransform();

    if (doTransform())
    {
        forwardT_ = p.forwardT();
    }

    rank_ = p.rank();

    // Size remains unknown until the addressing is calculated
}


// * * * * * * * * * * * * * * * * Desstructor * * * * * * * * * * * * * * * //

processorAmgInterfaceField::~processorAmgInterfaceField()
{
    deleteDemandDrivenData(addrPtr_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

label processorAmgInterfaceField::size() const
{
    return faceCells().size();
}


const unallocLabelList& processorAmgInterfaceField::faceCells() const
{
    if (!addrPtr_)
    {
        FatalErrorIn
        (
            "const labelField& processorAmgInterfaceField::faceCells() const"
        )   << "addressing not available"
            << abort(FatalError);
    }

    return *addrPtr_;
}


tmp<labelField> processorAmgInterfaceField::interfaceInternalField
(
    const unallocLabelList& internalData
) const
{
    return interfaceInternalField<label>(internalData);
}


void processorAmgInterfaceField::initTransfer
(
    const unallocLabelList& interfaceData,
    const bool bufferedTransfer
) const
{
    send(interfaceData, bufferedTransfer);
}


tmp<labelField> processorAmgInterfaceField::transfer
(
    const unallocLabelList& interfaceData
) const
{
    return receive<label>(this->size());
}


void processorAmgInterfaceField::initInternalFieldTransfer
(
    const unallocLabelList& iF,
    const bool bufferedTransfer
) const
{
    send(interfaceInternalField(iF)(), bufferedTransfer);
}


tmp<labelField> processorAmgInterfaceField::internalFieldTransfer
(
    const unallocLabelList&
) const
{
    return receive<label>(this->size());
}


tmp<scalarField> processorAmgInterfaceField::coeffs
(
    const labelField& localRestrictAddressing,
    const labelField& neighbourRestrictAddressing,
    const scalarField& fineCoeffs
) const
{
    // Make a lookup table of entries for owner/neighbour
    HashTable<SLList<label>, label, Hash<label> > neighbourLookup
        (localRestrictAddressing.size());

    HashTable<SLList<scalar>, label, Hash<label> > neiCoeffs
        (localRestrictAddressing.size());

    label nCoarseFaces = 0;

    forAll (localRestrictAddressing, fineFaceI)
    {
        label curMaster = -1;
        label curSlave = -1;

        // Do switching on master/slave indexes based on the owner/neighbour of
        // the processor index such that both sides get the same answer.
        if (myProcNo() < neighbProcNo())
        {
            // Master side
            curMaster = localRestrictAddressing[fineFaceI];
            curSlave = neighbourRestrictAddressing[fineFaceI];
        }
        else
        {
            // Slave side
            curMaster = neighbourRestrictAddressing[fineFaceI];
            curSlave = localRestrictAddressing[fineFaceI];
        }

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
           "tmp<scalarField> processorAmgInterfaceField::coeffsAddressing\n"
           "(\n"
           "    const labelField& localRestrictAddressing,\n"
           "    const labelField& neighbourRestrictAddressing,\n"
           "    const scalarField& fineCoeffs\n"
           ") const"
       )   << "coefficients and addressing already calculated."
           << abort(FatalError);
    }

    // Allocate addressing and coefficients

    addrPtr_ = new labelField(nCoarseFaces, -1);
    labelField& coarseAddr = *addrPtr_;

    tmp<scalarField> tcoarseCoeffs(new scalarField(nCoarseFaces));
    scalarField& coarseCoeffs = tcoarseCoeffs();

    labelList contents = neighbourLookup.toc();

    // Reset face counter for re-use
    nCoarseFaces = 0;

    if (myProcNo() < neighbProcNo())
    {
        // On master side, the owner addressing is stored in table of contents
        forAll (contents, masterI)
        {
            SLList<label>& curNbrs = neighbourLookup.find(contents[masterI])();
            SLList<scalar>& curNeiCoeffs = neiCoeffs.find(contents[masterI])();

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
                coarseAddr[nCoarseFaces] = contents[masterI];
                coarseCoeffs[nCoarseFaces] = curNeiCoeffsIter();
                nCoarseFaces++;
            }
        }
    }
    else
    {
        // On slave side, the owner addressing is stored in linked lists
        forAll (contents, masterI)
        {
            SLList<label>& curNbrs = neighbourLookup.find(contents[masterI])();
            SLList<scalar>& curNeiCoeffs = neiCoeffs.find(contents[masterI])();

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
                coarseAddr[nCoarseFaces] = curNbrsIter();
                coarseCoeffs[nCoarseFaces] = curNeiCoeffsIter();
                nCoarseFaces++;
            }
        }
    }

    return tcoarseCoeffs;
}


// Initialise neighbour processor internal cell data
void processorAmgInterfaceField::initInterfaceMatrixUpdate
(
    const scalarField& psiInternal,
    scalarField&,
    const lduMatrix&,
    const scalarField&,
    const direction,
    const bool bufferedTransfer
) const
{
    compressedSend(interfaceInternalField(psiInternal)(), bufferedTransfer);
}


// Transfer internal cell data to neighbour processor
// and receive and return neighbour processor internal cell data
void processorAmgInterfaceField::updateInterfaceMatrix
(
    const scalarField&,
    scalarField& result,
    const lduMatrix&,
    const scalarField& coeffs,
    const direction cmpt
) const
{
    scalarField pnf(compressedReceive<scalar>(coeffs.size()));
    transformCoupleField(pnf, cmpt);

    const unallocLabelList& addr = faceCells();

    forAll(addr, elemI)
    {
        result[addr[elemI]] -= coeffs[elemI]*pnf[elemI];
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
