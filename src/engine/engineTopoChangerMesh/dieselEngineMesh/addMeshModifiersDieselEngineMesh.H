//    List<polyMeshModifier*> tm(3*nValves() + 2);
//    DynamicList<polyMeshModifier*> tm;

    label nMods = 0;

    for (label valveI = 0; valveI < nValves(); valveI++)
    {
        
        
        // Add attach-detach for valve
        if
        (
            valves_[valveI].detachInCylinderPatchID().active()
         && valves_[valveI].detachInPortPatchID().active()
        )
        {

            Info << "Adding a attach/detach boundary mesh modifier to the valve " <<
            valveI + 1 << endl;
        
            topoChanger_.setSize(nMods+1);
//            tm.append
            topoChanger_.hook
            (
                new attachDetach
                (
                    "valveAttachDetach" + Foam::name(valveI + 1),
                    nMods,
                    topoChanger_,
                    "detachFaceZoneV" + Foam::name(valveI + 1),
                    valves_[valveI].detachInCylinderPatchID().name(),
                    valves_[valveI].detachInPortPatchID().name(),
                    scalarField(0),
                    true                // Manual triggering
                )    
            );
            nMods++;
            Info << "Attach detach" << endl;
        }
        
        if
        (
            valves_[valveI].downInCylinderPatchID().active()
         && valves_[valveI].downInPortPatchID().active()
        )
        {
            
            Info << "Adding a sliding interface mesh modifier to the valve " <<
            valveI + 1 << endl;
            
            topoChanger_.setSize(nMods+1);
            
            topoChanger_.hook
            (
                new slidingInterface
                (
                    "valveSliderDown" + Foam::name(valveI + 1),
                    nMods,
                    topoChanger_,
                    "downPortZoneV" + Foam::name(valveI + 1),
                    "downCylZoneV" + Foam::name(valveI + 1),
                    "cutPointsDownV" + Foam::name(valveI + 1),
                    "cutFaceZoneDownV" + Foam::name(valveI + 1),
                    valves_[valveI].downInPortPatchID().name(),
                    valves_[valveI].downInCylinderPatchID().name(),
                    slidingInterface::INTEGRAL, // always integral
                    true,  // attach-detach action
                    intersection::VISIBLE
                )    
            );
            nMods++;
            
            Info << "valveSliderDown" + Foam::name(valveI + 1) << endl;
            Info << nMods << endl;            
        }


        if
        (
            valves_[valveI].upInCylinderPatchID().active()
         && valves_[valveI].upInPortPatchID().active()
        )
        {
            
            Info << "Adding a sliding interface mesh modifier to the valve " <<
            valveI + 1 << endl;
            
            topoChanger_.setSize(nMods+1);
            
            topoChanger_.hook
            (
                new slidingInterface
                (
                    "valveSliderUp" + Foam::name(valveI + 1),
                    nMods,
                    topoChanger_,
                    "upPortZoneV" + Foam::name(valveI + 1),
                    "upCylZoneV" + Foam::name(valveI + 1),
                    "cutPointsUpV" + Foam::name(valveI + 1),
                    "cutFaceZoneUpV" + Foam::name(valveI + 1),
                    valves_[valveI].upInPortPatchID().name(),
                    valves_[valveI].upInCylinderPatchID().name(),
                    slidingInterface::INTEGRAL, // always integral
                    true,  // attach-detach action
                    intersection::VISIBLE
                )    
            );
            nMods++;
            
            Info << "valveSliderUp" + Foam::name(valveI + 1) << endl;
            Info << nMods << endl;            
        }
                
            
        
    }

    // Add piston layer addition
    if (piston().patchID().active())
    {

        Info << "Adding a layer addition/removal mesh modifier to the piston" << endl;
    
            topoChanger_.setSize(nMods+1);
//        tm.append
        topoChanger_.hook
        (
            new layerAdditionRemoval
            (
                "pistonLayer",
                nMods,
                topoChanger_,
                "pistonLayerFaces",
                piston().minLayer(),
                piston().maxLayer()
            )    
        );
        nMods++;
        Info << "pistonLayer" << endl;
        Info << nMods << endl;            
    }


    Info << "Adding " << nMods << " topology modifiers" << endl;

