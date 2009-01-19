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
    GGIinterpolation

Description
    Mass-conservative face interpolation of face data between two
    primitivePatches

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

SourceFiles
    GGIinterpolation.C
    GGIinterpolate.C
    GGIinterpolationWeights.C

\*---------------------------------------------------------------------------*/

#ifndef GGIinterpolation_H
#define GGIinterpolation_H

#include "className.H"
#include "labelList.H"
#include "scalarField.H"
#include "pointField.H"
#include "FieldFields.H"
#include "faceList.H"
#include "intersection.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class GGIinterpolationName Declaration
\*---------------------------------------------------------------------------*/

TemplateName(GGIinterpolation);


/*---------------------------------------------------------------------------*\
                        Class GGIinterpolation Declaration
\*---------------------------------------------------------------------------*/

template<class MasterPatch, class SlavePatch>
class GGIinterpolation
:
    public GGIinterpolationName
{
    // Private data

        //- Reference to the master patch
        const MasterPatch& masterPatch_;

        //- Reference to the slave patch
        const SlavePatch& slavePatch_;

        //- Type of intersection algorithm to use in projection
        intersection::algorithm alg_;

        //- Direction projection to use in projection
        intersection::direction dir_;


    // Static data

        //- Maximum number of iterations
        static label maxIter_;

        //- Facet area error tolerance
        static scalar areaErrorTol_;

        //- Facet area sum overshoot tolerance
        //  Face neighbours will be enriched until their sum does not become
        //  greater than (1 + sumFacetTol_)*magFaceArea
        static scalar sumFacetTol_;



    // Demand-driven data

        // Master-to-slave interpolation

            //- Master to slave addressing
            mutable labelListList* masterAddrPtr_;

            //- Master to slave  weights
            mutable scalarListList* masterWeightsPtr_;


        // Slave-to-master interpolation

            //- Slave to master addressing
            mutable labelListList* slaveAddrPtr_;

            //- Slave to master weights
            mutable scalarListList* slaveWeightsPtr_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        GGIinterpolation(const GGIinterpolation&);

        //- Disallow default bitwise assignment
        void operator=(const GGIinterpolation&);


        // Helper functions for demand-driven data

            //- Calculate addressing and weights
            void calcAddressing() const;

            //- Clear all geometry and addressing
            void clearOut();


        // Interpolation data access

            //- Return reference to master addressing
            const labelListList& masterAddr() const;

            //- Return reference to master weights
            const scalarListList& masterWeights() const;

            //- Return reference to slave addressing
            const labelListList& slaveAddr() const;

            //- Return reference to slave weights
            const scalarListList& slaveWeights() const;

            //- Interpolate given source and target, addressing and weights
            template<class Type>
            static void interpolate
            (
                const Field<Type>& ff,
                Field<Type>& result,
                const labelListList& addr,
                const scalarListList& weights
            );

public:

    // Constructors

        //- Construct from components
        GGIinterpolation
        (
            const MasterPatch& masterPatch,
            const SlavePatch& slavePatch,
            const intersection::algorithm alg = intersection::VISIBLE,
            const intersection::direction dir = intersection::VECTOR
        );


    // Destructor

        ~GGIinterpolation();


    // Member Functions

        // Access

            //- Return ype of intersection algorithm to use in projection
            intersection::algorithm projectionAlgo() const
            {
                return alg_;
            }

            //- Return direction projection to use in projection
            intersection::direction projectionDir() const
            {
                return dir_;
            }


        // Interpolation functions

            //- Interpolate from master to slave
            template<class Type>
            tmp<Field<Type> > masterToSlave(const Field<Type>& pf) const;

            template<class Type>
            tmp<Field<Type> > masterToSlave
            (
                const tmp<Field<Type> >& tpf
            ) const;

            //- Interpolate from slave to master
            template<class Type>
            tmp<Field<Type> > slaveToMaster(const Field<Type>& pf) const;

            template<class Type>
            tmp<Field<Type> > slaveToMaster
            (
                const tmp<Field<Type> >& tpf
            ) const;


        // Edit

            //- Correct weighting factors for moving mesh.
            bool movePoints();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

#ifdef NoRepository
#   include "GGIinterpolation.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
