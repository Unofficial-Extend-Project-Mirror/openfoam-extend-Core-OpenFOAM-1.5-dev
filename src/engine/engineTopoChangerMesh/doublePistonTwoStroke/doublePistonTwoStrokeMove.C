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
#include "componentMixedTetPolyPatchVectorField.H"
#include "mapPolyMesh.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
void Foam::doublePistonTwoStroke::makeLayersLive()
{
    const polyTopoChanger& morphs = topoChanger_;

    // Enable layering
    forAll (morphs, modI)
    {
        if (typeid(morphs[modI]) == typeid(layerAdditionRemoval))
        {
            morphs[modI].enable();
        }
        else if (typeid(morphs[modI]) == typeid(slidingInterface))
        {
            morphs[modI].disable();
        }
        else
        {
            FatalErrorIn("void Foam::doublePistonTwoStroke::makeLayersLive()")
                << "Don't know what to do with mesh modifier "
                << modI << " of type " << morphs[modI].type()
                << abort(FatalError);
        }
    }
}

void Foam::doublePistonTwoStroke::makeSlidersLive()
{
    const polyTopoChanger& morphs = topoChanger_;

    // Enable sliding interface
    forAll (morphs, modI)
    {
        if (typeid(morphs[modI]) == typeid(layerAdditionRemoval))
        {
            morphs[modI].disable();
        }
        else if (typeid(morphs[modI]) == typeid(slidingInterface))
        {
            morphs[modI].enable();
        }
        else
        {
            FatalErrorIn("void movingSquaresTM::makeLayersLive()")
                << "Don't know what to do with mesh modifier "
                << modI << " of type " << morphs[modI].type()
                << abort(FatalError);
        }
    }

}

bool Foam::doublePistonTwoStroke::attached() const
{
    const polyTopoChanger& morphs = topoChanger_;

    bool result = false;

    forAll (morphs, modI)
    {
        if (typeid(morphs[modI]) == typeid(slidingInterface))
        {
            result =
                result
             || refCast<const slidingInterface>(morphs[modI]).attached();
        }
    }

    // Check thal all sliders are in sync (debug only)
    forAll (morphs, modI)
    {
        if (typeid(morphs[modI]) == typeid(slidingInterface))
        {
            if
            (
                result
             != refCast<const slidingInterface>(morphs[modI]).attached()
            )
            {
                FatalErrorIn("bool movingSquaresTM::attached() const")
                    << "Slider " << modI << " named " << morphs[modI].name()
                    << " out of sync: Should be" << result
                    << abort(FatalError);
            }
        }
    }

    return result;
}


