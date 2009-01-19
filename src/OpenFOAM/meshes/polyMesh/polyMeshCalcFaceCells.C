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

\*---------------------------------------------------------------------------*/

#include "polyMesh.H"
#include "primitiveMesh.H"
#include "boolList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Assumes faces, owner, neighbour ordered such that active faces come
// first. Also patches should cover only used part of faces.
void polyMesh::initMesh()
{
    if (debug)
    {
        Info<< "void polyMesh::initMesh() : "
            << "calculating faceCells" << endl;
    }

    label nCells = -1;

    forAll(allOwner_, facei)
    {
        nCells = max(nCells, max(allOwner_[facei], allNeighbour_[facei]));
    }

    nCells++;


    label nUsedFaces = 0;
    label nInternalFaces = 0;

    // Use patch info if provided, use all faces otherwise
    if (boundary_.size())
    {
        nUsedFaces =
            boundary_[boundary_.size()-1].start()
          + boundary_[boundary_.size()-1].size();
        nInternalFaces = boundary_[0].start();
    }
    else
    {
        // No patch info. Assume all faces are used.
        nUsedFaces = allOwner_.size();

        forAll(allNeighbour_, faceI)
        {
            if (allNeighbour_[faceI] == -1)
            {
                break;
            }
            else
            {
                nInternalFaces++;
            }
        }
    }


    label nUsedPoints = points_.size();

    // If not all faces are being used, check that all unused
    // faces are at the back of the list and check the number of
    // used points.
    if (nUsedFaces < allOwner_.size())
    {
        if (debug)
        {
            Info<< "void polyMesh::initMesh() : "
                << "unused faces detected.  "
                << "Number of used faces: " << nUsedFaces 
                << ".  Total number of faces: " << allOwner_.size() << endl;
        }

        for (label i = nUsedFaces; i < allOwner_.size(); i++)
        {
            if (allOwner_[i] >= 0)
            {
                FatalErrorIn("void polyMesh::initMesh()")
                    << "Error in face ordering: mixed used and unused "
                    << "faces at the end of face list." << nl
                    << "Number of used faces: " << nUsedFaces
                    << "  and face " << i << " is owned by cell "
                    << allOwner_[i]
                    << abort(FatalError);
            }
        }

        // Mark the points used by live faces.
        boolList usedPoints(points_.size(), false);

        for (label faceI = 0; faceI < nUsedFaces; faceI++)
        {
            const face& curFace = faces_[faceI];

            forAll (curFace, pointI)
            {
                usedPoints[curFace[pointI]] = true;
            }
        }

        forAll (usedPoints, pointI)
        {
            if (!usedPoints[pointI])
            {
                nUsedPoints = pointI;
                break;
            }
        }

        if (nUsedPoints < points_.size())
        {
            if (debug)
            {
                Info<< "void polyMesh::initMesh() : unused points "
                    << "detected.  Number of used points: "
                    << nUsedPoints << ". Total number of points: "
                    << points_.size() << endl;
            }

            for (label i = nUsedPoints; i < points_.size(); i++)
            {
                if (usedPoints[i])
                {
                    FatalErrorIn("void polyMesh::initMesh()")
                        << "Error in point ordering: mixed used and unused "
                        << "points at the end of point list." << nl
                        << "Number of used points: " << nUsedPoints
                        << "  and point " << i << " is used by a live face."
                        << abort(FatalError);
                }
            }
        }
    }

    // Reset the primitiveMesh
    primitiveMesh::reset
    (
        nUsedPoints,
        nInternalFaces,
        nUsedFaces,
        nCells,
        points_,
        faces_,
        allOwner_,
        allNeighbour_
    );

    string meshInfo =
        "nCells:" + Foam::name(nCells)
      + " nActiveFaces:" + Foam::name(nUsedFaces)
      + " nActivePoints:" + Foam::name(nUsedPoints);

    allOwner_.note() = meshInfo;
    allNeighbour_.note() = meshInfo;
}


