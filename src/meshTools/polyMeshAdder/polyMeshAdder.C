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
    Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

Class
    polyMeshAdder

\*----------------------------------------------------------------------------*/

#include "polyMeshAdder.H"
#include "mapAddedPolyMesh.H"
#include "IOobject.H"
#include "faceCoupleInfo.H"
#include "processorPolyPatch.H"
#include "SortableList.H"
#include "Time.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

//- Append all mapped elements of a list to a DynamicList
void Foam::polyMeshAdder::append
(
    const labelList& map,
    const labelList& lst,
    DynamicList<label>& dynLst
)
{
    dynLst.setSize(dynLst.size() + lst.size());

    forAll(lst, i)
    {
        label newElem = map[lst[i]];

        if (newElem != -1)
        {
            dynLst.append(newElem);
        }
    }
}


// Return the vertices (can be 0) inbetween the two endpoints of meshEdge.
// Returns them in order, from meshEdge[0] to meshEdge[1].
Foam::labelList Foam::polyMeshAdder::stringEdges
(
    const pointField& cutPoints,
    const edge& meshEdge,
    const pointField& meshPoints,
    const labelList& cutToAllPoints,
    const edgeList& cutEdges,
    const labelList& cutEdgeLabels
)
{
    DynamicList<label> orderedVertices(cutEdgeLabels.size());

    // Current vertex (in all point labels)
    label vertI = meshEdge[0];
    // Current edge (in cutEdge labels)
    label edgeI = -1;

    while (vertI != -1 && vertI != meshEdge[1])
    {
        label prevVertI = vertI;

        vertI = -1;

        // Find the edge that connects to vertI
        forAll(cutEdgeLabels, i)
        {
            label cutEdgeI = cutEdgeLabels[i];
            const edge& cutEdge = cutEdges[cutEdgeI];
            edge allEdge
            (
                cutToAllPoints[cutEdge[0]],
                cutToAllPoints[cutEdge[1]]
            );

            if (cutEdgeI != edgeI && allEdge[0] == prevVertI)
            {
                vertI = allEdge[1];
                edgeI = cutEdgeI;

                if (vertI != meshEdge[1])
                {
                    orderedVertices.append(vertI);
                }
                break;
            }
            else if (cutEdgeI != edgeI && allEdge[1] == prevVertI)
            {
                vertI = allEdge[0];
                edgeI = cutEdgeI;

                if (vertI != meshEdge[1])
                {
                    orderedVertices.append(vertI);
                }
                break;
            }
        }
    }


    if (vertI == -1)
    {
        Pout<< "cutEdges:" << nl;

        forAll(cutEdgeLabels, i)
        {
            label cutEdgeI = cutEdgeLabels[i];
            const edge& cutEdge = cutEdges[cutEdgeI];
            edge allEdge
            (
                cutToAllPoints[cutEdge[0]],
                cutToAllPoints[cutEdge[1]]
            );
            Pout<< "    " << allEdge << " coord:" << cutPoints[cutEdge[0]]
                 << cutPoints[cutEdge[1]] << nl;
        }
        Pout<< endl;

        FatalErrorIn("polyMeshAdder::stringEdges")
            << "Did not find connected string of cut edges" << nl
            << "that sits inbetween the endpoints of edge " << meshEdge
            << " coords " << meshPoints[meshEdge[0]] << ' '
            << meshPoints[meshEdge[1]] << abort(FatalError);
    }

    return orderedVertices.shrink();
}


// Get index of patch in new set of patchnames/types
Foam::label Foam::polyMeshAdder::patchIndex
(
    const polyPatch& p,
    DynamicList<word>& allPatchNames,
    DynamicList<word>& allPatchTypes
)
{
    // Find the patch name on the list.  If the patch is already there
    // and patch types match, return index
    const word& pType = p.type();
    const word& pName = p.name();

    label patchI = findIndex(allPatchNames, pName);

    if (patchI == -1)
    {
        // Patch not found. Append to the list
        allPatchNames.append(pName);
        allPatchTypes.append(pType);

        return allPatchNames.size() - 1;
    }
    else if (allPatchTypes[patchI] == pType)
    {
        // Found name and types match
        return patchI;
    }
    else
    {
        // Found the name, but type is different

        // Duplicate name is not allowed.  Create a composite name from the
        // patch name and case name
        const word& caseName = p.boundaryMesh().mesh().time().caseName();

        allPatchNames.append(pName + "_" + caseName);
        allPatchTypes.append(pType);

        Pout<< "label patchIndex(const polyPatch& p) : "
            << "Patch " << p.index() << " named "
            << pName << " in mesh " << caseName
            << " already exists, but patch types"
            << " do not match.\nCreating a composite name as "
            << allPatchNames[allPatchNames.size() - 1] << endl;

        return allPatchNames.size() - 1;
    }
}


// Get index of zone in new set of zone names
Foam::label Foam::polyMeshAdder::zoneIndex
(
    const word& curName,
    DynamicList<word>& names
)
{
    label zoneI = findIndex(names, curName);

    if (zoneI != -1)
    {
        return zoneI;
    }
    else
    {
        // Not found.  Add new name to the list
        names.append(curName);

        return names.size() - 1;
    }
}


void Foam::polyMeshAdder::mergePatchNames
(
    const polyBoundaryMesh& patches0,
    const polyBoundaryMesh& patches1,

    DynamicList<word>& allPatchNames,
    DynamicList<word>& allPatchTypes,

    labelList& from1ToAllPatches,
    labelList& fromAllTo1Patches
)
{
    // Insert the mesh0 patches and zones
    append(patches0.names(), allPatchNames);
    append(patches0.types(), allPatchTypes);


    // Patches
    // ~~~~~~~
    // Patches from 0 are taken over as is; those from 1 get either merged
    // (if they share name and type) or appended.
    // Empty patches are filtered out much much later on.

    // Add mesh1 patches and build map both ways.
    from1ToAllPatches.setSize(patches1.size());

    forAll(patches1, patchI)
    {
        from1ToAllPatches[patchI] = patchIndex
        (
            patches1[patchI],
            allPatchNames,
            allPatchTypes
        );
    }
    allPatchTypes.shrink();
    allPatchNames.shrink();

    // Invert 1 to all patch map
    fromAllTo1Patches.setSize(allPatchNames.size());
    fromAllTo1Patches = -1;

    forAll(from1ToAllPatches, i)
    {
        fromAllTo1Patches[from1ToAllPatches[i]] = i;
    }
}


