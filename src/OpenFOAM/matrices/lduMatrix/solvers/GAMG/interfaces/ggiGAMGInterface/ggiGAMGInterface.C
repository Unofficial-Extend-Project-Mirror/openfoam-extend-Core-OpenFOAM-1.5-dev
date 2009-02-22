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

#include "ggiGAMGInterface.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(ggiGAMGInterface, 0);
    addToRunTimeSelectionTable
    (
        GAMGInterface,
        ggiGAMGInterface,
        lduInterface
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::ggiGAMGInterface::ggiGAMGInterface
(
    const lduPrimitiveMesh& lduMesh,
    const lduInterface& fineInterface,
    const labelField& localRestrictAddressing,
    const labelField& neighbourRestrictAddressing
)
:
    GAMGInterface
    (
        lduMesh,
        fineInterface,
        localRestrictAddressing,
        neighbourRestrictAddressing
    ),
    fineGgiInterface_(refCast<const ggiLduInterface>(fineInterface))
{
    // Make a lookup table of entries for owner/neighbour.
    // All sizes are guessed at the size of fine interface
    // HJ, 19/Feb/2009

    HashTable<SLList<label>, label, Hash<label> > neighboursTable
    (
        localRestrictAddressing.size()
    );

    // Table of face-sets to be agglomerated
    HashTable<SLList<SLList<label> >, label, Hash<label> > faceFaceTable
    (
        localRestrictAddressing.size()
    );

    // Table of face-sets weights to be agglomerated
    HashTable<SLList<SLList<scalar> >, label, Hash<label> > faceFaceWeightsTable
    (
        localRestrictAddressing.size()
    );

    // Count the number of coarse faces
    label nCoarseFaces = 0;

    // Count the number of agglomeration pairs
    label nAgglomPairs = 0;

    // On the fine level, addressing is made in a labelListList
    if (fineGgiInterface_.fineLevel())
    {
        const labelListList& fineAddr = fineGgiInterface_.addressing();
        const scalarListList& fineWeights = fineGgiInterface_.weights();

        forAll (fineAddr, ffI)
        {
            const labelList& curFineNbrs = fineAddr[ffI];
            const scalarList& curFineWeigts = fineWeights[ffI];

            forAll (curFineNbrs, nbrI)
            {
                label curMaster = -1;
                label curSlave = -1;

                // My label = ffI
                // Nbr label = nnI
                const label nnI = curFineNbrs[nbrI];
                const scalar curNW = curFineWeigts[nbrI];

                if (fineGgiInterface_.master())
                {
                    // Master side
                    curMaster = localRestrictAddressing[ffI];
                    curSlave = neighbourRestrictAddressing[nnI];
                }
                else
                {
                    // Slave side
                    curMaster = neighbourRestrictAddressing[nnI];
                    curSlave = localRestrictAddressing[ffI];
                }

                // Look for the master cell.  If it has already got a face,
                // add the coefficient to the face.  If not, create a new
                // face.
                if (neighboursTable.found(curMaster))
                {
                    // Check all current neighbours to see if the current
                    // slave already exists.  If so, add the coefficient.

                    SLList<label>& curNbrs = neighboursTable.find(curMaster)();

                    SLList<SLList<label> >& curFaceFaces =
                        faceFaceTable.find(curMaster)();

                    SLList<SLList<scalar> >& curFaceWeights =
                        faceFaceWeightsTable.find(curMaster)();

                    bool nbrFound = false;

                    SLList<label>::iterator nbrsIter = curNbrs.begin();

                    SLList<SLList<label> >::iterator faceFacesIter =
                        curFaceFaces.begin();

                    SLList<SLList<scalar> >::iterator faceFaceWeightsIter =
                        curFaceWeights.begin();

                    for
                    (
                        ;
                        nbrsIter != curNbrs.end(),
                        faceFacesIter != curFaceFaces.end(),
                        faceFaceWeightsIter != curFaceWeights.end();
                        ++nbrsIter, ++faceFacesIter, ++faceFaceWeightsIter
                    )
                    {
                        if (nbrsIter() == curSlave)
                        {
                            nbrFound = true;
                            faceFacesIter().append(ffI);
                            faceFaceWeightsIter().append(curNW);
                            nAgglomPairs++;
                            break;
                        }
                    }

                    if (!nbrFound)
                    {
                        curNbrs.append(curSlave);
                        curFaceFaces.append(ffI);
                        curFaceWeights.append(curNW);

                        // New coarse face created
                        nCoarseFaces++;
                        nAgglomPairs++;
                    }
                }
                else
                {
                    // This master has got no neighbours yet.  Add a neighbour
                    // and a coefficient, thus creating a new face
                    neighboursTable.insert(curMaster, SLList<label>(curSlave));
                    faceFaceTable.insert
                    (
                        curMaster,
                        SLList<SLList<label> >(ffI)
                    );
                    faceFaceWeightsTable.insert
                    (
                        curMaster,
                        SLList<SLList<scalar> >(curNW)
                    );

                    // New coarse face created
                    nCoarseFaces++;
                    nAgglomPairs++;
                }
            } // end for all current neighbours
        } // end for all fine faces
    }
    else
    {
        // Coarse level, addressing is stored in faceCells
        forAll (localRestrictAddressing, ffi)
        {
            label curMaster = -1;
            label curSlave = -1;

            // Do switching on master/slave indexes based on the
            // owner/neighbour of the processor index such that
            // both sides get the same answer.
            if (master())
            {
                // Master side
                curMaster = localRestrictAddressing[ffi];
                curSlave = neighbourRestrictAddressing[ffi];
            }
            else
            {
                // Slave side
                curMaster = neighbourRestrictAddressing[ffi];
                curSlave = localRestrictAddressing[ffi];
            }

            // Look for the master cell.  If it has already got a face,
            // add the coefficient to the face.  If not, create a new face.
            if (neighboursTable.found(curMaster))
            {
                // Check all current neighbours to see if the current slave
                // already exists and if so, add the fine face
                // to the agglomeration.

                SLList<label>& curNbrs = neighboursTable.find(curMaster)();

                SLList<SLList<label> >& curFaceFaces =
                    faceFaceTable.find(curMaster)();

                    SLList<SLList<scalar> >& curFaceWeights =
                        faceFaceWeightsTable.find(curMaster)();

                bool nbrFound = false;

                SLList<label>::iterator nbrsIter = curNbrs.begin();

                SLList<SLList<label> >::iterator faceFacesIter =
                    curFaceFaces.begin();

                SLList<SLList<scalar> >::iterator faceFaceWeightsIter =
                    curFaceWeights.begin();

                for
                (
                    ;
                    nbrsIter != curNbrs.end(),
                    faceFacesIter != curFaceFaces.end(),
                    faceFaceWeightsIter != curFaceWeights.end();
                    ++nbrsIter, ++faceFacesIter, ++faceFaceWeightsIter
                )
                {
                    if (nbrsIter() == curSlave)
                    {
                        nbrFound = true;
                        faceFacesIter().append(ffi);
                        // Add dummy weight
                        faceFaceWeightsIter().append(1.0);
                        nAgglomPairs++;
                        break;
                    }
                }

                if (!nbrFound)
                {
                    curNbrs.append(curSlave);
                    curFaceFaces.append(ffi);
                    // Add dummy weight
                    curFaceWeights.append(1.0);

                    // New coarse face created
                    nCoarseFaces++;
                    nAgglomPairs++;
                }
            }
            else
            {
                // This master has got no neighbours yet.  Add a neighbour
                // and a coefficient, thus creating a new face
                neighboursTable.insert(curMaster, SLList<label>(curSlave));
                faceFaceTable.insert(curMaster, SLList<SLList<label> >(ffi));
                // Add dummy weight
                faceFaceWeightsTable.insert
                (
                    curMaster,
                    SLList<SLList<scalar> >(1.0)
                );

                // New coarse face created
                nCoarseFaces++;
                nAgglomPairs++;
            }
        } // end for all fine faces
    }

    faceCells_.setSize(nCoarseFaces, -1);
    fineAddressing_.setSize(nAgglomPairs, -1);
    restrictAddressing_.setSize(nAgglomPairs, -1);
    restrictWeights_.setSize(nAgglomPairs);

    labelList contents = neighboursTable.toc();

    // Sort makes sure the order is identical on both sides.
    // HJ, 20/Feb.2009
    sort(contents);

    // Reset face counter for re-use
    nCoarseFaces = 0;
    nAgglomPairs = 0;

    if (master())
    {
        // On master side, the owner addressing is stored in table of contents
        forAll (contents, masterI)
        {
            SLList<label>& curNbrs = neighboursTable.find(contents[masterI])();

            SLList<SLList<label> >& curFaceFaces =
                faceFaceTable.find(contents[masterI])();

            SLList<SLList<scalar> >& curFaceWeights =
                faceFaceWeightsTable.find(contents[masterI])();

            SLList<label>::iterator nbrsIter = curNbrs.begin();
            SLList<SLList<label> >::iterator faceFacesIter =
                curFaceFaces.begin();

            SLList<SLList<scalar> >::iterator faceFaceWeightsIter =
                curFaceWeights.begin();

            for
            (
                ;
                nbrsIter != curNbrs.end(),
                faceFacesIter != curFaceFaces.end(),
                faceFaceWeightsIter != curFaceWeights.end();
                ++nbrsIter, ++faceFacesIter, ++faceFaceWeightsIter
            )
            {
                faceCells_[nCoarseFaces] = contents[masterI];

                SLList<label>::iterator facesIter = faceFacesIter().begin();
                SLList<scalar>::iterator weightsIter =
                    faceFaceWeightsIter().begin();

                for
                (
                    ;
                    facesIter != faceFacesIter().end(),
                    weightsIter != faceFaceWeightsIter().end();
                    ++facesIter, ++weightsIter
                )
                {
                    fineAddressing_[nAgglomPairs] = facesIter();
                    restrictAddressing_[nAgglomPairs] = nCoarseFaces;
                    restrictWeights_[nAgglomPairs] = weightsIter();
                    nAgglomPairs++;
                }

                nCoarseFaces++;
            }
        }
    }
    else
    {
        // On slave side, the owner addressing is stored in linked lists
        forAll (contents, masterI)
        {
            SLList<label>& curNbrs = neighboursTable.find(contents[masterI])();

            SLList<SLList<label> >& curFaceFaces =
                faceFaceTable.find(contents[masterI])();

            SLList<SLList<scalar> >& curFaceWeights =
                faceFaceWeightsTable.find(contents[masterI])();

            SLList<label>::iterator nbrsIter = curNbrs.begin();
            SLList<SLList<label> >::iterator faceFacesIter =
                curFaceFaces.begin();

            SLList<SLList<scalar> >::iterator faceFaceWeightsIter =
                curFaceWeights.begin();
            for
            (
                ;
                nbrsIter != curNbrs.end(),
                faceFacesIter != curFaceFaces.end(),
                faceFaceWeightsIter != curFaceWeights.end();
                ++nbrsIter, ++faceFacesIter, ++faceFaceWeightsIter
            )
            {
                faceCells_[nCoarseFaces] = nbrsIter();

                SLList<label>::iterator facesIter = faceFacesIter().begin();
                SLList<scalar>::iterator weightsIter =
                    faceFaceWeightsIter().begin();

                for
                (
                    ;        
                    facesIter != faceFacesIter().end(),
                    weightsIter != faceFaceWeightsIter().end();
                    ++facesIter, ++weightsIter
                )
                {
                    fineAddressing_[nAgglomPairs] = facesIter();
                    restrictAddressing_[nAgglomPairs] = nCoarseFaces;
                    restrictWeights_[nAgglomPairs] = weightsIter();
                    nAgglomPairs++;
                }

                nCoarseFaces++;
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Destructor * * * * * * * * * * * * * * * //

Foam::ggiGAMGInterface::~ggiGAMGInterface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::ggiGAMGInterface::master() const
{
    return fineGgiInterface_.master();
}


bool Foam::ggiGAMGInterface::fineLevel() const
{
    return false;
}


Foam::label Foam::ggiGAMGInterface::shadowIndex() const
{
    return fineGgiInterface_.shadowIndex();
}


const Foam::ggiGAMGInterface& Foam::ggiGAMGInterface::shadow() const
{
    return refCast<const ggiGAMGInterface>(ldu().interfaces()[shadowIndex()]);
}


const Foam::labelListList& Foam::ggiGAMGInterface::addressing() const
{
    FatalErrorIn("const labelListList& ggiGAMGInterface::addressing() const")
        << "Requested fine addressing at coarse level"
        << abort(FatalError);
    return labelListList::null();
}


const Foam::scalarListList& Foam::ggiGAMGInterface::weights() const
{
    FatalErrorIn("const labelListList& ggiGAMGInterface::weights() const")
        << "Requested fine addressing at coarse level"
        << abort(FatalError);
    return scalarListList::null();
}


const Foam::tensorField& Foam::ggiGAMGInterface::forwardT() const
{
    return fineGgiInterface_.forwardT();
}


const Foam::tensorField& Foam::ggiGAMGInterface::reverseT() const
{
    return fineGgiInterface_.reverseT();
}


void Foam::ggiGAMGInterface::initTransfer
(
    const Pstream::commsTypes commsType,
    const unallocLabelList& interfaceData
) const
{
    labelTransferBuffer_ = interfaceData;
}


Foam::tmp<Foam::labelField> Foam::ggiGAMGInterface::transfer
(
    const Pstream::commsTypes,
    const unallocLabelList& interfaceData
) const
{
    return this->shadow().labelTransferBuffer();
}


void Foam::ggiGAMGInterface::initInternalFieldTransfer
(
    const Pstream::commsTypes commsType,
    const unallocLabelList& iF
) const
{
    labelTransferBuffer_ = interfaceInternalField(iF);
}


void Foam::ggiGAMGInterface::initInternalFieldTransfer
(
    const Pstream::commsTypes commsType,
    const scalarField& iF
) const
{
    fieldTransferBuffer_ = interfaceInternalField(iF);
}


Foam::tmp<Foam::labelField> Foam::ggiGAMGInterface::internalFieldTransfer
(
    const Pstream::commsTypes,
    const unallocLabelList&
) const
{
    return shadow().labelTransferBuffer();
}


Foam::tmp<Foam::scalarField> Foam::ggiGAMGInterface::internalFieldTransfer
(
    const Pstream::commsTypes,
    const scalarField&
) const
{
    return shadow().fieldTransferBuffer();
}


// ************************************************************************* //
