// The FOAM Project // File: dieselEngineMeshPiston.C
/*
-------------------------------------------------------------------------------
 =========         | Class Implementation
 \\      /         |
  \\    /          | Name:   dieselEngineMeshPiston
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
#include "attachDetach.H"
#include "surfaceFields.H"
#include "regionSplit.H"
#include "componentMixedTetPolyPatchVectorField.H"
#include "mapPolyMesh.H"

#include "tetPolyMesh.H"
#include "tetPointFields.H"
#include "elementFields.H"
#include "fixedValueTetPolyPatchFields.H"
#include "slipTetPolyPatchFields.H"

#include "tetFem.H"

#include "symmetryFvPatch.H"
#include "wedgeFvPatch.H"
#include "emptyFvPatch.H"
#include "zeroGradientTetPolyPatchFields.H"
#include "tetDecompositionMotionSolver.H"

#include "fixedValueTetPolyPatchFields.H"
#include "mixedTetPolyPatchFields.H"
#include "slipTetPolyPatchFields.H"
#include "zeroGradientTetPolyPatchFields.H"



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
void Foam::dieselEngineMesh::makeLayersLive()
{ 
    // Enable layering
    forAll (topoChanger_, modI)
    {
        if (isA<layerAdditionRemoval>(topoChanger_[modI]))
        {
            topoChanger_[modI].enable();
        }
        else if (isA<slidingInterface>(topoChanger_[modI]))
        {
            topoChanger_[modI].disable();
        }
        else if (isA<attachDetach>(topoChanger_[modI]))
        {
            topoChanger_[modI].enable();
        }
        else
        {
            FatalErrorIn("void Foam::engineTopoFvMesh::makeLayersLive()")
                << "Don't know what to do with mesh modifier "
                << modI << " of type " << topoChanger_[modI].type()
                << abort(FatalError);
        }
    }
}

void Foam::dieselEngineMesh::makeSlidersLive()
{
 
    // Enable sliding interface
    forAll (topoChanger_, modI)
    {
        if (isA<layerAdditionRemoval>(topoChanger_[modI]))
        {
            topoChanger_[modI].disable();
        }
        else if (isA<slidingInterface>(topoChanger_[modI]))
        {
            topoChanger_[modI].enable();
        }
        else if (isA<attachDetach>(topoChanger_[modI]))
        {
            topoChanger_[modI].enable();
        }
        else
        {
            FatalErrorIn("void Foam::dieselEngineMesh::makeLayersLive()")
                << "Don't know what to do with mesh modifier "
                << modI << " of type " << topoChanger_[modI].type()
                << abort(FatalError);
        }
    }

}

bool Foam::dieselEngineMesh::attached() const
{
    const polyTopoChanger& morphs = topoChanger_;

    bool result = false;

    forAll (morphs, modI)
    {
        if (typeid(morphs[modI]) == typeid(slidingInterface))
        {
            result =
                result
             || refCast<const slidingInterface>(morphs[modI]).attached();
        }
    }

    // Check thal all sliders are in sync (debug only)
    forAll (morphs, modI)
    {
        if (typeid(morphs[modI]) == typeid(slidingInterface))
        {
            if
            (
                result 
             != refCast<const slidingInterface>(morphs[modI]).attached()
            )
            {
                FatalErrorIn("bool dieselEngineMesh::attached() const")
                    << "Slider " << modI << " named " << morphs[modI].name()
                    << " out of sync: Should be" << result
                    << abort(FatalError);
            }
        }
    }

    return result;
}

void Foam::dieselEngineMesh::valveDetach()
{
    // Enable sliding interface
    forAll (topoChanger_, modI)
    {
        if (isA<attachDetach>(topoChanger_[modI]))
        {
            const attachDetach& ad =
                refCast<const attachDetach>(topoChanger_[modI]);

            const word masterName = ad.masterPatchID().name();

            // Find the valve with that name
            label valveIndex = -1;

            forAll (valves_, valveI)
            {
                if
                (
                    valves_[valveI].detachInCylinderPatchID().name()
                 == masterName
                )
                {
                    valveIndex = valveI;
                    break;
                }
            }

            if (valveIndex < 0)
            {
                FatalErrorIn
                (
                    "void Foam::engineTopoFvMesh::prepareValveDetach()"
                )   << "Cannot match patch for attach/detach " << modI
                    << abort(FatalError);
            }

            if (debug)
            {
                Info<< " valveI: " << valveIndex << " attached: "
                    << ad.attached()
                    << " valve open: " << valves_[valveIndex].isOpen()
                    << endl;
            }

            ad.setDetach();
            
        }
    }
}

void Foam::dieselEngineMesh::valveAttach()
{
    // Enable sliding interface
    forAll (topoChanger_, modI)
    {
        if (isA<attachDetach>(topoChanger_[modI]))
        {
            const attachDetach& ad =
                refCast<const attachDetach>(topoChanger_[modI]);

            const word masterName = ad.masterPatchID().name();

            // Find the valve with that name
            label valveIndex = -1;

            forAll (valves_, valveI)
            {
                if
                (
                    valves_[valveI].detachInCylinderPatchID().name()
                 == masterName
                )
                {
                    valveIndex = valveI;
                    break;
                }
            }

            if (valveIndex < 0)
            {
                FatalErrorIn
                (
                    "void Foam::engineTopoFvMesh::prepareValveDetach()"
                )   << "Cannot match patch for attach/detach " << modI
                    << abort(FatalError);
            }

            if (debug)
            {
                Info<< " valveI: " << valveIndex << " attached: "
                    << ad.attached()
                    << " valve open: " << valves_[valveIndex].isOpen()
                    << endl;
            }

            ad.setAttach();
            
        }
    }
 
}

void Foam::dieselEngineMesh::prepareValveDetach()
{
    // Enable sliding interface
    forAll (topoChanger_, modI)
    {
        if (isA<attachDetach>(topoChanger_[modI]))
        {
            const attachDetach& ad =
                refCast<const attachDetach>(topoChanger_[modI]);

            const word masterName = ad.masterPatchID().name();

            // Find the valve with that name
            label valveIndex = -1;

            forAll (valves_, valveI)
            {
                if
                (
                    valves_[valveI].detachInCylinderPatchID().name()
                 == masterName
                )
                {
                    valveIndex = valveI;
                    break;
                }
            }

            if (valveIndex < 0)
            {
                FatalErrorIn
                (
                    "void Foam::engineTopoFvMesh::prepareValveDetach()"
                )   << "Cannot match patch for attach/detach " << modI
                    << abort(FatalError);
            }

            if (debug)
            {
                Info<< " valveI: " << valveIndex << " attached: "
                    << ad.attached()
                    << " valve open: " << valves_[valveIndex].isOpen()
                    << endl;
            }

            if (valves_[valveIndex].isOpen())
            {
                ad.setAttach();
            }
            else
            {
                ad.setDetach();
            }
        }
    }
}


bool Foam::dieselEngineMesh::update()
{

    Info << "bool Foam::dieselEngineMesh::update()" << endl;
    
    tetDecompositionMotionSolver& mSolver =
        refCast<tetDecompositionMotionSolver>(msPtr_());


    // Detaching the interface
    if (attached())
    {
    
        Info << "Decoupling sliding interfaces" << endl;
        makeSlidersLive();
//        valveDetach();
        autoPtr<mapPolyMesh> topoChangeMap1 = topoChanger_.changeMesh();
        
        Info << "sliding interfaces successfully decoupled!!!" << endl;
        
        bool meshChanged1 = topoChangeMap1.valid();

        if (meshChanged1)
        {
            mSolver.updateMesh(topoChangeMap1());
        }
        
    }
    else
    {
        Info << "Sliding interfaces decoupled" << endl;
//        valveDetach();
    }
    
    makeLayersLive();
    

    // Find piston mesh modifier
    const label pistonLayerID =
        topoChanger_.findModifierID("pistonLayer");    

    scalar deltaZ = engTime().pistonDisplacement().value();
    Info<< "deltaZ = " << deltaZ << " Piston at:" << pistonPosition()
        << endl;

    scalar oldVirtualPistonPosition = virtualPistonPosition();

    pistonPosition_ = max(boundaryMesh()[boundaryMesh().findPatchID(piston().patchID().name())].localPoints()).z();    
    
    Info << "pistonLayerID: " << pistonLayerID << endl;
    
    const layerAdditionRemoval& pistonLayers =
        dynamicCast<const layerAdditionRemoval>
        (
            topoChanger_[pistonLayerID]
        );

    bool realDeformation = deformation();

    if (virtualPistonPosition()+deltaZ > deckHeight()-engTime().clearance().value()-SMALL)
    {
        realDeformation = true;
    }
    
    
    if (realDeformation)
    {
        // Disable layer addition
        Info << "**Mesh deformation mode" << endl;
        topoChanger_[pistonLayerID].disable();
    }
    else
    {
        // Enable layer addition
        Info << "**Piston layering mode" << endl;
        topoChanger_[pistonLayerID].enable();
    }
    

    scalar minLayerThickness = pistonLayers.minLayerThickness();
    
    autoPtr<mapPolyMesh> topoChangeMap = topoChanger_.changeMesh();

    pointField newPoints = points();

    bool meshChanged = topoChangeMap.valid();
    
    if (meshChanged)
    {
        mSolver.updateMesh(topoChangeMap());
        
        if (topoChangeMap().hasMotionPoints())
        {
            movePoints(topoChangeMap().preMotionPoints());
            newPoints = topoChangeMap().preMotionPoints();
        }
        setV0();
        resetMotion();
    }
    
    
    if(!deformation())
    {
        tetPointVectorField& motionU = mSolver.motionU();

#       include "movePistonPointsLayeringDieselEngineMesh.H"
        Info << "movePoints" << endl;
	    movePoints(newPoints);
        Info << "setBoundaryMotion" << endl;
#       include "setValveMotionBoundaryConditionDieselEngineMesh.H"

        // Set piston velocity
        if (piston().patchID().active())
        {

            componentMixedTetPolyPatchVectorField& pp =
                refCast<componentMixedTetPolyPatchVectorField>
                (
                    motionU.boundaryField()[piston().patchID().index()]
                );

            pp.refValue() = vector::zero;
        
        }
    
        motionU.correctBoundaryConditions();

        Info << "mSolver" << endl;
//        mSolver.solve();       

        DynamicList<label> pistonBowlPoints(mSolver.curPoints()().size()/100);

#       include "setDieselEngineMeshConstraintsNoDeformation.H"        
        
        mSolver.solve(labelList(pistonBowlPoints.shrink()),pointsVelocity);       

        Info << "newPoints" << endl;	
        newPoints = mSolver.curPoints();
        Info << "movePoints 2" << endl;	
        movePoints(newPoints);
        setVirtualPositions();
    }
    else
    {
        tetPointVectorField& motionU = mSolver.motionU();
#       include "setValveMotionBoundaryConditionDieselEngineMesh.H"
#       include "setPistonMotionBoundaryConditionDieselEngineMesh.H"

        DynamicList<label> pistonBowlPoints(mSolver.curPoints()().size()/100);
        
#       include "setDieselEngineMeshConstraints.H"        
        
        mSolver.solve(labelList(pistonBowlPoints.shrink()),pointsVelocity);       

        newPoints = mSolver.curPoints();
        movePoints(newPoints);
        setVirtualPositions();
    }



    {
    
        pointField oldPointsNew = oldAllPoints();
    

        // Attach the interface
        Info << "Coupling sliding interfaces" << endl;
        makeSlidersLive();
        Info << "Coupled" << endl;

        // Changing topology by hand
        autoPtr<mapPolyMesh> topoChangeMap3 = topoChanger_.changeMesh();
        Info << "changeMesh" << endl;
        bool meshChanged3 = topoChangeMap3.valid();
        Info << "mesh3" << endl;
        
        Info << "Sliding interfaces coupled: " << attached() << endl;

        if (meshChanged3)
        {
            mSolver.updateMesh(topoChangeMap3());

            if (topoChangeMap3->hasMotionPoints())
            {
//                movePoints(topoChangeMap3->preMotionPoints());
//                resetMotion();
//                setV0();
            }
                    
            
//            if(correctPointsMotion_)
            {
            
                // correct the motion after attaching the sliding interface
            
                pointField mappedOldPointsNew(allPoints().size());

                mappedOldPointsNew.map(oldPointsNew, topoChangeMap3->pointMap());
            
                pointField newPoints = allPoints();

                movePoints(mappedOldPointsNew);
            
                resetMotion();
                setV0();
                movePoints(newPoints);
            }
            
        }
    
    }
        
    return true;
    
}