Foam::labelList Foam::polyMeshAdder::getPatchStarts
(
    const polyBoundaryMesh& patches
)
{
    labelList patchStarts(patches.size());
    forAll(patches, patchI)
    {
        patchStarts[patchI] = patches[patchI].start();
    }
    return patchStarts;
}


Foam::labelList Foam::polyMeshAdder::getPatchSizes
(
    const polyBoundaryMesh& patches
)
{
    labelList patchSizes(patches.size());
    forAll(patches, patchI)
    {
        patchSizes[patchI] = patches[patchI].size();
    }
    return patchSizes;
}


Foam::List<Foam::polyPatch*> Foam::polyMeshAdder::combinePatches
(
    const polyMesh& mesh0,
    const polyMesh& mesh1,
    const polyBoundaryMesh& allBoundaryMesh,
    const label nAllPatches,
    const labelList& fromAllTo1Patches,

    const label nInternalFaces,
    const labelList& nFaces,

    labelList& from0ToAllPatches,
    labelList& from1ToAllPatches
)
{
    const polyBoundaryMesh& patches0 = mesh0.boundaryMesh();
    const polyBoundaryMesh& patches1 = mesh1.boundaryMesh();


    // Compacted new patch list.
    DynamicList<polyPatch*> allPatches(nAllPatches);


    // Map from 0 to all patches (since gets compacted)
    from0ToAllPatches.setSize(patches0.size());
    from0ToAllPatches = -1;

    label startFaceI = nInternalFaces;

    // Copy patches0 with new sizes. First patches always come from
    // mesh0 and will always be present.
    for (label patchI = 0; patchI < patches0.size(); patchI++)
    {
        // Originates from mesh0. Clone with new size & filter out empty
        // patch.
        label filteredPatchI;

        if (nFaces[patchI] == 0 && isA<processorPolyPatch>(patches0[patchI]))
        {
            Pout<< "Removing zero sized mesh0 patch "
                << patches0[patchI].name() << endl;

            filteredPatchI = -1;
        }
        else
        {
            filteredPatchI = allPatches.size();

            allPatches.append
            (
                patches0[patchI].clone
                (
                    allBoundaryMesh,
                    filteredPatchI,
                    nFaces[patchI],
                    startFaceI
                ).ptr()
            );
            startFaceI += nFaces[patchI];
        }

        // Record new index in allPatches
        from0ToAllPatches[patchI] = filteredPatchI;

        // Check if patch was also in mesh1 and update its addressing if so.
        if (fromAllTo1Patches[patchI] != -1)
        {
            from1ToAllPatches[fromAllTo1Patches[patchI]] = filteredPatchI;
        }
    }

    // Copy unique patches of mesh1.
    forAll(from1ToAllPatches, patchI)
    {
        label allPatchI = from1ToAllPatches[patchI];

        if (allPatchI >= patches0.size())
        {
            // Patch has not been merged with any mesh0 patch.

            label filteredPatchI;

            if
            (
                nFaces[allPatchI] == 0
             && isA<processorPolyPatch>(patches1[patchI])
            )
            {
                Pout<< "Removing zero sized mesh1 patch "
                    << patches1[patchI].name() << endl;

                filteredPatchI = -1;
            }
            else
            {
                filteredPatchI = allPatches.size();

                allPatches.append
                (
                    patches1[patchI].clone
                    (
                        allBoundaryMesh,
                        filteredPatchI,
                        nFaces[allPatchI],
                        startFaceI
                    ).ptr()
                );
                startFaceI += nFaces[allPatchI];
            }

            from1ToAllPatches[patchI] = filteredPatchI;
        }
    }

    allPatches.shrink();

    return allPatches;
}


Foam::labelList Foam::polyMeshAdder::getFaceOrder
(
    const cellList& cells,
    const label nInternalFaces,
    const labelList& owner,
    const labelList& neighbour
)
{
    labelList oldToNew(owner.size(), -1);

    // Leave boundary faces in order
    for (label faceI = nInternalFaces; faceI < owner.size(); faceI++)
    {
        oldToNew[faceI] = faceI;
    }

    // First unassigned face
    label newFaceI = 0;

    forAll(cells, cellI)
    {
        const labelList& cFaces = cells[cellI];

        SortableList<label> nbr(cFaces.size());

        forAll(cFaces, i)
        {
            label faceI = cFaces[i];

            label nbrCellI = neighbour[faceI];

            if (nbrCellI != -1)
            {
                // Internal face. Get cell on other side.
                if (nbrCellI == cellI)
                {
                    nbrCellI = owner[faceI];
                }

                if (cellI < nbrCellI)
                {
                    // CellI is master
                    nbr[i] = nbrCellI;
                }
                else
                {
                    // nbrCell is master. Let it handle this face.
                    nbr[i] = -1;
                }
            }
            else
            {
                // External face. Do later.
                nbr[i] = -1;
            }
        }

        nbr.sort();

        forAll(nbr, i)
        {
            if (nbr[i] != -1)
            {
                oldToNew[cFaces[nbr.indices()[i]]] = newFaceI++;
            }
        }
    }


    // Check done all faces.
    forAll(oldToNew, faceI)
    {
        if (oldToNew[faceI] == -1)
        {
            FatalErrorIn
            (
                "polyMeshAdder::getFaceOrder"
                "(const cellList&, const label, const labelList&"
                ", const labelList&) const"
            )   << "Did not determine new position"
                << " for face " << faceI
                << abort(FatalError);
        }
    }

    return oldToNew;
}


