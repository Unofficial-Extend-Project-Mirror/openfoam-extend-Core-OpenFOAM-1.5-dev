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

\*----------------------------------------------------------------------------*/

#include "faceCoupleInfo.H"
#include "polyMesh.H"
#include "matchPoints.H"
#include "indirectPrimitivePatch.H"
#include "meshTools.H"
#include "octreeDataFace.H"
#include "octree.H"
#include "OFstream.H"
#include "SortableList.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(faceCoupleInfo, 0);

}

const Foam::scalar Foam::faceCoupleInfo::angleTol_ = 1E-3;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

//- Write edges
void Foam::faceCoupleInfo::writeOBJ
(
    const fileName& fName,
    const edgeList& edges,
    const pointField& points,
    const bool compact
)
{
    OFstream str(fName);

    labelList pointMap(points.size(), -1);

    if (compact)
    {
        label newPointI = 0;

        forAll(edges, edgeI)
        {
            const edge& e = edges[edgeI];

            forAll(e, eI)
            {
                label pointI = e[eI];

                if (pointMap[pointI] == -1)
                {
                    pointMap[pointI] = newPointI++;

                    meshTools::writeOBJ(str, points[pointI]);
                }
            }
        }
    }
    else
    {
        forAll(points, pointI)
        {
            meshTools::writeOBJ(str, points[pointI]);
        }

        pointMap = identity(points.size());
    }

    forAll(edges, edgeI)
    {
        const edge& e = edges[edgeI];

        str<< "l " << pointMap[e[0]]+1 << ' ' << pointMap[e[1]]+1 << nl;
    }
}


//- Writes edges.
void Foam::faceCoupleInfo::writeOBJ
(
    const fileName& fName,
    const pointField& points0,
    const pointField& points1
)
{
    Pout<< "Writing connections as edges to " << fName << endl;

    OFstream str(fName);

    label vertI = 0;

    forAll(points0, i)
    {
        meshTools::writeOBJ(str, points0[i]);
        vertI++;
        meshTools::writeOBJ(str, points1[i]);
        vertI++;
        str << "l " << vertI-1 << ' ' << vertI << nl;
    }
}


//- Writes face and point connectivity as .obj files.
void Foam::faceCoupleInfo::writePointsFaces() const
{
    const indirectPrimitivePatch& m = masterPatch();
    const indirectPrimitivePatch& s = slavePatch();
    const primitiveFacePatch& c = cutFaces();

    // Patches
    {
        OFstream str("masterPatch.obj");
        Pout<< "Writing masterPatch to " << str.name() << endl;
        meshTools::writeOBJ(str, m.localFaces(), m.localPoints());
    }
    {
        OFstream str("slavePatch.obj");
        Pout<< "Writing slavePatch to " << str.name() << endl;
        meshTools::writeOBJ(str, s.localFaces(), s.localPoints());
    }
    {
        OFstream str("cutFaces.obj");
        Pout<< "Writing cutFaces to " << str.name() << endl;
        meshTools::writeOBJ(str, c.localFaces(), c.localPoints());
    }

    // Point connectivity
    {
        Pout<< "Writing cutToMasterPoints to cutToMasterPoints.obj" << endl;

        labelList masterToCutPoints(invert(m.nPoints(), cutToMasterPoints()));

        writeOBJ
        (
            "cutToMasterPoints.obj",
            m.localPoints(),
            pointField
            (
                IndirectList<point>(c.localPoints(), masterToCutPoints)()
            )
        );
    }
    {
        Pout<< "Writing cutToSlavePoints to cutToSlavePoints.obj" << endl;

        labelList slaveToCutPoints(invert(s.nPoints(), cutToSlavePoints()));

        writeOBJ
        (
            "cutToSlavePoints.obj",
            s.localPoints(),
            pointField
            (
                IndirectList<point>(c.localPoints(), slaveToCutPoints)()
            )
        );
    }

    // Face connectivity
    {
        Pout<< "Writing cutToMasterFaces to cutToMasterFaces.obj" << endl;

        pointField equivMasterFaces(c.size());

        forAll(cutToMasterFaces(), cutFaceI)
        {
            label masterFaceI = cutToMasterFaces()[cutFaceI];

            if (masterFaceI != -1)
            {
                equivMasterFaces[cutFaceI] = m[masterFaceI].centre(m.points());
            }
            else
            {
                WarningIn("writePointsFaces()")
                    << "No master face for cut face " << cutFaceI
                    << " at position " << c[cutFaceI].centre(c.points())
                    << endl;

                equivMasterFaces[cutFaceI] = vector::zero;
            }
        }

        writeOBJ
        (
            "cutToMasterFaces.obj",
            calcFaceCentres<List>(c, cutPoints(), 0, c.size()),
            equivMasterFaces
        );
    }

    {
        OFstream str();
        Pout<< "Writing cutToSlaveFaces to cutToSlaveFaces.obj" << endl;

        pointField equivSlaveFaces(c.size());

        forAll(cutToSlaveFaces(), cutFaceI)
        {
            label slaveFaceI = cutToSlaveFaces()[cutFaceI];

            equivSlaveFaces[cutFaceI] = s[slaveFaceI].centre(s.points());
        }

        writeOBJ
        (
            "cutToSlaveFaces.obj",
            calcFaceCentres<List>(c, cutPoints(), 0, c.size()),
            equivSlaveFaces
        );
    }

    Pout<< endl;
}


