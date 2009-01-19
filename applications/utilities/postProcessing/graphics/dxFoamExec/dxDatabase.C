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

\*---------------------------------------------------------------------------*/

#include "dxDatabase.H"
#include "foamValid.H"
#include "dxFvMesh.H"
#include "long.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
dxDatabase::dxDatabase
(
    const word& name,
    const fileName& rootPath,
    const fileName& caseName,
    const fileName& systemName,
    const fileName& constantName
)
:
    Time
    (
        name,
        rootPath,
        caseName,
        systemName,
        constantName
    ),
    refCount_(1),
    meshPtr_(NULL),
    field_(NULL)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

dxDatabase::~dxDatabase()
{
    Info<< "dxDatabase::~dxDatabase() : deleting dxDatabase "
        << long(this)
        << endl; 

    if (refCount_ != 0)
    {
        FatalErrorIn("dxDatabase::~dxDatabase()")
            << "Problem: refCount is " << refCount_
            << "  meshPtr_ is " << long(meshPtr_)
            << abort(FatalError);
    }

    if (meshPtr_)
    {
        delete meshPtr_;

        meshPtr_ = NULL;
    }

    if (validField())
    {
        Info<<  "dxDatabase::~dxDatabase() : deleting field "
            << long(field_) << endl;

        DXDelete(reinterpret_cast<Object>(field_));
        clearField();
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