void polyMesh::initMesh(cellList& c)
{
    if (debug)
    {
        Info<< "void polyMesh::initMesh(cellList& c) : "
            << "calculating faceCells" << endl;
    }

    const pointField& p = allPoints();
    const faceList& f = allFaces();

    allOwner_.setSize(f.size());
    allNeighbour_.setSize(f.size());

    labelList& own = allOwner_;
    labelList& nei = allNeighbour_;

    own = -1;
    nei = -1;

    boolList markedFaces(f.size(), false);

    label nInternalFaces = 0;

    forAll (c, cellI)
    {
        // get reference to face labels for current cell
        const labelList& cellfaces = c[cellI];

        forAll (cellfaces, faceI)
        {
            if (!markedFaces[cellfaces[faceI]])
            {
                // First visit: owner
                own[cellfaces[faceI]] = cellI;

                markedFaces[cellfaces[faceI]] = true;
            }
            else
            {
                // Second visit: neighbour
                nei[cellfaces[faceI]] = cellI;
                nInternalFaces++;
            }
        }
    }

    // Count the number of real faces.
    // Note: if there are unused faces in the mesh, they should be
    // clustered at the end of the list

    label nUsedPoints = p.size();
    label nUsedFaces = own.size();

    forAll (own, faceI)
    {
        if (own[faceI] < 0)
        {
            nUsedFaces = faceI;
            break;
        }
    }

    // If not all faces are being used, check that all unused
    // faces are at the back of the list and check the number of
    // used points.
    if (nUsedFaces < own.size())
    {
        if (debug)
        {
            Info<< "void polyMesh::initMesh(cellList& c) : "
                << "unused faces detected.  "
                << "Number of used faces: " << nUsedFaces 
                << ".  Total number of faces: " << own.size() << endl;
        }

        for (label i = nUsedFaces; i < own.size(); i++)
        {
            if (own[i] >= 0)
            {
                FatalErrorIn("void polyMesh::initMesh(cellList& c)")
                    << "Error in face ordering: mixed used and unused "
                    << "faces at the end of face list." << nl
                    << "Number of used faces: " << nUsedFaces
                    << "  and face " << i << " is owned by cell " << own[i]
                    << abort(FatalError);
            }
        }

        // Mark the points used by live faces.
        boolList usedPoints(p.size(), false);

        for (label faceI = 0; faceI < nUsedFaces; faceI++)
        {
            const face& curFace = f[faceI];

            forAll (curFace, pointI)
            {
                usedPoints[curFace[pointI]] = true;
            }
        }

        forAll (usedPoints, pointI)
        {
            if (!usedPoints[pointI])
            {
                nUsedPoints = pointI;
                break;
            }
        }

        if (nUsedPoints < p.size())
        {
            if (debug)
            {
                Info<< "void polyMesh::initMesh(cellList& c) : unused points "
                    << "detected.  Number of used points: "
                    << nUsedPoints << ". Total number of points: "
                    << p.size() << endl;
            }

            for (label i = nUsedPoints; i < p.size(); i++)
            {
                if (usedPoints[i])
                {
                    FatalErrorIn("void polyMesh::initMesh(cellList& c)")
                        << "Error in point ordering: mixed used and unused "
                        << "points at the end of point list." << nl
                        << "Number of used points: " << nUsedPoints
                        << "  and point " << i << " is used by a live face."
                        << abort(FatalError);
                }
            }
        }
    }

    // Reset the primitiveMesh
    primitiveMesh::reset
    (
        nUsedPoints,
        nInternalFaces,
        nUsedFaces,
        c.size(),
        p,
        f,
        own,
        nei,
        c
    );

    string meshInfo =
        "nCells:" + Foam::name(nCells())
      + " nActiveFaces:" + Foam::name(nUsedFaces)
      + " nActivePoints:" + Foam::name(nUsedPoints);

    allOwner_.note() = meshInfo;
    allNeighbour_.note() = meshInfo;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