// Adds primitives (cells, faces, points)
// Cells:
//  - all of mesh0
//  - all of mesh1
// Faces:
//  - all uncoupled of mesh0
//  - all coupled faces
//  - all uncoupled of mesh1
// Points:
//  - all coupled
//  - all uncoupled of mesh0
//  - all uncoupled of mesh1
void Foam::polyMeshAdder::mergePrimitives
(
    const polyMesh& mesh0,
    const polyMesh& mesh1,
    const faceCoupleInfo& coupleInfo,

    const label nAllPatches,                // number of patches in the new mesh
    const labelList& fromAllTo1Patches,
    const labelList& from1ToAllPatches,

    pointField& allPoints,
    labelList& from0ToAllPoints,
    labelList& from1ToAllPoints,

    faceList& allFaces,
    labelList& allOwner,
    labelList& allNeighbour,
    label& nInternalFaces,
    labelList& nFacesPerPatch,
    label& nCells,

    labelList& from0ToAllFaces,
    labelList& from1ToAllFaces,
    labelList& from1ToAllCells
)
{
    const polyBoundaryMesh& patches0 = mesh0.boundaryMesh();
    const polyBoundaryMesh& patches1 = mesh1.boundaryMesh();

    const primitiveFacePatch& cutFaces = coupleInfo.cutFaces();
    const indirectPrimitivePatch& masterPatch = coupleInfo.masterPatch();
    const indirectPrimitivePatch& slavePatch = coupleInfo.slavePatch();


    // Points
    // ~~~~~~

    // Storage for new points
    allPoints.setSize(mesh0.nPoints() + mesh1.nPoints());
    label allPointI = 0;

    from0ToAllPoints.setSize(mesh0.nPoints());
    from0ToAllPoints = -1;
    from1ToAllPoints.setSize(mesh1.nPoints());
    from1ToAllPoints = -1;

    // Copy coupled points (on cut)
    {
        const pointField& cutPoints = coupleInfo.cutPoints();
        const labelList& cutToMasterPoints = coupleInfo.cutToMasterPoints();
        const labelList& cutToSlavePoints = coupleInfo.cutToSlavePoints();

        forAll(cutPoints, i)
        {
            allPoints[allPointI] = cutPoints[i];

            if (cutToMasterPoints[i] != -1)
            {
                // There is corresponding master point. Update mapping.
                label masterPointI = cutToMasterPoints[i];
                label mesh0PointI = masterPatch.meshPoints()[masterPointI];
                from0ToAllPoints[mesh0PointI] = allPointI;
            }

            if (cutToSlavePoints[i] != -1)
            {
                // There is corresponding slave point. Update mapping.
                label slavePointI = cutToSlavePoints[i];
                label mesh1PointI = slavePatch.meshPoints()[slavePointI];
                from1ToAllPoints[mesh1PointI] = allPointI;
            }
            allPointI++;
        }
    }

    // Copy shared points which are not on cut.
    forAll(coupleInfo.sharedToMasterPoints(), i)
    {
        label masterPointI = coupleInfo.sharedToMasterPoints()[i];
        label slavePointI = coupleInfo.sharedToSlavePoints()[i];

        if (from0ToAllPoints[masterPointI] != -1)
        {
            FatalErrorIn
            (
                "polyMeshAdder::mergePrimitives(..)"
            )   << "Mesh0 point:" << masterPointI
                << " coord:" << mesh0.points()[masterPointI]
                << " which is matched to non-cut mesh1 point:" << slavePointI
                << " coord:" << mesh1.points()[slavePointI]
                << endl
                << " already seems to be in the merged mesh as:"
                << from0ToAllPoints[masterPointI]
                << " coord:" << allPoints[from0ToAllPoints[masterPointI]]
                << abort(FatalError);
        }
        if (from1ToAllPoints[slavePointI] != -1)
        {
            FatalErrorIn
            (
                "polyMeshAdder::mergePrimitives(..)"
            )   << "Mesh1 point:" << slavePointI
                << " coord:" << mesh1.points()[slavePointI]
                << " which is matched to non-cut mesh0 point:" << masterPointI
                << " coord:" << mesh0.points()[masterPointI]
                << endl
                << " already seems to be in the merged mesh as:"
                << from1ToAllPoints[slavePointI]
                << " coord:" << allPoints[from1ToAllPoints[slavePointI]]
                << abort(FatalError);
        }


        allPoints[allPointI] = mesh0.points()[masterPointI];
        from0ToAllPoints[masterPointI] = allPointI;
        from1ToAllPoints[slavePointI] = allPointI;

        allPointI++;
    }

    // Add uncoupled mesh0 points
    forAll(mesh0.points(), pointI)
    {
        if (from0ToAllPoints[pointI] == -1)
        {
            allPoints[allPointI] = mesh0.points()[pointI];
            from0ToAllPoints[pointI] = allPointI;
            allPointI++;
        }
    }

    // Add uncoupled mesh1 points
    forAll(mesh1.points(), pointI)
    {
        if (from1ToAllPoints[pointI] == -1)
        {
            allPoints[allPointI] = mesh1.points()[pointI];
            from1ToAllPoints[pointI] = allPointI;
            allPointI++;
        }
    }

    allPoints.setSize(allPointI);


    // Faces
    // ~~~~~

    // Sizes per patch
    nFacesPerPatch.setSize(nAllPatches);
    nFacesPerPatch = 0;

    // Storage for faces and owner/neighbour
    allFaces.setSize(mesh0.nFaces() + mesh1.nFaces());
    allOwner.setSize(allFaces.size());
    allOwner = -1;
    allNeighbour.setSize(allFaces.size());
    allNeighbour = -1;
    label allFaceI = 0;

    from0ToAllFaces.setSize(mesh0.nFaces());
    from0ToAllFaces = -1;
    from1ToAllFaces.setSize(mesh1.nFaces());
    from1ToAllFaces = -1;

    // Copy mesh0 internal faces (always uncoupled)
    for (label faceI = 0; faceI < mesh0.nInternalFaces(); faceI++)
    {
        allFaces[allFaceI] = renumber(from0ToAllPoints, mesh0.faces()[faceI]);
        allOwner[allFaceI] = mesh0.faceOwner()[faceI];
        allNeighbour[allFaceI] = mesh0.faceNeighbour()[faceI];
        from0ToAllFaces[faceI] = allFaceI++;
    }

    // Copy coupled faces. Every coupled face has an equivalent master and
    // slave. Also uncount as boundary faces all the newly coupled faces.
    const labelList& cutToMasterFaces = coupleInfo.cutToMasterFaces();
    const labelList& cutToSlaveFaces = coupleInfo.cutToSlaveFaces();

    forAll(cutFaces, i)
    {
        label masterFaceI = cutToMasterFaces[i];

        label mesh0FaceI = masterPatch.addressing()[masterFaceI];

        label slaveFaceI = cutToSlaveFaces[i];

        label mesh1FaceI = slavePatch.addressing()[slaveFaceI];

        if (from0ToAllFaces[mesh0FaceI] == -1)
        {
            // First occurrence of face
            from0ToAllFaces[mesh0FaceI] = allFaceI;

            // External face becomes internal so uncount
            label patch0 = patches0.whichPatch(mesh0FaceI);
            nFacesPerPatch[patch0]--;
        }
        if (from1ToAllFaces[mesh1FaceI] == -1)
        {
            from1ToAllFaces[mesh1FaceI] = allFaceI;

            label patch1 = patches1.whichPatch(mesh1FaceI);
            nFacesPerPatch[from1ToAllPatches[patch1]]--;
        }

        // Copy cut face (since cutPoints are copied first no renumbering
        // nessecary)
        allFaces[allFaceI] = cutFaces[i];
        allOwner[allFaceI] = mesh0.faceOwner()[mesh0FaceI];
        allNeighbour[allFaceI] = mesh1.faceOwner()[mesh1FaceI] + mesh0.nCells();

        allFaceI++;
    }

    // Copy mesh1 internal faces (always uncoupled)
    for (label faceI = 0; faceI < mesh1.nInternalFaces(); faceI++)
    {
        allFaces[allFaceI] = renumber(from1ToAllPoints, mesh1.faces()[faceI]);
        allOwner[allFaceI] = mesh1.faceOwner()[faceI] + mesh0.nCells();
        allNeighbour[allFaceI] = mesh1.faceNeighbour()[faceI] + mesh0.nCells();
        from1ToAllFaces[faceI] = allFaceI++;
    }

    nInternalFaces = allFaceI;

    // Copy (unmarked/uncoupled) external faces in new order.
    for (label allPatchI = 0; allPatchI < nAllPatches; allPatchI++)
    {
        if (allPatchI < patches0.size())
        {
            // Patch is present in mesh0
            const polyPatch& pp = patches0[allPatchI];

            nFacesPerPatch[allPatchI] += pp.size();

            label faceI = pp.start();

            forAll(pp, i)
            {
                if (from0ToAllFaces[faceI] == -1)
                {
                    // Is uncoupled face since has not yet been dealt with
                    allFaces[allFaceI] = renumber
                    (
                        from0ToAllPoints,
                        mesh0.faces()[faceI]
                    );
                    allOwner[allFaceI] = mesh0.faceOwner()[faceI];
                    allNeighbour[allFaceI] = -1;

                    from0ToAllFaces[faceI] = allFaceI++;
                }
                faceI++;
            }
        }
        if (fromAllTo1Patches[allPatchI] != -1)
        {
            // Patch is present in mesh1
            const polyPatch& pp = patches1[fromAllTo1Patches[allPatchI]];

            nFacesPerPatch[allPatchI] += pp.size();

            label faceI = pp.start();

            forAll(pp, i)
            {
                if (from1ToAllFaces[faceI] == -1)
                {
                    // Is uncoupled face
                    allFaces[allFaceI] = renumber
                    (
                        from1ToAllPoints,
                        mesh1.faces()[faceI]
                    );
                    allOwner[allFaceI] =
                        mesh1.faceOwner()[faceI]
                      + mesh0.nCells();
                    allNeighbour[allFaceI] = -1;

                    from1ToAllFaces[faceI] = allFaceI++;
                }
                faceI++;
            }
        }
    }
    allFaces.setSize(allFaceI);
    allOwner.setSize(allFaceI);
    allNeighbour.setSize(allFaceI);


    // Correct all edgeFaces connected to masterPatch or slavePatch for split
    // edges. (All point-but-not-edge connected faces should be ok already)
    {
        const labelList& cutToMasterPoints = coupleInfo.cutToMasterPoints();
        const labelList& cutToMasterEdges = coupleInfo.cutToMasterEdges();
        const labelList& cutToSlavePoints = coupleInfo.cutToSlavePoints();
        const edgeList& cutEdges = cutFaces.edges();

        // Build map from cutPoints to allPoints (requires all cut points
        // to be either in master or slave patch)
        labelList fromCutToAllPoints(cutToMasterPoints.size(), -1);

        const labelList& mmp = masterPatch.meshPoints();
        const labelList& smp = slavePatch.meshPoints();

        forAll(cutToMasterPoints, cutPointI)
        {
            label masterPointI = cutToMasterPoints[cutPointI];
            label slavePointI = cutToSlavePoints[cutPointI];

            if (masterPointI != -1)
            {
                fromCutToAllPoints[cutPointI] =
                    from0ToAllPoints[mmp[masterPointI]];
            }
            else if (slavePointI != -1)
            {
                fromCutToAllPoints[cutPointI] =
                    from1ToAllPoints[smp[slavePointI]];
            }
            else
            {
                FatalErrorIn("polyMeshAdder")
                    << "cutPoint not from master nor slave"
                    << abort(FatalError);
            }
        }


        // Build table from all edge to list of added points.

        HashTable<labelList, edge, Hash<edge> > allEdgeToAddedPointsMap
        (
            masterPatch.nEdges()
        );


        // Add master points
        // ~~~~~~~~~~~~~~~~~

        // Get list of cut edges per master edge.
        labelListList masterToCutEdges
        (
            invertOneToMany
            (
                masterPatch.nEdges(),
                cutToMasterEdges
            )
        );


        forAll(masterToCutEdges, masterEdgeI)
        {
            const edge& masterEdge = masterPatch.edges()[masterEdgeI];

            // Edge in allPoints
            const edge allEdge
            (
                from0ToAllPoints[mmp[masterEdge[0]]],
                from0ToAllPoints[mmp[masterEdge[1]]]
            );

            // Get the string of vertices (in allPoint addressing) in
            // the same order as allEdge (so from allEdge[0] to
            // allEdge[1])
            labelList sortedVertices
            (
                stringEdges
                (
                    cutFaces.localPoints(),
                    allEdge,
                    allPoints,
                    fromCutToAllPoints,
                    cutEdges,
                    masterToCutEdges[masterEdgeI]
                )
            );

            if (sortedVertices.size() > 0)
            {
                // Store the vertices as a map of allPoints.
                allEdgeToAddedPointsMap.insert(allEdge, sortedVertices);
            }
        }


        // Add slave points
        // ~~~~~~~~~~~~~~~~~

        // Get list of cut edges per slave edge.
        const labelList& cutToSlaveEdges = coupleInfo.cutToSlaveEdges();
        labelListList slaveToCutEdges
        (
            invertOneToMany
            (
                slavePatch.nEdges(),
                cutToSlaveEdges
            )
        );

        forAll(slaveToCutEdges, slaveEdgeI)
        {
            const edge& slaveEdge = slavePatch.edges()[slaveEdgeI];

            // Edge in allPoints
            const edge allEdge
            (
                from1ToAllPoints[smp[slaveEdge[0]]],
                from1ToAllPoints[smp[slaveEdge[1]]]
            );

            // Get the string of vertices (in allPoint addressing) in
            // the same order as allEdge (so from allEdge[0] to
            // allEdge[1])
            labelList sortedVertices
            (
                stringEdges
                (
                    cutFaces.localPoints(),
                    allEdge,
                    allPoints,
                    fromCutToAllPoints,
                    cutEdges,
                    slaveToCutEdges[slaveEdgeI]
                )
            );

            if (sortedVertices.size() > 0)
            {
                // Store the vertices as a map of allPoints.
                allEdgeToAddedPointsMap.insert(allEdge, sortedVertices);
            }
        }


        // Redo allFaces
        // ~~~~~~~~~~~~~
        // Since we do not want to build full edge addressing just loop over all
        // faces.
        forAll(allFaces, allFaceI)
        {
            const face& f = allFaces[allFaceI];

            DynamicList<label> newFace(f.size());

            forAll(f, fp)
            {
                label v0 = f[fp];
                label v1 = f.nextLabel(fp);

                newFace.append(v0);

                HashTable<labelList, edge, Hash<edge> >::const_iterator iter =
                    allEdgeToAddedPointsMap.find(edge(v0, v1));

                if (iter != allEdgeToAddedPointsMap.end())
                {
                    const edge& e = iter.key();
                    const labelList& addedPoints = iter();

                    if (e[0] == v0)
                    {
                        forAll(addedPoints, i)
                        {
                            newFace.append(addedPoints[i]);
                        }
                    }
                    else
                    {
                        forAllReverse(addedPoints, i)
                        {
                            newFace.append(addedPoints[i]);
                        }
                    }
                }
            }

            if (newFace.size() != f.size())
            {
                newFace.shrink();

                //Pout<< "Modified face " << f << " to " << newFace << endl;
                allFaces[allFaceI].transfer(newFace);
                newFace.clear();
            }
        }
    }

    // Now we have a full facelist and owner/neighbour addressing.


    // Cells
    // ~~~~~

    from1ToAllCells.setSize(mesh1.nCells());
    from1ToAllCells = -1;

    forAll(mesh1.cells(), i)
    {
        from1ToAllCells[i] = i + mesh0.nCells();
    }

    // Make cells (= cell-face addressing)
    nCells = mesh0.nCells() + mesh1.nCells();
    cellList allCells(nCells);
    primitiveMesh::calcCells(allCells, allOwner, allNeighbour, nCells);

    // Reorder faces for upper-triangular order.
    labelList oldToNew
    (
        getFaceOrder
        (
            allCells,
            nInternalFaces,
            allOwner,
            allNeighbour
        )
    );

    inplaceReorder(oldToNew, allFaces);
    inplaceReorder(oldToNew, allOwner);
    inplaceReorder(oldToNew, allNeighbour);
    inplaceRenumber(oldToNew, from0ToAllFaces);
    inplaceRenumber(oldToNew, from1ToAllFaces);
}


