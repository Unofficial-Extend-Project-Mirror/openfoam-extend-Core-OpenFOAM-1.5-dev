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

#include "pairPotential.H"
#include "IOstreams.H"

bool Foam::pairPotential::writeEnergyAndForceTables(Ostream& os) const
{
    Info<< "Writing energy and force tables to file for potential "
        << name_ << endl;

    List< Pair <scalar> > eTab(energyTable());

    List< Pair <scalar> > fTab(forceTable());

    forAll(eTab, e)
    {
        os<< eTab[e].first()
          << token::SPACE
          << eTab[e].second()
          << token::SPACE
          << fTab[e].second()
          << nl;
    }

    return os.good();
}


// ************************************************************************* //
