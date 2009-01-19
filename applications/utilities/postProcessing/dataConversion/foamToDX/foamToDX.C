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
    Translates FOAM data to OpenDX format.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "volPointInterpolation.H"
#include "IOobjectList.H"
#include "OFstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::noParallel();

    const label nTypes = 2;
    const word fieldTypes[] =
    {
        volScalarField::typeName,
        volVectorField::typeName
    };

#   include "addTimeOptions.H"
#   include "setRootCase.H"

#   include "createTime.H"
#   include "createMesh.H"

    pointMesh pMesh(mesh);
    volPointInterpolation pInterp(mesh, pMesh);

    fileName dxDataPath(runTime.path()/"dxData");
    mkDir(dxDataPath);

    fileName dxDataFile = dxDataPath/"timeSeries.dx";
    OFstream timeOut(dxDataFile);

#   include "writeDXgrid.H"

    label memberNumber = 0;

    // Get times list
    instantList Times = runTime.times();

    // set startTime and endTime depending on -time and -latestTime options
#   include "checkTimeOptions.H"

    for (label i=startTime; i<endTime; i++)
    {
        runTime.setTime(Times[i], i);

        mesh.readUpdate();

        IOobjectList objects(mesh, runTime.timeName());

        Info<< "Creating timestep " << Times[i].name() << endl;

        fileName dxDataFile(dxDataPath/Times[i].name() + ".data");
        OFstream dataOut(dxDataFile);

#       include "writeDXdata.H"
#       include "writeDXseries.H"
    }
    Info << "\nEnd\n" << endl;

    return 0;
}


// ************************************************************************* //
