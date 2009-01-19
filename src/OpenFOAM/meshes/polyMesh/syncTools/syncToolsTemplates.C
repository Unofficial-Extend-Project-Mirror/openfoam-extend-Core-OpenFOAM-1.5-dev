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

\*----------------------------------------------------------------------------*/

#include "syncTools.H"
#include "polyMesh.H"
#include "processorPolyPatch.H"
#include "cyclicPolyPatch.H"
#include "globalMeshData.H"
#include "contiguous.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <class T>
Foam::List<T> Foam::transform
(
    const tensor& rotTensor,
    UList<T>& field
)
{
    List<T> newField(field.size());

    forAll(field, i)
    {
        newField[i] = Foam::transform(rotTensor, field[i]);
    }
    return newField;
}


template <class T>
void Foam::syncTools::transformList
(
    const tensorField& rotTensor,
    UList<T>& field
)
{
    if (rotTensor.size() == 1)
    {
        forAll(field, i)
        {
            field[i] = Foam::transform(rotTensor[0], field[i]);
        }
    }
    else if (rotTensor.size() == field.size())
    {
        forAll(field, i)
        {
            field[i] = Foam::transform(rotTensor[i], field[i]);
        }
    }
    else
    {
        FatalErrorIn
        (
            "syncTools::transformList(const vectorField&, UList<T>&)"
        )   << "Sizes of field and transformation not equal. field:"
            << field.size() << " transformation:" << rotTensor.size()
            << abort(FatalError);
    }
}


template <class T>
void Foam::syncTools::separateList
(
    const vectorField& separation,
    UList<T>& field
)
{}