void Foam::faceCoupleInfo::writeEdges() const
{
    const indirectPrimitivePatch& m = masterPatch();
    const indirectPrimitivePatch& s = slavePatch();
    const primitiveFacePatch& c = cutFaces();

    // Edge connectivity
    {
        OFstream str("cutToMasterEdges.obj");
        Pout<< "Writing cutToMasterEdges to " << str.name() << endl;

        label vertI = 0;

        forAll(cutToMasterEdges_, cutEdgeI)
        {
            if (cutToMasterEdges_[cutEdgeI] != -1)
            {
                const edge& masterEdge =
                    m.edges()[cutToMasterEdges_[cutEdgeI]];
                const edge& cutEdge = c.edges()[cutEdgeI];

                meshTools::writeOBJ(str, m.localPoints()[masterEdge[0]]);
                vertI++;
                meshTools::writeOBJ(str, m.localPoints()[masterEdge[1]]);
                vertI++;
                meshTools::writeOBJ(str, c.localPoints()[cutEdge[0]]);
                vertI++;
                meshTools::writeOBJ(str, c.localPoints()[cutEdge[1]]);
                vertI++;
                str << "l " << vertI-3 << ' ' << vertI-2 << nl;
                str << "l " << vertI-3 << ' ' << vertI-1 << nl;
                str << "l " << vertI-3 << ' ' << vertI << nl;
                str << "l " << vertI-2 << ' ' << vertI-1 << nl;
                str << "l " << vertI-2 << ' ' << vertI << nl;
                str << "l " << vertI-1 << ' ' << vertI << nl;
            }
        }
    }
    {
        OFstream str("cutToSlaveEdges.obj");
        Pout<< "Writing cutToSlaveEdges to " << str.name() << endl;

        label vertI = 0;

        labelList slaveToCut(invert(s.nEdges(), cutToSlaveEdges()));

        forAll(slaveToCut, edgeI)
        {
            if (slaveToCut[edgeI] != -1)
            {
                const edge& slaveEdge = s.edges()[edgeI];
                const edge& cutEdge = c.edges()[slaveToCut[edgeI]];

                meshTools::writeOBJ(str, s.localPoints()[slaveEdge[0]]);
                vertI++;
                meshTools::writeOBJ(str, s.localPoints()[slaveEdge[1]]);
                vertI++;
                meshTools::writeOBJ(str, c.localPoints()[cutEdge[0]]);
                vertI++;
                meshTools::writeOBJ(str, c.localPoints()[cutEdge[1]]);
                vertI++;
                str << "l " << vertI-3 << ' ' << vertI-2 << nl;
                str << "l " << vertI-3 << ' ' << vertI-1 << nl;
                str << "l " << vertI-3 << ' ' << vertI << nl;
                str << "l " << vertI-2 << ' ' << vertI-1 << nl;
                str << "l " << vertI-2 << ' ' << vertI << nl;
                str << "l " << vertI-1 << ' ' << vertI << nl;
            }
        }
    }

    Pout<< endl;
}


// Given an edgelist and a map for the points on the edges it tries to find
// the corresponding patch edges.
Foam::labelList Foam::faceCoupleInfo::findMappedEdges
(
    const edgeList& edges,
    const labelList& pointMap,
    const indirectPrimitivePatch& patch
)
{
    labelList toPatchEdges(edges.size());

    forAll(toPatchEdges, edgeI)
    {
        const edge& e = edges[edgeI];

        label v0 = pointMap[e[0]];
        label v1 = pointMap[e[1]];

        toPatchEdges[edgeI] = 
            meshTools::findEdge
            (
                patch.edges(),
                patch.pointEdges()[v0],
                v0,
                v1
            );
    }
    return toPatchEdges;
}


// Detect a cut edge which originates from two boundary faces having different
// polyPatches.
bool Foam::faceCoupleInfo::regionEdge
(
    const polyMesh& slaveMesh,
    const label slaveEdgeI
) const
{
    const labelList& eFaces = slavePatch().edgeFaces()[slaveEdgeI];

    if (eFaces.size() == 1)
    {
        return true;
    }
    else
    {
        // Count how many different patches connected to this edge.

        label patch0 = -1;

        forAll(eFaces, i)
        {
            label faceI = eFaces[i];

            label meshFaceI = slavePatch().addressing()[faceI];

            label patchI = slaveMesh.boundaryMesh().whichPatch(meshFaceI);

            if (patch0 == -1)
            {
                patch0 = patchI;
            }
            else if (patchI != patch0)
            {
                // Found two different patches connected to this edge.
                return true;
            }
        }
        return false;
    }
}


// Find edge using pointI that is most aligned with vector between
// master points
Foam::label Foam::faceCoupleInfo::mostAlignedCutEdge
(
    const bool report,
    const polyMesh& slaveMesh,
    const bool patchDivision,
    const label pointI,
    const label edgeStart,
    const label edgeEnd
) const
{
    const pointField& localPoints = cutFaces().localPoints();

    const labelList& pEdges = cutFaces().pointEdges()[pointI];

    if (report)
    {
        Pout<< "mostAlignedEdge : finding nearest edge among "
            << IndirectList<edge>(cutFaces().edges(), pEdges)()
            << " connected to point " << pointI
            << " coord:" << localPoints[pointI]
            << " running between " << edgeStart << " coord:"
            << localPoints[edgeStart]
            << " and " << edgeEnd << " coord:"
            << localPoints[edgeEnd]
            << endl;
    }

    // Find the edge that gets us nearest end.

    label maxEdgeI = -1;
    scalar maxCos = -GREAT;

    forAll(pEdges, i)
    {
        label edgeI = pEdges[i];

        if
        (
           !(
                patchDivision
             && cutToMasterEdges_[edgeI] == -1
            )
         || (
                patchDivision
             && regionEdge(slaveMesh, cutToSlaveEdges_[edgeI])
            )
        )
        {
            const edge& e = cutFaces().edges()[edgeI];

            label otherPointI = e.otherVertex(pointI);

            if (otherPointI == edgeEnd)
            {
                // Shortcut: found edge end point.
                if (report)
                {
                    Pout<< "    mostAlignedEdge : found end point " << edgeEnd
                        << endl;
                }
                return edgeI;
            }

            // Get angle between edge and edge to masterEnd

            vector eVec(localPoints[otherPointI] - localPoints[pointI]);

            scalar magEVec = mag(eVec);

            if (magEVec < VSMALL)
            {
                WarningIn("faceCoupleInfo::mostAlignedEdge")
                    << "Crossing zero sized edge " << edgeI
                    << " coords:" << localPoints[otherPointI]
                    << localPoints[pointI]
                    << " when walking from " << localPoints[edgeStart]
                    << " to " << localPoints[edgeEnd]
                    << endl;
                return edgeI;
            }

            eVec /= magEVec;

            vector eToEndPoint(localPoints[edgeEnd] - localPoints[otherPointI]);
            eToEndPoint /= mag(eToEndPoint);

            scalar cosAngle = eVec & eToEndPoint;

            if (report)
            {
                Pout<< "    edge:" << e << " points:" << localPoints[pointI]
                    << localPoints[otherPointI]
                    << "  vec:" << eVec
                    << "  vecToEnd:" << eToEndPoint
                    << " cosAngle:" << cosAngle
                    << endl;
            }

            if (cosAngle > maxCos)
            {
                maxCos = cosAngle;
                maxEdgeI = edgeI;
            }
        }
    }

    if (maxCos > 1 - angleTol_)
    {
        return maxEdgeI;
    }
    else
    {
        return -1;
    }
}


