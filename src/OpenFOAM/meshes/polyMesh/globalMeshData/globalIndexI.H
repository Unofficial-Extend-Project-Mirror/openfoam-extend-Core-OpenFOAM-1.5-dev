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

#include "ListOps.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const Foam::labelList& Foam::globalIndex::offsets() const
{
    return offsets_;
}


inline Foam::label Foam::globalIndex::size() const
{
    return offsets_[Pstream::nProcs()-1];
}


inline Foam::label Foam::globalIndex::toGlobal(const label i) const
{
    return
    (
        Pstream::myProcNo() == 0
      ? i
      : i + offsets_[Pstream::myProcNo()-1]
    );
}

//- Is on local processor
inline bool Foam::globalIndex::isLocal(const label i) const
{
    label localI =
    (
        Pstream::myProcNo() == 0
      ? i
      : i - offsets_[Pstream::myProcNo()-1]
    );

    return localI >= 0 && localI < offsets_[Pstream::myProcNo()];
}


inline Foam::label Foam::globalIndex::toLocal(const label procI, const label i)
 const
{
    label localI = (procI == 0 ? i : i - offsets_[procI-1]);

    if (localI < 0 || localI >= offsets_[procI])
    {
        FatalErrorIn("globalIndex::toLocal(const label, const label)")
            << "Global " << i << " does not belong on processor "
            << procI << endl << "Offsets:" << offsets_
            << abort(FatalError);
    }
    return localI;
}


inline Foam::label Foam::globalIndex::toLocal(const label i) const
{
    return toLocal(Pstream::myProcNo(), i);
}


inline Foam::label Foam::globalIndex::whichProcID(const label i) const
{
    label index = findLower(offsets_, i+1);

    if (index == Pstream::nProcs()-1)
    {
        FatalErrorIn("globalIndex::whichProcID(const label)")
            << "Global " << i << " does not belong on any processor."
            << " Offsets:" << offsets_
            << abort(FatalError);
    }

    return index+1;
}


// ************************************************************************* //