////XXXXXX
//template <class T, class CombineOp>
//void Foam::syncTools::syncPointList
//(
//    const polyMesh& mesh,
//    const primitivePatch& allBoundary,
//    Map<T>& pointValues,        // from allBoundary point label to value
//    const CombineOp& cop,
//    const T& nullValue,
//    const bool applySeparation
//)
//{
//    if (allBoundary.size() != mesh.nFaces()-mesh.nInternalFaces())
//    {
//        FatalErrorIn
//        (
//            "syncTools<class T, class CombineOp>::syncPointList"
//            "(const polyMesh&, const indirectPrimitivePatch&, Map<T>&"
//            ", const CombineOp&, const T&, const bool)"
//        )   << "Number of faces in allBoundary:" << allBoundary.size()
//            << " is not equal to the number of boundary faces in the mesh "
//            << mesh.nPoints() << abort(FatalError);
//    }
//
//    const polyBoundaryMesh& patches = mesh.boundaryMesh();
//
//    if (!hasCouples(patches))
//    {
//        return;
//    }
//
//
//    // Build map from point on allBoundary to boundary faces and index in
//    // boundary face.
//    labelList allFace(allBoundary.nPoints(), -1);
//    labelList allIndex(allBoundary.nPoints());
//
//    forAll(pointFaces, allpointI)
//    {
//        const labelList& pFaces = pointFaces[allPointI];
//
//        forAll(pFaces, 
//
//    }
//
//    // Is there any coupled patch with transformation?
//    bool hasTransformation = false;
//
//    if (Pstream::parRun())
//    {
//        // Send
//        DynamicList<label> 
//
//
//        forAll(patches, patchI)
//        {
//            if
//            (
//                isA<processorPolyPatch>(patches[patchI])
//             && patches[patchI].nPoints() > 0
//            )
//            {
//                const processorPolyPatch& procPatch =
//                    refCast<const processorPolyPatch>(patches[patchI]);
//
//
//                // Collect local face and index
//                DynamicList<label> localFace(procPatch.nPoints());
//                labelList localIndex(procPatch.nPoints());
//
//                forAllConstIter(Map<T>, pointValues, iter)
//                {
//                    label allPointI = iter.key();
//                    const T& val = iter();
//
//                    const labelList& pFaces =
//                            allBoundary.pointFaces()[allPointI];
//
//                    forAll(pFaces, i)
//                    {
//                        label meshFaceI = mesh.nInternalFaces() + pFaces[i];
//                        label patchFaceI = meshFaceI - procPatch.start();
//
//                        if (patchFaceI >= 0 && patchFaceI < procPatch.size())
//                        {
//                            localFaces.append(patchFaceI);
//
//                            const face& f = procPatch[patchFaceI];
//                            localIndex.append
//                            (
//                                findIndex
//                                (
//                                    f,
//                                    allBoundary.meshPoints()[allPointI]
//                                )
//                            );
//                        }
//                    }
//                }
//
//
//                // Get data per patchPoint in neighbouring point numbers.
//                List<T> patchInfo(procPatch.nPoints(), nullValue);
//
//                const labelList& nbrPts = procPatch.neighbPoints();
//
//                forAll(meshPts, pointI)
//                {
//                    patchInfo[nbrPts[pointI]] = pointValues[meshPts[pointI]];
//                }
//
//                if (contiguous<T>())
//                {
//                    OPstream::write
//                    (
//                        procPatch.neighbProcNo(),
//                        reinterpret_cast<const char*>(patchInfo.begin()),
//                        patchInfo.byteSize(),
//                        true                    // buffered
//                    );
//                }
//                else
//                {
//                    OPstream toNeighb(procPatch.neighbProcNo());
//                    toNeighb << patchInfo;
//                }
//            }
//        }
//
//
//        // Receive and combine.
//
//        forAll(patches, patchI)
//        {
//            if
//            (
//                isA<processorPolyPatch>(patches[patchI])
//             && patches[patchI].nPoints() > 0
//            )
//            {
//                const processorPolyPatch& procPatch =
//                    refCast<const processorPolyPatch>(patches[patchI]);
//                checkTransform(procPatch, applySeparation);
//
//                List<T> nbrPatchInfo(procPatch.nPoints());
//
//                if (contiguous<T>())
//                {
//                    IPstream::read
//                    (
//                        procPatch.neighbProcNo(),
//                        reinterpret_cast<char*>(nbrPatchInfo.begin()),
//                        nbrPatchInfo.byteSize()
//                    );
//                }
//                else
//                {
//                    IPstream fromNeighb(procPatch.neighbProcNo());
//                    fromNeighb >> nbrPatchInfo;
//                }
//
//                if (!procPatch.parallel())
//                {
//                    hasTransformation = true;
//                    transformList(procPatch.reverseT(), nbrPatchInfo);
//                }
//                else if (applySeparation && procPatch.separated())
//                {
//                    hasTransformation = true;
//                    separateList(-procPatch.separation(), nbrPatchInfo);
//                }
//
//                const labelList& meshPts = procPatch.meshPoints();
//
//                forAll(meshPts, pointI)
//                {
//                    label meshPointI = meshPts[pointI];
//                    cop(pointValues[meshPointI], nbrPatchInfo[pointI]);
//                }
//            }
//        }
//    }
//
//    // Do the cyclics.
//    forAll(patches, patchI)
//    {
//        if (isA<cyclicPolyPatch>(patches[patchI]))
//        {
//            const cyclicPolyPatch& cycPatch =
//                refCast<const cyclicPolyPatch>(patches[patchI]);
//
//            checkTransform(cycPatch, applySeparation);
//
//            const labelList& meshPts = cycPatch.meshPoints();
//            const edgeList& coupledPoints = cycPatch.coupledPoints();
//
//            List<T> half0Values(coupledPoints.size());
//            List<T> half1Values(coupledPoints.size());
//
//            forAll(coupledPoints, i)
//            {
//                const edge& e = coupledPoints[i];
//
//                label point0 = meshPts[e[0]];
//                label point1 = meshPts[e[1]];
//
//                half0Values[i] = pointValues[point0];
//                half1Values[i] = pointValues[point1];
//            }
//
//            if (!cycPatch.parallel())
//            {
//                hasTransformation = true;
//                transformList(cycPatch.forwardT(), half0Values);
//            }
//            else if (applySeparation && cycPatch.separated())
//            {
//                hasTransformation = true;
//
//                const vectorField& v =
//                    cycPatch.coupledPolyPatch::separation();
//                separateList(v, half0Values);
//                separateList(-v, half1Values);
//            }
//
//            forAll(coupledPoints, i)
//            {
//                const edge& e = coupledPoints[i];
//
//                label point0 = meshPts[e[0]];
//                label point1 = meshPts[e[1]];
//
//                cop(pointValues[point0], half1Values[i]);
//                cop(pointValues[point1], half0Values[i]);
//            }
//        }
//    }
//
//    //- Note: hasTransformation is only used for warning messages so
//    //  reduction not strictly nessecary.
//    //reduce(hasTransformation, orOp<bool>());
//
//    // Synchronize multiple shared points.
//    const globalMeshData& pd = mesh.globalData();
//
//    if (pd.nGlobalPoints() > 0)
//    {
//        if (hasTransformation)
//        {
//            WarningIn
//            (
//                "syncTools<class T, class CombineOp>::syncPointList"
//                "(const polyMesh&, UList<T>&, const CombineOp&, const T&"
//                ", const bool)"
//            )   << "There are decomposed cyclics in this mesh with"
//                << " transformations." << endl
//                << "This is not supported. The result will be incorrect"
//                << endl;
//        }
//        else
//        {
//            // Values on shared points.
//            List<T> sharedPts(pd.nGlobalPoints(), nullValue);
//
//            forAll(pd.sharedPointLabels(), i)
//            {
//                label meshPointI = pd.sharedPointLabels()[i];
//
//                // Fill my entries in the shared points
//                sharedPts[pd.sharedPointAddr()[i]] = pointValues[meshPointI];
//            }
//
//            // Combine on master.
//            Pstream::listCombineGather(sharedPts, cop);
//            Pstream::listCombineScatter(sharedPts);
//
//            // Now we will all have the same information. Merge it back with
//            // my local information.
//            forAll(pd.sharedPointLabels(), i)
//            {
//                label meshPointI = pd.sharedPointLabels()[i];
//                pointValues[meshPointI] = sharedPts[pd.sharedPointAddr()[i]];
//            }
//        }
//    }
//}
////XXXXXX


