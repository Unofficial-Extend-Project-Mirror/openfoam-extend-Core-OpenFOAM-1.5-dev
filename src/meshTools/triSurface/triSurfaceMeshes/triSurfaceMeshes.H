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
    triSurfaceMeshes

Description
    Container for triSurfaces read from files.

SourceFiles
    triSurfaceMeshes.C

\*---------------------------------------------------------------------------*/

#ifndef triSurfaceMeshes_H
#define triSurfaceMeshes_H

#include "triSurfaceMesh.H"
#include "fileNameList.H"
#include "treeDataTriSurface.H"
#include "indexedOctree.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Class forward declarations

/*---------------------------------------------------------------------------*\
                           Class triSurfaceMeshes Declaration
\*---------------------------------------------------------------------------*/

class triSurfaceMeshes
:
    public PtrList<triSurfaceMesh>
{
    // Private data

        //- Names of surfaces
        fileNameList names_;

        //- Indices of all surfaces. Precalculated and stored.
        const labelList allSurfaces_;

        //- Trees
        mutable autoPtr<PtrList<indexedOctree<treeDataTriSurface> > > treesPtr_;


    // Private Member Functions

        void calcTrees() const;

        //- Disallow default bitwise copy construct
        triSurfaceMeshes(const triSurfaceMeshes&);

        //- Disallow default bitwise assignment
        void operator=(const triSurfaceMeshes&);


public:

    // Constructors

        //- Construct from directory (io.instance()) and list of local filenames
        triSurfaceMeshes(const IOobject& io, const fileNameList&);

        //- Clear derived storage
        void clearOut();


    // Member Functions

        //- Get all surfaces in directory
        static fileNameList allNames(const IOobject&);

        //- Get current list of surfaces
        const fileNameList& names() const
        {
            return names_;
        }

        //- Get trees for searching
        const PtrList<indexedOctree<treeDataTriSurface> >& trees() const
        {
            if (!treesPtr_.valid())
            {
                calcTrees();
            }
            return treesPtr_();
        }


        //- Find any intersection. Return hit point information and surface
        //  number
        label findAnyIntersection
        (
            const labelList& surfacesToTest,
            const point& start,
            const point& end,
            pointIndexHit&
        ) const;

        label findAnyIntersection
        (
            const point& start,
            const point& end,
            pointIndexHit&
        ) const;


        //- Find all intersections in order from start to end. Returns for
        //  every hit the surface and the hit info.
        void findAllIntersections
        (
            const labelList& surfacesToTest,
            const point& start,
            const point& end,
            labelList& surfaces,
            List<pointIndexHit>& surfaceHits
        ) const;

        void findAllIntersections
        (
            const point& start,
            const point& end,
            labelList& surfaces,
            List<pointIndexHit>& surfaceHits
        ) const;


        //- Find intersections of edge nearest to both endpoints.
        void findNearestIntersection
        (
            const labelList& surfacesToTest,
            const point& start,
            const point& end,

            label& surface1,        // surface index
            pointIndexHit& hit1,    // hit point information
            label& surface2,
            pointIndexHit& hit2
        ) const;

        void findNearestIntersection
        (
            const point& start,
            const point& end,

            label& surface1,        // surface index
            pointIndexHit& hit1,    // hit point information
            label& surface2,
            pointIndexHit& hit2
        ) const;


        //- Find nearest. Return -1 or surface and nearest point (in
        //  pointIndexHit)
        label findNearest
        (
            const labelList& surfacesToTest,
            const point&,
            const scalar nearestDistSqr,
            pointIndexHit&
        ) const;

        label findNearest
        (
            const point&,
            const scalar nearestDistSqr,
            pointIndexHit&
        ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
