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

Application
    setAlpha

Description
    Set the initial field of alpha.

\*---------------------------------------------------------------------------*/

#include "Time.H"

#include "fvCFD.H"
#include "OSspecific.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

#   include "setRootCase.H"

#   include "createTime.H"
#   include "createMesh.H"
#   include "createFields.H"
#   include "readInitProperties.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    const volVectorField& centres = mesh.C();

    forAll(centres, celli)
    {
        if 
        (
            (centres[celli].x() <= x0) 
        )
        {
            p[celli] = pL;
            T[celli] = TL;
            U[celli] = UL;
        }
        else
        {
            p[celli] = pR;
            T[celli] = TR;
            U[celli] = UR;
        }
    }

    p.correctBoundaryConditions();
    p.write();

    T.correctBoundaryConditions();
    T.write();

    U.correctBoundaryConditions();
    U.write();

    Info<< "End\n" << endl;

    return(0);
}


// ************************************************************************* //
