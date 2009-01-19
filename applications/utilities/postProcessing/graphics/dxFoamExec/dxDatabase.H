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
    dxDatabase

Description

SourceFiles
    dxDatabase.C

\*---------------------------------------------------------------------------*/

#ifndef dxDatabase_H
#define dxDatabase_H

#include "Time.H"
#include "dxFoamExec.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class dxFvMesh;

/*---------------------------------------------------------------------------*\
                           Class dxDatabase Declaration
\*---------------------------------------------------------------------------*/

class dxDatabase
:
    public Time
{
    // Private data

        // Number of items referring to me
        label refCount_;

        // Pointer to Dx object for dxFvMesh
        dxFvMesh* meshPtr_;

        // Pointer to Dx object for lagrangian positions
        DXField field_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        dxDatabase(const dxDatabase&);

        //- Disallow default bitwise assignment
        void operator=(const dxDatabase&);

public:

    // Constructors

        //- Construct from components
        dxDatabase
        (
            const word& name,
            const fileName& rootPath,
            const fileName& caseName,
            const fileName& systemName = "system",
            const fileName& constantName = "constant"
        );


    // Destructor

        ~dxDatabase();


    // Member Functions

        // Access

            label refCount() const
            {
                return refCount_;
            }

            //- Check for valid mesh
            bool validMesh() const
            {
                return meshPtr_ != NULL;
            }

            //- Get mesh
            dxFvMesh& mesh() const
            {
                if (!validMesh())
                {
                    FatalErrorIn("dxDatabase::mesh()")
                        << "No mesh set on database" << abort(FatalError);
                }
                return *meshPtr_;
            }

            //- Check for valid field
            bool validField() const
            {
                return field_ != NULL;
            }

            //- Get field
            DXField field() const
            {
                if (!validField())
                {
                    FatalErrorIn("dxDatabase::field()")
                        << "No field set on database" << abort(FatalError);
                }
                return field_;
            }

        // Edit

            void incCount()
            {
                refCount_++;
            }

            label decCount()
            {
                return --refCount_;
            }

            void setMesh(dxFvMesh& mesh)
            {
                meshPtr_ = &mesh;
            }

            void clearMesh()
            {
                meshPtr_ = NULL;
            }

            void setField(DXField& field)
            {
                field_ = field;
            }

            void clearField()
            {
                field_ = NULL;
            }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

extern "C"
{
    Error deleteDatabase(Pointer dxFoamDatabasePtr);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif

// ************************************************************************* //
