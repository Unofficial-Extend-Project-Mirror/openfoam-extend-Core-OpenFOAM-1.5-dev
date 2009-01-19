// use tetrahedral decomposition of the engine mesh

{
    tetPointVectorField& motionU = mSolver.motionU();

    // Set valve velocity
    forAll (valves_, valveI)
    {
        
        Info << "Valve n. " << valveI + 1 << " velocity = " <<  valves_[valveI].curVelocity() << endl;

        vector valveVel =
            valves_[valveI].curVelocity()*valves_[valveI].cs().axis();
        
        Info << "valveVel = " << valveVel << endl;
        
        // If valve is present in geometry, set the motion
        if (valves_[valveI].bottomPatchID().active())
        {
            // Bottom of the valve moves with given velocity
            motionU.boundaryField()[valves_[valveI].bottomPatchID().index()] ==
                valveVel;

            if (debug)
            {
                Info<< "Valve " << valveI << " lift: "
                    << valves_[valveI].curLift()
                    << " velocity: " << valves_[valveI].curVelocity()
                    << endl;
            }
        }
            Info << "bottom" << endl;

        if (valves_[valveI].poppetPatchID().active())
        {
            // Top of the valve does not move
            motionU.boundaryField()[valves_[valveI].poppetPatchID().index()] ==
                valveVel;
        }
        Info << "poppet" << endl;

        if (valves_[valveI].curtainInCylinderPatchID().active())
        {

            // Top of the valve does not move
            motionU.boundaryField()[valves_[valveI].curtainInCylinderPatchID().index()] ==
                valveVel;
                            
        }
        Info << "curtain" << endl;

        if (valves_[valveI].curtainInPortPatchID().active())
        {
            // Top of the valve does not move
            motionU.boundaryField()[valves_[valveI].curtainInPortPatchID().index()] ==
                valveVel;
        }
        Info << "curtain in port" << endl;
        
        if (valves_[valveI].detachInPortPatchID().active())
        {
            motionU.boundaryField()[valves_[valveI].detachInPortPatchID().index()] == vector::zero;
        }
        Info << "detach in port" << endl;

        if (valves_[valveI].detachInCylinderPatchID().active())
        {
            motionU.boundaryField()[valves_[valveI].detachInCylinderPatchID().index()] == vector::zero;
        }
        Info << "detach in cyl" << endl;
		
        if
        (
            valves_[valveI].stemPatchID().active() 
            && 
            valves_[valveI].detachInPortPatchID().active()
            && 
            valves_[valveI].detachInCylinderPatchID().active()
        )
        {
        
            label stemIndex = valves_[valveI].stemPatchID().index();
            label poppetIndex = valves_[valveI].poppetPatchID().index();
            label detachIndex = valves_[valveI].detachInPortPatchID().index();
            
            const coordinateSystem& cSysV = valves_[valveI].cs();
                        
            const pointField& stemGlobal = 
                    motionU.boundaryField()[stemIndex].patch().localPoints();
            
            const pointField& poppetGlobal = 
                    motionU.boundaryField()[poppetIndex].patch().localPoints();

            const pointField& detachGlobal = 
                    motionU.boundaryField()[detachIndex].patch().localPoints();

            scalarField stemLocal =  cSysV.localPosition(stemGlobal)().component(vector::Z);               
            scalarField poppetLocal =  cSysV.localPosition(poppetGlobal)().component(vector::Z);               
            scalarField detachLocal =  cSysV.localPosition(detachGlobal)().component(vector::Z);               
                
            scalar maxDetach = max(detachLocal);
            scalar maxPoppet = max(poppetLocal);
                
            Info << "maxDetach = " << maxDetach << endl;
            Info << "maxPoppet = " << maxPoppet << endl;
				
            motionU.boundaryField()[stemIndex] == (1.0 - pos(stemLocal - maxDetach))*valveVel*(maxDetach -
            stemLocal)/(maxDetach-maxPoppet);
            
        }

        if
        (
            valves_[valveI].stemPatchID().active() 
            && 
            !valves_[valveI].detachInPortPatchID().active()
            && 
            !valves_[valveI].detachInCylinderPatchID().active()
        )
        {
            label stemIndex = valves_[valveI].stemPatchID().index();

            const coordinateSystem& cSysV = valves_[valveI].cs();
                        
            const pointField& stemGlobal = 
                    motionU.boundaryField()[stemIndex].patch().localPoints();
            
            scalarField stemLocal =  cSysV.localPosition(stemGlobal)().component(vector::Z);               
                
            scalar maxStem = max(stemLocal);
            scalar minStem = min(stemLocal);
                
            Info << "maxStem = " << maxStem << endl;
            Info << "minStem = " << minStem << endl;
				
            motionU.boundaryField()[stemIndex] == valveVel*(maxStem -
            stemLocal)/(maxStem-minStem);
        
        }

	}

//  Setting the boundary position

    {
        label cylinderHeadIndex = boundaryMesh().findPatchID("cylinderHead");
        
        // Top of the valve does not move
        motionU.boundaryField()[cylinderHeadIndex] ==
            vector::zero;
    }
    
    motionU.correctBoundaryConditions();
        
    
    
}