void Foam::polyMeshAdder::mergePointZones
(
    const pointZoneMesh& pz0,
    const pointZoneMesh& pz1,
    const labelList& from0ToAllPoints,
    const labelList& from1ToAllPoints,

    DynamicList<word>& zoneNames,
    labelList& from1ToAll,
    List<DynamicList<label> >& pzPoints
)
{
    zoneNames.setSize(pz0.size() + pz1.size());

    // Names
    append(pz0.names(), zoneNames);

    from1ToAll.setSize(pz1.size());

    forAll (pz1, zoneI)
    {
        from1ToAll[zoneI] = zoneIndex(pz1[zoneI].name(), zoneNames);
    }
    zoneNames.shrink();

    // Point labels per merged zone
    pzPoints.setSize(zoneNames.size());

    forAll(pz0, zoneI)
    {
        DynamicList<label>& newZone = pzPoints[zoneI];

        newZone.setSize(pz0[zoneI].size());

        append(from0ToAllPoints, pz0[zoneI], newZone);
    }

    // Now we have full addressing for points so do the pointZones of mesh1.
    forAll(pz1, zoneI)
    {
        // Relabel all points of zone and add to correct pzPoints.
        DynamicList<label>& newZone = pzPoints[from1ToAll[zoneI]];

        newZone.setSize(newZone.size() + pz1[zoneI].size());

        append(from1ToAllPoints, pz1[zoneI], newZone);
    }

    forAll(pzPoints, i)
    {
        pzPoints[i].shrink();
    }
}


