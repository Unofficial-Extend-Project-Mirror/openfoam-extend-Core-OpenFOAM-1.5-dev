    // valves

    forAll(valves_,valveI)
    {
        
        bool isMoving(false);
        
        if(mag(valves_[valveI].curVelocity()) > 0)
        {
            Info << "Valve n. " << valveI << " is moving" << endl;
            isMoving = true;
        }
         
        scalar valveDeltaZ = valves_[valveI].curVelocity()*engTime().deltaT().value() ;
        
        Info << "Valve displacement for valve " << valveI << " = " << valveDeltaZ << endl;

        if(valves_[valveI].poppetPatchID().active() && valves_[valveI].isOpen())
        {
            if(valves_[valveI].curLift() >  valves_[valveI].deformationLift()) 
            {
                const scalarField& movingPointsTop = movingPointsMaskTop(valveI);

                List<bool> valveTopPoint(newPoints.size(), false);

                label valveTopPtsIndex = pointZones().findZoneID("valveTopPointsV"+ Foam::name(valveI+1));
                const labelList& valveTopPoints = pointZones()[valveTopPtsIndex];

                forAll(valveTopPoints, i)
                {
                    label pointI = valveTopPoints[i];
                    valveTopPoint[pointI] = true;
                }
        
                forAll(valveTopPoints, i)
                {
                    point& p = newPoints[valveTopPoints[i]];
                    p.z() +=    valveDeltaZ*
                                (valves_[valveI].cs().axis().z()/mag(valves_[valveI].cs().axis()))*
                                movingPointsTop[valveTopPoints[i]];
                }

                deleteDemandDrivenData(movingPointsMaskTopPtr_);
            }
            else
            {
                    poppetDeformation = true;
            }
            
            

        }
        else
        {
            Info << "Valve " << valveI << " is CLOSED!!!" << endl;
        }
        
        if(valves_[valveI].bottomPatchID().active() && valves_[valveI].isOpen())
        {
            const scalarField& movingPointsBottom = movingPointsMaskBottom(valveI);
            {

                List<bool> valveBottomPoint(newPoints.size(), false);

                label valveBottomPtsIndex = pointZones().findZoneID("valveBottomPointsV"+ Foam::name(valveI+1));
                const labelList& valveBottomPoints = pointZones()[valveBottomPtsIndex];

                forAll(valveBottomPoints, i)
                {
                    label pointI = valveBottomPoints[i];
                    valveBottomPoint[pointI] = true;
                }
                                        
                forAll(valveBottomPoints, i)
                {
                    point& p = newPoints[valveBottomPoints[i]];
                    p.z() +=    valveDeltaZ*
                                (valves_[valveI].cs().axis().z()/mag(valves_[valveI].cs().axis()))*
                                movingPointsBottom[valveBottomPoints[i]];
                }

            }
            
            deleteDemandDrivenData(movingPointsMaskBottomPtr_);

        }
        else
        {
            Info << "Valve " << valveI << " is CLOSED!!!" << endl;
        }
    }


