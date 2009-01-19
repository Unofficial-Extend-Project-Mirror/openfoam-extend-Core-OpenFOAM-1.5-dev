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
    particle

Description

\*---------------------------------------------------------------------------*/

#ifndef particle_H
#define particle_H

#include "vector.H"
#include "IDLList.H"
#include "labelList.H"
#include "pointField.H"
#include "faceList.H"
#include "typeInfo.H"
#include "OFstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

template<class particle>
class Cloud;

class wedgePolyPatch;
class symmetryPolyPatch;
class cyclicPolyPatch;
class processorPolyPatch;
class wallPolyPatch;
class polyPatch;

// * * * * * * Forward declaration of template friend fuctions * * * * * * * //

template<class particleType>
class particle;

template<class particleType>
Ostream& operator<<
(
    Ostream&,
    const particle<particleType>&
);


/*---------------------------------------------------------------------------*\
                           Class particle Declaration
\*---------------------------------------------------------------------------*/

template<class particleType>
class particle
:
    public IDLList<particleType>::link
{
    // Private data

        //- Reference to the particle cloud
        const Cloud<particleType>& cloud_;

        //- Position of particle
        vector position_;

        //- Index of the cell it is in
        label celli_;

        //- Face index if the particle is on a face otherwise -1
        label facei_;

        //- Fraction of time-step completed
        scalar stepFraction_;


    // Private member functions

        //- Return the 'lambda' value for the position, p, on the face,
        // where, p = from + lamda*(to - from)
        // for non-static meshes
        inline scalar lambda
        (
            const vector& from,
            const vector& to,
            const label facei,
            const scalar stepFraction
        );

        //- Return the 'lambda' value for the position, p, on the face,
        // where, p = from + lamda*(to - from) 
        // for static meshes
        inline scalar lambda
        (
            const vector& from,
            const vector& to,
            const label facei
        );

        //- Return the faces between position and cell centre
        labelList findFaces
        (
            const vector& position
        );

        //- Return the faces between position and cell centre
        labelList findFaces
        (
            const vector& position,
            const label celli,
            const scalar stepFraction
        );


protected:

        //- Overridable function to handle the particle hitting a wedgePatch
        template<class TrackingData>
        void hitWedgePatch
        (
            const wedgePolyPatch&,
            TrackingData& td
        );

        //- Overridable function to handle the particle hitting a symmetryPatch
        template<class TrackingData>
        void hitSymmetryPatch
        (
            const symmetryPolyPatch&,
            TrackingData& td
        );

        //- Overridable function to handle the particle hitting a cyclicPatch
        template<class TrackingData>
        void hitCyclicPatch
        (
            const cyclicPolyPatch&,
            TrackingData& td
        );

        //- Overridable function to handle the particle hitting a processorPatch
        template<class TrackingData>
        void hitProcessorPatch
        (
            const processorPolyPatch&,
            TrackingData& td
        );

        //- Overridable function to handle the particle hitting a wallPatch
        template<class TrackingData>
        void hitWallPatch
        (
            const wallPolyPatch&,
            TrackingData& td
        );

        //- Overridable function to handle the particle hitting a general patch
        template<class TrackingData>
        void hitPatch
        (
            const polyPatch&,
            TrackingData& td
        );

        //- Transform the position the particle
        //  according to the given transformation tensor
        void transformPosition(const tensor& T);

        //- Transform the physical properties of the particle
        //  according to the given transformation tensor
        void transformProperties(const tensor& T);

        //- Transform the physical properties of the particle
        //  according to the given separation vector
        void transformProperties(const vector& separation);

        //- Convert global addressing to the processor patch
        //  local equivalents
        template<class TrackingData>
        void prepareForParallelTransfer(const label patchi, TrackingData& td);

        //- Convert processor patch addressing to the global equivalents
        //  and set the celli to the face-neighbour
        template<class TrackingData>
        void correctAfterParallelTransfer(const label patchi, TrackingData& td);


public:

    friend class Cloud<particleType>;


    //- Runtime type information
    TypeName("particle");


    // Constructors

        //- Construct from components
        particle
        (
            const Cloud<particleType>&,
            const vector& position,
            const label celli
        );

        //- Construct from Istream
        particle
        (
            const Cloud<particleType>&,
            Istream&,
            bool readFields = true
        );

        //- Factory class to read-construct particles used for parallel transfer
        class iNew
        {
            const Cloud<particleType>& cloud_;

        public:

            iNew(const Cloud<particleType>& cloud)
            :
                cloud_(cloud)
            {}

            autoPtr<particleType> operator()(Istream& is) const
            {
                // Read fields irrespective of default argument
                return autoPtr<particleType>
                    (new particleType(cloud_, is, true));
            }
        };


    // Destructor

        virtual ~particle()
        {}


    // Member Functions

        // Access

            //- Return true if particle is in cell
            inline bool inCell();

            //- Return true if position is in cell i
            inline bool inCell
            (
                const vector& position,
                const label celli,
                const scalar stepFraction
            );

            //- Return current particle position
            inline const vector& position() const;

            //- Return current cell particle is in
            inline label cell() const;

            //- Return current face particle is on otherwise -1
            inline label face() const;

            //- Return reference to the particle cloud
            inline const Cloud<particleType>& cloud() const;

            //- Return the impact model to be used, soft or hard (default).
            inline bool softImpact() const;


        // Check

            //- Is the particle on the boundary/( or outside the domain)?
            inline bool onBoundary() const;

            //- Which patch is particle on
            inline label patch(const label facei) const;

            //- Which face of this patch is this particle on
            inline label patchFace(const label patchi, const label facei) const;
    
            //- The nearest distance to a wall that 
            //  the particle can be in the n direction
            inline scalar wallImpactDistance(const vector& n) const;

            //- Return the fraction of time-step completed
            inline scalar& stepFraction();

            //-  Return the fraction of time-step completed
            inline scalar stepFraction() const;


        // Track

            //- Track particle to end of trajectory
            //  or until it hits the boundary.
            //  On entry 'stepFraction()' should be set to the fraction of the
            //  time-step at which the tracking starts and on exit it contains
            //  the fraction of the time-step completed.
            //  Returns the boundary face index if the track stops at the
            //  boundary, -1 otherwise.
            template<class TrackingData>
            label track
            (
                const vector& endPosition,
                TrackingData& td
            );

            //- Calls the templated track with dummy TrackingData
            label track(const vector& endPosition);

            //- Track particle to a given position and returns 1.0 if the 
            //  trajectory is completed without hitting a face otherwise
            //  stops at the face and returns the fraction of the trajectory
            //  completed.
            //  on entry 'stepFraction()' should be set to the fraction of the
            //  time-step at which the tracking starts.
            template<class TrackingData>
            scalar trackToFace
            (
                const vector& endPosition,
                TrackingData& td
            );

            //- calls the templated trackToFace with dummy TrackingData
            scalar trackToFace(const vector& endPosition);


    // Ostream Operator

        friend Ostream& operator<< <particleType>
        (
            Ostream&,
            const particle<particleType>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "particleI.H"

#define defineParticleTypeNameAndDebug(Type, DebugSwitch)                     \
    template<>                                                                \
    const Foam::word particle<Type>::typeName(#Type);                         \
    template<>                                                                \
    int particle<Type>::debug(Foam::debug::debugSwitch(#Type, DebugSwitch));

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "particle.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