void Foam::polyMeshAdder::mergeFaceZones
(
    const faceZoneMesh& fz0,
    const faceZoneMesh& fz1,
    const labelList& from0ToAllFaces,
    const labelList& from1ToAllFaces,

    DynamicList<word>& zoneNames,
    labelList& from1ToAll,
    List<DynamicList<label> >& fzFaces,
    List<DynamicList<bool> >& fzFlips
)
{
    zoneNames.setSize(fz0.size() + fz1.size());
    
    append(fz0.names(), zoneNames);

    from1ToAll.setSize(fz1.size());

    forAll (fz1, zoneI)
    {
        from1ToAll[zoneI] = zoneIndex(fz1[zoneI].name(), zoneNames);
    }
    zoneNames.shrink();


    // Create temporary lists for faceZones.
    fzFaces.setSize(zoneNames.size());
    fzFlips.setSize(zoneNames.size());
    forAll(fz0, zoneI)
    {
        DynamicList<label>& newZone = fzFaces[zoneI];
        DynamicList<bool>& newFlip = fzFlips[zoneI];

        newZone.setSize(fz0[zoneI].size());
        newFlip.setSize(newZone.size());

        const labelList& addressing = fz0[zoneI];
        const boolList& flipMap = fz0[zoneI].flipMap();

        forAll(addressing, i)
        {
            label faceI = addressing[i];

            if (from0ToAllFaces[faceI] != -1)
            {
                newZone.append(from0ToAllFaces[faceI]);
                newFlip.append(flipMap[i]);
            }
        }
    }

    // Now we have full addressing for faces so do the faceZones of mesh1.
    forAll(fz1, zoneI)
    {
        DynamicList<label>& newZone = fzFaces[from1ToAll[zoneI]];
        DynamicList<bool>& newFlip = fzFlips[from1ToAll[zoneI]];

        newZone.setSize(newZone.size() + fz1[zoneI].size());
        newFlip.setSize(newZone.size());

        const labelList& addressing = fz1[zoneI];
        const boolList& flipMap = fz1[zoneI].flipMap();

        forAll(addressing, i)
        {
            label faceI = addressing[i];

            if (from1ToAllFaces[faceI] != -1)
            {
                newZone.append(from1ToAllFaces[faceI]);
                newFlip.append(flipMap[i]);
            }
        }
    }

    forAll(fzFaces, i)
    {
        fzFaces[i].shrink();
        fzFlips[i].shrink();
    }
}