template <class T, class CombineOp>
void Foam::syncTools::syncPointList
(
    const polyMesh& mesh,
    UList<T>& pointValues,
    const CombineOp& cop,
    const T& nullValue,
    const bool applySeparation
)
{
    if (pointValues.size() != mesh.nPoints())
    {
        FatalErrorIn
        (
            "syncTools<class T, class CombineOp>::syncPointList"
            "(const polyMesh&, UList<T>&, const CombineOp&, const T&"
            ", const bool)"
        )   << "Number of values " << pointValues.size()
            << " is not equal to the number of points in the mesh "
            << mesh.nPoints() << abort(FatalError);
    }

    const polyBoundaryMesh& patches = mesh.boundaryMesh();

    if (!hasCouples(patches))
    {
        return;
    }

    // Is there any coupled patch with transformation?
    bool hasTransformation = false;

    if (Pstream::parRun())
    {
        // Send

        forAll(patches, patchI)
        {
            if
            (
                isA<processorPolyPatch>(patches[patchI])
             && patches[patchI].nPoints() > 0
            )
            {
                const processorPolyPatch& procPatch =
                    refCast<const processorPolyPatch>(patches[patchI]);

                // Get data per patchPoint in neighbouring point numbers.
                List<T> patchInfo(procPatch.nPoints());

                const labelList& meshPts = procPatch.meshPoints();
                const labelList& nbrPts = procPatch.neighbPoints();

                forAll(meshPts, pointI)
                {
                    patchInfo[nbrPts[pointI]] = pointValues[meshPts[pointI]];
                }

                if (contiguous<T>())
                {
                    OPstream::write
                    (
                        procPatch.neighbProcNo(),
                        reinterpret_cast<const char*>(patchInfo.begin()),
                        patchInfo.byteSize(),
                        true                    // buffered
                    );
                }
                else
                {
                    OPstream toNeighb(procPatch.neighbProcNo());
                    toNeighb << patchInfo;
                }
            }
        }


        // Receive and combine.

        forAll(patches, patchI)
        {
            if
            (
                isA<processorPolyPatch>(patches[patchI])
             && patches[patchI].nPoints() > 0
            )
            {
                const processorPolyPatch& procPatch =
                    refCast<const processorPolyPatch>(patches[patchI]);
                checkTransform(procPatch, applySeparation);

                List<T> nbrPatchInfo(procPatch.nPoints());

                if (contiguous<T>())
                {
                    IPstream::read
                    (
                        procPatch.neighbProcNo(),
                        reinterpret_cast<char*>(nbrPatchInfo.begin()),
                        nbrPatchInfo.byteSize()
                    );
                }
                else
                {
                    IPstream fromNeighb(procPatch.neighbProcNo());
                    fromNeighb >> nbrPatchInfo;
                }

                if (!procPatch.parallel())
                {
                    hasTransformation = true;
                    transformList(procPatch.reverseT(), nbrPatchInfo);
                }
                else if (applySeparation && procPatch.separated())
                {
                    hasTransformation = true;
                    separateList(-procPatch.separation(), nbrPatchInfo);
                }

                const labelList& meshPts = procPatch.meshPoints();

                forAll(meshPts, pointI)
                {
                    label meshPointI = meshPts[pointI];
                    cop(pointValues[meshPointI], nbrPatchInfo[pointI]);
                }
            }
        }
    }

    // Do the cyclics.
    forAll(patches, patchI)
    {
        if (isA<cyclicPolyPatch>(patches[patchI]))
        {
            const cyclicPolyPatch& cycPatch =
                refCast<const cyclicPolyPatch>(patches[patchI]);

            checkTransform(cycPatch, applySeparation);

            const labelList& meshPts = cycPatch.meshPoints();
            const edgeList& coupledPoints = cycPatch.coupledPoints();

            List<T> half0Values(coupledPoints.size());
            List<T> half1Values(coupledPoints.size());

            forAll(coupledPoints, i)
            {
                const edge& e = coupledPoints[i];

                label point0 = meshPts[e[0]];
                label point1 = meshPts[e[1]];

                half0Values[i] = pointValues[point0];
                half1Values[i] = pointValues[point1];
            }

            if (!cycPatch.parallel())
            {
                hasTransformation = true;
                transformList(cycPatch.forwardT(), half0Values);
            }
            else if (applySeparation && cycPatch.separated())
            {
                hasTransformation = true;

                const vectorField& v =
                    cycPatch.coupledPolyPatch::separation();
                separateList(v, half0Values);
                separateList(-v, half1Values);
            }

            forAll(coupledPoints, i)
            {
                const edge& e = coupledPoints[i];

                label point0 = meshPts[e[0]];
                label point1 = meshPts[e[1]];

                cop(pointValues[point0], half1Values[i]);
                cop(pointValues[point1], half0Values[i]);
            }
        }
    }

    //- Note: hasTransformation is only used for warning messages so
    //  reduction not strictly nessecary.
    //reduce(hasTransformation, orOp<bool>());

    // Synchronize multiple shared points.
    const globalMeshData& pd = mesh.globalData();

    if (pd.nGlobalPoints() > 0)
    {
        if (hasTransformation)
        {
            WarningIn
            (
                "syncTools<class T, class CombineOp>::syncPointList"
                "(const polyMesh&, UList<T>&, const CombineOp&, const T&"
                ", const bool)"
            )   << "There are decomposed cyclics in this mesh with"
                << " transformations." << endl
                << "This is not supported. The result will be incorrect"
                << endl;
        }
        else
        {
            // Values on shared points.
            List<T> sharedPts(pd.nGlobalPoints(), nullValue);

            forAll(pd.sharedPointLabels(), i)
            {
                label meshPointI = pd.sharedPointLabels()[i];

                // Fill my entries in the shared points
                sharedPts[pd.sharedPointAddr()[i]] = pointValues[meshPointI];
            }

            // Combine on master.
            Pstream::listCombineGather(sharedPts, cop);
            Pstream::listCombineScatter(sharedPts);

            // Now we will all have the same information. Merge it back with
            // my local information.
            forAll(pd.sharedPointLabels(), i)
            {
                label meshPointI = pd.sharedPointLabels()[i];
                pointValues[meshPointI] = sharedPts[pd.sharedPointAddr()[i]];
            }
        }
    }
}