// Return max distance from any point on cutF to masterF
Foam::scalar Foam::faceCoupleInfo::maxDistance
(
    const face& cutF,
    const pointField& cutPoints,
    const face& masterF,
    const pointField& masterPoints
)
{
    scalar maxDist = -GREAT;

    forAll(cutF, fp)
    {
        const point& cutPt = cutPoints[cutF[fp]];

        pointHit pHit = masterF.nearestPoint(cutPt, masterPoints);

        maxDist = max(maxDist, pHit.distance());
    }
    return maxDist;
}


void Foam::faceCoupleInfo::findPerfectMatchingFaces
(
    const primitiveMesh& mesh0,
    const primitiveMesh& mesh1,
    const scalar absTol,

    labelList& mesh0Faces,
    labelList& mesh1Faces
)
{
    // Face centres of external faces (without invoking
    // mesh.faceCentres since mesh might have been clearedOut)

    pointField fc0
    (
        calcFaceCentres<List>
        (
            mesh0.faces(),
            mesh0.points(),
            mesh0.nInternalFaces(),
            mesh0.nFaces() - mesh0.nInternalFaces()
        )
    );

    pointField fc1
    (
        calcFaceCentres<List>
        (
            mesh1.faces(),
            mesh1.points(),
            mesh1.nInternalFaces(),
            mesh1.nFaces() - mesh1.nInternalFaces()
        )
    );


    if (debug)
    {
        Pout<< "Face matching tolerance : " << absTol << endl;
    }


    // Match geometrically
    labelList from1To0;
    bool matchedAllFaces = matchPoints
    (
        fc1,
        fc0,
        scalarField(fc1.size(), absTol),
        false,
        from1To0
    );

    if (matchedAllFaces)
    {
        Warning
            << "faceCoupleInfo::faceCoupleInfo : "
            << "Matched ALL " << fc1.size()
            << " boundary faces of mesh0 to boundary faces of mesh1." << endl
            << "This is only valid if the mesh to add is fully"
            << " enclosed by the mesh it is added to." << endl;
    }


    // Collect matches.
    label nMatched = 0;

    mesh0Faces.setSize(fc0.size());
    mesh1Faces.setSize(fc1.size());

    forAll(from1To0, i)
    {
        if (from1To0[i] != -1)
        {
            mesh1Faces[nMatched] = i + mesh1.nInternalFaces();
            mesh0Faces[nMatched] = from1To0[i] + mesh0.nInternalFaces();

            nMatched++;
        }
    }
    
    mesh0Faces.setSize(nMatched);
    mesh1Faces.setSize(nMatched);
}


void Foam::faceCoupleInfo::findSlavesCoveringMaster
(
    const primitiveMesh& mesh0,
    const primitiveMesh& mesh1,
    const scalar absTol,

    labelList& mesh0Faces,
    labelList& mesh1Faces
)
{
    // Construct octree from all mesh0 boundary faces
    octreeDataFace shapes(mesh0);

    treeBoundBox overallBb(mesh0.points());

    octree<octreeDataFace> tree
    (
        overallBb,  // overall search domain
        shapes,     // all information needed to do checks on cells
        1,          // min levels
        20.0,       // maximum ratio of cubes v.s. cells
        10.0
    );


    if (debug)
    {
        Pout<< "findSlavesCoveringMaster :"
            << " constructed octree for mesh0 boundary faces" << endl;
    }

    // Search nearest face for every mesh1 boundary face.

    labelHashSet mesh0Set(mesh0.nFaces() - mesh0.nInternalFaces());
    labelHashSet mesh1Set(mesh1.nFaces() - mesh1.nInternalFaces());

    for
    (
        label mesh1FaceI = mesh1.nInternalFaces();
        mesh1FaceI < mesh1.nFaces();
        mesh1FaceI++
    )
    {
        const face& f1 = mesh1.faces()[mesh1FaceI];

        // Generate face centre (prevent cellCentres() reconstruction)
        point fc(f1.centre(mesh1.points()));

        // Search in bounding box of face only.
        treeBoundBox tightest(f1.points(mesh1.points()));

        scalar tightestDist = GREAT;

        label index = tree.findNearest(fc, tightest, tightestDist);


        if (index != -1)
        {
            label mesh0FaceI = shapes.meshFaces()[index];

            // Check if points of f1 actually lie on top of mesh0 face
            // This is the bit that might fail since if f0 is severely warped
            // and f1's points are not present on f0 (since subdivision)
            // then the distance of the points to f0 might be quite large
            // and the test will fail. This all should in fact be some kind
            // of walk from known corresponding points/edges/faces.
            scalar dist =
                maxDistance
                (
                    f1,
                    mesh1.points(),
                    mesh0.faces()[mesh0FaceI],
                    mesh0.points()
                );

            if (dist < absTol)
            {
                mesh0Set.insert(mesh0FaceI);
                mesh1Set.insert(mesh1FaceI);
            }
        }
    }

    if (debug)
    {
        Pout<< "findSlavesCoveringMaster :"
            << " matched " << mesh1Set.size() << " mesh1 faces to "
            << mesh0Set.size() << " mesh0 faces" << endl;


        Pout<< "Writing lines between face centres of mesh0 and mesh1 to"
            << " obj format file mesh0ToMesh1.obj" << endl;

        OFstream str("mesh0ToMesh1.obj");        
    }

    mesh0Faces = mesh0Set.toc();
    mesh1Faces = mesh1Set.toc();
}


