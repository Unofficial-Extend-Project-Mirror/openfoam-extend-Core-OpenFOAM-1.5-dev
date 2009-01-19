/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Description

\*---------------------------------------------------------------------------*/


#include "doublePistonTwoStroke.H"
#include "slidingInterface.H"
#include "layerAdditionRemoval.H"
#include "surfaceFields.H"
#include "regionSplit.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::doublePistonTwoStroke::addZonesAndModifiers()
{
    // Add the zones and mesh modifiers to operate piston motion

    if
    (
        pointZones().size() > 0
     || faceZones().size() > 0
     || cellZones().size() > 0
     || topoChanger_.size() > 0
    ) 
    {
        Info<< "Time = " << engTime().theta() << endl;
        Info<< "void Foam::doublePistonTwoStroke::addZonesAndModifiers() : "
            << "Zones and modifiers already present.  Skipping."
            << endl;
        
        setVirtualPistonPosition();
        setVirtualPiston2Position();
        checkAndCalculate();
        
        return;


    }


    if
    (
        engTime().engineDict().found("zOffsetGambit")
     && engTime().engineDict().found("zDisplGambit")
    )
    {
        Info << "Assembling the cylinder mesh" << endl;
        
        scalar zOffset
        (
            readScalar(engTime().engineDict().lookup("zOffsetGambit"))
        );

        scalar zDispl
        (
            readScalar(engTime().engineDict().lookup("zDisplGambit"))
        );
        
        pointField pDispl = points();

        forAll(points(), pointI)
        {
            const point p = points()[pointI];

            if(p.z() >= zOffset)
            {
                pDispl[pointI].z() -= zDispl;
            }
        }

        movePoints(pDispl);
        write();
        resetMotion();

        Info << "Cylinder mesh assembled" << endl;
    }


    Info << "checkAndCalculate()" << endl;
    checkAndCalculate();
    

    Info<< "Time = " << engTime().theta() << endl
        << "Adding zones to the engine mesh" << endl;


    //fz = 8: virtual piston, outSidePort, insidePort, cutFaceZone,virtual piston2, outSidePortOut, insidePortOut, cutFaceZone2
    //pz = 3: piston points, headpoints, cutPointZone, piston2 points, cutPointZone2
    //cz = 1: moving mask

    List<pointZone*> pz(5);
    List<faceZone*> fz(8);
    List<cellZone*> cz(1);

    label nPointZones = 0;
    label nFaceZones = 0;
    label nCellZones = 0;

    // Add the piston layer addition removal zone
    if (piston().patchID().active())
    {

        // Piston position

        Info << "Adding face zone for piston layer addition/removal" << endl;

        label pistonPatchID = piston().patchID().index();

        scalar zPist = max(boundary()[pistonPatchID].patch().localPoints()).z();

        Info << "zpist: " << zPist << endl;

        scalar zPistV = zPist + offSet();

        Info << " zPistV: " << zPistV << endl;

        labelList zone1(faceCentres().size());
        boolList flipZone1(faceCentres().size(), false);
        label nZoneFaces1 = 0;

        bool foundAtLeastOne = false;
        scalar zHigher = GREAT;
        scalar zLower = GREAT;
        scalar dh = GREAT;
        scalar dl = GREAT;

        Info << "Start looping over faces" << endl;
        forAll (faceCentres(), faceI)
        {

// The points have to be in the cylinder and not in the ports....

            scalar zc = faceCentres()[faceI].z();
            scalar xc = faceCentres()[faceI].x();
            scalar yc = faceCentres()[faceI].y();
            
            vector n = faceAreas()[faceI]/mag(faceAreas()[faceI]);
            scalar dd = n & vector(0,0,1);
//            Info << "face: " << faceI <<  " dd: " << dd << endl;
            if(sqrt(sqr(xc)+sqr(yc)) <  0.5 * engTime().bore().value())
            {
            
                if (dd > 0.1)
                {
                    if (zPistV - zc > 0 && zPistV - zc < dl)
                    {
                        zLower = zc;
                        dl = zPistV - zc;
                    }
            
                    if (zc - zPistV > 0 && zc - zPistV < dh)
                    {
                        zHigher = zc;
                        dh = zc - zPistV;
                    }
            
                    if
                    (
                        zc > zPistV - delta()
                        && zc < zPistV + delta()
                    )
                    {
                        foundAtLeastOne = true;
                        if ((faceAreas()[faceI] & vector(0,0,1)) < 0)
                        {
                            flipZone1[nZoneFaces1] = true;
                        }
                
                        zone1[nZoneFaces1] = faceI;
                        nZoneFaces1++;
                    }
                }
            }
            
            
        }

        // if no cut was found use the layer above
        if (!foundAtLeastOne)
        {
            Info << "no cut was found use the layer above" << endl;
            zPistV = zHigher;

            forAll (faceCentres(), faceI)
            {
                scalar zc = faceCentres()[faceI].z();
                
                scalar xc = faceCentres()[faceI].x();
                scalar yc = faceCentres()[faceI].y();

                vector n = faceAreas()[faceI]/mag(faceAreas()[faceI]);
                scalar dd = n & vector(0,0,1);
                
                if(sqrt(sqr(xc)+sqr(yc)) <  0.5 * engTime().bore().value())
                {
            
                    if (dd > 0.1)
                    {

                        if
                        (
                            zc > zPistV - delta()
                            && zc < zPistV + delta()
                        )
                        {
                            if ((faceAreas()[faceI] & vector(0,0,1)) < 0)
                            {
                                flipZone1[nZoneFaces1] = true;
                            }
                    
                            zone1[nZoneFaces1] = faceI;
                            nZoneFaces1++;
                        }
                    }
                }

            }
        
        }

        zone1.setSize(nZoneFaces1);
        flipZone1.setSize(nZoneFaces1);
    
        fz[nFaceZones]=
            new faceZone
            (
                "pistonLayerFaces",
                zone1,
                flipZone1,
                nFaceZones,
                faceZones()
            );
        
        nFaceZones++;

        Info << "Points below the piston which move with the piston displacement" << endl;
        // Points below the piston which moves with the piston displacement
        DynamicList<label> pistonPoints(nPoints() / 10);
    
/*        // Points which don't move (= cylinder head)
        DynamicList<label> headPoints(nPoints() / 10);

        label nHeadPoints = 0;
*/
        forAll (points(), pointI)
        {
            scalar zCoord = points()[pointI].z();

            if (zCoord < zPistV + delta())
            {
                pistonPoints.append(pointI);
            }
        }

        pz[nPointZones] =
            new pointZone
            (
                "pistonPoints",
                pistonPoints.shrink(),
                nPointZones,
                pointZones()
            );

        nPointZones++;

    }

    // Add the piston2 layer addition removal zone
    label piston2Index = -1;
    forAll(boundary(), i)
    {
        if (boundary()[i].name() == "piston2")
        {
            piston2Index = i;
        }
    }
    if (piston2Index != -1)
    {

        // Piston position

        Info << "Adding face zone for piston2 layer addition/removal" << endl;

//        label pistonPatchID = piston().patchID().index();

        scalar zPist2 = min(boundary()[piston2Index].patch().localPoints()).z();

        Info << "zpist2: " << zPist2 << endl;

        scalar zPist2V = zPist2 - offSet();

        Info << " zPist2V: " << zPist2V << endl;

        labelList zone1(faceCentres().size());
        boolList flipZone1(faceCentres().size(), false);
        label nZoneFaces1 = 0;

        bool foundAtLeastOne = false;
        scalar zHigher = GREAT;
        scalar zLower = GREAT;
        scalar dh = GREAT;
        scalar dl = GREAT;

        Info << "Start looping over faces" << endl;
        forAll (faceCentres(), faceI)
        {

// The points have to be in the cylinder and not in the ports....

            scalar zc = faceCentres()[faceI].z();
            scalar xc = faceCentres()[faceI].x();
            scalar yc = faceCentres()[faceI].y();
            
            vector n = faceAreas()[faceI]/mag(faceAreas()[faceI]);
            scalar dd = n & vector(0,0,1);

            if(sqrt(sqr(xc)+sqr(yc)) <  0.5 * engTime().bore().value())
            {
            
                if (mag(dd) > 0.1)
                {
                    if (zPist2V - zc > 0 && zPist2V - zc < dl)     //boven Virtual piston2 position
                    {
                        zLower = zc;
                        dl = zPist2V - zc;
                    }
            
                    if (zc - zPist2V > 0 && zc - zPist2V < dh)      //Onder Virtual piston2 position
                    {
                        zHigher = zc;
                        dh = zc - zPist2V;                          //aangepast aan zPist2V ipv zHigher??
                    }
            
                    if
                    (
                        zc > zPist2V - delta()
                        && zc < zPist2V + delta()
                    )
                    {
                        foundAtLeastOne = true;
                        if ((faceAreas()[faceI] & vector(0,0,1)) > 0)              //aangepast!!!!
                        {
                            flipZone1[nZoneFaces1] = true;
                        }
                
                        zone1[nZoneFaces1] = faceI;
                        nZoneFaces1++;
                    }
                }
            }
            
            
        }

        // if no cut was found use the layer above
        if (!foundAtLeastOne)
        {
            Info << "no cut was found use the layer below" << endl;
            zPist2V = zLower;

            forAll (faceCentres(), faceI)
            {
                scalar zc = faceCentres()[faceI].z();
                
                scalar xc = faceCentres()[faceI].x();
                scalar yc = faceCentres()[faceI].y();

                vector n = faceAreas()[faceI]/mag(faceAreas()[faceI]);
                scalar dd = n & vector(0,0,1);
                
                if(sqrt(sqr(xc)+sqr(yc)) <  0.5 * engTime().bore().value())
                {
            
                    if (mag(dd) > 0.1)
                    {

                        if
                        (
                            zc > zPist2V - delta()
                            && zc < zPist2V + delta()
                        )
                        {
                            if ((faceAreas()[faceI] & vector(0,0,1)) > 0)
                            {
                                flipZone1[nZoneFaces1] = true;
                            }
                    
                            zone1[nZoneFaces1] = faceI;
                            nZoneFaces1++;
                        }
                    }
                }

            }
        
        }

        zone1.setSize(nZoneFaces1);
        flipZone1.setSize(nZoneFaces1);
    
        fz[nFaceZones]=
            new faceZone
            (
                "piston2LayerFaces",
                zone1,
                flipZone1,
                nFaceZones,
                faceZones()
            );
        
        nFaceZones++;

        Info << "Points above piston2 which move with the piston displacement" << endl;
        // Points below the piston which moves with the piston displacement
        DynamicList<label> piston2Points(nPoints() / 10);
    
        // Points which don't move (= cylinder head)
        DynamicList<label> headPoints(nPoints() / 10);         // all points move !!!

        label nHeadPoints = 0;
        forAll (points(), pointI)
        {
            scalar zCoord = points()[pointI].z();

            if (zCoord > zPist2V - delta())
            {
                piston2Points.append(pointI);                     // Of moet dit een aparte lijst zijn??!!!
            }
//            else if (zCoord > deckHeight()-zPist2V + delta())
//            {
//                headPoints.append(pointI);
//                nHeadPoints++; 
//           }
        }
            
        
        Info << "Number of head points = " << nHeadPoints << endl;
        pz[nPointZones] = 
            new pointZone
            (
                "headPoints",
                headPoints.shrink(),
                nPointZones,
                pointZones()
            );

        nPointZones++;
        
        pz[nPointZones] =
            new pointZone
            (
                "piston2Points",
                piston2Points.shrink(),
                nPointZones,
                pointZones()
            );

        nPointZones++;

    }

//  Einde piston2


//  Sliding interface for scavenging ports

    if(foundScavPorts())
    {    
        
        // Inner slider
        
        const polyPatch& innerScav =
            boundaryMesh()[boundaryMesh().findPatchID(scavInCylPatchName_)];

        labelList isf(innerScav.size());

        forAll (isf, i)
        {
            isf[i] = innerScav.start() + i;
        }

        fz[nFaceZones] = new faceZone
        (
            scavInCylPatchName_ + "Zone",
            isf,
            boolList(innerScav.size(), false),
            nFaceZones,
            faceZones()
        );

        nFaceZones++;


        // Outer slider        

        const polyPatch& outerScav =
            boundaryMesh()[boundaryMesh().findPatchID(scavInPortPatchName_)];

        labelList osf(outerScav.size());

        forAll (osf, i)
        {
            osf[i] = outerScav.start() + i;
        }

        fz[nFaceZones] = new faceZone
        (
            scavInPortPatchName_ + "Zone",
            osf,
            boolList(outerScav.size(), false),
            nFaceZones,
            faceZones()
        );

        nFaceZones++;
        
        
        fz[nFaceZones] = new faceZone
        (
            "cutFaceZone",
            labelList(0),
            boolList(0, false),
            nFaceZones,
            faceZones()
        );
        
        nFaceZones++;

        Info << "cut p" << endl;
        
        pz[nPointZones] = new pointZone
        (
            "cutPointZone",
            labelList(0),
            nPointZones,
            pointZones()
        );
        
        nPointZones++;
                
    }
    
    {

        labelList movingCells(nCells());
        label nMovingCells = 0;

        scalar pistonX = piston().cs().origin().x();
        scalar pistonY = piston().cs().origin().y();
        
        forAll(cellCentres(),cellI)
        {
            const vector& v = cellCentres()[cellI];
            
            
            if(sqrt(sqr(v.x()-pistonX)+sqr(v.y()-pistonY)) < 0.5*engTime().bore().value())
            {
                movingCells[nMovingCells] = cellI;
                nMovingCells++;
            }
            
        }
        
        movingCells.setSize(nMovingCells);
        Info << "Number of cells in the moving region: " << nMovingCells << endl;

        cz[nCellZones] = new cellZone
        (
            "movingCells",
            movingCells,
            nCellZones,
            cellZones()
        );
        
        nCellZones++;
    }

    
    Info<< "Adding " << nPointZones << " point, "
        << nFaceZones << " face zones and " << nCellZones << " cell zones" << endl;

    pz.setSize(nPointZones);
    fz.setSize(nFaceZones);
    cz.setSize(nCellZones);
    addZones(pz, fz, cz);

    List<polyMeshModifier*> tm(3);
    label nMods = 0;

    // Add piston layer addition
    if (piston().patchID().active())
    {

        topoChanger_.setSize(topoChanger_.size() + 1);
    
        topoChanger_.set
        (
            nMods,
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
    }

    // Add piston2 layer addition
    if (piston2Index != -1)
    {

        topoChanger_.setSize(topoChanger_.size() + 1);

        topoChanger_.set
        (
            nMods,
            new layerAdditionRemoval
            (
                "piston2Layer",
                nMods,
                topoChanger_,
                "piston2LayerFaces",
                piston().minLayer(),
                piston().maxLayer()
            )
        );
        nMods++;
    }

    if(foundScavPorts())
    {
        topoChanger_.setSize(topoChanger_.size() + 1);
        topoChanger_.set
        (
            nMods,
            new slidingInterface
            (
                "portInSlider",
                nMods,
                topoChanger_,
                scavInPortPatchName_ + "Zone",
                scavInCylPatchName_ + "Zone",
                "cutPointZone",
                "cutFaceZone",
                scavInPortPatchName_,
                scavInCylPatchName_,
//                slidingInterface::PARTIAL,
                slidingInterface::INTEGRAL,
                true,                          // Attach-detach action
                intersection::VISIBLE         // Projection algorithm
            )
        );
    
        nMods++;
    }
        

    Info << "Adding " << nMods << " topology modifiers" << endl;

    // Calculating the virtual piston position

    setVirtualPistonPosition();
    setVirtualPiston2Position();

    topoChanger_.setSize(nMods);

    // Write mesh
    topoChanger_.writeOpt() = IOobject::AUTO_WRITE;
    write();

    Info << "virtualPistonPosition = " << virtualPistonPosition() << endl;
    Info << "piston position = " << pistonPosition() << endl;
    Info << "virtualPiston2Position = " << virtualPiston2Position() << endl;
    Info << "piston2 position = " << piston2Position() << endl;

}


// ************************************************************************* //
