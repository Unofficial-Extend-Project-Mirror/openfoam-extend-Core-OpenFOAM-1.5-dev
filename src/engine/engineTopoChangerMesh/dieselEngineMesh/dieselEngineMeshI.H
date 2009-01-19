/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2005 OpenCFD Ltd.
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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

inline const enginePiston& dieselEngineMesh::piston() const
{
    return piston_;
}


inline const dieselValveBank& dieselEngineMesh::valves() const
{
    return valves_;
}
                 
inline const scalar& dieselEngineMesh::deformSwitch() const
{
    return deformSwitch_;
}

inline const scalar& dieselEngineMesh::delta() const
{
    return delta_;
}

inline const scalar& dieselEngineMesh::offSet() const
{
    return offSet_;
}


inline const scalar& dieselEngineMesh::pistonPosition() const
{
    return pistonPosition_;
}

inline scalar& dieselEngineMesh::pistonPosition()
{
    return pistonPosition_;
}

inline const scalar& dieselEngineMesh::virtualPistonPosition() const
{
    return virtualPistonPosition_;
}

inline scalar& dieselEngineMesh::virtualPistonPosition()
{
    return virtualPistonPosition_;
}


inline const scalar& dieselEngineMesh::deckHeight() const
{
    return deckHeight_;
}

inline scalar& dieselEngineMesh::deckHeight()
{
    return deckHeight_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