// Grow cutToMasterFace across 'internal' edges.
Foam::label Foam::faceCoupleInfo::growCutFaces
(
    Map<labelList>& candidates
)
{
    if (debug)
    {
        Pout<< "growCutFaces :"
            << " growing cut faces to masterPatch" << endl;
    }

    label nTotChanged = 0;

    while (true)
    {
        const labelListList& cutFaceEdges = cutFaces().faceEdges();
        const labelListList& cutEdgeFaces = cutFaces().edgeFaces();

        label nChanged = 0;

        forAll(cutToMasterFaces_, cutFaceI)
        {
            const label masterFaceI = cutToMasterFaces_[cutFaceI];

            if (masterFaceI != -1)
            {
                // We now have a cutFace for which we have already found a
                // master face. Grow this masterface across any internal edge
                // (internal: no corresponding master edge)

                const labelList& fEdges = cutFaceEdges[cutFaceI];

                forAll(fEdges, i)
                {
                    const label cutEdgeI = fEdges[i];

                    if (cutToMasterEdges_[cutEdgeI] == -1)
                    {
                        // So this edge:
                        // - internal to the cutPatch (since all region edges
                        //   marked before)
                        // - on cutFaceI which corresponds to masterFace.
                        // Mark all connected faces with this masterFace.

                        const labelList& eFaces = cutEdgeFaces[cutEdgeI];

                        forAll(eFaces, j)
                        {
                            const label faceI = eFaces[j];

                            if (cutToMasterFaces_[faceI] == -1)
                            {
                                cutToMasterFaces_[faceI] = masterFaceI;
                                candidates.erase(faceI);
                                nChanged++;
                            }
                            else if (cutToMasterFaces_[faceI] != masterFaceI)
                            {
                                const pointField& cutPoints =
                                    cutFaces().points();
                                const pointField& masterPoints =
                                    masterPatch().points();

                                const edge& e = cutFaces().edges()[cutEdgeI];

                                label myMaster = cutToMasterFaces_[faceI];
                                const face& myF = masterPatch()[myMaster];

                                const face& nbrF = masterPatch()[masterFaceI];

                                FatalErrorIn
                                (
                                    "faceCoupleInfo::growCutFaces()"
                                )   << "Cut face "
                                    << cutFaces()[faceI].points(cutPoints)
                                    << " has master "
                                    << myMaster
                                    << " but also connects to nbr face "
                                    << cutFaces()[cutFaceI].points(cutPoints)
                                    << " with master " << masterFaceI
                                    << nl
                                    << "myMasterFace:"
                                    << myF.points(masterPoints)
                                    << "  nbrMasterFace:"
                                    << nbrF.points(masterPoints) << nl
                                    << "Across cut edge " << cutEdgeI
                                    << " coord:"
                                    << cutFaces().localPoints()[e[0]]
                                    << cutFaces().localPoints()[e[1]]
                                    << abort(FatalError);
                            }
                        }
                    }
                }
            }
        }

        if (debug)
        {
            Pout<< "growCutFaces : Grown an additional " << nChanged
                << " cut to master face" << " correspondence" << endl;
        }

        nTotChanged += nChanged;

        if (nChanged == 0)
        {
            break;
        }
    }

    return nTotChanged;
}


void Foam::faceCoupleInfo::checkMatch() const
{
    const pointField& cutLocalPoints = cutFaces().localPoints();

    const pointField& masterLocalPoints = masterPatch().localPoints();
    const faceList& masterLocalFaces = masterPatch().localFaces();

    forAll(cutToMasterEdges_, cutEdgeI)
    {
        const edge& e = cutFaces().edges()[cutEdgeI];

        if (cutToMasterEdges_[cutEdgeI] == -1)
        {
            // Internal edge. Check that master face is same on both sides.
            const labelList& cutEFaces = cutFaces().edgeFaces()[cutEdgeI];

            label masterFaceI = -1;

            forAll(cutEFaces, i)
            {
                label cutFaceI = cutEFaces[i];

                if (cutToMasterFaces_[cutFaceI] != -1)
                {
                    if (masterFaceI == -1)
                    {
                        masterFaceI = cutToMasterFaces_[cutFaceI];
                    }
                    else if (masterFaceI != cutToMasterFaces_[cutFaceI])
                    {
                        label myMaster = cutToMasterFaces_[cutFaceI];
                        const face& myF = masterLocalFaces[myMaster];

                        const face& nbrF = masterLocalFaces[masterFaceI];

                        FatalErrorIn
                        (
                            "faceCoupleInfo::checkMatch()"
                        )
                            << "Internal CutEdge " << e
                            << " coord:"
                            << cutLocalPoints[e[0]]
                            << cutLocalPoints[e[1]]
                            << " connects to master " << myMaster
                            << " and to master " << masterFaceI << nl
                            << "myMasterFace:"
                            << myF.points(masterLocalPoints)
                            << "  nbrMasterFace:"
                            << nbrF.points(masterLocalPoints)
                            << abort(FatalError);
                    }
                }
            }
        }
    }
}


// Extends matching information by elimination across cutFaces using more
// than one region edge. Updates cutToMasterFaces_ and sets candidates
// which is for every cutface on a region edge the possible master faces.
Foam::label Foam::faceCoupleInfo::matchEdgeFaces(Map<labelList>& candidates)
{
    // For every unassigned cutFaceI the possible list of master faces.
    candidates.clear();
    candidates.resize(cutFaces().size());

    label nChanged = 0;

    forAll(cutToMasterEdges_, cutEdgeI)
    {
        label masterEdgeI = cutToMasterEdges_[cutEdgeI];

        if (masterEdgeI != -1)
        {
            // So cutEdgeI is matched to masterEdgeI. For all cut faces using
            // the cut edge store the master face as a candidate match.

            const labelList& cutEFaces = cutFaces().edgeFaces()[cutEdgeI];
            const labelList& masterEFaces =
                masterPatch().edgeFaces()[masterEdgeI];

            forAll(cutEFaces, i)
            {
                label cutFaceI = cutEFaces[i];

                if (cutToMasterFaces_[cutFaceI] == -1)
                {
                    // So this face (cutFaceI) is on an edge (cutEdgeI) that
                    // is used by some master faces (masterEFaces). Check if
                    // the cutFaceI and some of these masterEFaces share more
                    // than one edge (which uniquely defines face)

                    // Combine master faces with current set of candidate
                    // master faces.
                    Map<labelList>::iterator fnd = candidates.find(cutFaceI);

                    if (fnd == candidates.end())
                    {
                        // No info yet for cutFaceI. Add all master faces as
                        // candidates
                        candidates.insert(cutFaceI, masterEFaces);
                    }
                    else
                    {
                        // From some other cutEdgeI there are already some
                        // candidate master faces. Check the overlap with
                        // the current set of master faces.
                        const labelList& masterFaces = fnd();

                        DynamicList<label> newCandidates(masterFaces.size());

                        forAll(masterEFaces, j)
                        {
                            if (findIndex(masterFaces, masterEFaces[j]) != -1)
                            {
                                newCandidates.append(masterEFaces[j]);
                            }
                        }

                        if (newCandidates.size() == 1)
                        {
                            // We found a perfect match. Delete entry from
                            // candidates map.
                            cutToMasterFaces_[cutFaceI] = newCandidates[0];
                            candidates.erase(cutFaceI);
                            nChanged++;
                        }
                        else
                        {
                            // Should not happen?
                            //Pout<< "On edge:" << cutEdgeI
                            //    << " have connected masterFaces:"
                            //    << masterEFaces
                            //    << " and from previous edge we have"
                            //    << " connected masterFaces" << masterFaces
                            //    << " . Overlap:" << newCandidates << endl;

                            fnd() = newCandidates.shrink();
                        }
                    }
                }

            }
        }
    }

    if (debug)
    {
        Pout<< "matchEdgeFaces : Found " << nChanged
            << " faces where there was"
            << " only one remaining choice for cut-master correspondence"
            << endl;
    }

    return nChanged;
}


