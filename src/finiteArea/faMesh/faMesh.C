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

\*---------------------------------------------------------------------------*/

#include "faMesh.H"
#include "faGlobalMeshData.H"
#include "Time.H"
#include "polyMesh.H"
#include "primitiveMesh.H"
#include "demandDrivenData.H"
#include "IndirectList.H"
#include "areaFields.H"
#include "edgeFields.H"
#include "faMeshLduAddressing.H"
#include "wedgeFaPatch.H"
#include "faPatchData.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(faMesh, 0);

word faMesh::meshSubDir = "faMesh";


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void faMesh::setPrimitiveMeshData()
{
    if (debug)
    {
        Info<< "void faMesh::setPrimitiveMeshData() const : "
            << "Setting primitive data" << endl;
    }

    const indirectPrimitivePatch& bp = patch();

    // Set faMesh edges
    edges_.setSize(bp.nEdges());

    label edgeI = -1;


    label nIntEdges = bp.nInternalEdges();

    for (label curEdge = 0; curEdge < nIntEdges; curEdge++)
    {
        edges_[++edgeI] =
            bp.edges()[curEdge];
    }

    forAll (boundary(), patchI)
    {
        forAll (boundary()[patchI], eI)
        {
            edges_[++edgeI] =
                bp.edges()[boundary()[patchI][eI]];
        }
    }

    nEdges_ = edges_.size();
    nInternalEdges_ = nIntEdges;


    // Set edge owner and neighbour
    edgeOwner_.setSize(nEdges());
    edgeNeighbour_.setSize(nInternalEdges());

    edgeI = -1;

    const labelListList& bpef = bp.edgeFaces();

    for (label curEdge = 0; curEdge < nIntEdges; curEdge++)
    {
        edgeOwner_[++edgeI] = bpef[curEdge][0];

        edgeNeighbour_[edgeI] = bpef[curEdge][1];
    }

    forAll (boundary(), patchI)
    {
        forAll (boundary()[patchI], eI)
        {
            edgeOwner_[++edgeI] = bpef[boundary()[patchI][eI]][0];
        }
    }

    // Set number of faces
    nFaces_ = bp.size();

    // Set number of points
    nPoints_ = bp.nPoints();
}


void faMesh::clearGeomNotAreas() const
{
    if (debug)
    {
        Info<< "void faMesh::clearGeomNotAreas() const : "
            << "Clearing geometry" << endl;
    }

    deleteDemandDrivenData(SPtr_);
    deleteDemandDrivenData(patchPtr_);
    deleteDemandDrivenData(patchStartsPtr_);
    deleteDemandDrivenData(LePtr_);
    deleteDemandDrivenData(magLePtr_);
    deleteDemandDrivenData(centresPtr_);
    deleteDemandDrivenData(edgeCentresPtr_);
    deleteDemandDrivenData(faceAreaNormalsPtr_);
    deleteDemandDrivenData(edgeAreaNormalsPtr_);
    deleteDemandDrivenData(pointAreaNormalsPtr_);
    deleteDemandDrivenData(faceCurvaturesPtr_);
    deleteDemandDrivenData(edgeTransformTensorsPtr_);
}


void faMesh::clearGeom() const
{
    if (debug)
    {
        Info<< "void faMesh::clearGeom() const : "
            << "Clearing geometry" << endl;
    }

    clearGeomNotAreas();
    deleteDemandDrivenData(S0Ptr_);
    deleteDemandDrivenData(S00Ptr_);
    deleteDemandDrivenData(correctPatchPointNormalsPtr_);
}


void faMesh::clearAddressing() const
{
    if (debug)
    {
        Info<< "void faMesh::clearAddressing() const : "
            << "Clearing addressing" << endl;
    }

    deleteDemandDrivenData(lduPtr_);
}


