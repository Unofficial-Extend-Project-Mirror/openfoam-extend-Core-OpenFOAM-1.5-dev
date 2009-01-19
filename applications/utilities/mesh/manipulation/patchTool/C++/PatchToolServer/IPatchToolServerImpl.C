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

#include <unistd.h>

// Foam header files.
#include "long.H"
#include "dictionary.H"
#include "wordList.H"
#include "stringList.H"
#include "fileNameList.H"
#include "IFstream.H"
#include "OStringStream.H"
#include "instantList.H"
#include "IOPtrList.H"
#include "OSspecific.H"
#include "polyMesh.H"
#include "Time.H"

// FoamX header files.
#include "IPatchToolServerImpl.H"
#include "LogEntry.H"
#include "NameServer.H"
#include "Orb.H"
#include "Paths.H"


// Namespaces
using namespace FoamXServer::PatchToolServer;
#include "FoamXNameSpaces.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Copy boolList into Corba BoolList
void FoamX::IPatchToolServerImpl::copy
(
    const boolList& lst,
    BoolList_out& corbaList
)
{
    corbaList->length(lst.size());

    CORBA::Long vertI = 0;
    forAll(lst, i)
    {

        // As recommended by 'Advanced Corba programming with C++' : map
        // bool as integer 1(true) or 0(false)
        if (lst[i])
        {
            (*corbaList)[vertI++] = 1;
        }
        else
        {
            (*corbaList)[vertI++] = 0;
        }
    }
}


// Copy labelList into Corba LongList
void FoamX::IPatchToolServerImpl::copy
(
    const labelList& lst,
    LongList_out& corbaList
)
{
    corbaList->length(lst.size());

    CORBA::Long vertI = 0;
    forAll(lst, i)
    {
        (*corbaList)[vertI++] = lst[i];
    }
}


// Copy scalarField into Corba FloatList
void FoamX::IPatchToolServerImpl::copy
(
    const scalarField& field,
    FloatList_out& corbaList
)
{
    corbaList->length(field.size());

    CORBA::Long vertI = 0;
    forAll(field, sfI)
    {
        (*corbaList)[vertI++] = field[sfI];
    }
}


// Copy vectorField into Corba FloatList
void FoamX::IPatchToolServerImpl::copy
(
    const vectorField& field,
    FloatList_out& corbaList
)
{
    corbaList->length(3*field.size());

    CORBA::Long floatI = 0;
    forAll(field, pointI)
    {
        const point& coord = field[pointI];
        (*corbaList)[floatI++] = float(coord.x());
        (*corbaList)[floatI++] = float(coord.y());
        (*corbaList)[floatI++] = float(coord.z());
    }
}


