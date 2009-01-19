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

#include "twoStrokeEngine.H"
#include "slidingInterface.H"
#include "layerAdditionRemoval.H"
#include "surfaceFields.H"
#include "regionSplit.H"
#include "pointSet.H"
#include "faceSet.H"
#include "cellSet.H"
#include "SortableList.H"


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::twoStrokeEngine::addZonesAndModifiers()
{
    // Add the zones and mesh modifiers to operate piston motion

    if
    (
      faceZones().size() > 0
    ) 
    {
        Info<< "Time = " << engTime().theta() << endl;
        Info<< "void twoStrokeEngine::addZonesAndModifiers() : "
            << "Zones and modifiers already present.  Skipping."
            << endl;
        
        if (topoChanger_.size() == 0)
        {
            FatalErrorIn
            (
                "void twoStrokeEngine::addZonesAndModifiers()"
            )   << "Mesh modifiers not read properly"
                << abort(FatalError);
        }

        setVirtualPistonPosition();
        checkAndCalculate();
        
        return;


    }

    Info << "checkAndCalculate()" << endl;
    checkAndCalculate();
    

    Info<< "Time = " << engTime().theta() << endl
        << "Adding zones to the engine mesh" << endl;


    //fz = 4: virtual piston, outSidePort, insidePort, cutFaceZone 
    //pz = 2: piston points, cutPointZone
    //cz = 1: moving mask

    DynamicList<pointZone*> pz(3);
    DynamicList<faceZone*> fz(4);
    DynamicList<cellZone*> cz(1);

    label nPointZones = 0;
    label nFaceZones = 0;
    label nCellZones = 0;

    Info << "Adding piston layer faces" << endl;

#   include "addPistonLayerFaces.H" 
    
//  adding head points (does not move)
    
    {
        
        pointSet headPointSet(*this, headPointsSetName_);
    
        Info << "Number of head points = " << headPointSet.size() << endl;
        pz[nPointZones] = 
            new pointZone
            (
                "headPoints",
                headPointSet.toc(),
                nPointZones,
                pointZones()
            );

        nPointZones++;
        
    }
    
//  Sliding interface for scavenging ports

    if(foundScavPorts())
    {    
        
        // Inner slider
        
        const polyPatch& innerScav =
            boundaryMesh()[boundaryMesh().findPatchID(scavInCylPatchName_)];

        labelList isf(innerScav.size());

        forAll (isf, i)
        {
            isf[i] = innerScav.start() + i;
        }

        fz[nFaceZones] = new faceZone
        (
            scavInCylPatchName_ + "Zone",
            isf,
            boolList(innerScav.size(), false),
            nFaceZones,
            faceZones()
        );

        nFaceZones++;

        // Outer slider        

        const polyPatch& outerScav =
            boundaryMesh()[boundaryMesh().findPatchID(scavInPortPatchName_)];
                
        labelList osf(outerScav.size());

        forAll (osf, i)
        {
            osf[i] = outerScav.start() + i;
        }

        fz[nFaceZones] = new faceZone
        (
            scavInPortPatchName_ + "Zone",
            osf,
            boolList(outerScav.size(), false),
            nFaceZones,
            faceZones()
        );

        nFaceZones++;
        
        fz[nFaceZones] = new faceZone
        (
            "cutFaceZone",
            labelList(0),
            boolList(0, false),
            nFaceZones,
            faceZones()
        );
        
        nFaceZones++;

        Info << "cut p" << endl;
        
        pz[nPointZones] = new pointZone
        (
            "cutPointZone",
            labelList(0),
            nPointZones,
            pointZones()
        );
        
        nPointZones++;
                
    }
    


    {

        cellSet movingCells(*this, movingCellSetName_);
        
        cz[nCellZones] = new cellZone
        (
            "movingCells",
            movingCells.toc(),
            nCellZones,
            cellZones()
        );
        
        nCellZones++;
    }
    
    Info<< "Adding " << nPointZones << " point, "
        << nFaceZones << " face zones and " << nCellZones << " cell zones" << endl;

    pz.setSize(nPointZones);
    fz.setSize(nFaceZones);
    cz.setSize(nCellZones);
    addZones(pz, fz, cz);

    List<polyMeshModifier*> tm(2);
    label nMods = 0;

    // Add piston layer addition

#   include "addPistonLayerAdditionRemovalMeshModifier.H"

    if(foundScavPorts())
    {
        topoChanger_.setSize(topoChanger_.size() + 1);
        
        topoChanger_.set
        (
            nMods,
            new slidingInterface
            (
                "valveSlider",
                nMods,
                topoChanger_,
                scavInPortPatchName_ + "Zone",
                scavInCylPatchName_ + "Zone",
                "cutPointZone",
                "cutFaceZone",
                scavInPortPatchName_,
                scavInCylPatchName_,
                slidingInterface::INTEGRAL,
//                slidingInterface::PARTIAL,
                true,                          // Attach-detach action
                intersection::VISIBLE         // Projection algorithm
            )
        );
    
        nMods++;
    }
        
    Info << "Adding " << nMods << " topology modifiers" << endl;

    // Calculating the virtual piston position

    setVirtualPistonPosition();


    topoChanger_.setSize(nMods);

    // Write mesh modifiers
    topoChanger_.writeOpt() = IOobject::AUTO_WRITE;
    topoChanger_.write();
    write();
        
    Info << "virtualPistonPosition = " << virtualPistonPosition() << endl;
    Info << "piston position = " << pistonPosition() << endl;
    
}


// ************************************************************************* //
