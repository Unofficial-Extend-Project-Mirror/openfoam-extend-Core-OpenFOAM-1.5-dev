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

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*----------------------------------------------------------------------------*/

#include "surfaceGraph.H"
#include "addToRunTimeSelectionTable.H"
#include "volFields.H"
#include "writePatchGraph.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(surfaceGraph, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        surfaceGraph,
        dictionary
    );
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

Foam::direction Foam::surfaceGraph::setDirection(const word& key) const
{
    if (key == "x" || key == "X")
    {
        return 0;
    }
    else if (key == "y" || key == "Y")
    {
        return 1;
    }
    else if (key == "z" || key == "Z")
    {
        return 2;
    }

    FatalErrorIn
    (
        "direction surfaceGraph::setDirection(const word& key) const"
    )   << "Invalid direction " << key << abort(FatalError);

    // Dummy return to keep compiler happy
    return 0;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::surfaceGraph::surfaceGraph
(
    const Time& t,
    const dictionary& dict
)
:
    functionObject(),
    time_(t),
    regionName_(polyMesh::defaultRegion),
    fieldNames_(dict.lookup("fields")),
    patchNames_(dict.lookup("patches")),
    direction_(setDirection(dict.lookup("direction"))),
    graphFormat_(dict.lookup("format"))
{
    if (dict.found("region"))
    {
        dict.lookup("region") >> regionName_;
    }

    Info<< "Creating surfaceGraph for patches "
        << fieldNames_ << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::surfaceGraph::start()
{
    return true;
}


bool Foam::surfaceGraph::execute()
{
    if (time_.outputTime())
    {
        const fvMesh& mesh =
            time_.lookupObject<fvMesh>(regionName_);

        forAll (fieldNames_, objectI)
        {
            const volScalarField& f =
                mesh.lookupObject<volScalarField>(fieldNames_[objectI]);

            forAll (patchNames_, patchI)
            {
                const label curPatch =
                    mesh.boundaryMesh().findPatchID(patchNames_[patchI]);

                if (curPatch > -1)
                {
                    // Patch found.  Make a graph
                    writePatchGraph(f, curPatch, direction_, graphFormat_);
                }
                else
                {
                    FatalErrorIn
                    (
                        "bool surfaceGraph::execute()"
                    )   << "Patch named " << patchNames_[patchI]
                        << " not found.  Available patches are: "
                        << mesh.boundaryMesh().names()
                        << abort(FatalError);
                }
            }
        }
    }

    return true;
}


bool Foam::surfaceGraph::read(const dictionary& dict)
{
    fieldNames_ = wordList(dict.lookup("objects"));
    patchNames_ = wordList(dict.lookup("patches"));
    direction_ = setDirection(dict.lookup("direction"));
    graphFormat_ = word(dict.lookup("format"));

    return false;
}

// ************************************************************************* //