void Foam::polyMeshAdder::mergeCellZones
(
    const cellZoneMesh& cz0,
    const cellZoneMesh& cz1,
    const labelList& from1ToAllCells,

    DynamicList<word>& zoneNames,
    labelList& from1ToAll,
    List<DynamicList<label> >& czCells
)
{
    zoneNames.setSize(cz0.size() + cz1.size());
    
    append(cz0.names(), zoneNames);

    from1ToAll.setSize(cz1.size());
    forAll (cz1, zoneI)
    {
        from1ToAll[zoneI] = zoneIndex(cz1[zoneI].name(), zoneNames);
    }
    zoneNames.shrink();


    // Create temporary lists for cellZones.
    czCells.setSize(zoneNames.size());
    forAll(cz0, zoneI)
    {
        czCells[zoneI].setSize(cz0[zoneI].size());
        // Insert mesh0 cells
        append(cz0[zoneI], czCells[zoneI]);
    }


    // Cell mapping is trivial.
    forAll(cz1, zoneI)
    {
        DynamicList<label>& newZone = czCells[from1ToAll[zoneI]];

        newZone.setSize(newZone.size() + cz1[zoneI].size());

        append(from1ToAllCells, cz1[zoneI], newZone);
    }

    forAll(czCells, i)
    {
        czCells[i].shrink();
    }
}


void Foam::polyMeshAdder::mergeZones
(
    const polyMesh& mesh0,
    const polyMesh& mesh1,
    const labelList& from0ToAllPoints,
    const labelList& from0ToAllFaces,

    const labelList& from1ToAllPoints,
    const labelList& from1ToAllFaces,
    const labelList& from1ToAllCells,

    DynamicList<word>& pointZoneNames,
    List<DynamicList<label> >& pzPoints,

    DynamicList<word>& faceZoneNames,
    List<DynamicList<label> >& fzFaces,
    List<DynamicList<bool> >& fzFlips,

    DynamicList<word>& cellZoneNames,
    List<DynamicList<label> >& czCells
)
{
    labelList from1ToAllPZones;
    mergePointZones
    (
        mesh0.pointZones(),
        mesh1.pointZones(),
        from0ToAllPoints,
        from1ToAllPoints,

        pointZoneNames,
        from1ToAllPZones,
        pzPoints
    );

    labelList from1ToAllFZones;
    mergeFaceZones
    (
        mesh0.faceZones(),
        mesh1.faceZones(),
        from0ToAllFaces,
        from1ToAllFaces,

        faceZoneNames,
        from1ToAllFZones,
        fzFaces,
        fzFlips
    );

    labelList from1ToAllCZones;
    mergeCellZones
    (
        mesh0.cellZones(),
        mesh1.cellZones(),
        from1ToAllCells,

        cellZoneNames,
        from1ToAllCZones,
        czCells
    );
}


