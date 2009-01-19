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
#include "layerAdditionRemoval.H"
#include "attachDetach.H"
#include "componentMixedTetPolyPatchVectorField.H"
#include "mapPolyMesh.H"
#include "polyTopoChange.H"
#include "addToRunTimeSelectionTable.H"
#include "GeometricField.H"
#include "volMesh.H"
#include "engineTime.H"
#include "pointField.H"
#include "fvPatchField.H"
#include "Switch.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(dieselEngineMesh, 0);
    addToRunTimeSelectionTable(engineTopoChangerMesh, dieselEngineMesh, IOobject);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //



    
bool Foam::dieselEngineMesh::realDeformation() const
{

    if (virtualPistonPosition() + engTime().pistonDisplacement().value() > deckHeight_ - SMALL)
    {
        return true;
    }
    else
    {
        return deformation();
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
Foam::dieselEngineMesh::dieselEngineMesh
(
    const IOobject& io
)
:
    engineTopoChangerMesh(io),
    piston_(*this, engTime().engineDict().subDict("piston")),
    valves_(*this, engTime().engineDict().lookup("dieselEngineMesh")),
    deformSwitch_(readScalar(engTime().engineDict().lookup("deformAngle"))),
    delta_(readScalar(engTime().engineDict().lookup("delta"))),
    offSet_(readScalar(engTime().engineDict().lookup("offSet"))),
    pistonPosition_(-GREAT),
    virtualPistonPosition_(-GREAT),
    deckHeight_(GREAT),
    msPtr_(motionSolver::New(*this)),
    valvePlaneTol_(readScalar(engTime().engineDict().lookup("valvePlaneTol")))   

{
    // Add zones and modifiers if not already there.
    addZonesAndModifiers();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void Foam::dieselEngineMesh::setBoundaryVelocity(volVectorField& U)
{
    
    
    // Set valve velociaty
    forAll (valves(), valveI)
    {
            
        vector valveVel =
            valves()[valveI].curVelocity()*valves()[valveI].cs().axis();

        // If valve is present in geometry, set the motion
        if (valves()[valveI].upInPortPatchID().active())
        {
            // Bottom of the valve moves with given velocity
            U.boundaryField()[valves()[valveI].upInPortPatchID().index()] ==
                valveVel;
        }

        // If valve is present in geometry, set the motion
        if (valves()[valveI].upInCylinderPatchID().active())
        {
            // Bottom of the valve moves with given velocity
            U.boundaryField()[valves()[valveI].upInCylinderPatchID().index()] ==
                valveVel;
        }

        // If valve is present in geometry, set the motion
        if (valves()[valveI].downInPortPatchID().active())
        {
            // Bottom of the valve moves with given velocity
            U.boundaryField()[valves()[valveI].downInPortPatchID().index()] ==
                valveVel;
        }

        // If valve is present in geometry, set the motion
        if (valves()[valveI].downInCylinderPatchID().active())
        {
            // Bottom of the valve moves with given velocity
            U.boundaryField()[valves()[valveI].downInCylinderPatchID().index()] ==
                valveVel;
        }
        
        // If valve is present in geometry, set the motion
        if (valves()[valveI].stemPatchID().active())
        {
            // Bottom of the valve moves with given velocity
            U.boundaryField()[valves()[valveI].stemPatchID().index()] ==
                valveVel;
        }

        // If valve is present in geometry, set the motion
        if (valves()[valveI].detachInPortPatchID().active())
        {
            // Bottom of the valve moves with given velocity
            U.boundaryField()[valves()[valveI].detachInPortPatchID().index()] ==
                vector::zero;
            U.oldTime().boundaryField()[valves()[valveI].detachInPortPatchID().index()] ==
                vector::zero;
        }

        // If valve is present in geometry, set the motion
        if (valves()[valveI].detachInCylinderPatchID().active())
        {
            // Bottom of the valve moves with given velocity
            U.boundaryField()[valves()[valveI].detachInCylinderPatchID().index()] ==
               vector::zero;
            U.oldTime().boundaryField()[valves()[valveI].detachInCylinderPatchID().index()] ==
               vector::zero;
        }
     
    }

}


bool Foam::dieselEngineMesh::isACylinderHeadFace
(
    const labelList& cylHeadFaces, 
    const label face
)
{
    
    if(face >= cylHeadFaces[0] && face <= cylHeadFaces[cylHeadFaces.size()-1])
    {
        return true;
    }

    return false;
}



// ************************************************************************* //