// Gets a list of cutFaces (that use a master edge) and the candidate
// master faces.
// Finds most aligned master face.
Foam::label Foam::faceCoupleInfo::geometricMatchEdgeFaces
(
    Map<labelList>& candidates
)
{
    const pointField& cutPoints = cutFaces().points();

    label nChanged = 0;

    // Mark all master faces that have been matched so far.

    labelListList masterToCutFaces
    (
        invertOneToMany
        (
            masterPatch().size(),
            cutToMasterFaces_
        )
    );

    forAllConstIter(Map<labelList>, candidates, iter)
    {
        label cutFaceI = iter.key();

        const face& cutF = cutFaces()[cutFaceI];

        if (cutToMasterFaces_[cutFaceI] == -1)
        {
            const labelList& masterFaces = iter();

            // Find the best matching master face.
            scalar minDist = GREAT;
            label minMasterFaceI = -1;

            forAll(masterFaces, i)
            {
                label masterFaceI = masterFaces[i];

                if (masterToCutFaces[masterFaces[i]].size() == 0)
                {
                    scalar dist =
                        maxDistance
                        (
                            cutF,
                            cutPoints,
                            masterPatch()[masterFaceI],
                            masterPatch().points()
                        );

                    if (dist < minDist)
                    {
                        minDist = dist;
                        minMasterFaceI = masterFaceI;
                    }
                }
            }

            if (minMasterFaceI != -1)
            {
                cutToMasterFaces_[cutFaceI] = minMasterFaceI;
                masterToCutFaces[minMasterFaceI] = cutFaceI;
                nChanged++;
            }
        }
    }

    // (inefficiently) force candidates to be uptodate.
    forAll(cutToMasterFaces_, cutFaceI)
    {
        if (cutToMasterFaces_[cutFaceI] != -1)
        {
            candidates.erase(cutFaceI);
        }
    }


    if (debug)
    {
        Pout<< "geometricMatchEdgeFaces : Found " << nChanged
            << " faces where there was"
            << " only one remaining choice for cut-master correspondence"
            << endl;
    }

    return nChanged;
}


// Calculate the set of cut faces inbetween master and slave patch
// assuming perfect match (and optional face ordering on slave)
void Foam::faceCoupleInfo::perfectPointMatch
(
    const scalar absTol,
    const bool slaveFacesOrdered
)
{
    if (debug)
    {
        Pout<< "perfectPointMatch :"
            << " Matching master and slave to cut."
            << " Master and slave faces are identical" << nl;

        if (slaveFacesOrdered)
        {
            Pout<< "and master and slave faces are ordered"
                << " (on coupled patches)" << endl;
        }
        else
        {
            Pout<< "and master and slave faces are not ordered"
                << " (on coupled patches)" << endl;
        }
    }

    // Assume perfect match: intersection of the two patches is the masterPatch
    cutPoints_ = masterPatch().localPoints();

    cutFacesPtr_.reset
    (
        new primitiveFacePatch
        (
            masterPatch().localFaces(),
            cutPoints_
        )
    );


    // Cut is copy of master so addressing to master is trivial.
    cutToMasterFaces_.setSize(cutFaces().size());
    forAll(cutToMasterFaces_, i)
    {
        cutToMasterFaces_[i] = i;
    }
    cutToMasterPoints_.setSize(cutFaces().nPoints());
    forAll(cutToMasterPoints_, i)
    {
        cutToMasterPoints_[i] = i;
    }



    // Master edges are bit more tricky in that they are numbered in
    // upper-triangular order so might differ from master to cut? Is this true?
    cutToMasterEdges_ = findMappedEdges
    (
        cutFaces().edges(),
        cutToMasterPoints_,
        masterPatch()
    );



    // Addressing from cut to slave.

    // Cut faces to slave patch.
    bool matchedAllFaces = false;
 
    if (slaveFacesOrdered)
    {
        cutToSlaveFaces_.setSize(cutFaces().size());
        forAll(cutToSlaveFaces_, i)
        {
            cutToSlaveFaces_[i] = i;
        }

        matchedAllFaces = (cutFaces().size() == slavePatch().size());
    }
    else
    {
        // Faces do not have to be ordered (but all have
        // to match). Note: Faces will be already ordered if we enter here from
        // construct from meshes.
        matchedAllFaces = matchPoints
        (
            calcFaceCentres<List>
            (
                cutFaces(),
                cutPoints_,
                0,
                cutFaces().size()
            ),
            calcFaceCentres<IndirectList>
            (
                slavePatch(),
                slavePatch().points(),
                0,
                slavePatch().size()
            ),
            scalarField(slavePatch().size(), absTol),
            true,
            cutToSlaveFaces_
        );
    }


    if (!matchedAllFaces)
    {
        FatalErrorIn
        (
            "faceCoupleInfo::perfectPointMatch"
            "(const scalar&, const bool)"
        )   << "Did not match all of the master faces to the slave faces"
            << endl
            << "This usually means that the slave patch and master patch"
            << " do not align to within " << absTol << " meter."
            << abort(FatalError);
    }


    // Cut points to slave patch
    bool matchedAllPoints = matchPoints
    (
        cutPoints_,
        slavePatch().localPoints(),
        scalarField(cutPoints_.size(), absTol),
        true,
        cutToSlavePoints_
    );

    if (!matchedAllPoints)
    {
        FatalErrorIn
        (
            "faceCoupleInfo::perfectPointMatch"
            "(const scalar&, const bool)"
        )   << "Did not match all of the master points to the slave points"
            << endl
            << "This usually means that the slave patch and master patch"
            << " do not align to within " << absTol << " meter."
            << abort(FatalError);
    }

    // Cut edges to slave patch
    cutToSlaveEdges_ = findMappedEdges
    (
        cutFaces().edges(),
        cutToSlavePoints_,
        slavePatch()
    );
}