void faMesh::clearOut() const
{
    clearGeom();
    clearAddressing();
    deleteDemandDrivenData(globalMeshDataPtr_);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

faMesh::faMesh(const polyMesh& pMesh)
:
    GeoMesh<polyMesh>(pMesh),
    edgeInterpolation(*this),
    faceLabels_
    (
        IOobject
        (
            "faceLabels",
            time().findInstance(meshDir(), "faceLabels"),
            meshSubDir,
            mesh_,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    boundary_
    (
        IOobject
        (
            "boundary",
            time().findInstance(meshDir(), "boundary"),
            meshSubDir,
            mesh_,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        ),
        *this
    ),
    patchPtr_(NULL),
    lduPtr_(NULL),
    SPtr_(NULL),
    S0Ptr_(NULL),
    S00Ptr_(NULL),
    patchStartsPtr_(NULL),
    LePtr_(NULL),
    magLePtr_(NULL),
    centresPtr_(NULL),
    edgeCentresPtr_(NULL),
    faceAreaNormalsPtr_(NULL),
    edgeAreaNormalsPtr_(NULL),
    pointAreaNormalsPtr_(NULL),
    faceCurvaturesPtr_(NULL),
    edgeTransformTensorsPtr_(NULL),
    correctPatchPointNormalsPtr_(NULL),
    globalMeshDataPtr_(NULL),
    moving_(false),
    curMotionTimeIndex_(pMesh.time().timeIndex())
{
    if (debug)
    {
        Info<< "faMesh::faMesh(...) : "
            << "Creating faMesh from IOobject" << endl;
    }

    setPrimitiveMeshData();

    // Calculate topology for the patches (processor-processor comms etc.)
    boundary_.updateMesh();

    // Calculate the geometry for the patches (transformation tensors etc.)
    boundary_.calcGeometry();

    if (file(pMesh.time().timePath()/"S0"))
    {
        S0Ptr_ = new DimensionedField<scalar, areaMesh>
        (
            IOobject
            (
                "S0",
                time().timeName(),
                meshSubDir,
                mesh_,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            *this
        );
    }
}


// Construct from components without boundary.
faMesh::faMesh
(
    const polyMesh& pMesh,
    const labelList& faceLabels
)
:
    GeoMesh<polyMesh>(pMesh),
    edgeInterpolation(*this),
    faceLabels_
    (
        IOobject
        (
            "faceLabels",
            mesh_.facesInstance(),
            meshSubDir,
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        faceLabels
    ),
    boundary_
    (
        IOobject
        (
            "boundary",
            mesh_.facesInstance(),
            meshSubDir,
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        *this,
        0
    ),
    patchPtr_(NULL),
    lduPtr_(NULL),
    SPtr_(NULL),
    S0Ptr_(NULL),
    S00Ptr_(NULL),
    patchStartsPtr_(NULL),
    LePtr_(NULL),
    magLePtr_(NULL),
    centresPtr_(NULL),
    edgeCentresPtr_(NULL),
    faceAreaNormalsPtr_(NULL),
    edgeAreaNormalsPtr_(NULL),
    pointAreaNormalsPtr_(NULL),
    faceCurvaturesPtr_(NULL),
    edgeTransformTensorsPtr_(NULL),
    correctPatchPointNormalsPtr_(NULL),
    globalMeshDataPtr_(NULL),
    moving_(false),
    curMotionTimeIndex_(pMesh.time().timeIndex())
{
    if (debug)
    {
        Info<< "faMesh::faMesh(...) : "
            << "Creating faMesh from components" << endl;
    }
}


// Construct from definition field
faMesh::faMesh
(
    const polyMesh& m,
    const fileName& defFile
)
:
    GeoMesh<polyMesh>(m),
    edgeInterpolation(*this),
    faceLabels_
    (
        IOobject
        (
            "faceLabels",
            mesh_.facesInstance(),
            meshSubDir,
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        List<label>(0)
    ),
    boundary_
    (
        IOobject
        (
            "boundary",
            mesh_.facesInstance(),
            meshSubDir,
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        *this,
        0
    ),
    patchPtr_(NULL),
    lduPtr_(NULL),
    SPtr_(NULL),
    S0Ptr_(NULL),
    S00Ptr_(NULL),
    patchStartsPtr_(NULL),
    LePtr_(NULL),
    magLePtr_(NULL),
    centresPtr_(NULL),
    edgeCentresPtr_(NULL),
    faceAreaNormalsPtr_(NULL),
    edgeAreaNormalsPtr_(NULL),
    pointAreaNormalsPtr_(NULL),
    faceCurvaturesPtr_(NULL),
    edgeTransformTensorsPtr_(NULL),
    correctPatchPointNormalsPtr_(NULL),
    globalMeshDataPtr_(NULL),
    moving_(false),
    curMotionTimeIndex_(m.time().timeIndex())
{
    if (debug)
    {
        Info<< "faMesh::faMesh(...) : "
            << "Creating faMesh from definition file" << endl;
    }   

    // Reading faMeshDefinition dictionary
    IOdictionary faMeshDefinition
    (
        IOobject
        (
            defFile,
            mesh_.time().constant(),
            meshSubDir,
            mesh_,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );
    
    wordList polyMeshPatches
    (
        faMeshDefinition.lookup("polyMeshPatches")
    );

    dictionary bndDict = faMeshDefinition.subDict("boundary");

    wordList faPatchNames = bndDict.toc();
    
    List<faPatchData> faPatches(faPatchNames.size() + 1);

    forAll (faPatchNames, patchI)
    {
        dictionary curPatchDict =
            bndDict.subDict(faPatchNames[patchI]);

        faPatches[patchI].name_ = faPatchNames[patchI];

        faPatches[patchI].type_ = 
            word(curPatchDict.lookup("type"));

        faPatches[patchI].ownPolyPatchID_ =
            mesh_.boundaryMesh().findPatchID
            (
                word(curPatchDict.lookup("ownerPolyPatch"))
            );

        faPatches[patchI].ngbPolyPatchID_  =
            mesh_.boundaryMesh().findPatchID
            (
                word(curPatchDict.lookup("neighbourPolyPatch"))
            );
    }


    // Setting faceLabels list size
    label size = 0;

    labelList patchIDs(polyMeshPatches.size(), -1);

    forAll (polyMeshPatches, patchI)
    {
        patchIDs[patchI] =
            mesh_.boundaryMesh().findPatchID(polyMeshPatches[patchI]);

        size += mesh_.boundaryMesh()[patchIDs[patchI]].size();
    }

    faceLabels_ = labelList(size, -1);


    // Filling of faceLabels list
    label faceI = -1;

    sort(patchIDs);

    forAll (polyMeshPatches, patchI)
    {
        label start = mesh_.boundaryMesh()[patchIDs[patchI]].start();
        
        label size  = mesh_.boundaryMesh()[patchIDs[patchI]].size();

        for(label i = 0; i < size; i++)
        {
            faceLabels_[++faceI] = start + i;
        }
    }


    // Determination of faPatch ID for each boundary edge.
    // Result is in the bndEdgeFaPatchIDs list
    labelList faceCells(faceLabels_.size(), -1);

    forAll (faceCells, faceI)
    {
        label faceID = faceLabels_[faceI];

        faceCells[faceI] = mesh_.faceOwner()[faceID];
    }

    labelList meshEdges =
        patch().meshEdges
        (
            mesh_.edges(),
            mesh_.cellEdges(),
            faceCells
        );

    const labelListList& edgeFaces = mesh_.edgeFaces();

    const label nTotalEdges = patch().nEdges();
    const label nInternalEdges = patch().nInternalEdges();

    labelList bndEdgeFaPatchIDs(nTotalEdges - nInternalEdges, -1);

    for (label edgeI = nInternalEdges; edgeI < nTotalEdges; edgeI++)
    {
        label curMeshEdge = meshEdges[edgeI];

        labelList curEdgePatchIDs(2, -1);

        label patchI = -1;

        forAll (edgeFaces[curMeshEdge], faceI)
        {
            label curFace = edgeFaces[curMeshEdge][faceI];

            label curPatchID = mesh_.boundaryMesh().whichPatch(curFace);

            if (curPatchID != -1)
            {
                curEdgePatchIDs[++patchI] = curPatchID;
            }
        }

        for(label pI = 0; pI < faPatches.size() - 1; pI++)
        {
            if
            (
                (
                    curEdgePatchIDs[0] == faPatches[pI].ownPolyPatchID_
                 && curEdgePatchIDs[1] == faPatches[pI].ngbPolyPatchID_
                )
             ||
                (
                    curEdgePatchIDs[1] == faPatches[pI].ownPolyPatchID_
                 && curEdgePatchIDs[0] == faPatches[pI].ngbPolyPatchID_
                )
            )
            {
                bndEdgeFaPatchIDs[edgeI - nInternalEdges] = pI;
                break;
            }
        }
    }


    // Set edgeLabels for each faPatch
    for(label pI=0; pI<(faPatches.size()-1); pI++)
    {
        SLList<label> tmpList;

        forAll (bndEdgeFaPatchIDs, eI)
        {
            if (bndEdgeFaPatchIDs[eI] == pI)
            {
                tmpList.append(nInternalEdges + eI);
            }
        }

        faPatches[pI].edgeLabels_ = tmpList;
    }

    // Check for undefined edges
    SLList<label> tmpList;

    forAll (bndEdgeFaPatchIDs, eI)
    {
        if (bndEdgeFaPatchIDs[eI] == -1)
        {
            tmpList.append(nInternalEdges + eI);
        }
    }

    if (tmpList.size() > 0)
    {
        label pI = faPatches.size()-1;

        faPatches[pI].name_ = "undefined";
        faPatches[pI].type_ = "patch";
        faPatches[pI].edgeLabels_ = tmpList;
    }

    // Add good patches to faMesh
    SLList<faPatch*> faPatchLst;

    for(label pI = 0; pI < faPatches.size(); pI++)
    {
        faPatches[pI].dict_.add("type", faPatches[pI].type_);
        faPatches[pI].dict_.add("edgeLabels", faPatches[pI].edgeLabels_);
        faPatches[pI].dict_.add
        (
            "ngbPolyPatchIndex",
            faPatches[pI].ngbPolyPatchID_
        );

        if(faPatches[pI].edgeLabels_.size() > 0)
        {
            faPatchLst.append
            (
                faPatch::New
                (
                    faPatches[pI].name_,
                    faPatches[pI].dict_,
                    pI,
                    boundary()
                ).ptr()
            );
        }
    }

    addFaPatches(List<faPatch*>(faPatchLst));

    setPrimitiveMeshData();    

    // Calculate topology for the patches (processor-processor comms etc.)
    boundary_.updateMesh();

    // Calculate the geometry for the patches (transformation tensors etc.)
    boundary_.calcGeometry();

    if (file(mesh_.time().timePath()/"S0"))
    {
        S0Ptr_ = new DimensionedField<scalar, areaMesh>
        (
            IOobject
            (
                "S0",
                time().timeName(),
                meshSubDir,
                mesh_,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            *this
        );
    }
}


// Construct from polyPatch
faMesh::faMesh
(
    const polyMesh& m,
    const label polyPatchID
)
:
    GeoMesh<polyMesh>(m),
    edgeInterpolation(*this),
    faceLabels_
    (
        IOobject
        (
            "faceLabels",
            mesh_.facesInstance(),
            meshSubDir,
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        labelList(m.boundaryMesh()[polyPatchID].size(), -1)
    ),
    boundary_
    (
        IOobject
        (
            "boundary",
            mesh_.facesInstance(),
            meshSubDir,
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        *this,
        0
    ),
    patchPtr_(NULL),
    lduPtr_(NULL),
    SPtr_(NULL),
    S0Ptr_(NULL),
    S00Ptr_(NULL),
    patchStartsPtr_(NULL),
    LePtr_(NULL),
    magLePtr_(NULL),
    centresPtr_(NULL),
    edgeCentresPtr_(NULL),
    faceAreaNormalsPtr_(NULL),
    edgeAreaNormalsPtr_(NULL),
    pointAreaNormalsPtr_(NULL),
    faceCurvaturesPtr_(NULL),
    edgeTransformTensorsPtr_(NULL),
    correctPatchPointNormalsPtr_(NULL),
    globalMeshDataPtr_(NULL),
    moving_(false),
    curMotionTimeIndex_(m.time().timeIndex())
{
    if (debug)
    {
        Info<< "faMesh::faMesh(...) : "
            << "Creating faMesh from polyPatch" << endl;
    }

    // Set faceLabels
    forAll(faceLabels_, faceI)
    {
        faceLabels_[faceI] = 
            mesh_.boundaryMesh()[polyPatchID].start() + faceI;
    }

    // Add one faPatch
    const indirectPrimitivePatch& bp = patch();

    const label nTotalEdges = bp.nEdges();

    const label nInternalEdges = bp.nInternalEdges();

    labelList edgeLabels(nTotalEdges-nInternalEdges, -1);

    forAll(edgeLabels, edgeI)
    {
        edgeLabels[edgeI] = nInternalEdges + edgeI;
    }

    dictionary patchDict;

    patchDict.add("type", "patch");
    patchDict.add("edgeLabels", edgeLabels);
    patchDict.add("ngbPolyPatchIndex", -1);
    
    List<faPatch*> faPatchLst(1);

    faPatchLst[0] = 
        faPatch::New("default", patchDict, 0, boundary()).ptr();

    addFaPatches(faPatchLst);

    setPrimitiveMeshData();    

    // Calculate topology for the patches (processor-processor comms etc.)
    boundary_.updateMesh();

    // Calculate the geometry for the patches (transformation tensors etc.)
    boundary_.calcGeometry();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

faMesh::~faMesh()
{
    clearOut();
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

fileName faMesh::meshDir() const
{
    return mesh_.dbDir()/meshSubDir;
}


const Time& faMesh::time() const
{
    return mesh_.time();
}


const indirectPrimitivePatch& faMesh::patch() const
{
    if (!patchPtr_)
    {
        patchPtr_ = new indirectPrimitivePatch
        (
            IndirectList<face>
            (
                mesh_.allFaces(),
                faceLabels_
            ),
            mesh_.allPoints()
        );
    }
        
    return *patchPtr_;
}


indirectPrimitivePatch& faMesh::patch()
{
    if (!patchPtr_)
    {
        patchPtr_ = new indirectPrimitivePatch
        (
            IndirectList<face>
            (
                mesh_.allFaces(), 
                faceLabels_
            ),
            mesh_.allPoints()
        );
    }
        
    return *patchPtr_;
}


const pointField& faMesh::points() const
{
    return patch().localPoints();
}


const edgeList& faMesh::edges() const
{
    return edges_;
}


const faceList& faMesh::faces() const
{
    return patch().localFaces();
}


void faMesh::addFaPatches(const List<faPatch*>& p)
{
    if (debug)
    {
        Info<< "void faMesh::addFaPatches(const List<faPatch*>& p) : "
            << "Adding patches to faMesh" << endl;
    }

    if (boundary().size() > 0)
    {
        FatalErrorIn("void faMesh::addPatches(const List<faPatch*>& p)")
            << "boundary already exists"
            << abort(FatalError);
    }

    boundary_.setSize(p.size());

    forAll(p, patchI)
    {
        boundary_.set(patchI, p[patchI]);
    }

    setPrimitiveMeshData();

    boundary_.checkDefinition();

    // Calculate the geometry for the patches (transformation tensors etc.)
    boundary_.calcGeometry();
}


const objectRegistry& faMesh::db() const
{
    return mesh_.db();
}


const faBoundaryMesh& faMesh::boundary() const
{
    return boundary_;
}


const labelList& faMesh::patchStarts() const
{
    if (!patchStartsPtr_)
    {
        calcPatchStarts();
    }

    return *patchStartsPtr_;
}


const edgeVectorField& faMesh::Le() const
{
    if (!LePtr_)
    {
        calcLe();
    }

    return *LePtr_;
}


const edgeScalarField& faMesh::magLe() const
{
    if (!magLePtr_)
    {
        calcMagLe();
    }

    return *magLePtr_;
}


const areaVectorField& faMesh::centres() const
{
    if (!centresPtr_)
    {
        calcCentres();
    }

    return *centresPtr_;
}


const edgeVectorField& faMesh::edgeCentres() const
{
    if (!edgeCentresPtr_)
    {
        calcEdgeCentres();
    }

    return *edgeCentresPtr_;
}


const DimensionedField<scalar, areaMesh>& faMesh::S() const
{
    if (!SPtr_)
    {
        calcS();
    }

    return *SPtr_;
}


const DimensionedField<scalar, areaMesh>& faMesh::S0() const
{
    if (!S0Ptr_)
    {
        FatalErrorIn("faMesh::S0() const")
            << "S0 is not available"
            << abort(FatalError);
    }

    return *S0Ptr_;
}


const DimensionedField<scalar, areaMesh>& faMesh::S00() const
{
    if (!S00Ptr_)
    {
        S00Ptr_ = new DimensionedField<scalar, areaMesh>
        (
            IOobject
            (
                "S00",
                time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            S0()
        );

        S0Ptr_->writeOpt() = IOobject::AUTO_WRITE;
    }

    return *S00Ptr_;
}


const areaVectorField& faMesh::faceAreaNormals() const
{
    if (!faceAreaNormalsPtr_)
    {
        calcFaceAreaNormals();
    }

    return *faceAreaNormalsPtr_;
}


const edgeVectorField& faMesh::edgeAreaNormals() const
{
    if (!edgeAreaNormalsPtr_)
    {
        calcEdgeAreaNormals();
    }

    return *edgeAreaNormalsPtr_;
}


const vectorField& faMesh::pointAreaNormals() const
{
    if (!pointAreaNormalsPtr_)
    {
        calcPointAreaNormals();
    }

    return *pointAreaNormalsPtr_;
}


const areaScalarField& faMesh::faceCurvatures() const
{
    if (!faceCurvaturesPtr_)
    {
        calcFaceCurvatures();
    }

    return *faceCurvaturesPtr_;
}


const FieldField<Field, tensor>& faMesh::edgeTransformTensors() const
{
    if (!edgeTransformTensorsPtr_)
    {
        calcEdgeTransformTensors();
    }

    return *edgeTransformTensorsPtr_;
}


// Return global mesh data
const faGlobalMeshData& faMesh::globalData() const
{
    if (!globalMeshDataPtr_)
    {
        globalMeshDataPtr_ = new faGlobalMeshData(*this);
    }

    return *globalMeshDataPtr_;
}


const lduAddressing& faMesh::lduAddr() const
{
    if (!lduPtr_)
    {
        calcLduAddressing();
    }

    return *lduPtr_;
}


tmp<scalarField> faMesh::movePoints(const vectorField& newPoints)
{
    moving_ = true;

    // Grab old time areas if the time has been incremented
    if (curMotionTimeIndex_ < operator()().time().timeIndex())
    {
        if (S00Ptr_ && S0Ptr_)
        {
            *S00Ptr_ = *S0Ptr_;
        }

        if (S0Ptr_)
        {
            *S0Ptr_ = S();
        }
        else
        {
            S0Ptr_ = new DimensionedField<scalar, areaMesh>
            (
                IOobject
                (
                    "S0",
                    time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                S()
            );
        }

        curMotionTimeIndex_ = operator()().time().timeIndex();
    }

    clearGeomNotAreas();

    patch().movePoints(newPoints);
    boundary_.movePoints(newPoints);
    edgeInterpolation::movePoints();

    tmp<scalarField> tresult(new scalarField(nEdges(), 0.0));
    
    return tresult;
}


//- Return true if point normals should be corrected for a patch
bool faMesh::correctPatchPointNormals(const label patchID) const
{
    if((patchID < 0) || (patchID >= boundary().size()))
    {
        FatalErrorIn
        (
            "bool correctPatchPointNormals(const label patchID) const"
        )   << "patchID is not in the valid range"
            << abort(FatalError);
    }

    if(correctPatchPointNormalsPtr_)
    {
        return (*correctPatchPointNormalsPtr_)[patchID];
    }

    return false;
}


//- Set patch point normals corrections
boolList& faMesh::correctPatchPointNormals() const
{
    if(!correctPatchPointNormalsPtr_)
    {
        correctPatchPointNormalsPtr_ = 
            new boolList(boundary().size(), false);
    }

    return *correctPatchPointNormalsPtr_;
}


bool faMesh::write() const
{
    faceLabels_.write();
    boundary_.write();

    return false;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

bool faMesh::operator!=(const faMesh& m) const
{
    return &m != this;
}

bool faMesh::operator==(const faMesh& m) const
{
    return &m == this;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
