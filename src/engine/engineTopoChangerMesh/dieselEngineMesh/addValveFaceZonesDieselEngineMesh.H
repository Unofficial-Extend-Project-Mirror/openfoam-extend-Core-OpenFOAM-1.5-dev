{
    for (label valveI = 0; valveI < nValves(); valveI++)
    {
        // If both sides of the interface exist, add sliding interface
        // for a valve
        if
        (
            valves_[valveI].downInCylinderPatchID().active()
         && valves_[valveI].downInPortPatchID().active()
        )
        {
            Info<< "Adding sliding interface zones for down of valve "
                << valveI + 1 << endl;

            pz.append
            (
                new pointZone
                (
                    "cutPointsDownV" + Foam::name(valveI + 1),
                    labelList(0),
                    nPointZones,
                    pointZones()
                )            
            );
                            
            nPointZones++;

            const polyPatch& cylDown =
                boundaryMesh()
                    [valves_[valveI].downInCylinderPatchID().index()];

            labelList cylDownLabels(cylDown.size(), cylDown.start());

            forAll (cylDownLabels, i)
            {
                cylDownLabels[i] += i;
            }

            fz.append
            (
                new faceZone
                (
                    "downCylZoneV" + Foam::name(valveI + 1),
                    cylDownLabels,
                    boolList(cylDownLabels.size(), false),
                    nFaceZones,
                    faceZones()
                )
            );
                
            nFaceZones++;

            const polyPatch& portDown =
                boundaryMesh()
                    [valves_[valveI].downInPortPatchID().index()];

            labelList portDownLabels
            (
                portDown.size(),
                portDown.start()
            );

            forAll (portDownLabels, i)
            {
                portDownLabels[i] += i;
            }

            fz.append
            (                
                new faceZone
                (
                    "downPortZoneV" + Foam::name(valveI + 1),
                    portDownLabels,
                    boolList(portDownLabels.size(), false),
                    nFaceZones,
                    faceZones()
                )
            );
                
            nFaceZones++;

            // Add empty zone for cut faces

            fz.append
            (
                new faceZone
                (
                    "cutFaceZoneDownV" + Foam::name(valveI + 1),
                    labelList(0),
                    boolList(0, false),
                    nFaceZones,
                    faceZones()
                )    
            );

            nFaceZones++;

        }
        else
        {
            Info << "No valve up for valve " << valveI + 1 << endl;
        }


        // If both sides of the interface exist, add sliding interface
        // for a valve
        if
        (
            valves_[valveI].upInCylinderPatchID().active()
         && valves_[valveI].upInPortPatchID().active()
        )
        {
            Info<< "Adding sliding interface zones for up of valve "
                << valveI + 1 << endl;

            pz.append
            (
                new pointZone
                (
                    "cutPointsUpV" + Foam::name(valveI + 1),
                    labelList(0),
                    nPointZones,
                    pointZones()
                )            
            );
                            
            nPointZones++;

            const polyPatch& cylUp =
                boundaryMesh()
                    [valves_[valveI].upInCylinderPatchID().index()];

            labelList cylUpLabels(cylUp.size(), cylUp.start());

            forAll (cylUpLabels, i)
            {
                cylUpLabels[i] += i;
            }

            fz.append
            (
                new faceZone
                (
                    "upCylZoneV" + Foam::name(valveI + 1),
                    cylUpLabels,
                    boolList(cylUpLabels.size(), false),
                    nFaceZones,
                    faceZones()
                )
            );
                
            nFaceZones++;

            const polyPatch& portUp =
                boundaryMesh()
                    [valves_[valveI].upInPortPatchID().index()];

            labelList portUpLabels
            (
                portUp.size(),
                portUp.start()
            );

            forAll (portUpLabels, i)
            {
                portUpLabels[i] += i;
            }

            fz.append
            (                
                new faceZone
                (
                    "upPortZoneV" + Foam::name(valveI + 1),
                    portUpLabels,
                    boolList(portUpLabels.size(), false),
                    nFaceZones,
                    faceZones()
                )
            );
                
            nFaceZones++;

            // Add empty zone for cut faces

            fz.append
            (
                new faceZone
                (
                    "cutFaceZoneUpV" + Foam::name(valveI + 1),
                    labelList(0),
                    boolList(0, false),
                    nFaceZones,
                    faceZones()
                )    
            );

            nFaceZones++;

        }
        else
        {
            Info << "No valve up for valve " << valveI + 1 << endl;
        }
    }

}
