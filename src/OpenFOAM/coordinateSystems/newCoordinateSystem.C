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

#include "coordinateSystem.H"
#include "dictionary.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::autoPtr<Foam::coordinateSystem> Foam::coordinateSystem::New
(
    const word& coordType,
    const word& name,
    const point& origin,
    const vector& axis,
    const vector& dir
)
{
    if (debug)
    {
        Pout<< "coordinateSystem::New(const word&, const word&, "
            << "const vector&, const vector&, const vector&) : "
               "constructing coordinateSystem"
            << endl;
    }

    origAxisDirConstructorTable::iterator cstrIter =
        origAxisDirConstructorTablePtr_->find(coordType);

    if (cstrIter == origAxisDirConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "coordinateSystem::New(const word&, const word&, "
            "const vector&, const vector&, const vector&) : "
            "constructing coordinateSystem"
        )   << "Unknown coordinateSystem type " << coordType << nl << nl
            << "Valid coordinateSystem types are :" << nl
            << origAxisDirConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<coordinateSystem>(cstrIter()(name, origin, axis, dir));
}


Foam::autoPtr<Foam::coordinateSystem> Foam::coordinateSystem::New
(
    const word& coordType,
    const word& name,
    const point& origin,
    const coordinateRotation& cr
)
{
    if (debug)
    {
        Pout<< "coordinateSystem::New(const word&, const word&, "
            << "const vector&, const coordinateRotation&) : "
               "constructing coordinateSystem"
            << endl;
    }

    origRotationConstructorTable::iterator cstrIter =
        origRotationConstructorTablePtr_->find(coordType);

    if (cstrIter == origRotationConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "coordinateSystem::New(const word&, const word&, "
            "const vector&, const coordinateRotation&) : "
            "constructing coordinateSystem"
        )   << "Unknown coordinateSystem type " << coordType << nl << nl
            << "Valid coordinateSystem types are :" << nl
            << origRotationConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<coordinateSystem>(cstrIter()(name, origin, cr));
}


Foam::autoPtr<Foam::coordinateSystem> Foam::coordinateSystem::New
(
    const word& name,
    const dictionary& dict
)
{
    if (debug)
    {
        Pout<< "coordinateSystem::New(const word&, const dictionary&) : "
            << "constructing coordinateSystem"
            << endl;
    }

    // default type is self
    word coordType(typeName_());
    if (dict.found("type"))
    {
	dict.lookup("type") >> coordType;
    }

    // can (must) construct base class directly
    if (coordType == typeName_())
    {
        return autoPtr<coordinateSystem>(new coordinateSystem(name, dict));
    }

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(coordType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalIOErrorIn
        (
            "coordinateSystem::New(const word&, const dictionary&)",
            dict
        )   << "Unknown coordinateSystem type " << coordType << nl << nl
            << "Valid coordinateSystem types are :" << nl
            << "[default: " << typeName_() << "]"
            << dictionaryConstructorTablePtr_->toc()
            << exit(FatalIOError);
    }

    return autoPtr<coordinateSystem>(cstrIter()(name, dict));
}


Foam::autoPtr<Foam::coordinateSystem> Foam::coordinateSystem::New
(
    Istream& is
)
{
    word name(is);
    dictionary dict(is);

    return New(name, dict);
}

// ************************************************************************* //