// Convert Corba LongList into labelList
Foam::labelList FoamX::IPatchToolServerImpl::copy(const LongList& elems)
{
    labelList result(elems.length());

    forAll(result, elemI)
    {
        result[elemI] = elems[elemI];
    }
    return result;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

FoamX::IPatchToolServerImpl::IPatchToolServerImpl
(
    Orb& orb,
    const fileName& rootDir,
    const fileName& caseName
)
:
    rootDir_(rootDir),
    caseName_(caseName),
    db_(Time::controlDictName, rootDir_, caseName_),
    bMesh_(),
    hostContext_(Foam::hostName()),
    userContext_(Foam::userName()),
    caseDictName_(rootDir_/caseName_/"system/controlDict"),
    orb_(orb)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::IPatchToolServerImpl"
        "(Orb& orb, const fileName& rootDir, const fileName& caseName)";

    LogEntry log(functionName, __FILE__, __LINE__);


    try
    {
        // Connect to name server.
        NameServer fxNameServer(orb_.orbPtr_);

        //----------------------------------------------------------------------
        // Register object with the naming service. Will throw an exception
        // if any errors occurs.

//objectName_ =
//    "PatchToolServer"/userContext_/rootDir_.name()/caseName_;
//
//// Create intermediate contexts; do not fail if context already exists.
//fxNameServer.createContexts(objectName_.path(), false);

        objectName_ = "PatchToolServer";


        // Bind object under key hostName/userName/caseroot/casename.
        FoamXServer::PatchToolServer::IPatchToolServer_var caseRef =
            _this();

        // Throw exception if already bound.
        Info<< "Bind under:" << objectName_ << Foam::endl;
        fxNameServer.bindObject(objectName_, caseRef, true);

        // Decrement the reference count of the object implementation, so
        // that it will be properly cleaned up when the POA has determined
        // that it is no longer needed.
        _remove_ref();
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

FoamX::IPatchToolServerImpl::~IPatchToolServerImpl()
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::~IPatchToolServerImpl()";

    LogEntry log(functionName, __FILE__, __LINE__);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

char* FoamX::IPatchToolServerImpl::caseRoot()
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::caseRoot()";

    LogEntry log(functionName, __FILE__, __LINE__);

    // Duplicate and return.
    return CORBA::string_dup(rootDir_.c_str());
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

char* FoamX::IPatchToolServerImpl::caseName()
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::caseName()";

    LogEntry log(functionName, __FILE__, __LINE__);

    // Duplicate and return.
    return CORBA::string_dup(caseName_.c_str());
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

FoamXServer::StringList* FoamX::IPatchToolServerImpl::availableTimeSteps()
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::availableTimeSteps()";

    LogEntry log(functionName, __FILE__, __LINE__);

    StringList* timeStepList = NULL;

    try
    {
        // Get list of time directories.
        instantList times = db_.times();

        // Copy time steps into a new string list and return.
        timeStepList = new StringList();
        timeStepList->length(times.size());
        forAll(times, i)
        {
            (*timeStepList)[i] = times[i].name().c_str();
        }
    }
    CATCH_ALL(functionName);

    return timeStepList;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

char* FoamX::IPatchToolServerImpl::getTime()
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::getTime()";

    try
    {
        // Duplicate and return.
        return CORBA::string_dup(db_.timeName().c_str());
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::setTime
(
    const char* timeName,
    const CORBA::Long timeIndex
)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::setTime(const char*, const CORBA::Long)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        // Do stuff
        scalar timeValue(readScalar(IStringStream(timeName)()));

        db_.setTime(timeValue, timeIndex);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::read()
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::read()";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        polyMesh mesh
        (
            IOobject
            (
                polyMesh::defaultRegion,
                db_.constant(),
                db_
            )
        );

        bMesh_.read(mesh);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::readTriSurface(const char *fName)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::readTriSurface()";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        if (!exists(fName))
        {
            throw FoamXError
            (
                FoamXServer::E_FAIL,
                Foam::string("Cannot find file") + fName,
                functionName,
                __FILE__, __LINE__
            );
        }

        bMesh_.readTriSurface(fName);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::write()
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::write()";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        // Read original mesh
        polyMesh mesh
        (
            IOobject
            (
                polyMesh::defaultRegion,
                db_.constant(),
                db_
            )
        );

        // Obtain nearest face in bMesh for each boundary face in mesh
        labelList nearest
        (
            bMesh_.getNearest
            (
                mesh,
                vector(GREAT, GREAT, GREAT)
            )
        );

        db_++;

        // Update mesh with new patches and insert face changes into meshMod.
        bMesh_.patchify
        (
            nearest,                // nearest bMesh face
            mesh.boundaryMesh(),    // original boundary faces
            mesh
        );

        // Write resulting mesh
        mesh.write();
    }
    CATCH_ALL(functionName);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::writeTriSurface(const char *fName)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::writeTriSurface()";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        bMesh_.writeTriSurface(fName);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::getPoints(FloatList_out allPoints)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::getPoints(FloatList_out)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        // Copy points into Corba return structure

        allPoints = new FloatList();

        copy(bMesh_.mesh().points(), allPoints);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CORBA::Long FoamX::IPatchToolServerImpl::getNFaces()
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::getNFaces()";

    LogEntry log(functionName, __FILE__, __LINE__);

    CORBA::Long nFaces = 0;

    try
    {
        nFaces = bMesh_.mesh().size();
    }
    CATCH_ALL(functionName);

    return nFaces;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CORBA::Long FoamX::IPatchToolServerImpl::getNEdges()
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::getNEdges()";

    LogEntry log(functionName, __FILE__, __LINE__);

    CORBA::Long nEdges = 0;

    try
    {
        nEdges = bMesh_.mesh().nEdges();
    }
    CATCH_ALL(functionName);

    return nEdges;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::getEdges(LongList_out verts)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::getEdges(LongList_out)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        verts = new LongList();

        const edgeList& edges = bMesh_.mesh().edges();

        verts->length(2 * edges.size());

        label vertI = 0;

        forAll(edges, edgeI)
        {
            const edge& e = edges[edgeI];

            (*verts)[vertI++] = e.start();
            (*verts)[vertI++] = e.end();
        }
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::setFeatureEdges(const CORBA::Double minCos)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::setFeatureEdges(const CORBA::Double)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        bMesh_.setFeatureEdges(minCos);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::getFeaturePoints(FloatList_out points)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::getFeaturePoints(FloatList_out)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        // Copy points into Corba return structure

        points = new FloatList();

        copy(bMesh_.featurePoints(), points);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::getFeatureEdges(LongList_out verts)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::getFeatureEdges(LongList_out)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        verts = new LongList();

        const edgeList& edges = bMesh_.featureEdges();

        verts->length(2 * edges.size());

        label vertI = 0;

        forAll(edges, edgeI)
        {
            const edge& e = edges[edgeI];

            (*verts)[vertI++] = e.start();
            (*verts)[vertI++] = e.end();
        }
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::getFeatureToEdge(LongList_out edgeLabels)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::getFeatureToEdge(LongList_out)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        edgeLabels = new LongList();

        copy(bMesh_.featureToEdge(), edgeLabels);
    }
    CATCH_ALL(functionName);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::getEdgeToFeature(LongList_out featLabels)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::getEdgeToFeature(LongList_out)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        featLabels = new LongList();

        copy(bMesh_.edgeToFeature(), featLabels);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::getFeatureSegments
(
    LongLongList_out featLabels
)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::getFeatureSegments(LongLongList_out)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        const labelListList& segments = bMesh_.featureSegments();

        featLabels = new LongLongList();

        featLabels->length(segments.size());

        forAll(segments, segmentI)
        {
            const labelList& segment = segments[segmentI];

            LongList& corbaSegment = (*featLabels)[segmentI];

            corbaSegment.length(segment.size());

            forAll(segment, elemI)
            {
                corbaSegment[elemI] = segment[elemI];
            }
        }
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::setExtraEdges(const CORBA::Long edgeI)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::setExtraEdges(const CORBA::Long)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        bMesh_.setExtraEdges(edgeI);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::getExtraEdges(LongList_out edgeLabels)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::getExtraEdges(LongList_out)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        edgeLabels = new LongList();

        copy(bMesh_.extraEdges(), edgeLabels);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CORBA::Long FoamX::IPatchToolServerImpl::whichPatch(const CORBA::Long faceI)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::whichPatch(const long)";

    LogEntry log(functionName, __FILE__, __LINE__);

    CORBA::Long patchI = 0;

    try
    {
        patchI = bMesh_.whichPatch(faceI);
    }
    CATCH_ALL(functionName);

    return patchI;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CORBA::Long FoamX::IPatchToolServerImpl::findPatchID(const char* patchName)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::findPatchID(const char*)";

    LogEntry log(functionName, __FILE__, __LINE__);

    CORBA::Long patchI = 0;

    try
    {
        patchI = bMesh_.findPatchID(Foam::word(patchName));
    }
    CATCH_ALL(functionName);

    return patchI;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::getPatches(SurfacePatchList_out patches)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::getPatches(SurfacePatchList_out)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        const PtrList<boundaryPatch>& bPatches = bMesh_.patches();

        patches = new SurfacePatchList();

        patches->length(bPatches.size());

        forAll(bPatches, bPatchI)
        {
            const boundaryPatch& sp = bPatches[bPatchI];

            (*patches)[bPatchI].name =
                CORBA::string_dup(sp.name().c_str());

            (*patches)[bPatchI].geometricType =
                CORBA::string_dup(sp.physicalType().c_str());

            (*patches)[bPatchI].size = sp.size();

            (*patches)[bPatchI].start = sp.start();
        }
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::addPatch(const char* patchName)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::addPatch(const char*)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        bMesh_.addPatch(patchName);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::deletePatch(const char* patchName)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::deletePatch(const char*)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        bMesh_.deletePatch(patchName);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::changePatchType
(
    const char* patchName,
    const char* patchType
)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::changePatchType"
        "(const char*, const char*)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        bMesh_.changePatchType(patchName, patchType);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::changeFaces
(   
    const LongList& patchIDs,
    LongList_out oldToNew
)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::changeFaces"
        "(const LongList&, LongList_out)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        // Old to new map.
        labelList faceMap(patchIDs.length());

        bMesh_.changeFaces(copy(patchIDs), faceMap);

        oldToNew = new LongList();

        copy(faceMap, oldToNew);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CORBA::Long FoamX::IPatchToolServerImpl::getNTris
(
    const CORBA::Long startFaceI,
    const CORBA::Long nFaces,
    LongList_out nTris
)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::getNTris"
        "(const CORBA::Long, const CORBA::Long, LongList_out)";

    LogEntry log(functionName, __FILE__, __LINE__);

    CORBA::Long totalNTris = 0;

    try
    {
        labelList foamNTris;

        totalNTris = bMesh_.getNTris(startFaceI, nFaces, foamNTris);

        // Copy foamNtris into corba array
        nTris = new LongList();

        copy(foamNTris, nTris);
    }
    CATCH_ALL(functionName);

    return totalNTris;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::triangulate
(
    const CORBA::Long startFaceI,
    const CORBA::Long nFaces,
    const CORBA::Long totalNTris,
    LongList_out triVerts
)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::triangulate"
        "(const CORBA::Long, const CORBA::Long, const CORBA::Long"
        ", LongList_out)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        // Triangulate face into foamVerts
        labelList foamVerts;

        bMesh_.triangulate
        (
            startFaceI,
            nFaces,
            totalNTris,
            foamVerts
        );

        // Copy foamVerts into corba array
        triVerts = new LongList();

        copy(foamVerts, triVerts);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CORBA::Long FoamX::IPatchToolServerImpl::getNPoints
(
    const CORBA::Long startFaceI,
    const CORBA::Long nFaces
)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::getNPoints"
        "(const CORBA::Long, const CORBA::Long)";

    LogEntry log(functionName, __FILE__, __LINE__);

    CORBA::Long nPoints = 0;

    try
    {
        nPoints = bMesh_.getNPoints(startFaceI, nFaces);
    }
    CATCH_ALL(functionName);

    return nPoints;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::triangulateLocal
(
    const CORBA::Long startFaceI,
    const CORBA::Long nFaces,
    const CORBA::Long totalNTris,
    LongList_out triVerts,
    LongList_out localToGlobal
)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::triangulateLocal"
        "(const CORBA::Long, const CORBA::Long, const CORBA::Long"
        ", LongList_out, LongList_out)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        // Triangulate face into foamVerts
        labelList foamVerts;
        labelList foamVertMap;

        bMesh_.triangulateLocal
        (
            startFaceI,
            nFaces,
            totalNTris,
            foamVerts,
            foamVertMap
        );

        // Copy foamVerts into corba array
        triVerts = new LongList();

        copy(foamVerts, triVerts);

        localToGlobal = new LongList();

        copy(foamVertMap, localToGlobal);

    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::markFaces
(
    const LongList& protectedEdges,
    const CORBA::Long faceI,
    BoolList_out visited
)
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::markFaces"
        "(const LongList&, const CORBA::Long, BoolList_out)";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        boolList visitedFace(bMesh_.mesh().size(), false);

        bMesh_.markFaces(copy(protectedEdges), faceI, visitedFace);

        visited = new BoolList();

        copy(visitedFace, visited);
    }
    CATCH_ALL(functionName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void FoamX::IPatchToolServerImpl::close()
{
    static const char* functionName =
        "FoamX::IPatchToolServerImpl::close()";

    LogEntry log(functionName, __FILE__, __LINE__);

    try
    {
        Info<< "Close:" << functionName << Foam::endl;

        // Unregister object from the naming service.
        NameServer fxNameServer(orb_.orbPtr_);
        fxNameServer.unbindObject(objectName_);

//        // Remove contexts
//        fxNameServer.removeContexts
//        (
//            hostContext_,
//            objectName_.path()
//        );

        fxNameServer.disconnect();

        // Shutdown th'Orb.
        orb_.orbPtr_->shutdown(false);       // Do not wait for completion.
    }
    CATCH_ALL(functionName);
}

// ************************************************************************* //
