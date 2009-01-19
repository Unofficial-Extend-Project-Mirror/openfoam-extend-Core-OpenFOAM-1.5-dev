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

#include "probesFunctionObject.H"
#include "addToRunTimeSelectionTable.H"
#include "IOprobes.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(probesFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        probesFunctionObject,
        dictionary
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::probesFunctionObject::probesFunctionObject
(
    const Time& t,
    const dictionary& dict
)
:
    functionObject(),
    time_(t),
    dict_(dict),
    regionName_(polyMesh::defaultRegion),
    dictName_()
{
    if (dict.found("region"))
    {
        dict.lookup("region") >> regionName_;
    }

    if (dict.found("dictionary"))
    {
        dict.lookup("dictionary") >> dictName_;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::probesFunctionObject::start()
{
    if (dictName_.size())
    {
        probesPtr_.reset
        (
            new IOprobes
            (
                time_.lookupObject<objectRegistry>(regionName_),
                dictName_
            )
        );
    }
    else
    {
        probesPtr_.reset
        (
            new probes
            (
                time_.lookupObject<objectRegistry>(regionName_),
                dict_
            )
        );
    }

    return true;
}


bool Foam::probesFunctionObject::execute()
{
    probesPtr_->write();

    return true;
}


bool Foam::probesFunctionObject::read(const dictionary& dict)
{
    if (dict != dict_)
    {
        dict_ = dict;
        return start();
    }
    else
    {
        return false;
    }
}

// ************************************************************************* //
