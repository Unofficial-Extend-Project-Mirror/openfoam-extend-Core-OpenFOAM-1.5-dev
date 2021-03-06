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
    Add two surfaces. Does geometric merge on points. Does not check for
    overlapping/intersecting triangles.

    Keeps patches separate by renumbering.

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "fileName.H"
#include "triSurface.H"
#include "OFstream.H"
#include "IFstream.H"
#include "triFace.H"
#include "triFaceList.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Main program:

int main(int argc, char *argv[])
{
    argList::noParallel();
    argList::validArgs.clear();
    argList::validArgs.append("Foam surface file");
    argList::validArgs.append("Foam surface file");
    argList::validArgs.append("Foam output file");
    argList::validOptions.insert("points", "pointsFile");
    argList::validOptions.insert("mergeRegions", "");
    argList args(argc, argv);

    fileName inFileName1(args.additionalArgs()[0]);
    fileName inFileName2(args.additionalArgs()[1]);
    fileName outFileName(args.additionalArgs()[2]);

    bool addPoint = args.options().found("points");
    bool mergeRegions = args.options().found("mergeRegions");

    if (addPoint)
    {
        Info<< "Reading a surface and adding points from a file"
            << "; merging the points and writing the surface to another file"
            << nl << endl;

        Info<< "Surface  : " << inFileName1<< nl
            << "Points   : " << args.options()["points"] << nl
            << "Writing  : " << outFileName << nl << endl;
    }
    else
    {
        Info<< "Reading two surfaces"
            << "; merging points and writing the surface to another file"
            << nl << endl;

        if (mergeRegions)
        {
            Info<< "Regions from the two files will get merged" << nl
                << "Do not use this option if you want to keep the regions"
                << " separate" << nl << endl;
        }
        else
        {
            Info<< "Regions from the two files will not get merged" << nl
                << "Regions from " << inFileName2 << " will get offset so"
                << " as not to overlap with the regions in " << inFileName1
                << nl << endl;
        }


        Info<< "Surface1 : " << inFileName1<< nl
            << "Surface2 : " << inFileName2<< nl
            << "Writing  : " << outFileName << nl << endl;
    }

    const triSurface surface1(inFileName1);

    Info<< "Surface1:" << endl;
    surface1.writeStats(Info);
    Info<< endl;

    const pointField& points1 = surface1.points();

    // Final surface
    triSurface combinedSurf;

    if (addPoint)
    {
        IFstream pointStr(args.options()["points"]);
        pointField extraPoints(pointStr);

        Info<< "Additional Points:" << extraPoints.size() << endl;

        vectorField pointsAll(points1);
        label pointI = pointsAll.size();
        pointsAll.setSize(pointsAll.size() + extraPoints.size());

        forAll(extraPoints, i)
        {
            pointsAll[pointI++] = extraPoints[i];
        }

        combinedSurf = triSurface(surface1, surface1.patches(), pointsAll);
    }
    else
    {
        const triSurface surface2(inFileName2);

        Info<< "Surface2:" << endl;
        surface2.writeStats(Info);
        Info<< endl;


        // Make new storage
        List<labelledTri> facesAll(surface1.size() + surface2.size());

        const pointField& points2 = surface2.points();

        vectorField pointsAll(points1.size() + points2.size());


        label pointi = 0;
        // Copy points1 into pointsAll
        forAll(points1, point1i)
        {
            pointsAll[pointi++] = points1[point1i];
        }
        // Add surface2 points
        forAll(points2, point2i)
        {
            pointsAll[pointi++] = points2[point2i];
        }


        label trianglei = 0;
        label maxRegion1 = labelMin;

        // Copy triangles1 into trianglesAll
        // Determine max region.

        forAll(surface1, faceI)
        {
            facesAll[trianglei] = surface1[faceI];

            maxRegion1 = max(maxRegion1, facesAll[trianglei].region());

            trianglei++;
        }

        label nRegions1 = maxRegion1 + 1;

        if (!mergeRegions)
        {
            Info<< "Surface " << inFileName1 << " has " << nRegions1 << " regions"
                << nl
                << "All region numbers in " << inFileName2 << " will be offset"
                << " by this amount" << nl << endl;
        }

        // Add (renumbered) surface2 triangles
        label maxRegion2 = labelMin;

        forAll(surface2, faceI)
        {
            const labelledTri& tri = surface2[faceI];

            labelledTri& destTri = facesAll[trianglei++];

            destTri[0] = tri[0] + points1.size();
            destTri[1] = tri[1] + points1.size();
            destTri[2] = tri[2] + points1.size();

            maxRegion2  = max(maxRegion2, tri.region());

            if (mergeRegions)
            {
                destTri.region() = tri.region();
            }
            else
            {
                destTri.region() = tri.region() + nRegions1;
            }
        }

        label nRegions2 = maxRegion2 + 1;

        geometricSurfacePatchList newPatches;

        if (mergeRegions)
        {
            // Overwrite
            newPatches.setSize(max(nRegions1, nRegions2));

            forAll(surface1.patches(), patchI)
            {
                newPatches[patchI] = surface1.patches()[ patchI];
            }
            forAll(surface2.patches(), patchI)
            {
                newPatches[patchI] = surface2.patches()[ patchI];
            }
        }
        else
        {
            Info<< "Regions from " << inFileName2 << " have been renumbered:"
                << nl
                << "    old\tnew" << nl;

            for (label regionI = 0; regionI < nRegions2; regionI++)
            {
                Info<< "    " << regionI << '\t' << regionI+nRegions1
                    << nl;
            }
            Info<< nl;

            newPatches.setSize(nRegions1 + nRegions2);

            label newPatchI = 0;

            forAll(surface1.patches(), patchI)
            {
                newPatches[newPatchI++] = surface1.patches()[ patchI];
            }

            forAll(surface2.patches(), patchI)
            {
                newPatches[newPatchI++] = surface2.patches()[ patchI];
            }
        }
 

        Info<< "New patches:" << nl;
        forAll(newPatches, patchI)
        {
            Info<< "    " << patchI << '\t' << newPatches[patchI].name() << nl;
        }
        Info<< endl;


        // Construct new surface mesh
        combinedSurf = triSurface(facesAll, newPatches, pointsAll);
    }

    // Merge all common points and do some checks
    combinedSurf.cleanup(true);

    Info<< "Merged surface:" << endl;

    combinedSurf.writeStats(Info);

    Info<< endl;

    Info << "Writing : " << outFileName << endl;

    // No need to 'group' while writing since all in correct order anyway.
    combinedSurf.write(outFileName);

    Info << "End\n" << endl;

    return 0;
}


// ************************************************************************* //
