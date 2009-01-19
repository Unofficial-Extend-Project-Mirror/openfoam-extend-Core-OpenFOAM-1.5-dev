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


#ifndef doublePistonTwoStroke_H
#define doublePistonTwoStroke_H

#include "engineTopoChangerMesh.H"
#include "simpleEnginePiston.H"
#include "motionSolver.H"
#include "polyPatchID.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Class forward declarations

/*---------------------------------------------------------------------------*\
                           Class doublePistonTwoStroke Declaration
\*---------------------------------------------------------------------------*/

class doublePistonTwoStroke
:
    public engineTopoChangerMesh
{
    // Private data

        //- Piston patch
        simpleEnginePiston piston_;
        
        //- Upper bowl patch
        word scavInCylPatchName_;
        
        //- Lower bowl patch
        word scavInPortPatchName_;

        //- Markup field for points.  Moving points marked with 1
        mutable scalarField* movingPointsMaskPtr_;

        //- Layering-to-deformation switch in crank-angle degrees
        scalar deformSwitch_;
        
        //- Tolerance used when the piston faceZone is created
        scalar delta_;

        //- Distance from the piston patch
        scalar offSet_;

        //- Piston Position
        scalar pistonPosition_;
        scalar piston2Position_;

        //- Virtual piston position (pistonPosition + offSet)
        scalar virtualPistonPosition_;
        scalar virtualPiston2Position_;

        //- deckHeight
        scalar deckHeight_;

        //- Coordinate system
        autoPtr<coordinateSystem> csPtr_;

        //- Are there scavenging ports?
        bool foundScavPorts_;

        //- Tolerance for closure of scavenging ports
        scalar scavPortsTol_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        doublePistonTwoStroke(const doublePistonTwoStroke&);

        //- Disallow default bitwise assignment
        void operator=(const doublePistonTwoStroke&);

        //- Make layering modifiers live
        void makeLayersLive();

        //- Make Sliding interface modifiers live
        void makeSlidersLive();

        //- Is the sliding interface attached?
        bool attached() const;

        //- Check if all patches exist, then calculate virtualPistonPosition, 
        //- pistonPosition and deckHeight for the first time
        void checkAndCalculate();
        
        //- Calculate the virtualPistonPosition, 
        void setVirtualPistonPosition();
        void setVirtualPiston2Position();

        //- Correct the vertical motion for the points below the "virtual piston", 
        void correctVerticalMotion();
        
        //- Apply the moving mask, the points of the ports do not move. 
        void applyMovingMask();
        
        //- Calculate moving masks
        void calcMovingMasks() const;

        //- Return moving points mask
        const scalarField& movingPointsMask() const;

        bool realDeformation() const;
        
        //- Are the scavenging ports opened?
        bool portsOpened() const;

                
public:

    //- Runtime type information
    TypeName("doublePistonTwoStroke");


    // Constructors

        //- Construct from database
        explicit doublePistonTwoStroke(const IOobject& io);


    // Destructor - default


    // Member Functions

        //- Return coordinate system
        const coordinateSystem& cs() const
        {
            return csPtr_();
        }

        const scalar& delta() const
        {
            return delta_;
        }

        scalar& delta() 
        {
            return delta_;
        }
        
        const scalar& offSet() const
        {
            return offSet_;
        }

        scalar& offSet() 
        {
            return offSet_;
        }
        
        const scalar& pistonPosition() const
        {
            return pistonPosition_;
        }

        scalar& pistonPosition() 
        {
            return pistonPosition_;
        }
        
        const scalar& piston2Position() const
        {
            return piston2Position_;
        }

        scalar& piston2Position() 
        {
            return piston2Position_;
        }

        const scalar& virtualPistonPosition() const
        {
            return virtualPistonPosition_;
        }

        scalar& virtualPistonPosition() 
        {
            return virtualPistonPosition_;
        }
        
        const scalar& virtualPiston2Position() const
        {
            return virtualPiston2Position_;
        }

        scalar& virtualPiston2Position() 
        {
            return virtualPiston2Position_;
        }

        const scalar& deckHeight() const
        {
            return deckHeight_;
        }

        scalar& deckHeight() 
        {
            return deckHeight_;
        }
        
        const simpleEnginePiston& piston() const
        {
            return piston_;
        }

        const bool& foundScavPorts() const
        {
            return foundScavPorts_;
        }
        
        //- Return true for mesh deformation mode
        bool deformation() const
        {
            return
                engTime().thetaRevolution() > -deformSwitch_
             && engTime().thetaRevolution() < deformSwitch_;
        }
        

        //- Add valve and piston zones and modifiers
        void addZonesAndModifiers();

        //- Move and morph
        virtual bool update();
        
        //- Set boundary velocities
        void setBoundaryVelocity(volVectorField& U);
        

        //- Calculate the piston2 position from the engine geometry
        //  and given crank angle.
        scalar piston2Position(const scalar theta) const;
        
        //- Return current piston2 position
//        dimensionedScalar piston2Position() const;

        //- Return piston2 displacement for current time step
        dimensionedScalar piston2Displacement() const;
        
        
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
