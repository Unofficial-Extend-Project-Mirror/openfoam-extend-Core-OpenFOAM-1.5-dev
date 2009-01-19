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

Class
    probes

Description
    Set of locations to sample.
    Call probes.write() to sample&write files.

SourceFiles
    probes.C

\*---------------------------------------------------------------------------*/

#ifndef probes_H
#define probes_H

#include "OFstream.H"
#include "HashPtrTable.H"
#include "vectorField.H"
#include "boolList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class objectRegistry;
class dictionary;
class fvMesh;

/*---------------------------------------------------------------------------*\
                           Class probes Declaration
\*---------------------------------------------------------------------------*/

class probes
{
    // Private data

        const objectRegistry& obr_;

        // Read from dictonary

            //- Names of fields to probe
            wordList fieldNames_;

            //- Locations to probe
            vectorField probeLocations_;


        // Calculated

            //- Indices (into fieldNames_) of scalar/vector/tensor fields
            wordList scalarFields_;
            wordList vectorFields_;
            wordList sphericalTensorFields_;
            wordList symmTensorFields_;
            wordList tensorFields_;

            //- Cells to be probed (obtained from the locations)
            labelList cellList_;

            //- Current open files
            HashPtrTable<OFstream> probeFilePtrs_;


    // Private Member Functions

        //- Find cells containing probes
        void findCells(const fvMesh&);

        //- Find the fields in the list of the given type
        template<class T>
        void findFields(wordList& typeFieldNames, boolList& foundFields);

        //- Sample and write field
        template<class T>
        void sampleAndWrite(const word& fieldName);

        //- Sample and write all the fields of the given type
        template <class T>
        void sampleAndWrite(const wordList& typeFields);

        //- Disallow default bitwise copy construct and assignment
        probes(const probes&);
        void operator=(const probes&);


public:

    //- Debug switch
    static int debug;


    // Constructors

        //- Construct for given objectRegistry and dictionary
        probes(const objectRegistry&, const dictionary&);


    // Destructor

        virtual ~probes();


    // Member Functions

        //- Sample single field on all sample locations
        template <class T>
        Field<T> sample(const word& fieldName) const;

        //- Sample and write
        void write();

        //- Read the probes
        void read(const dictionary&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "probesTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