// Calculate the set of cut faces inbetween master and slave patch
// assuming that slave patch is subdivision of masterPatch.
void Foam::faceCoupleInfo::subDivisionMatch
(
    const polyMesh& slaveMesh,
    const bool patchDivision,
    const scalar absTol
)
{
    if (debug)
    {
        Pout<< "subDivisionMatch :"
            << " Matching master and slave to cut."
            << " Slave can be subdivision of master but all master edges"
            << " have to be covered by slave edges." << endl;
    }

    // Assume slave patch is subdivision of the master patch so cutFaces is a
    // copy of the slave (but reversed since orientation has to be like master).
    cutPoints_ = slavePatch().localPoints();
    {
        faceList cutFaces(slavePatch().size());

        forAll(cutFaces, i)
        {
            cutFaces[i] = slavePatch().localFaces()[i].reverseFace();
        }
        cutFacesPtr_.reset(new primitiveFacePatch(cutFaces, cutPoints_));
    }

    // Cut is copy of slave so addressing to slave is trivial.
    cutToSlaveFaces_.setSize(cutFaces().size());
    forAll(cutToSlaveFaces_, i)
    {
        cutToSlaveFaces_[i] = i;
    }
    cutToSlavePoints_.setSize(cutFaces().nPoints());
    forAll(cutToSlavePoints_, i)
    {
        cutToSlavePoints_[i] = i;
    }
    // Cut edges to slave patch
    cutToSlaveEdges_ = findMappedEdges
    (
        cutFaces().edges(),
        cutToSlavePoints_,
        slavePatch()
    );


if (debug)
{
    Pout<< "subDivisionMatch :"
        << " addressing for slave patch fully done."
        << " Dumping region edges to regionEdges.obj" << endl;

    OFstream str("regionEdges.obj");
    label vertI = 0;

    forAll(slavePatch().edges(), slaveEdgeI)
    {
        if (regionEdge(slaveMesh, slaveEdgeI))
        {
            const edge& e = slavePatch().edges()[slaveEdgeI];

            meshTools::writeOBJ(str, slavePatch().localPoints()[e[0]]);
            vertI++;
            meshTools::writeOBJ(str, slavePatch().localPoints()[e[1]]);
            vertI++;
            str<< "l " << vertI-1 << ' ' << vertI << nl;
        }
    }
}


    // Addressing from cut to master.

    // Cut points to master patch
    // - determine master points to cut points. Has to be full!
    // - invert to get cut to master
    if (debug)
    {
        Pout<< "subDivisionMatch :"
            << " matching master points to cut points" << endl;
    }

    labelList masterToCutPoints;
    bool matchedAllPoints = matchPoints
    (
        masterPatch().localPoints(),
        cutPoints_,
        scalarField(masterPatch().nPoints(), absTol),
        true,
        masterToCutPoints
    );

    if (!matchedAllPoints)
    {
        FatalErrorIn
        (
            "faceCoupleInfo::subDivisionMatch(const scalar)"
        )   << "Did not match all of the master points to the slave points"
            << endl
            << "This usually means that the slave patch is not a"
            << " subdivision of the master patch"
            << abort(FatalError);
    }

    // Invert masterToCut points. Not all cut (= slave) points have to
    // be present on the master since can be subdivision so inverse might
    // contain -1.
    cutToMasterPoints_ = invert(cutFaces().nPoints(), masterToCutPoints);



    // Do masterEdges to cutEdges :
    // - mark all edges between two masterEdge endpoints. (geometric test since
    //   this is the only distinction)
    // - this gives the borders inbetween which all cutfaces come from
    //   a single master face.
    if (debug)
    {
        Pout<< "subDivisionMatch :"
            << " matching cut edges to master edges" << endl;
    }

    const edgeList& masterEdges = masterPatch().edges();
    const pointField& masterPoints = masterPatch().localPoints();

    const edgeList& cutEdges = cutFaces().edges();

    cutToMasterEdges_.setSize(cutFaces().nEdges());
    cutToMasterEdges_ = -1;

    forAll(masterEdges, masterEdgeI)
    {
        const edge& masterEdge = masterEdges[masterEdgeI];

        label cutPoint0 = masterToCutPoints[masterEdge[0]];
        label cutPoint1 = masterToCutPoints[masterEdge[1]];

        // Find edges between cutPoint0 and cutPoint1.

        label cutPointI = cutPoint0;
        do
        {
            // Find edge (starting at pointI on cut), aligned with master
            // edge.
            label cutEdgeI =
                mostAlignedCutEdge
                (
                    false,
                    slaveMesh,
                    patchDivision,
                    cutPointI,
                    cutPoint0,
                    cutPoint1
                );

            if (cutEdgeI == -1)
            {
                // Problem. Report while matching to produce nice error message
                mostAlignedCutEdge
                (
                    true,
                    slaveMesh,
                    patchDivision,
                    cutPointI,
                    cutPoint0,
                    cutPoint1
                );

                Pout<< "Dumping unmatched pointEdges to errorEdges.obj"
                    << endl;

                writeOBJ
                (
                    "errorEdges.obj",
                    IndirectList<edge>
                    (
                        cutFaces().edges(),
                        cutFaces().pointEdges()[cutPointI]
                    )(),
                    cutFaces().localPoints(),
                    false
                );

                FatalErrorIn
                (
                    "faceCoupleInfo::subDivisionMatch(const scalar)"
                )   << "Problem in finding cut edges corresponding to"
                    << " master edge " << masterEdge
                    << " points:" << masterPoints[masterEdge[0]]
                    << ' ' << masterPoints[masterEdge[1]]
                    << " corresponding cut edge: (" << cutPoint0
                    << ' ' << cutPoint1
                    << abort(FatalError);
            }

            cutToMasterEdges_[cutEdgeI] = masterEdgeI;

            cutPointI = cutEdges[cutEdgeI].otherVertex(cutPointI);

        } while(cutPointI != cutPoint1);
    }

    if (debug)
    {
        // Write all matched edges.
        writeEdges();
    }


    // Check: every master edge present in cutFaces.
    {
        labelListList masterToCutEdges
        (
            invertOneToMany
            (   
                masterPatch().nEdges(),
                cutToMasterEdges_
            )
        );

        forAll(masterToCutEdges, masterEdgeI)
        {
            if (masterToCutEdges[masterEdgeI].size() == 0)
            {
                const edge& e = masterPatch().edges()[masterEdgeI];

                FatalErrorIn
                (
                    "faceCoupleInfo::subDivisionMatch(const scalar)"
                )   << "Did not match all of master edges to cutFace edges"
                    << nl
                    << "First unmatched edge:" << masterEdgeI << " endPoints:"
                    << masterPatch().localPoints()[e[0]]
                    << masterPatch().localPoints()[e[1]]
                    << endl
                    << "This usually means that the slave patch is not a"
                    << " subdivision of the master patch"
                    << abort(FatalError);
            }
        }
    }

    // Now that we have marked the cut edges that lie on top of master edges
    // we can find cut faces that have two (or more) of these edges.
    // Note that there can be multiple faces having two or more matched edges
    // since the cut faces can form a non-manifold surface(?)
    // So the matching is done as an elimination process where for every
    // cutFace on a matched edge we store the possible master faces and
    // eliminate more and more until we only have one possible master face
    // left.

    if (debug)
    {
        Pout<< "subDivisionMatch :"
            << " matching (topological) cut faces to masterPatch" << endl;
    }

    // For every unassigned cutFaceI the possible list of master faces.
    Map<labelList> candidates(cutFaces().size());

    cutToMasterFaces_.setSize(cutFaces().size());
    cutToMasterFaces_ = -1;

    while (true)
    {
        // See if there are any edges left where there is only one remaining
        // candidate.
        label nChanged = matchEdgeFaces(candidates);

        checkMatch();


        // Now we should have found a face correspondence for every cutFace
        // that uses two or more cutEdges. Floodfill from these across
        // 'internal' cutedges (i.e. edges that do not have a master
        // equivalent) to determine some more cutToMasterFaces
        nChanged += growCutFaces(candidates);

        checkMatch();

        if (nChanged == 0)
        {
            break;
        }
    }

    if (debug)
    {
        Pout<< "subDivisionMatch :"
            << " matching (geometric) cut faces to masterPatch" << endl;
    }

    // Above should have matched all cases fully. Cannot prove this yet so
    // do any remaining unmatched edges by a geometric test.
    while (true)
    {
        // See if there are any edges left where there is only one remaining
        // candidate.
        label nChanged = geometricMatchEdgeFaces(candidates);

        checkMatch();

        nChanged += growCutFaces(candidates);

        checkMatch();

        if (nChanged == 0)
        {
            break;
        }
    }
    

    // All cut faces matched?
    forAll(cutToMasterFaces_, cutFaceI)
    {
        if (cutToMasterFaces_[cutFaceI] == -1)
        {
            const face& cutF = cutFaces()[cutFaceI];

            FatalErrorIn
            (
                "faceCoupleInfo::subDivisionMatch(const scalar)"
            )   << "Did not match all of cutFaces to a master face" << nl
                << "First unmatched cut face:" << cutFaceI << " with points:"
                << IndirectList<point>(cutFaces().points(), cutF)()
                << nl
                << "This usually means that the slave patch is not a"
                << " subdivision of the master patch"
                << abort(FatalError);
        }
    }

    if (debug)
    {
        Pout<< "subDivisionMatch :"
            << " finished matching master and slave to cut" << endl;
    }

    if (debug)
    {
        writePointsFaces();
    }
}