void Foam::polyMeshAdder::addZones
(
    const DynamicList<word>& pointZoneNames,
    const List<DynamicList<label> >& pzPoints,

    const DynamicList<word>& faceZoneNames,
    const List<DynamicList<label> >& fzFaces,
    const List<DynamicList<bool> >& fzFlips,

    const DynamicList<word>& cellZoneNames,
    const List<DynamicList<label> >& czCells,

    polyMesh& mesh
)
{
    List<pointZone*> pZones(pzPoints.size());
    forAll(pZones, i)
    {
        pZones[i] = new pointZone
        (
            pointZoneNames[i],
            pzPoints[i],
            i,
            mesh.pointZones()
        );
    }
    
    List<faceZone*> fZones(fzFaces.size());
    forAll(fZones, i)
    {
        fZones[i] = new faceZone
        (
            faceZoneNames[i],
            fzFaces[i],
            fzFlips[i],
            i,
            mesh.faceZones()
        );
    }

    List<cellZone*> cZones(czCells.size());
    forAll(cZones, i)
    {
        cZones[i] = new cellZone
        (
            cellZoneNames[i],
            czCells[i],
            i,
            mesh.cellZones()
        );
    }

    mesh.addZones
    (
        pZones,
        fZones,
        cZones
    );
}



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Returns new mesh and sets
// - map from new cell/face/point/patch to either mesh0 or mesh1
//
// mesh0Faces/mesh1Faces: corresponding faces on both meshes.
Foam::autoPtr<Foam::polyMesh> Foam::polyMeshAdder::add
(
    const IOobject& io,
    const polyMesh& mesh0,
    const polyMesh& mesh1,
    const faceCoupleInfo& coupleInfo,
    autoPtr<mapAddedPolyMesh>& mapPtr
)
{
    const polyBoundaryMesh& patches0 = mesh0.boundaryMesh();
    const polyBoundaryMesh& patches1 = mesh1.boundaryMesh();


    DynamicList<word> allPatchNames(patches0.size() + patches1.size());
    DynamicList<word> allPatchTypes(allPatchNames.size());

    // Patch maps
    labelList from1ToAllPatches(patches1.size());
    labelList fromAllTo1Patches(allPatchNames.size(), -1);

    mergePatchNames
    (
        patches0,
        patches1,
        allPatchNames,
        allPatchTypes,
        from1ToAllPatches,
        fromAllTo1Patches
    );


    // New points
    pointField allPoints;

    // Map from mesh0/1 points to allPoints.
    labelList from0ToAllPoints(mesh0.nPoints(), -1);
    labelList from1ToAllPoints(mesh1.nPoints(), -1);

    // New faces
    faceList allFaces;
    label nInternalFaces;

    // New cells
    labelList allOwner;
    labelList allNeighbour;
    label nCells;

    // Sizes per patch
    labelList nFaces(allPatchNames.size(), 0);

    // Maps
    labelList from0ToAllFaces(mesh0.nFaces(), -1);
    labelList from1ToAllFaces(mesh1.nFaces(), -1);

    // Map
    labelList from1ToAllCells(mesh1.nCells(), -1);

    mergePrimitives
    (
        mesh0,
        mesh1,
        coupleInfo,

        allPatchNames.size(),
        fromAllTo1Patches,
        from1ToAllPatches,

        allPoints,
        from0ToAllPoints,
        from1ToAllPoints,

        allFaces,
        allOwner,
        allNeighbour,
        nInternalFaces,
        nFaces,
        nCells,

        from0ToAllFaces,
        from1ToAllFaces,
        from1ToAllCells
    );


    // Zones
    // ~~~~~

    DynamicList<word> pointZoneNames;
    List<DynamicList<label> > pzPoints;

    DynamicList<word> faceZoneNames;
    List<DynamicList<label> > fzFaces;
    List<DynamicList<bool> > fzFlips;

    DynamicList<word> cellZoneNames;
    List<DynamicList<label> > czCells;

    mergeZones
    (
        mesh0,
        mesh1,

        from0ToAllPoints,
        from0ToAllFaces,

        from1ToAllPoints,
        from1ToAllFaces,
        from1ToAllCells,

        pointZoneNames,
        pzPoints,

        faceZoneNames,
        fzFaces,
        fzFlips,

        cellZoneNames,
        czCells
    );


    // Patches
    // ~~~~~~~

    // Map from 0 to all patches (since gets compacted)
    labelList from0ToAllPatches(patches0.size(), -1);

    List<polyPatch*> allPatches
    (
        combinePatches
        (
            mesh0,
            mesh1,
            patches0,           // Should be boundaryMesh() on new mesh.
            allPatchNames.size(),
            fromAllTo1Patches,
            mesh0.nInternalFaces()
          + mesh1.nInternalFaces()
          + coupleInfo.cutFaces().size(),
            nFaces,

            from0ToAllPatches,
            from1ToAllPatches
        )
    );


    // Map information
    // ~~~~~~~~~~~~~~~

    mapPtr.reset
    (
        new mapAddedPolyMesh
        (
            mesh0.nPoints(),
            mesh0.nFaces(),
            mesh0.nCells(),

            mesh1.nPoints(),
            mesh1.nFaces(),
            mesh1.nCells(),

            from0ToAllPoints,
            from0ToAllFaces,
            identity(mesh0.nCells()),

            from1ToAllPoints,
            from1ToAllFaces,
            from1ToAllCells,

            from0ToAllPatches,
            from1ToAllPatches,
            getPatchSizes(patches0),
            getPatchStarts(patches0)
        )
    );



    // Now we have extracted all information from all meshes.
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Construct mesh
    autoPtr<polyMesh> tmesh
    (
        new polyMesh
        (
            io,
            allPoints,
            allFaces,
            allOwner,
            allNeighbour
        )
    );
    polyMesh& mesh = tmesh();

    // Add zones to new mesh.
    addZones
    (
        pointZoneNames,
        pzPoints,

        faceZoneNames,
        fzFaces,
        fzFlips,

        cellZoneNames,
        czCells,
        mesh
    );

    // Add patches to new mesh
    mesh.addPatches(allPatches);

    return tmesh;
}


