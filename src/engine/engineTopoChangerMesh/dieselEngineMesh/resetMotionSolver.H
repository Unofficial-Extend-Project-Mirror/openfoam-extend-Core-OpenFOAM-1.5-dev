{
    tetPointVectorField& motionU = mSolver.motionU();

    vector pistonVel =
            piston().cs().axis()*engineTime_.pistonSpeed().value();

    // Set valve velocity
    forAll (valves_, valveI)
    {
        // If valve is present in geometry, set the motion
        if (valves_[valveI].bottomPatchID().active())
        {
            vector valveVel =
                valves_[valveI].curVelocity()*valves_[valveI].cs().axis();

            // Bottom of the valve moves with given velocity
            motionU.boundaryField()[valves_[valveI].bottomPatchID().index()] ==
                vector::zero;

        }

        if (valves_[valveI].poppetPatchID().active())
        {
            // Top of the valve does not move
            motionU.boundaryField()[valves_[valveI].poppetPatchID().index()] ==
                vector::zero;
        }

        if (valves_[valveI].curtainInCylinderPatchID().active())
        {

            pp.refValue() = vector::zero;
            pp.valueFraction() = vector::zero;                
                            
        }

        if (valves_[valveI].curtainInPortPatchID().active())
        {
            label cipIndex = valves_[valveI].curtainInPortPatchID().index();
            
            componentMixedTetPolyPatchVectorField& pp =
                refCast<componentMixedTetPolyPatchVectorField>
                (
                    motionU.boundaryField()[cipIndex]
                );
            
            pp.refValue() = vector::zero;
            pp.valueFraction() = vector::zero;                

        }
        
        {
            label linerIndex = boundaryMesh().findPatchID("liner");
            
            componentMixedTetPolyPatchVectorField& pp =
                refCast<componentMixedTetPolyPatchVectorField>
                (
                    motionU.boundaryField()[linerIndex]
                );
            
            pp.refValue() = vector::zero;
            pp.valueFraction() = vector::zero;                

        }
        
    }

    // Set piston velocity
    if (piston().patchID().active())
    {

        if (debug)
        {
            Info<< "Piston velocity: " << pistonVel;
        }

        componentMixedTetPolyPatchVectorField& pp =
            refCast<componentMixedTetPolyPatchVectorField>
            (
                motionU.boundaryField()[piston().patchID().index()]
            );

        pp.refValue() = vector::zero();
        
    }
    
    mSolver.solve();

//  Setting the boundary position

    {
        label cylinderHeadIndex = boundaryMesh().findPatchID("cylinderHead");
        
        // Top of the valve does not move
        motionU.boundaryField()[cylinderHeadIndex] ==
            vector::zero;
    }
        
    
}