template <class T, class CombineOp>
void Foam::syncTools::syncPointList
(
    const polyMesh& mesh,
    const labelList& meshPoints,
    UList<T>& pointValues,
    const CombineOp& cop,
    const T& nullValue,
    const bool applySeparation
)
{
    if (pointValues.size() != meshPoints.size())
    {
        FatalErrorIn
        (
            "syncTools<class T, class CombineOp>::syncPointList"
            "(const polyMesh&, const labelList&, UList<T>&, const CombineOp&"
            ", const T&, const bool)"
        )   << "Number of values " << pointValues.size()
            << " is not equal to the number of points "
            << meshPoints.size() << abort(FatalError);
    }

    if (!hasCouples(mesh.boundaryMesh()))
    {
        return;
    }

    List<T> meshValues(mesh.nPoints(), nullValue);

    forAll(meshPoints, i)
    {
        meshValues[meshPoints[i]] = pointValues[i];
    }

    syncTools::syncPointList
    (
        mesh,
        meshValues,
        cop,            // combine op
        nullValue,      // null value
        applySeparation // separation
    );

    forAll(meshPoints, i)
    {
        pointValues[i] = meshValues[meshPoints[i]];
    }
}


template <class T, class CombineOp>
void Foam::syncTools::syncEdgeList
(
    const polyMesh& mesh,
    UList<T>& edgeValues,
    const CombineOp& cop,
    const T& nullValue,
    const bool applySeparation
)
{
    if (edgeValues.size() != mesh.nEdges())
    {
        FatalErrorIn
        (
            "syncTools<class T, class CombineOp>::syncEdgeList"
            "(const polyMesh&, UList<T>&, const CombineOp&, const T&"
            ", const bool)"
        )   << "Number of values " << edgeValues.size()
            << " is not equal to the number of edges in the mesh "
            << mesh.nEdges() << abort(FatalError);
    }

    const polyBoundaryMesh& patches = mesh.boundaryMesh();

    if (!hasCouples(patches))
    {
        return;
    }

    // Is there any coupled patch with transformation?
    bool hasTransformation = false;

    if (Pstream::parRun())
    {
        // Send

        forAll(patches, patchI)
        {
            if
            (
                isA<processorPolyPatch>(patches[patchI])
             && patches[patchI].nEdges() > 0
            )
            {
                const processorPolyPatch& procPatch =
                    refCast<const processorPolyPatch>(patches[patchI]);

                // Get region per patch edge in neighbouring edge numbers.
                List<T> patchInfo(procPatch.nEdges());

                const labelList& meshEdges = procPatch.meshEdges();
                const labelList& neighbEdges = procPatch.neighbEdges();

                forAll(meshEdges, edgeI)
                {
                    patchInfo[neighbEdges[edgeI]] =
                        edgeValues[meshEdges[edgeI]];
                }

                if (contiguous<T>())
                {
                    OPstream::write
                    (
                        procPatch.neighbProcNo(),
                        reinterpret_cast<const char*>(patchInfo.begin()),
                        patchInfo.byteSize(),
                        true                    // buffered
                    );
                }
                else
                {
                    OPstream toNeighb(procPatch.neighbProcNo());
                    toNeighb << patchInfo;
                }
            }
        }

        // Receive and combine.

        forAll(patches, patchI)
        {
            if
            (
                isA<processorPolyPatch>(patches[patchI])
             && patches[patchI].nEdges() > 0
            )
            {
                const processorPolyPatch& procPatch =
                    refCast<const processorPolyPatch>(patches[patchI]);

                checkTransform(procPatch, applySeparation);

                const labelList& meshEdges = procPatch.meshEdges();

                // Receive from neighbour. Is per patch edge the region of the
                // neighbouring patch edge. 
                List<T> nbrPatchInfo(procPatch.nEdges());

                if (contiguous<T>())
                {
                    IPstream::read
                    (
                        procPatch.neighbProcNo(),
                        reinterpret_cast<char*>(nbrPatchInfo.begin()),
                        nbrPatchInfo.byteSize()
                    );
                }
                else
                {
                    IPstream fromNeighb(procPatch.neighbProcNo());
                    fromNeighb >> nbrPatchInfo;
                }

                if (!procPatch.parallel())
                {
                    hasTransformation = true;
                    transformList(procPatch.reverseT(), nbrPatchInfo);
                }
                else if (applySeparation && procPatch.separated())
                {
                    hasTransformation = true;
                    separateList(-procPatch.separation(), nbrPatchInfo);
                }


                forAll(meshEdges, edgeI)
                {
                    label meshEdgeI = meshEdges[edgeI];

                    cop(edgeValues[meshEdgeI], nbrPatchInfo[edgeI]);
                }
            }
        }
    }

    // Do the cyclics.
    forAll(patches, patchI)
    {
        if (isA<cyclicPolyPatch>(patches[patchI]))
        {
            const cyclicPolyPatch& cycPatch =
                refCast<const cyclicPolyPatch>(patches[patchI]);

            checkTransform(cycPatch, applySeparation);

            const edgeList& coupledEdges = cycPatch.coupledEdges();
            const labelList& meshEdges = cycPatch.meshEdges();

            List<T> half0Values(coupledEdges.size());
            List<T> half1Values(coupledEdges.size());

            forAll(coupledEdges, i)
            {
                const edge& e = coupledEdges[i];

                label meshEdge0 = meshEdges[e[0]];
                label meshEdge1 = meshEdges[e[1]];

                half0Values[i] = edgeValues[meshEdge0];
                half1Values[i] = edgeValues[meshEdge1];
            }

            if (!cycPatch.parallel())
            {
                hasTransformation = true;
                transformList(cycPatch.forwardT(), half0Values);
            }
            else if (applySeparation && cycPatch.separated())
            {
                hasTransformation = true;

                const vectorField& v =
                    cycPatch.coupledPolyPatch::separation();
                separateList(v, half0Values);
                separateList(-v, half1Values);
            }

            forAll(coupledEdges, i)
            {
                const edge& e = coupledEdges[i];

                label meshEdge0 = meshEdges[e[0]];
                label meshEdge1 = meshEdges[e[1]];

                cop(edgeValues[meshEdge0], half1Values[i]);
                cop(edgeValues[meshEdge1], half0Values[i]);
            }
        }
    }

    //- Note: hasTransformation is only used for warning messages so
    //  reduction not strictly nessecary.
    //reduce(hasTransformation, orOp<bool>());

    // Do the multiple shared edges
    const globalMeshData& pd = mesh.globalData();

    if (pd.nGlobalEdges() > 0)
    {
        if (hasTransformation)
        {
            WarningIn
            (
                "syncTools<class T, class CombineOp>::syncEdgeList"
                "(const polyMesh&, UList<T>&, const CombineOp&, const T&"
                ", const bool)"
            )   << "There are decomposed cyclics in this mesh with"
                << " transformations." << endl
                << "This is not supported. The result will be incorrect"
                << endl;
        }
        else
        {
            // Values on shared edges.
            List<T> sharedPts(pd.nGlobalEdges(), nullValue);

            forAll(pd.sharedEdgeLabels(), i)
            {
                label meshEdgeI = pd.sharedEdgeLabels()[i];

                // Fill my entries in the shared edges
                sharedPts[pd.sharedEdgeAddr()[i]] = edgeValues[meshEdgeI];
            }

            // Combine on master.
            Pstream::listCombineGather(sharedPts, cop);
            Pstream::listCombineScatter(sharedPts);

            // Now we will all have the same information. Merge it back with
            // my local information.
            forAll(pd.sharedEdgeLabels(), i)
            {
                label meshEdgeI = pd.sharedEdgeLabels()[i];
                edgeValues[meshEdgeI] = sharedPts[pd.sharedEdgeAddr()[i]];
            }
        }
    }
}


