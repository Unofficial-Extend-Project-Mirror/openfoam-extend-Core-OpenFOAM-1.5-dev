        tetPointVectorField& motionU = mSolver.motionU();

//	Info << motionU << endl;	
        
//	Info << "info 1" << endl;
	
        forAll(valves(), valveI)
        {   
        
//	    Info << "Info 2" << endl;
	        
            const engineValve&  curValve = valves()[valveI];
            
//	    Info << "Info 3" << endl;
	    
            vector valveVel = 
                valves_[valveI].curVelocity()*valves_[valveI].cs().axis();
		
//		Info << "info 4" << endl;
		
            if(valves()[valveI].curtainInCylinderPatchID().active())
            {
            	Info << "index[i] = " << curValve.curtainInPortPatchID().index() << endl;
                motionU.boundaryField()[curValve.curtainInCylinderPatchID().index()] == valveVel;
            }

//		Info << "info 5" << endl;
		
            if(valves()[valveI].curtainInPortPatchID().active())
            {
            	Info << "index = " << curValve.curtainInPortPatchID().index() << endl;
		
//		Info << "motionU boundary field" << endl;
//            	Info << motionU.boundaryField()[curValve.curtainInPortPatchID().index()] << endl;
		
		
                motionU.boundaryField()[curValve.curtainInPortPatchID().index()] == valveVel;
	    	}
		
//		Info << "info 6" << endl;
		
            if(valves()[valveI].poppetPatchID().active())
            {
                motionU.boundaryField()[curValve.poppetPatchID().index()] == valveVel;
            }
		
//		Info << "info 7" << endl;
		
            if(valves()[valveI].bottomPatchID().active())
            {
                motionU.boundaryField()[curValve.bottomPatchID().index()] == valveVel;
            }

//		Info << "info 8" << endl;
		
            if(valves()[valveI].detachInPortPatchID().active())
            {
                motionU.boundaryField()[curValve.detachInPortPatchID().index()] == vector::zero;
                motionU.boundaryField()[curValve.detachInPortPatchID().index()] = vector::zero;
            }
		
//		Info << "info 9" << endl;
		
            if(valves()[valveI].detachInCylinderPatchID().active())
            {
                motionU.boundaryField()[curValve.detachInCylinderPatchID().index()] == vector::zero;
                motionU.boundaryField()[curValve.detachInPortPatchID().index()] = vector::zero;
            }
	 
		
//		Info << "info 10" << endl;
		
        	if(valves_[valveI].stemPatchID().active())
        	{
        
            	label stemIndex = valves_[valveI].stemPatchID().index();
            	label poppetIndex = valves_[valveI].poppetPatchID().index();
            	label detachIndex = valves_[valveI].detachInPortPatchID().index();
            
            	const coordinateSystem& cSysV = valves_[valveI].cs();
            
            	if(valves_[valveI].stemPatchID().active())
        	{
        
				componentMixedTetPolyPatchVectorField& pp =
        			refCast<componentMixedTetPolyPatchVectorField>
            		(
            			motionU.boundaryField()[valves_[valveI].stemPatchID().index()]
            		);

           }
            
                const pointField& stemGlobal = 
                    motionU.boundaryField()[stemIndex].patch().localPoints();
            
                const pointField& poppetGlobal = 
                    motionU.boundaryField()[poppetIndex].patch().localPoints();

                const pointField& detachGlobal = 
                    motionU.boundaryField()[detachIndex].patch().localPoints();

                scalarField stemLocal =  cSysV.toLocal(stemGlobal)().component(vector::Z);               
                scalarField poppetLocal =  cSysV.toLocal(poppetGlobal)().component(vector::Z);               
                scalarField detachLocal =  cSysV.toLocal(detachGlobal)().component(vector::Z);               
                
//                scalar maxDetach = max(stemLocal);
                scalar maxDetach = max(detachLocal);
                scalar maxPoppet = max(poppetLocal);
                
                Info << "maxDetach = " << maxDetach << endl;
                Info << "maxPoppet = " << maxPoppet << endl;
                /*
                pp = (1.0 - pos(stemLocal - detachLocal))*valveVel*(maxDetach -
                stemLocal)/(maxDetach-maxPoppet);
				*/
				
           	 	

                motionU.boundaryField()[stemIndex] == (1.0 - pos(stemLocal - detachLocal))*valveVel*(maxDetach -
                stemLocal)/(maxDetach-maxPoppet);
				
//				Info << motionU.boundaryField()[stemIndex] << endl;
                
            
        }
/*		
		if(valves_[valveI].stemPatchID().active())
        	{
        
				componentMixedTetPolyPatchVectorField& pp =
        			refCast<componentMixedTetPolyPatchVectorField>
            		(
            			motionU.boundaryField()[valves_[valveI].stemPatchID().index()]
            		);

           }
*/
	}
        

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
	
        
        
