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

        if (valves_[valveI].downInCylinderPatchID().active())
        {

            // Top of the valve does not move
            motionU.boundaryField()[valves_[valveI].downInCylinderPatchID().index()] ==
                valveVel;
                            
        }
        Info << "down in cylinder" << endl;

        if (valves_[valveI].downInPortPatchID().active())
        {
            // Top of the valve does not move
            motionU.boundaryField()[valves_[valveI].downInPortPatchID().index()] ==
                valveVel;
        }
        Info << "down in port" << endl;

        if (valves_[valveI].upInCylinderPatchID().active())
        {

            // Top of the valve does not move
            motionU.boundaryField()[valves_[valveI].upInCylinderPatchID().index()] ==
                vector::zero;
                            
        }
        Info << "curtain" << endl;

        if (valves_[valveI].upInPortPatchID().active())
        {
            // Top of the valve does not move
            motionU.boundaryField()[valves_[valveI].upInPortPatchID().index()] ==
                vector::zero;
        }
        Info << "curtain in port" << endl;

        if (valves_[valveI].sidePatchID().active())
        {
            // Top of the valve does not move
            motionU.boundaryField()[valves_[valveI].sidePatchID().index()] ==
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
