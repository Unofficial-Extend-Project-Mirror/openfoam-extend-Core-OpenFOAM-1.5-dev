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

\*----------------------------------------------------------------------------*/

#include "triSurfaceMeshes.H"
#include "Random.H"
#include "Time.H"
#include "SortableList.H"
#include "IOmanip.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::triSurfaceMeshes::calcTrees() const
{
    // Random number generator
    Random rndGen(65431);

    treesPtr_.reset(new PtrList<indexedOctree<treeDataTriSurface> >(size()));
    PtrList<indexedOctree<treeDataTriSurface> >& trees = treesPtr_();

    forAll(*this, i)
    {
        const triSurface& s = operator[](i);

        // bb of surface
        treeBoundBox bb(s.localPoints());

        trees.set
        (
            i,
            new indexedOctree<treeDataTriSurface>
            (
                treeDataTriSurface(s),
                bb.extend(rndGen, 1E-3),    // slightly randomize bb
                8,      // maxLevel
                10,     // leafsize
                3.0     // duplicity
            )
        );
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
Foam::triSurfaceMeshes::triSurfaceMeshes
(
    const IOobject& io,
    const fileNameList& names
)
:
    PtrList<triSurfaceMesh>(names.size()),
    names_(names),
    allSurfaces_(identity(names.size())),
    treesPtr_(NULL)
{
    forAll(names, i)
    {
        autoPtr<IOobject> surfaceIO = io.clone();
        surfaceIO().rename(names[i]);

        Pout<< "Loading surface " << surfaceIO().name() << endl;

        fileName fullPath = surfaceIO().filePath();

        if (fullPath.size() == 0)
        {
            FatalErrorIn
            (
                "triSurfaceMeshes::triSurfaceMeshes"
                "(const IOobject&, const fileNameList&)"
            )   << "Cannot load surface " << surfaceIO().name()
                << " starting from path " << surfaceIO().path()
                << exit(FatalError);
        }

        set(i, new triSurfaceMesh(surfaceIO(), fullPath));

        string oldPrefix(Pout.prefix());
        Pout.prefix() += "    ";
        operator[](i).writeStats(Pout);
        Pout.prefix() = oldPrefix;
    }
}


void Foam::triSurfaceMeshes::clearOut()
{
    treesPtr_.reset(NULL);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::fileNameList Foam::triSurfaceMeshes::allNames(const IOobject& io)
{
    return readDir(io.path(), fileName::FILE);
}


// Find any intersection
Foam::label Foam::triSurfaceMeshes::findAnyIntersection
(
    const labelList& surfaces,
    const point& start,
    const point& end,
    pointIndexHit& hitInfo
) const
{
    const PtrList<indexedOctree<treeDataTriSurface> >& surfaceQueries = trees();

    forAll(surfaces, i)
    {
        label surfI = surfaces[i];

        hitInfo = surfaceQueries[surfI].findLineAny(start, end);

        if (hitInfo.hit())
        {
            return surfI;
        }
    }
    return -1;
}


Foam::label Foam::triSurfaceMeshes::findAnyIntersection
(
    const point& start,
    const point& end,
    pointIndexHit& hitInfo
) const
{
    return findAnyIntersection
    (
        allSurfaces_,
        start,
        end,
        hitInfo
    );
}


// Find intersections of edge nearest to both endpoints.
void Foam::triSurfaceMeshes::findAllIntersections
(
    const labelList& surfaces,
    const point& start,
    const point& end,

    labelList& surfacesIndex,
    List<pointIndexHit>& surfaceHitInfo
) const
{
    const PtrList<indexedOctree<treeDataTriSurface> >& surfaceQueries = trees();

    DynamicList<label> hitSurfaces(surfaceQueries.size());
    DynamicList<pointIndexHit> hitInfos(surfaceQueries.size());
    DynamicList<scalar> hitDistSqr(surfaceQueries.size());

    const vector dirVec(end-start);
    const scalar magSqrDirVec(magSqr(dirVec));
    const vector smallVec(1E-6*dirVec);

    forAll(surfaces, i)
    {
        label surfI = surfaces[i];

        // Current starting point of ray.
        point pt = start;

        while (true)
        {
            // See if any intersection between pt and end
            pointIndexHit inter = surfaceQueries[surfI].findLine(pt, end);

            if (!inter.hit())
            {
                break;
            }
            hitSurfaces.append(surfI);
            hitInfos.append(inter);
            hitDistSqr.append(magSqr(inter.hitPoint() - start));

            pt = inter.hitPoint() + smallVec;

            if (((pt-start)&dirVec) > magSqrDirVec)
            {
                // Adding smallVec has taken us beyond end
                break;
            }
        }
    }
    hitSurfaces.shrink();
    hitInfos.shrink();
    hitDistSqr.shrink();

    // Sort and transfer to arguments

    surfacesIndex.setSize(hitSurfaces.size());
    surfaceHitInfo.setSize(hitSurfaces.size());

    // Sort from start to end.
    SortableList<scalar> sortedDist(hitDistSqr);

    forAll(sortedDist.indices(), newPos)
    {
        label oldPos = sortedDist.indices()[newPos];
        surfacesIndex[newPos] = hitSurfaces[oldPos];
        surfaceHitInfo[newPos] = hitInfos[oldPos];
    }
}


void Foam::triSurfaceMeshes::findAllIntersections
(
    const point& start,
    const point& end,

    labelList& surfacesIndex,
    List<pointIndexHit>& surfaceHitInfo
) const
{
    findAllIntersections
    (
        allSurfaces_,
        start,
        end,
        surfacesIndex,
        surfaceHitInfo
    );
}


// Find intersections of edge nearest to both endpoints.
void Foam::triSurfaceMeshes::findNearestIntersection
(
    const labelList& surfaces,
    const point& start,
    const point& end,

    label& surface1,
    pointIndexHit& hit1,
    label& surface2,
    pointIndexHit& hit2
) const
{
    const PtrList<indexedOctree<treeDataTriSurface> >& surfaceQueries = trees();

    surface1 = -1;
    // Initialize to endpoint
    hit1 = pointIndexHit(false, end, -1);

    forAll(surfaces, i)
    {
        label surfI = surfaces[i];

        if (hit1.rawPoint() == start)
        {
            break;
        }

        // See if any intersection between start and current nearest
        pointIndexHit inter = surfaceQueries[surfI].findLine
        (
            start,
            hit1.rawPoint()
        );

        if (inter.hit())
        {
            hit1 = inter;
            surface1 = surfI;
        }
    }


    // Find the nearest intersection from end to start. Note that we initialize
    // to the first intersection (if any).
    surface2 = surface1;
    hit2 = pointIndexHit(hit1);

    if (hit1.hit())
    {
        // Test from the end side.
        forAll(surfaces, i)
        {
            label surfI = surfaces[i];

            if (hit2.rawPoint() == end)
            {
                break;
            }

            // See if any intersection between end and current nearest
            pointIndexHit inter = surfaceQueries[surfI].findLine
            (
                end,
                hit2.rawPoint()
            );

            if (inter.hit())
            {
                hit2 = inter;
                surface2 = surfI;
            }
        }
    }
}


void Foam::triSurfaceMeshes::findNearestIntersection
(
    const point& start,
    const point& end,

    label& surface1,
    pointIndexHit& hit1,
    label& surface2,
    pointIndexHit& hit2
) const
{
    findNearestIntersection
    (
        allSurfaces_,
        start,
        end,
        surface1,
        hit1,
        surface2,
        hit2
    );
}


// Find nearest. Return -1 or nearest point
Foam::label Foam::triSurfaceMeshes::findNearest
(
    const labelList& surfaces,
    const point& pt,
    const scalar nearestDistSqr,
    pointIndexHit& nearestHit
) const
{
    const PtrList<indexedOctree<treeDataTriSurface> >& surfaceQueries = trees();

    // nearest surface
    label minSurface = -1;
    scalar minDistSqr = Foam::sqr(GREAT);

    forAll(surfaces, i)
    {
        label surfI = surfaces[i];

        pointIndexHit hit
        (
            surfaceQueries[surfI].findNearest(pt, nearestDistSqr)
        );

        if (hit.hit())
        {
            scalar distSqr = magSqr(hit.hitPoint()-pt);

            if (distSqr < minDistSqr)
            {
                minDistSqr = distSqr;
                minSurface = surfI;
                nearestHit = hit;
            }
        }
    }

    if (minSurface == -1)
    {
        // maxLevel unchanged. No interesting surface hit.
        nearestHit.setMiss();
    }

    return minSurface;
}


// Find nearest. Return -1 or nearest point
Foam::label Foam::triSurfaceMeshes::findNearest
(
    const point& pt,
    const scalar nearestDistSqr,
    pointIndexHit& nearestHit
) const
{
    return findNearest
    (
        allSurfaces_,
        pt,
        nearestDistSqr,
        nearestHit
    );
}


// ************************************************************************* //
