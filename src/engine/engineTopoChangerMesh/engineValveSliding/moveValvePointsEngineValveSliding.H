    // valves
    forAll(valves_,valveI)
    {
        vector valveVel =
            valves_[valveI].curVelocity()*valves_[valveI].cs().axis();

//         scalar valveDeltaX = valves_[valveI].curVelocity()*
//             valves_[valveI].cs().axis().x()*engTime().deltaT().value() ;
//         scalar valveDeltaY = valves_[valveI].curVelocity()*
//             valves_[valveI].cs().axis().y()*engTime().deltaT().value() ;
        scalar valveDeltaZ = valves_[valveI].curVelocity()*
            valves_[valveI].cs().axis().z()*engTime().deltaT().value() ;

        if(valves_[valveI].curLift() >= valves_[valveI].deformationLift())
        {
            bool isMoving(false);

            if(mag(valves_[valveI].curVelocity()) > 0)
            {
                Info<< "Valve n. " << valveI << " is moving with velocity = "
                    << valves_[valveI].curVelocity() << endl;
                isMoving = true;
            }

            Info<< "Valve displacement for valve " << valveI << " = "
                << valveDeltaZ << endl;

            if
            (
                valves_[valveI].poppetPatchID().active()
             && valves_[valveI].isOpen()
            )
            {
                List<bool> valveTopPoint(newPoints.size(), false);

                label layeringVPtsIndex =
                    pointZones().findZoneID
                    (
                        "movingPointsTopZoneV" + Foam::name(valveI+1)
                    );

                const labelList& layeringVPoints =
                    pointZones()[layeringVPtsIndex];

                forAll(layeringVPoints, i)
                {
                    point& p = newPoints[layeringVPoints[i]];

                    p += valveVel * engTime().deltaT().value();

/*


                    p.x() += valveDeltaX*
                         (valves_[valveI].cs().axis().x()/
                         mag(valves_[valveI].cs().axis()));
                    p.y() += valveDeltaY*
                         (valves_[valveI].cs().axis().y()/
                         mag(valves_[valveI].cs().axis()));
                    p.z() += valveDeltaZ*
                         (valves_[valveI].cs().axis().z()/
                         mag(valves_[valveI].cs().axis()));
*/
                    valveTopPoint[layeringVPoints[i]] = true;
                }

                label movingTopPtsVIndex =
                    pointZones().findZoneID
                    (
                        "movingPointsV" + Foam::name(valveI+1)
                    );

                const labelList& movingTopPointsV =
                    pointZones()[movingTopPtsVIndex];

                forAll(movingTopPointsV, i)
                {
                    if(!valveTopPoint[movingTopPointsV[i]])
                    {

                        point& p = newPoints[movingTopPointsV[i]];
                        p += valveVel * engTime().deltaT().value();

/*
                        p.x() += valveDeltaX*
                            (valves_[valveI].cs().axis().x()/
                            mag(valves_[valveI].cs().axis()));

                        p.y() += valveDeltaY*
                            (valves_[valveI].cs().axis().y()/
                            mag(valves_[valveI].cs().axis()));

                        p.z() += valveDeltaZ*
                            (valves_[valveI].cs().axis().z()/
                            mag(valves_[valveI].cs().axis()));
*/
                    }
                }
            }
            else
            {
                Info << "Valve " << valveI << " is CLOSED!!!" << endl;
            }

/*
	        {
                label movingPtsIndex =
                    pointZones().findZoneID
                    (
                        "movingPointsV"+Foam::name(valveI + 1)
                    );

                const labelList& movingPointsV = pointZones()[movingPtsIndex];

                forAll(movingPointsV, mpI)
                {
                    constrainedPoints.append(movingPointsV[mpI]);
                    constrainedVelocity.append(valveVel);

                    constraintSize++;
                }

            }
*/
        }
    }
