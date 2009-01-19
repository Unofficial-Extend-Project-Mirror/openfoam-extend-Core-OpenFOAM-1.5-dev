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
    the class holds the information necessary to renumber the mesh in such a way
    that the band of the matrix is reduced. The algorithm uses a simple search
    through the neighbour list.  At the same time, strict upper
    triangular face ordering is enforced.

\*---------------------------------------------------------------------------*/

#include "fvMeshBandCompression.H"
#include "bandCompression.H"
#include "SLList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

fvMeshBandCompression::fvMeshBandCompression(const IOobject& io)
:
    fvMesh(io),
    cellOrder_(bandCompression(cellCells()))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

fvMesh* fvMeshBandCompression::renumberedMesh() const
{
    // Renumber the cell list

    const cellList& oldCells = cells();
    const labelList& oldOwner = faceOwner();
    const labelList& oldNeighbour = faceNeighbour();

    cellList newCells(oldCells.size());

    // The order list gives the old cell label for every new cell
    const labelList& order = cellOrder();

    // The reverse order list gives the new cell label for every old cell
    labelList reverseOrder(order.size());

    forAll (order, cellI)
    {
        newCells[cellI] = oldCells[order[cellI]];

        reverseOrder[order[cellI]] = cellI;
    }

    // Renumber the faces.
    // Reverse face order gives the new face number for every old face
    labelList reverseFaceOrder(nFaces(), 0);

    // Mark the internal faces with -2 so that they are inserted first
    forAll (newCells, cellI)
    {
        const labelList& curFaces = newCells[cellI];

        forAll (curFaces, faceI)
        {
            reverseFaceOrder[curFaces[faceI]]--;
        }
    }

    // Order internal faces
    label nMarkedFaces = 0;

    forAll (newCells, cellI)
    {
        // Note:
        // Insertion cannot be done in one go as the faces need to be
        // added into the list in the increasing order of neighbour
        // cells.  Therefore, all neighbours will be detected first
        // and then added in the correct order.  

        const labelList& curFaces = newCells[cellI];

        // Record the neighbour cell
        labelList neiCells(curFaces.size(), -1);

        label nNeighbours = 0;

        forAll (curFaces, faceI)
        {
            if (reverseFaceOrder[curFaces[faceI]] == -2)
            {
                // Face is internal and gets reordered
                if (cellI == reverseOrder[oldOwner[curFaces[faceI]]])
                {
                    neiCells[faceI] =
                        reverseOrder[oldNeighbour[curFaces[faceI]]];
                }
                else if (cellI == reverseOrder[oldNeighbour[curFaces[faceI]]])
                {
                    neiCells[faceI] =
                        reverseOrder[oldOwner[curFaces[faceI]]];
                }
                else
                {
                    Info << "Screwed up!!!" << endl;
                }

                nNeighbours++;
            }
        }

        // Add the faces in the increasing order of neighbours
        for (label neiSearch = 0; neiSearch < nNeighbours; neiSearch++)
        {
            // Find the lowest neighbour which is still valid
            label nextNei = -1;
            label minNei = oldCells.size();

            forAll (neiCells, ncI)
            {
                if (neiCells[ncI] > -1 && neiCells[ncI] < minNei)
                {
                    nextNei = ncI;
                    minNei = neiCells[ncI];
                }
            }

            if (nextNei > -1)
            {
                // Face is internal and gets reordered
                reverseFaceOrder[curFaces[nextNei]] = nMarkedFaces;

                // Stop the neighbour from being used again
                neiCells[nextNei] = -1;

                nMarkedFaces++;
            }
            else
            {
                FatalErrorIn
                (
                    "fvMesh* fvMeshBandCompression::renumberedMesh() const"
                )   << "Error in internal face insertion"
                    << abort(FatalError);
            }
        }
    }

    // Insert the boundary faces into reordering list
    forAll (reverseFaceOrder, faceI)
    {
        if (reverseFaceOrder[faceI] < 0)
        {
            reverseFaceOrder[faceI] = nMarkedFaces;

            nMarkedFaces++;
        }
    }

    // Face order gives the old face label for every new face
    labelList faceOrder(reverseFaceOrder.size());

    forAll (faceOrder, faceI)
    {
        faceOrder[reverseFaceOrder[faceI]] = faceI;
    }

    // Renumber the cells
    forAll (newCells, cellI)
    {
        labelList oldF = newCells[cellI];

        labelList& newF = newCells[cellI];

        forAll (newF, fI)
        {
            newF[fI] = reverseFaceOrder[oldF[fI]];
        }
    }

    faceList newFaces(allFaces().size());

    const faceList& oldFaces = allFaces();

    forAll (newFaces, faceI)
    {
        newFaces[faceI] = oldFaces[faceOrder[faceI]];
    }

    // Turn the face that need to be turned

    // Only loop through internal faces
    forAll (oldNeighbour, faceI)
    {
        if
        (
            reverseOrder[oldNeighbour[faceOrder[faceI]]]
          < reverseOrder[oldOwner[faceOrder[faceI]]]
        )
        {
            newFaces[faceI] = newFaces[faceI].reverseFace();
        }
    }

    // Make a new mesh
    fvMesh* newMeshPtr = new fvMesh
    (
        IOobject
        (
            fvMesh::defaultRegion,
            time().constant(),
            time()
        ),
        allPoints(),
        newFaces,
        newCells
    );

    fvMesh& newMesh = *newMeshPtr;

    // Add boundaries

    const polyPatchList& oldPatches = boundaryMesh();

    List<polyPatch*> newPatches(oldPatches.size());

    forAll (newPatches, patchI)
    {
        newPatches[patchI] =
            oldPatches[patchI].clone(newMesh.boundaryMesh()).ptr();
    }

    newMesh.addFvPatches(newPatches);

    return newMeshPtr;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
