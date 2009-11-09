
    // Add the piston points zone
    if (piston().patchID().active())
    {
        pointSet movingPistonPoints(*this, piston().pistonPointSetName());

        pz.append
        (
            new pointZone
            (
                "movingPistonPoints",
                movingPistonPoints.toc(),
                nPointZones,
                pointZones()
            )
        );
        
        nPointZones++;        
               
    }
