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

Class
    mixerFvMesh

Description
    Mixer mesh using a sliding interface.

SourceFiles
    mixerFvMesh.C

\*---------------------------------------------------------------------------*/

#ifndef mixerFvMesh_H
#define mixerFvMesh_H

#include "topoChangerFvMesh.H"
#include "cylindricalCS.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class mixerFvMesh Declaration
\*---------------------------------------------------------------------------*/

class mixerFvMesh
:
    public topoChangerFvMesh
{
    // Private data

        //- Motion dictionary
        dictionary dict_;

        //- Coordinate system
        autoPtr<coordinateSystem> csPtr_;

        // - Rotational speed in rotations per minute (rpm)
        scalar rpm_;

        //- Rotating region marker point
        point rotatingRegionMarker_;

        //- Attach-detach action
        Switch attachDetach_;

        //- Markup field for points.  Moving points marked with 1
        mutable scalarField* movingPointsMaskPtr_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        mixerFvMesh(const mixerFvMesh&);

        //- Disallow default bitwise assignment
        void operator=(const mixerFvMesh&);


        //- Add mixer zones and modifiers
        void addZonesAndModifiers();

        //- Return true if sliding interface is attached
        bool attached() const;

        //- Calculate moving mask
        void calcMovingMask() const;

        //- Return moving points mask
        const scalarField& movingPointsMask() const;


public:

    //- Runtime type information
    TypeName("mixerFvMesh");


    // Constructors

        //- Construct from IOobject
        explicit mixerFvMesh(const IOobject& io);


    // Destructor

        virtual ~mixerFvMesh();


    // Member Functions

        //- Return coordinate system
        const coordinateSystem& cs() const
        {
            return csPtr_();
        }

        //- Update the mesh for both mesh motion and topology change
        virtual bool update();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
