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

#include "dxFoamExecInit.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(Foam::dxFoamExecInit, 0);


Foam::dxFoamExecInit Foam::dxFoamExecInit::dxFoamExecArgs("dxFoamExec");


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from string
Foam::dxFoamExecInit::dxFoamExecInit(const string& execName)
{
    // Tricky: argList wants non-const access to argv
    // So create a c_str from execName (owned by execName)
    // and make sure not to access execName within this context
    // (so c_str is preserved)

    char** argv = new char*[1];
    argv[0] = const_cast<char*>(execName.c_str());

    int nArgs = 1;
    argList::validArgs.clear();
    argList args(nArgs, argv);

    delete[] argv;

    // Set streams to throw error
    if (debug)
    {
        Info<< "Debugging on" << endl;
    }
    else
    {
        FatalError.throwExceptions();
        FatalIOError.throwExceptions();
    }
}


// ************************************************************************* //
