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

#include "probes.H"
#include "volFields.H"
#include "dictionary.H"
#include "Time.H"
#include "IOmanip.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

int probes::debug(debug::debugSwitch("probes", 0));

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void probes::findCells(const fvMesh& mesh)
{
    if (cellList_.size() == 0)
    {
        cellList_.setSize(probeLocations_.size());

        forAll(probeLocations_, probeI)
        {
            cellList_[probeI] = mesh.findCell(probeLocations_[probeI]);

            if (debug && cellList_[probeI] != -1)
            {
                Pout<< "probes : found point " << probeLocations_[probeI]
                    << " in cell " << cellList_[probeI] << endl;
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

probes::probes(const objectRegistry& obr, const dictionary& dict)
:
    obr_(obr),
    fieldNames_(0),
    probeLocations_(0),
    scalarFields_(0),
    vectorFields_(0),
    sphericalTensorFields_(0),
    symmTensorFields_(0),
    tensorFields_(0),
    cellList_(0),
    probeFilePtrs_(0)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

probes::~probes()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void probes::write()
{
    sampleAndWrite<scalar>(scalarFields_);
    sampleAndWrite<vector>(vectorFields_);
    sampleAndWrite<sphericalTensor>(sphericalTensorFields_);
    sampleAndWrite<symmTensor>(symmTensorFields_);
    sampleAndWrite<tensor>(tensorFields_);
}


void probes::read(const dictionary& dict)
{
    fieldNames_ = wordList(dict.lookup("fields"));
    probeLocations_ = vectorField(dict.lookup("probeLocations"));

    // Force all cell locations to be redetermined and find them
    cellList_.clear();
    findCells(refCast<const fvMesh>(obr_));

    // Check if all probes have been found.
    forAll(cellList_, probeI)
    {
        label cellI = cellList_[probeI];

        // Check at least one processor with cell.
        reduce(cellI, maxOp<label>());

        if (cellI == -1)
        {
            if (Pstream::master())
            {
                WarningIn("probes::read()")
                    << "Did not find location " << probeLocations_[probeI]
                    << " in any cell. Skipping location." << endl;
            }
        }
        else
        {
            // Make sure location not on two domains.
            if (cellList_[probeI] != -1 && cellList_[probeI] != cellI)
            { 
                WarningIn("probes::read()")
                    << "Location " << probeLocations_[probeI]
                    << " seems to be on multiple domains:"
                    << " cell " << cellList_[probeI]
                    << " on my domain " << Pstream::myProcNo()
                    << " and cell " << cellI << " on some other domain."
                    << endl
                    << "This might happen if the probe location is on"
                    << " a processor patch. Change the location slightly"
                    << " to prevent this." << endl;
            }
        }
    }


    // Split fieldNames according to type.
    boolList foundFields(fieldNames_.size(), false);
    findFields<volScalarField>(scalarFields_, foundFields);
    findFields<volVectorField>(vectorFields_, foundFields);
    findFields<volSphericalTensorField>(sphericalTensorFields_, foundFields);
    findFields<volSymmTensorField>(symmTensorFields_, foundFields);
    findFields<volTensorField>(tensorFields_, foundFields);

    label validFieldi=0;
    forAll(fieldNames_, fieldi)
    {
        if (foundFields[fieldi])
        {
            fieldNames_[validFieldi++] = fieldNames_[fieldi];
        }
        else
        {
            WarningIn("probes::read()")
                << "Unknown field " << fieldNames_[fieldi]
                << " when reading dictionary " << dict.name() << endl
                << "    Can only probe registered volScalar, volVector, "
                << "volSphericalTensor, volSymmTensor and volTensor fields"
                << nl << endl;
        }
    }
    fieldNames_.setSize(validFieldi);


    if (Pstream::master())
    {
        if (debug)
        {
            Pout<< "Probing fields:" << fieldNames_ << nl
                << "Probing locations:" << probeLocations_ << nl
                << endl;
        }

        // Check if any fieldNames have been removed. If so close
        // the file.

        forAllIter(HashPtrTable<OFstream>, probeFilePtrs_, iter)
        {
            if (findIndex(fieldNames_, iter.key()) == -1)
            {
                // Field has been removed. Close file
                delete probeFilePtrs_.remove(iter);
            }
        }

        // Open new files for new fields. Keep existing files.

        probeFilePtrs_.resize(2*fieldNames_.size());

        forAll(fieldNames_, fieldI)
        {
            const word& fldName = fieldNames_[fieldI];

            // Check if added field. If so open a stream for it.

            if (!probeFilePtrs_.found(fldName))
            {
                // Create directory if does not exist.

                fileName probeDir;
                if (Pstream::parRun())
                {
                    // Put in undecomposed case (Note: gives problems for
                    // distributed data running)
                    probeDir =
                        obr_.time().path()
                        /".."
                        /"probes"
                        /obr_.time().timeName();
                }
                else
                {
                    probeDir =
                        obr_.time().path()
                        /"probes"
                        /obr_.time().timeName();
                }
                
                mkDir(probeDir);
                    
                OFstream* sPtr = new OFstream(probeDir/fldName);

                probeFilePtrs_.insert(fldName, sPtr);

                *sPtr<< '#' << setw(IOstream::defaultPrecision() + 6)
                     << "Time";
                
                forAll(probeLocations_, probeI)
                {
                    *sPtr << token::SPACE << probeLocations_[probeI];
                }
                *sPtr << nl;
            }
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
