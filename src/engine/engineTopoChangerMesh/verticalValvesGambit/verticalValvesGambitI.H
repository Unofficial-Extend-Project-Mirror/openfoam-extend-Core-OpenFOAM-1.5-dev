/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2005-2007 Tommaso Lucchini
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
    verticalValvesGambit

\*---------------------------------------------------------------------------*/

namespace Foam
{

inline const simpleEnginePiston& verticalValvesGambit::piston() const
{
    return piston_;
}


inline const verticalValveBank& verticalValvesGambit::valves() const
{
    return valves_;
}
                 
inline const scalar& verticalValvesGambit::deformSwitch() const
{
    return deformSwitch_;
}

inline const scalar& verticalValvesGambit::delta() const
{
    return delta_;
}

inline const scalar& verticalValvesGambit::offSet() const
{
    return offSet_;
}


inline const scalar& verticalValvesGambit::valveTopTol() const
{
    return valveTopTol_;
}


inline const scalar& verticalValvesGambit::pistonPosition() const
{
    return pistonPosition_;
}

inline scalar& verticalValvesGambit::pistonPosition()
{
    return pistonPosition_;
}

inline const scalar& verticalValvesGambit::virtualPistonPosition() const
{
    return virtualPistonPosition_;
}

inline scalar& verticalValvesGambit::virtualPistonPosition()
{
    return virtualPistonPosition_;
}


inline const scalarField& verticalValvesGambit::valveTopPosition() const
{
    return valveTopPosition_;
}

inline scalarField& verticalValvesGambit::valveTopPosition()
{
    return valveTopPosition_;
}


inline const scalarField& verticalValvesGambit::valveBottomPosition() const
{
    return valveBottomPosition_;
}

inline scalarField& verticalValvesGambit::valveBottomPosition()
{
    return valveBottomPosition_;
}

inline const scalarField& verticalValvesGambit::valvePistonPosition() const
{
    return valvePistonPosition_;
}

inline scalarField& verticalValvesGambit::valvePistonPosition()
{
    return valvePistonPosition_;
}

inline const scalar& verticalValvesGambit::deckHeight() const
{
    return deckHeight_;
}

inline scalar& verticalValvesGambit::deckHeight()
{
    return deckHeight_;
}

inline const scalarField& verticalValvesGambit::minValveZ() const
{
    return minValveZ_;
}

inline scalarField& verticalValvesGambit::maxValveZ()
{
    return minValveZ_;
}

inline const scalar& verticalValvesGambit::poppetValveTol() const
{
    return poppetValveTol_;
}

inline boolList& verticalValvesGambit::isReallyClosed()
{
    return isReallyClosed_;
}

inline const boolList& verticalValvesGambit::isReallyClosed() const
{
    return isReallyClosed_;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
