// The FOAM Project // File: dieselEngineMesh.C
/* 
-------------------------------------------------------------------------------
 =========         | Class Implementation
 \\      /         |
  \\    /          | Name:   dieselEngineMesh
   \\  /           | Family: engine
    \\/            |
    F ield         | FOAM version: 2.3
    O peration     |
    A and          | Copyright (C) 1991-2004 Nabla Ltd.
    M anipulation  |          All Rights Reserved.
-------------------------------------------------------------------------------
DESCRIPTION

AUTHOR

-------------------------------------------------------------------------------
*/

#include "dieselEngineMesh.H"
#include "slidingInterface.H"
#include "layerAdditionRemoval.H"
#include "surfaceFields.H"
#include "regionSplit.H"
#include "attachDetach.H"
// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::dieselEngineMesh::addZonesAndModifiers()
{
    // Add the zones and mesh modifiers to operate piston motion

    if
    (
/*
        pointZones().size() > 0
     || faceZones().size() > 0
     || cellZones().size() > 0
     || topoChanger_.size() > 0
*/
        pointZones().size() > 0
     && faceZones().size() > 0
     && topoChanger_.size() > 0

    ) 
    {
        Info<< "Time = " << engTime().theta() << endl;
        Info<< "void Foam::dieselEngineMesh::addZonesAndModifiers() : "
            << "Zones and modifiers already present.  Skipping."
            << endl;
        
        setVirtualPositions();
        checkAndCalculate();

        Info << "Point zones found = " << pointZones().size() << endl;
        Info << "Face zones found = " << faceZones().size() << endl;
        Info << "Cell zones found = " << cellZones().size() << endl;
        
        return;

    }
    else
    {
        pointZones().setSize(0);
        faceZones().setSize(0);
        cellZones().setSize(0);
        topoChanger_.setSize(0);
    }

    
    
    
    if(engTime().engineDict().found("zOffset") && engTime().engineDict().found("zDispl"))
    {
        
        Info << "Assembling the cylinder mesh" << endl;
        
        scalarField zOffset((engTime().engineDict().lookup("zOffset")));
        scalarField zDispl((engTime().engineDict().lookup("zDispl")));
        
        if(zOffset.size() != zDispl.size())
        {
            FatalErrorIn("Foam::dieselEngineMesh::addZonesAndModifiers(), different sizes for displacement and offset") << 
            abort(FatalError);
           
        }

        
        forAll(zOffset, si)
        {
                
            pointField pDispl = points();

            forAll(points(), pointI)
            {
                const point p = points()[pointI];
                if(p.z() >= zOffset[si])
                {
                    pDispl[pointI].z() -= zDispl[si];
                }
            }


            movePoints(pDispl);
        
        }
        
        write();
        resetMotion();

        Info << "Cylinder mesh assembled" << endl;
    }  



    Info << "checkAndCalculate()" << endl;
    checkAndCalculate();

    Info<< "Time = " << engTime().theta() << endl
        << "Adding zones to the engine mesh" << endl;

/*
    Point zones
    1) Piston points
*/
    
    DynamicList<pointZone*> pz;
 
/*
    Face zones
    1) Piston layer faces

*/
    DynamicList<faceZone*> fz;

/*
*/

    DynamicList<cellZone*> cz;

    label nPointZones = 0;
    label nFaceZones = 0;
    label nCellZones = 0;

/* 

    Adding the following faces zones:
    1:  pistonLayerFaces

    Adding the following point zones:
    1:  pistonPoints

*/

#   include "addPistonFacesPointZonesDieselEngineMesh.H"


//#   include "addAttachDetachFacesDieselEngineMesh.H"    
    
    Info<< "Adding " << nPointZones << " point, "
        << nFaceZones << " face zones and " << nCellZones << " cell zones" << endl;    

#   include "addValveFaceZonesDieselEngineMesh.H"    

    pz.setSize(nPointZones);
    Info << "setSize pz" << endl;
    fz.setSize(nFaceZones);
    Info << "setSize fz" << endl;
    cz.setSize(nCellZones);
    Info << "setSize cz" << endl;    
    
    addZones(pz, fz, cz);

#   include "addMeshModifiersDieselEngineMesh.H"

    // Calculating the virtual positions of piston and valves

    setVirtualPositions();

    // Write mesh
    topoChanger_.writeOpt() = IOobject::AUTO_WRITE;
    write();

    Info << "virtualPistonPosition = " << virtualPistonPosition() << endl;
    Info << "piston position = " << pistonPosition() << endl;
    
}


// ************************************************************************* //
