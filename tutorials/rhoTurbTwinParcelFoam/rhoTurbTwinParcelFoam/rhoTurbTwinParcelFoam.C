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
    rhoTurbFoam

Description
    Transient solver for compressible, turbulent flow with two thermo-clouds.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "basicThermo.H"
#include "compressible/RASModel/RASModel.H"

#include "basicThermoCloud.H"
#include "basicKinematicCloud.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

#   include "setRootCase.H"

#   include "createTime.H"
#   include "createMesh.H"
#   include "readEnvironmentalProperties.H"
#   include "createFields.H"
#   include "readPISOControls.H"
#   include "initContinuityErrs.H"
#   include "readTimeControls.H"
#   include "compressibleCourantNo.H"
#   include "setInitialDeltaT.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
#       include "readTimeControls.H"
#       include "readPISOControls.H"
#       include "compressibleCourantNo.H"
#       include "setDeltaT.H"

        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        Info<< "Evolving thermoCloud1" << endl;
        thermoCloud1.evolve();
        thermoCloud1.info();

        Info<< "Evolving kinematicCloud1" << endl;
        kinematicCloud1.evolve();
        kinematicCloud1.info();


#       include "rhoEqn.H"

        // --- PIMPLE loop
        for (int ocorr=1; ocorr<=nOuterCorr; ocorr++)
        {
#           include "UEqn.H"

            // --- PISO loop
            for (int corr=1; corr<=nCorr; corr++)
            {
#               include "hEqn.H"
#               include "pEqn.H"
            }
        }

        turbulence->correct();

        rho = thermo->rho();

        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return(0);
}


// ************************************************************************* //