Foam::labelList Foam::faceCoupleInfo::getBoundaryPoints
(
    const primitiveMesh& mesh,
    const labelList& excludePoints
)
{
    boolList isBoundaryPoint(mesh.nPoints(), false);
    label nPoints = 0;

    // Mark all boundary points
    for
    (
        label faceI = mesh.nInternalFaces();
        faceI < mesh.nFaces();
        faceI++
    )
    {
        const face& f = mesh.faces()[faceI];

        forAll(f, fp)
        {
            label pointI = f[fp];

            if (!isBoundaryPoint[pointI])
            {
                isBoundaryPoint[pointI] = true;
                nPoints++;
            }
        }
    }

    // Exclude points
    forAll(excludePoints, i)
    {
        label pointI = excludePoints[i];

        if (pointI != -1)
        {
            if (isBoundaryPoint[pointI])
            {
                isBoundaryPoint[pointI] = false;
                nPoints--;
            }
        }
    }


    // Pack
    labelList boundaryPoints(nPoints);
    nPoints = 0;

    forAll(isBoundaryPoint, pointI)
    {
        if (isBoundaryPoint[pointI])
        {
            boundaryPoints[nPoints++] = pointI;
        }
    }

    return boundaryPoints;
}


void Foam::faceCoupleInfo::findNonCutSharedPoints
(
    const primitiveMesh& masterMesh,
    const primitiveMesh& slaveMesh,
    const scalar absTol
)
{
    // Any points which are not on cut but are shared
    // (points that are on any of the cut faces will be already found)

    // Master boundary points (in masterMesh points, not masterPatch points)
    labelList masterBoundaryPoints
    (
        getBoundaryPoints
        (
            masterMesh,
            IndirectList<label>
            (
                masterPatch().meshPoints(),
                cutToMasterPoints_
            )()
        )
    );

    // Slave boundary points (in slaveMesh points, not slavePatch points)
    labelList slaveBoundaryPoints
    (
        getBoundaryPoints
        (
            slaveMesh,
            IndirectList<label>
            (
                slavePatch().meshPoints(),
                cutToSlavePoints_
            )()
        )
    );

    // Find any matches between the two sets.
    labelList masterBoundaryToSlave(masterBoundaryPoints.size());
    matchPoints
    (
        pointField
        (
            IndirectList<point>(masterMesh.points(), masterBoundaryPoints)()
        ),
        pointField
        (
            IndirectList<point>(slaveMesh.points(), slaveBoundaryPoints)()
        ),
        scalarField(masterBoundaryPoints.size(), absTol),
        false,
        masterBoundaryToSlave
    );

    // Convert indices into masterBoundaryPoints to masterMesh.
    sharedToMasterPoints_.setSize(masterBoundaryToSlave.size());
    sharedToSlavePoints_.setSize(masterBoundaryToSlave.size());
    label matchI = 0;

    forAll(masterBoundaryToSlave, i)
    {
        label slaveBPointI = masterBoundaryToSlave[i];

        if (slaveBPointI != -1)
        {
            sharedToMasterPoints_[matchI] = masterBoundaryPoints[i];
            sharedToSlavePoints_[matchI] = slaveBoundaryPoints[slaveBPointI];

            matchI++;
        }
    }
    sharedToMasterPoints_.setSize(matchI);
    sharedToSlavePoints_.setSize(matchI);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from mesh data
Foam::faceCoupleInfo::faceCoupleInfo
(
    const polyMesh& masterMesh,
    const polyMesh& slaveMesh,
    const scalar absTol,
    const bool perfectMatch
)
:
    masterPatchPtr_(NULL),
    slavePatchPtr_(NULL),
    cutPoints_(0),
    cutFacesPtr_(NULL),
    cutToMasterFaces_(0),
    cutToMasterPoints_(0),
    cutToMasterEdges_(0),
    cutToSlaveFaces_(0),
    cutToSlavePoints_(0),
    cutToSlaveEdges_(0),
    masterToCutPoints_(0),
    masterToCutEdges_(0),
    slaveToCutPoints_(0),
    slaveToCutEdges_(0),
    sharedToMasterPoints_(0),
    sharedToSlavePoints_(0)
{
    // Get faces on both meshes that are aligned.
    // (not ordered i.e. masterToMesh[0] does
    // not couple to slaveToMesh[0]. This ordering is done later on)
    labelList masterToMesh;
    labelList slaveToMesh;

    if (perfectMatch)
    {
        // Identical faces so use tight face-centre comparison.
        findPerfectMatchingFaces
        (
            masterMesh,
            slaveMesh,
            absTol,
            masterToMesh,
            slaveToMesh
        );
    }
    else
    {
        // Slave subdivision of master so use 'nearest'.
        findSlavesCoveringMaster
        (
            masterMesh,
            slaveMesh,
            absTol,
            masterToMesh,
            slaveToMesh
        );
    }

    // Construct addressing engines for both sides
    masterPatchPtr_.reset
    (
        new indirectPrimitivePatch
        (
            IndirectList<face>(masterMesh.faces(), masterToMesh),
            masterMesh.points()
        )
    );

    slavePatchPtr_.reset
    (
        new indirectPrimitivePatch
        (
            IndirectList<face>(slaveMesh.faces(), slaveToMesh),
            slaveMesh.points()
        )
    );


    if (perfectMatch)
    {
        // Faces are perfectly aligned but probably not ordered.
        perfectPointMatch(absTol, false);
    }
    else
    {
        subDivisionMatch(slaveMesh, false, absTol);
    }


    // Find points that are shared but not on the cut.
    findNonCutSharedPoints(masterMesh, slaveMesh, absTol);


    if (debug)
    {
        writePointsFaces();
        writeEdges();
    }
}


// Slave is subdivision of master patch.
// (so -both cover the same area -all of master points are present in slave)
Foam::faceCoupleInfo::faceCoupleInfo
(
    const polyMesh& masterMesh,
    const labelList& masterAddressing,
    const polyMesh& slaveMesh,
    const labelList& slaveAddressing,
    const scalar absTol,
    const bool perfectMatch,
    const bool orderedFaces,
    const bool patchDivision
)
:
    masterPatchPtr_
    (
        new indirectPrimitivePatch
        (
            IndirectList<face>(masterMesh.faces(), masterAddressing),
            masterMesh.points()
        )
    ),
    slavePatchPtr_
    (
        new indirectPrimitivePatch
        (
            IndirectList<face>(slaveMesh.faces(), slaveAddressing),
            slaveMesh.points()
        )
    ),
    cutPoints_(0),
    cutFacesPtr_(NULL),
    cutToMasterFaces_(0),
    cutToMasterPoints_(0),
    cutToMasterEdges_(0),
    cutToSlaveFaces_(0),
    cutToSlavePoints_(0),
    cutToSlaveEdges_(0),
    masterToCutPoints_(0),
    masterToCutEdges_(0),
    slaveToCutPoints_(0),
    slaveToCutEdges_(0),
    sharedToMasterPoints_(0),
    sharedToSlavePoints_(0)
{
    if (perfectMatch && (masterAddressing.size() != slaveAddressing.size()))
    {
        FatalErrorIn
        (
            "faceCoupleInfo::faceCoupleInfo(const primitiveMesh&"
            ", const primitiveMesh&, const scalar, const bool"
        )   << "Perfect match specified but number of master and slave faces"
            << " differ." << endl
            << "master:" << masterAddressing.size()
            << "  slave:" << slaveAddressing.size()
            << abort(FatalError);
    }

    if
    (
        masterAddressing.size() > 0
     && min(masterAddressing) < masterMesh.nInternalFaces()
    )
    {
        FatalErrorIn
        (
            "faceCoupleInfo::faceCoupleInfo(const primitiveMesh&"
            ", const primitiveMesh&, const scalar, const bool"
        )   << "Supplied internal face on master mesh to couple." << nl
            << "Faces to be coupled have to be boundary faces."
            << abort(FatalError);
    }
    if
    (
        slaveAddressing.size() > 0
     && min(slaveAddressing) < slaveMesh.nInternalFaces()
    )
    {
        FatalErrorIn
        (
            "faceCoupleInfo::faceCoupleInfo(const primitiveMesh&"
            ", const primitiveMesh&, const scalar, const bool"
        )   << "Supplied internal face on slave mesh to couple." << nl
            << "Faces to be coupled have to be boundary faces."
            << abort(FatalError);
    }


    if (perfectMatch)
    {
        perfectPointMatch(absTol, orderedFaces);
    }
    else
    {
        subDivisionMatch(slaveMesh, patchDivision, absTol);
    }


    // Find points that are shared but not on the cut.
    findNonCutSharedPoints(masterMesh, slaveMesh, absTol);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::faceCoupleInfo::~faceCoupleInfo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::labelList Foam::faceCoupleInfo::faceLabels(const polyPatch& pp)
{
    labelList faces(pp.size());

    label faceI = pp.start();

    forAll(pp, i)
    {
        faces[i] = faceI++;
    }
    return faces;
}


Foam::Map<Foam::label> Foam::faceCoupleInfo::makeMap(const labelList& lst)
{
    Map<label> map(lst.size());

    forAll(lst, i)
    {
        if (lst[i] != -1)
        {
            map.insert(i, lst[i]);
        }
    }
    return map;
}


Foam::Map<Foam::labelList> Foam::faceCoupleInfo::makeMap
(
    const labelListList& lst
)
{
    Map<labelList> map(lst.size());

    forAll(lst, i)
    {
        if (lst[i].size() > 0)
        {
            map.insert(i, lst[i]);
        }
    }
    return map;
}


// ************************************************************************* //
