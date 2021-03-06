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

#include "polyPatch.H"
#include "dictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

autoPtr<polyPatch> polyPatch::New
(
    const word& patchType,
    const word& name,
    const label size,
    const label start,
    const label index,
    const polyBoundaryMesh& bm
)
{
    if (debug)
    {
        Info<< "polyPatch::New(const word&, const word&, const label, "
               "const label, const label, const polyBoundaryMesh&) : "
               "constructing polyPatch"
            << endl;
    }

    wordConstructorTable::iterator cstrIter =
        wordConstructorTablePtr_->find(patchType);

    if (cstrIter == wordConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "polyPatch::New(const word&, const word&, const label, "
            "const label, const label, const polyBoundaryMesh&) "
        )   << "Unknown polyPatch type " << patchType << " for patch " << name
            << endl << endl
            << "Valid polyPatch types are :" << endl
            << wordConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<polyPatch>(cstrIter()(name, size, start, index, bm));
}


autoPtr<polyPatch> polyPatch::New
(
    const word& name,
    const dictionary& dict,
    const label index,
    const polyBoundaryMesh& bm
)
{
    if (debug)
    {
        Info<< "polyPatch::New(const word&, const dictionary&, const label, "
               "const polyBoundaryMesh&) : constructing polyPatch"
            << endl;
    }

    word patchType(dict.lookup("type"));

    if (dict.found("geometricType"))
    {
        dict.lookup("geometricType") >> patchType;
    }

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(patchType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        if (!disallowGenericPolyPatch)
        {
            cstrIter = dictionaryConstructorTablePtr_->find("genericPatch");
        }

        if (cstrIter == dictionaryConstructorTablePtr_->end())
        {
            FatalIOErrorIn
            (
                "polyPatch::New(const word&, const dictionary&, "
                "const label, const polyBoundaryMesh&)",
                dict
            )   << "Unknown polyPatch type " << patchType
                << " for patch " << name
                << endl << endl
                << "Valid polyPatch types are :" << endl
                << dictionaryConstructorTablePtr_->toc()
                << exit(FatalIOError);
        }
    }

    return autoPtr<polyPatch>(cstrIter()(name, dict, index, bm));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
