    if (piston().patchID().active())
    {

        // add faces for piston layering
    
        faceSet pistonFaceSet(*this, piston().pistonFaceSetName());
        
        labelList pistonLabels(pistonFaceSet.toc());



        SortableList<label> pistonFacesSort(pistonLabels);
        
//        Info << "1" << endl;
                
//        Info << pistonFacesSort << endl;

//        Info << "2" << endl;
        
        pistonFacesSort.sort();
        
//        Info << pistonFacesSort << endl;
        
//        boolList flipPistonFaces(pistonFaceSet.size(), false);
        boolList flipPistonFaces(pistonFacesSort.size(), false);
//        label nSet = pistonFaceSet.size();
        label nSet = pistonFacesSort.size();
        label nFlip = 0;
                
        forAll (flipPistonFaces, facei)
        {
            scalar scalProd = (faceAreas()[pistonFacesSort[facei]] & vector(0, 0, 1));
            if(scalProd < 0)
            {
                flipPistonFaces[facei] = true;
                nFlip++;
            }  
        }
        
        Info << "nSet = " << nSet << endl;
        Info << "nFlip = " << nFlip << endl;
        
	    fz.append
        (
            new faceZone
            (
                "pistonLayerFaces",
//                pistonFaceSet.toc(),
                pistonFacesSort,
//                boolList(pistonFaceSet.size(), true),
                flipPistonFaces,
                nFaceZones,
                faceZones()
            )
        );
	
        nFaceZones++;
        
        pointSet movingPistonPoints(*this, piston().pistonPointSetName());

        pz.append
        (
            new pointZone
            (
                "pistonPoints",
                movingPistonPoints.toc(),
                nPointZones,
                pointZones()
            )
        );
       
        nPointZones++;

    }