// Inplace add mesh1 to mesh0
Foam::autoPtr<Foam::mapAddedPolyMesh> Foam::polyMeshAdder::add
(
    polyMesh& mesh0,
    const polyMesh& mesh1,
    const faceCoupleInfo& coupleInfo,
    const bool validBoundary
)
{
    const polyBoundaryMesh& patches0 = mesh0.boundaryMesh();
    const polyBoundaryMesh& patches1 = mesh1.boundaryMesh();

    DynamicList<word> allPatchNames(patches0.size() + patches1.size());
    DynamicList<word> allPatchTypes(allPatchNames.size());

    // Patch maps
    labelList from1ToAllPatches(patches1.size());
    labelList fromAllTo1Patches(allPatchNames.size(), -1);

    mergePatchNames
    (
        patches0,
        patches1,
        allPatchNames,
        allPatchTypes,
        from1ToAllPatches,
        fromAllTo1Patches
    );


    // New points
    pointField allPoints;

    // Map from mesh0/1 points to allPoints.
    labelList from0ToAllPoints(mesh0.nPoints(), -1);
    labelList from1ToAllPoints(mesh1.nPoints(), -1);

    // New faces
    faceList allFaces;
    labelList allOwner;
    labelList allNeighbour;
    label nInternalFaces;
    // Sizes per patch
    labelList nFaces(allPatchNames.size(), 0);
    label nCells;

    // Maps
    labelList from0ToAllFaces(mesh0.nFaces(), -1);
    labelList from1ToAllFaces(mesh1.nFaces(), -1);
    // Map
    labelList from1ToAllCells(mesh1.nCells(), -1);

    mergePrimitives
    (
        mesh0,
        mesh1,
        coupleInfo,

        allPatchNames.size(),
        fromAllTo1Patches,
        from1ToAllPatches,

        allPoints,
        from0ToAllPoints,
        from1ToAllPoints,

        allFaces,
        allOwner,
        allNeighbour,
        nInternalFaces,
        nFaces,
        nCells,

        from0ToAllFaces,
        from1ToAllFaces,
        from1ToAllCells
    );


    // Zones
    // ~~~~~

    DynamicList<word> pointZoneNames;
    List<DynamicList<label> > pzPoints;

    DynamicList<word> faceZoneNames;
    List<DynamicList<label> > fzFaces;
    List<DynamicList<bool> > fzFlips;

    DynamicList<word> cellZoneNames;
    List<DynamicList<label> > czCells;

    mergeZones
    (
        mesh0,
        mesh1,

        from0ToAllPoints,
        from0ToAllFaces,

        from1ToAllPoints,
        from1ToAllFaces,
        from1ToAllCells,

        pointZoneNames,
        pzPoints,

        faceZoneNames,
        fzFaces,
        fzFlips,

        cellZoneNames,
        czCells
    );


    // Patches
    // ~~~~~~~


    // Store mesh0 patch info before modifying patches0.
    labelList mesh0PatchSizes(getPatchSizes(patches0));
    labelList mesh0PatchStarts(getPatchStarts(patches0));

    // Map from 0 to all patches (since gets compacted)
    labelList from0ToAllPatches(patches0.size(), -1);

    // Inplace extend mesh0 patches (note that patches0.size() now also
    // has changed)
    polyBoundaryMesh& allPatches = 
        const_cast<polyBoundaryMesh&>(mesh0.boundaryMesh());
    allPatches.setSize(allPatchNames.size());

    label startFaceI = nInternalFaces;

    // Copy patches0 with new sizes. First patches always come from
    // mesh0 and will always be present.
    label allPatchI = 0;

    forAll(from0ToAllPatches, patch0)
    {
        // Originates from mesh0. Clone with new size & filter out empty
        // patch.

        if (nFaces[patch0] == 0 && isA<processorPolyPatch>(allPatches[patch0]))
        {
            Pout<< "Removing zero sized mesh0 patch " << allPatchNames[patch0]
                << endl;

            from0ToAllPatches[patch0] = -1;
            // Check if patch was also in mesh1 and update its addressing if so.
            if (fromAllTo1Patches[patch0] != -1)
            {
                from1ToAllPatches[fromAllTo1Patches[patch0]] = -1;
            }
        }
        else
        {
            // Clone.
            allPatches.set
            (
                allPatchI,
                allPatches[patch0].clone
                (
                    allPatches,
                    allPatchI,
                    nFaces[patch0],
                    startFaceI
                )
            );

            // Record new index in allPatches
            from0ToAllPatches[patch0] = allPatchI;

            // Check if patch was also in mesh1 and update its addressing if so.
            if (fromAllTo1Patches[patch0] != -1)
            {
                from1ToAllPatches[fromAllTo1Patches[patch0]] = allPatchI;
            }

            startFaceI += nFaces[patch0];

            allPatchI++;
        }
    }

    // Copy unique patches of mesh1.
    forAll(from1ToAllPatches, patch1)
    {
        label uncompactAllPatchI = from1ToAllPatches[patch1];

        if (uncompactAllPatchI >= from0ToAllPatches.size())
        {
            // Patch has not been merged with any mesh0 patch.

            if
            (
                nFaces[uncompactAllPatchI] == 0
             && isA<processorPolyPatch>(patches1[patch1])
            )
            {
                Pout<< "Removing zero sized mesh1 patch "
                    << allPatchNames[uncompactAllPatchI] << endl;

                from1ToAllPatches[patch1] = -1;
            }
            else
            {
                // Clone.
                allPatches.set
                (
                    allPatchI,
                    patches1[patch1].clone
                    (
                        allPatches,
                        allPatchI,
                        nFaces[uncompactAllPatchI],
                        startFaceI
                    )
                );

                // Record new index in allPatches
                from1ToAllPatches[patch1] = allPatchI;

                startFaceI += nFaces[uncompactAllPatchI];

                allPatchI++;
            }
        }
    }

    allPatches.setSize(allPatchI);


    // Construct map information before changing mesh0 primitives
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    autoPtr<mapAddedPolyMesh> mapPtr
    (
        new mapAddedPolyMesh
        (
            mesh0.nPoints(),
            mesh0.nFaces(),
            mesh0.nCells(),

            mesh1.nPoints(),
            mesh1.nFaces(),
            mesh1.nCells(),

            from0ToAllPoints,
            from0ToAllFaces,
            identity(mesh0.nCells()),

            from1ToAllPoints,
            from1ToAllFaces,
            from1ToAllCells,

            from0ToAllPatches,
            from1ToAllPatches,

            mesh0PatchSizes,
            mesh0PatchStarts
        )
    );



    // Now we have extracted all information from all meshes.
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    labelList patchSizes(getPatchSizes(allPatches));
    labelList patchStarts(getPatchStarts(allPatches));

    mesh0.resetPrimitives
    (
        allFaces.size(),
        allPoints,
        allFaces,
        allOwner,
        allNeighbour,
        patchSizes,     // size
        patchStarts,    // patchstarts
        validBoundary   // boundary valid?
    );

    // Add zones to new mesh.
    addZones
    (
        pointZoneNames,
        pzPoints,

        faceZoneNames,
        fzFaces,
        fzFlips,

        cellZoneNames,
        czCells,
        mesh0
    );

    return mapPtr;
}


// ************************************************************************* //