template <class T, class CombineOp>
void Foam::syncTools::syncBoundaryFaceList
(
    const polyMesh& mesh,
    UList<T>& faceValues,
    const CombineOp& cop,
    const bool applySeparation
)
{
    const label nBFaces = mesh.nFaces() - mesh.nInternalFaces();

    if (faceValues.size() != nBFaces)
    {
        FatalErrorIn
        (
            "syncTools<class T, class CombineOp>::syncBoundaryFaceList"
            "(const polyMesh&, UList<T>&, const CombineOp&"
            ", const bool)"
        )   << "Number of values " << faceValues.size()
            << " is not equal to the number of boundary faces in the mesh "
            << nBFaces << abort(FatalError);
    }

    const polyBoundaryMesh& patches = mesh.boundaryMesh();

    if (!hasCouples(patches))
    {
        return;
    }


    if (Pstream::parRun())
    {
        // Send

        forAll(patches, patchI)
        {
            if
            (
                isA<processorPolyPatch>(patches[patchI])
             && patches[patchI].size() > 0
            )
            {
                const processorPolyPatch& procPatch =
                    refCast<const processorPolyPatch>(patches[patchI]);

                label patchStart = procPatch.start()-mesh.nInternalFaces();

                if (contiguous<T>())
                {
                    OPstream::write
                    (
                        procPatch.neighbProcNo(),
                        reinterpret_cast<const char*>
                        (
                            &faceValues[patchStart]
                        ),
                        procPatch.size()*sizeof(T),
                        true                    // buffered

                    );
                }
                else
                {
                    OPstream toNeighb(procPatch.neighbProcNo());
                    toNeighb << 
                        SubList<T>(faceValues, procPatch.size(), patchStart);
                }
            }
        }


        // Receive and combine.

        forAll(patches, patchI)
        {
            if
            (
                isA<processorPolyPatch>(patches[patchI])
             && patches[patchI].size() > 0
            )
            {
                const processorPolyPatch& procPatch =
                    refCast<const processorPolyPatch>(patches[patchI]);

                List<T> nbrPatchInfo(procPatch.size());

                if (contiguous<T>())
                {
                    IPstream::read
                    (
                        procPatch.neighbProcNo(),
                        reinterpret_cast<char*>(nbrPatchInfo.begin()),
                        nbrPatchInfo.byteSize()
                    );
                }
                else
                {
                    IPstream fromNeighb(procPatch.neighbProcNo());
                    fromNeighb >> nbrPatchInfo;
                }

                if (!procPatch.parallel())
                {
                    transformList(procPatch.reverseT(), nbrPatchInfo);
                }
                else if (applySeparation && procPatch.separated())
                {
                    separateList(procPatch.separation(), nbrPatchInfo);
                }


                label bFaceI = procPatch.start()-mesh.nInternalFaces();

                forAll(nbrPatchInfo, i)
                {
                    cop(faceValues[bFaceI++], nbrPatchInfo[i]);
                }
            }
        }
    }

    // Do the cyclics.
    forAll(patches, patchI)
    {
        if (isA<cyclicPolyPatch>(patches[patchI]))
        {
            const cyclicPolyPatch& cycPatch =
                refCast<const cyclicPolyPatch>(patches[patchI]);

            label patchStart = cycPatch.start()-mesh.nInternalFaces();

            label half = cycPatch.size()/2;
            label half1Start = patchStart+half;

            List<T> half0Values(SubList<T>(faceValues, half, patchStart));
            List<T> half1Values(SubList<T>(faceValues, half, half1Start));

            if (!cycPatch.parallel())
            {
                transformList(cycPatch.reverseT(), half1Values);
                transformList(cycPatch.forwardT(), half0Values);
            }
            else if (applySeparation && cycPatch.separated())
            {
                const vectorField& v =
                    cycPatch.coupledPolyPatch::separation();
                separateList(v, half0Values);
                separateList(-v, half1Values);
            }

            label i0 = patchStart;
            forAll(half1Values, i)
            {
                cop(faceValues[i0++], half1Values[i]);
            }

            label i1 = half1Start;
            forAll(half0Values, i)
            {
                cop(faceValues[i1++], half0Values[i]);
            }
        }
    }
}