bool Foam::doublePistonTwoStroke::update()
{
    // Detaching the interface
    if (attached())
    {
        Info << "Decoupling sliding interfaces" << endl;

        makeSlidersLive();
        topoChanger_.changeMesh();

        Info << "sliding interfaces successfully decoupled!!!" << endl;
    }
    else
    {
        Info << "Sliding interfaces decoupled" << endl;
    }

    Info << "Executing layer action" << endl;

    // Piston Layering

    makeLayersLive();

    // Find piston mesh modifier
    const label pistonLayerID =
        topoChanger_.findModifierID("pistonLayer");

    const label piston2LayerID =
        topoChanger_.findModifierID("piston2Layer");

    if (pistonLayerID < 0)
    {
        FatalErrorIn("void doublePistonTwoStroke::update()")
            << "Piston modifier not found."
            << abort(FatalError);
    }
    if (piston2LayerID < 0)
    {
        FatalErrorIn("void doublePistonTwoStroke::update()")
            << "Piston2 modifier not found."
            << abort(FatalError);
    }

    scalar minLayerThickness = piston().minLayer();
    scalar deltaZ = engTime().pistonDisplacement().value();
    virtualPistonPosition() += deltaZ;
    scalar deltaZ2 = piston2Displacement().value();
    virtualPiston2Position() += deltaZ2;

    Info << "virtualPistonPosition = " << virtualPistonPosition()
    << ", deckHeight = " << deckHeight()
    << ", pistonPosition = " << pistonPosition() << endl;
    Info << "virtualPiston2Position = " << virtualPiston2Position()
    << ", piston2Position = " << piston2Position() << endl;

    if (realDeformation())
    {
        // Dectivate piston layer
        Info << "Mesh deformation mode" << endl;
        topoChanger_[pistonLayerID].disable();
        topoChanger_[piston2LayerID].disable();
    }
    else
    {
        // Activate piston layer
        Info << "Piston layering mode" << endl;
        topoChanger_[pistonLayerID].enable();
        topoChanger_[piston2LayerID].enable();
    }


    // Changing topology by hand
    autoPtr<mapPolyMesh> topoChangeMap = topoChanger_.changeMesh();

    // Work array for new points position.
    pointField newPoints = points();

    if (topoChangeMap->morphing())
    {

        if (topoChangeMap->hasMotionPoints())
        {
            Info << "Topology change; executing pre-motion" << endl;
            movePoints(topoChangeMap->preMotionPoints());
            newPoints = topoChangeMap->preMotionPoints();
        }
        setV0();
        resetMotion();
    }

    // Reset the position of layered interfaces

    boolList scaleDisp(nPoints(), true);
    label nScaled = nPoints();

    List<bool> pistonPoint(newPoints.size(), false);
    List<bool> piston2Point(newPoints.size(), false);
    List<bool> headPoint(newPoints.size(), false);

    label pistonPtsIndex = pointZones().findZoneID("pistonPoints");
    const labelList& pistonPoints = pointZones()[pistonPtsIndex];
    label piston2PtsIndex = pointZones().findZoneID("piston2Points");
    const labelList& piston2Points = pointZones()[piston2PtsIndex];

    label headPtsIndex = pointZones().findZoneID("headPoints");
    const labelList& headPoints = pointZones()[headPtsIndex];

    const scalarField& movingPointsM = movingPointsMask();

    forAll(pistonPoints, i)
    {
        label pointI = pistonPoints[i];
        pistonPoint[pointI] = true;
        point& p = newPoints[pointI];

        if (p.z() < pistonPosition() - 1.0e-6)
        {
            scaleDisp[pointI] = false;
            nScaled--;
        }
    }

    forAll(piston2Points, i)
    {
        label pointI = piston2Points[i];
        piston2Point[pointI] = true;
        point& p = newPoints[pointI];

        if (p.z() > piston2Position() + 1.0e-6)
        {
            scaleDisp[pointI] = false;
            nScaled--;
        }
    }

    forAll(headPoints, i)
    {
        headPoint[headPoints[i]] = true;
        scaleDisp[headPoints[i]] = false;
        nScaled--;
    }


    if (realDeformation())
    {
        forAll(scaleDisp, pointI)
        {
            point& p = newPoints[pointI];

            if (scaleDisp[pointI])
            {
                if (p.z() < deckHeight())
                {
                    p.z() += movingPointsM[pointI]*
                        deltaZ*
                        (deckHeight() - p.z())/
                        (deckHeight() - pistonPosition());
                }
                if (p.z() > deckHeight())
                {
                    p.z() += movingPointsM[pointI]*
                        deltaZ2*
                        (deckHeight() - p.z())/
                        (deckHeight() - piston2Position());
                }
            }
            else
            {
                if (!headPoint[pointI])
                {
                    if (p.z() < deckHeight())
                    {
                        p.z() += movingPointsM[pointI] * deltaZ;
                    }
                    if (p.z() > deckHeight())
                    {
                        p.z() += movingPointsM[pointI] * deltaZ2;
                    }
                }
            }
        }
    }
    else
    {
        // Always move piston
        scalar pistonTopZ = -GREAT;
        forAll(pistonPoints, i)
        {
            point& p = newPoints[pistonPoints[i]];
            p.z() += deltaZ*movingPointsM[pistonPoints[i]];
            pistonTopZ = max(pistonTopZ, p.z());
        }
        Info << "max pistonTopZ: " << pistonTopZ << endl;

        // NN! fix. only needed for compression
        if (deltaZ > 0.0)
        {
            // check if piston-points have moved beyond the layer above
            forAll(newPoints, i)
            {
                if (!pistonPoint[i])
                {
                    if (virtualPistonPosition() > newPoints[i].z())
                    {
                        newPoints[i].z() =
                            (1.0 - movingPointsM[i])*newPoints[i].z()
                          + movingPointsM[i]*
                            (pistonTopZ + 0.9*minLayerThickness);
                    }
                }
            }
        }


        // Always move piston2
        scalar piston2TopZ = GREAT;
        forAll(piston2Points, i)
        {
            point& p = newPoints[piston2Points[i]];
            p.z() += deltaZ2*movingPointsM[piston2Points[i]];
            piston2TopZ = min(piston2TopZ, p.z());
        }
        Info << "min piston2TopZ: " << piston2TopZ << endl;

        // NN! fix. only needed for compression
/*        if (deltaZ2 < 0.0)
        {
            // check if piston-points have moved beyond the layer above
            forAll(newPoints, i)
            {
                // 2 weggehaald!!! (piston2Point[i])
                if (!piston2Point[i])
                {
                    if (virtualPiston2Position() < newPoints[i].z())
                    {
                        newPoints[i].z() =
                            (-1.0 + movingPointsM[i])*newPoints[i].z()
                          - movingPointsM[i]*
                            (piston2TopZ - 0.9*minLayerThickness);
                    }
                }
            }
        }
*/
    }


    movePoints(newPoints);

    deleteDemandDrivenData(movingPointsMaskPtr_);

    pistonPosition() += deltaZ;
    piston2Position() += deltaZ2;

    {
        pointField oldPointsNew = oldPoints();
        pointField newPointsNew = points();

        // Attach the interface
        Info << "Coupling sliding interfaces" << endl;
        makeSlidersLive();

        // Changing topology by hand
        autoPtr<mapPolyMesh> topoChangeMap3 = topoChanger_.changeMesh();

        Info << "Sliding interfaces coupled: " << attached() << endl;

            if (attached())
            {
                // correct the motion after attaching the sliding interface

                pointField mappedOldPointsNew(allPoints().size());

                mappedOldPointsNew.map
                (
                    oldPointsNew,
                    topoChangeMap3->pointMap()
                );

                pointField newPoints = allPoints();

                movePoints(mappedOldPointsNew);

                resetMotion();
                setV0();
                movePoints(newPoints);
            }
    }

    return true;
}

