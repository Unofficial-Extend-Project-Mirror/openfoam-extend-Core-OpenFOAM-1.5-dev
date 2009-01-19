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

void Foam::doublePistonTwoStroke::checkAndCalculate()
{
    
    label pistonIndex = -1;
    bool foundPiston = false;

    label linerIndex = -1;
    bool foundLiner = false;

    label piston2Index = -1;
    bool foundPiston2 = false;
    
    
    forAll(boundary(), i)
    {
        Info << boundary()[i].name() << endl;
        if (boundary()[i].name() == "piston")
        {
            pistonIndex = i;
            foundPiston = true;
        }
        else if (boundary()[i].name() == "liner")
        {
            linerIndex = i;
            foundLiner = true;
        }
        else if (boundary()[i].name() == "piston2")
        {
            piston2Index = i;
            foundPiston2 = true;
        }
    }
    
    reduce(foundPiston, orOp<bool>());
    reduce(foundLiner, orOp<bool>());
    reduce(foundPiston2, orOp<bool>());

    if (!foundPiston)
    {
        FatalErrorIn("Foam::doublePistonTwoStroke::checkAndCalculate()")
            << " : cannot find piston patch"
            << abort(FatalError);
    }

    if (!foundLiner)
    { 
        FatalErrorIn("Foam::doublePistonTwoStroke::checkAndCalculate()")
            << " : cannot find liner patch"
            << abort(FatalError);
    }

    if (!foundPiston2)
    { 
        FatalErrorIn("Foam::doublePistonTwoStroke::checkAndCalculate()")
            << " : cannot find piston2 patch"
            << exit(FatalError);
    }

    {
        if (linerIndex != -1)
        {
            pistonPosition() =
                max(boundary()[pistonIndex].patch().localPoints()).z();
        }
        reduce(pistonPosition(), minOp<scalar>());

        if (piston2Index != -1)
        {
            piston2Position() = min
            (
                boundary()[piston2Index].patch().localPoints()
            ).z();
        }
        
        scalarField linerZ = boundary()[linerIndex].patch().localPoints().component(vector::Z);
        
        deckHeight() = average(linerZ);
        
        
        
        reduce(piston2Position(), minOp<scalar>());
        reduce(deckHeight(), minOp<scalar>());

        Info<< "deckHeight: " << deckHeight() << nl
            << "piston position: " << pistonPosition() << nl
            << "piston2 position: " << piston2Position() << endl;
    }
        

} 

void Foam::doublePistonTwoStroke::setVirtualPistonPosition()
{

    label pistonFaceIndex = faceZones().findZoneID("pistonLayerFaces");
         
    bool foundPistonFace = (pistonFaceIndex != -1);
    
    Info << "piston face index = " << pistonFaceIndex << endl; 
    
    if(!foundPistonFace)
    {
        FatalErrorIn("Foam::doublePistonTwoStroke::setVirtualPistonPosition()")
            << " : cannot find the pistonLayerFaces"
            << exit(FatalError);
    
    }
        
    const labelList& pistonFaces = faceZones()[pistonFaceIndex];
    forAll(pistonFaces, i)
    {
        const face& f = faces()[pistonFaces[i]];
        
        // should loop over facepoints...
        forAll(f, j)
        {
            virtualPistonPosition() = max(virtualPistonPosition(), points()[f[j]].z());
        }
    }
    
    reduce(virtualPistonPosition(), maxOp<scalar>());

}

void Foam::doublePistonTwoStroke::setVirtualPiston2Position()
{

    label piston2FaceIndex = faceZones().findZoneID("piston2LayerFaces");
         
    bool foundPiston2Face = (piston2FaceIndex != -1);
    
    Info << "piston2 face index = " << piston2FaceIndex << endl; 
    
    if(!foundPiston2Face)
    {
        FatalErrorIn("Foam::doublePistonTwoStroke::setVirtualPistonPosition()")
            << " : cannot find the piston2LayerFaces"
            << exit(FatalError);
    
    }
        
    const labelList& piston2Faces = faceZones()[piston2FaceIndex];
    forAll(piston2Faces, i)
    {
        const face& f = faces()[piston2Faces[i]];
        
        // should loop over facepoints...
        forAll(f, j)
        {
            virtualPiston2Position() = min(virtualPiston2Position(), points()[f[j]].z());
        }
    }
    
    reduce(virtualPiston2Position(), maxOp<scalar>());

}