template <class T, class CombineOp>
void Foam::syncTools::syncFaceList
(
    const polyMesh& mesh,
    UList<T>& faceValues,
    const CombineOp& cop,
    const bool applySeparation
)
{
    if (faceValues.size() != mesh.nFaces())
    {
        FatalErrorIn
        (
            "syncTools<class T, class CombineOp>::syncFaceList"
            "(const polyMesh&, UList<T>&, const CombineOp&"
            ", const bool)"
        )   << "Number of values " << faceValues.size()
            << " is not equal to the number of faces in the mesh "
            << mesh.nFaces() << abort(FatalError);
    }

    SubList<T> bndValues
    (
        faceValues,
        mesh.nFaces()-mesh.nInternalFaces(),
        mesh.nInternalFaces()
    );

    syncBoundaryFaceList
    (
        mesh,
        bndValues,
        cop,
        applySeparation
    );
}


template <class T>
void Foam::syncTools::swapBoundaryFaceList
(
    const polyMesh& mesh,
    UList<T>& faceValues,
    const bool applySeparation
)
{
    syncBoundaryFaceList(mesh, faceValues, eqOp<T>(), applySeparation);
}


template <class T>
void Foam::syncTools::swapFaceList
(
    const polyMesh& mesh,
    UList<T>& faceValues,
    const bool applySeparation
)
{
    syncFaceList(mesh, faceValues, eqOp<T>(), applySeparation);
}


// ************************************************************************* //
