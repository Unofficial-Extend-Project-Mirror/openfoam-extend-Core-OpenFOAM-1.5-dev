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

#include "processorPolyPatch.H"
#include "polyBoundaryMesh.H"
#include "addToRunTimeSelectionTable.H"
#include "dictionary.H"
#include "SubField.H"
#include "demandDrivenData.H"
#include "matchPoints.H"
#include "OFstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(processorPolyPatch, 0);

addToRunTimeSelectionTable(polyPatch, processorPolyPatch, Istream);
addToRunTimeSelectionTable(polyPatch, processorPolyPatch, dictionary);

scalar processorPolyPatch::areaMatchTol
(
    debug::tolerances("processorMatchTol", 1e-4)
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
processorPolyPatch::processorPolyPatch
(
    const word& name,
    const label size,
    const label start,
    const label index,
    const polyBoundaryMesh& bm,
    const int myProcNo,
    const int neighbProcNo
)
:
    coupledPolyPatch(name, size, start, index, bm),
    myProcNo_(myProcNo),
    neighbProcNo_(neighbProcNo),
    neighbFaceCentres_(),
    neighbFaceAreas_(),
    neighbFaceCellCentres_(),
    neighbPointsPtr_(NULL),
    neighbEdgesPtr_(NULL)
{}


// Construct from Istream
processorPolyPatch::processorPolyPatch
(
    Istream& is,
    const label index,
    const polyBoundaryMesh& bm
)
:
    coupledPolyPatch(is, index, bm),
    myProcNo_(readLabel(is)),
    neighbProcNo_(readLabel(is)),
    neighbFaceCentres_(),
    neighbFaceAreas_(),
    neighbFaceCellCentres_(),
    neighbPointsPtr_(NULL),
    neighbEdgesPtr_(NULL)
{}


// Construct from dictionary
processorPolyPatch::processorPolyPatch
(
    const word& name,
    const dictionary& dict,
    const label index,
    const polyBoundaryMesh& bm
)
:
    coupledPolyPatch(name, dict, index, bm),
    myProcNo_(readLabel(dict.lookup("myProcNo"))),
    neighbProcNo_(readLabel(dict.lookup("neighbProcNo"))),
    neighbFaceCentres_(),
    neighbFaceAreas_(),
    neighbFaceCellCentres_(),
    neighbPointsPtr_(NULL),
    neighbEdgesPtr_(NULL)
{}


//- Construct as copy, resetting the boundary mesh
processorPolyPatch::processorPolyPatch
(
    const processorPolyPatch& pp,
    const polyBoundaryMesh& bm
)
:
    coupledPolyPatch(pp, bm),
    myProcNo_(pp.myProcNo_),
    neighbProcNo_(pp.neighbProcNo_),
    neighbFaceCentres_(),
    neighbFaceAreas_(),
    neighbFaceCellCentres_(),
    neighbPointsPtr_(NULL),
    neighbEdgesPtr_(NULL)
{}


//- Construct as copy, resetting the face list and boundary mesh data
processorPolyPatch::processorPolyPatch
(
    const processorPolyPatch& pp,
    const polyBoundaryMesh& bm,
    const label index,
    const label newSize,
    const label newStart
)
:
    coupledPolyPatch(pp, bm, index, newSize, newStart),
    myProcNo_(pp.myProcNo_),
    neighbProcNo_(pp.neighbProcNo_),
    neighbFaceCentres_(),
    neighbFaceAreas_(),
    neighbFaceCellCentres_(),
    neighbPointsPtr_(NULL),
    neighbEdgesPtr_(NULL)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

processorPolyPatch::~processorPolyPatch()
{
    deleteDemandDrivenData(neighbPointsPtr_);
    deleteDemandDrivenData(neighbEdgesPtr_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void processorPolyPatch::initGeometry()
{
    if (Pstream::parRun())
    {
        OPstream toNeighbProc
        (
            neighbProcNo(),
          + 3*(sizeof(label) + size()*sizeof(vector))
        );

        toNeighbProc
            << faceCentres()
            << faceAreas()
            << faceCellCentres();
    }
}


void processorPolyPatch::calcGeometry()
{
    if (Pstream::parRun())
    {
        {
            IPstream fromNeighbProc
            (
                neighbProcNo(),
                3*(sizeof(label) + size()*sizeof(vector))
            );
            fromNeighbProc
                >> neighbFaceCentres_
                >> neighbFaceAreas_
                >> neighbFaceCellCentres_;
        }

        scalarField magSf = mag(faceAreas());

        forAll(magSf, facei)
        {
            scalar nmagSf = mag(neighbFaceAreas_[facei]);
            scalar avSf = (magSf[facei] + nmagSf)/2.0;

            if
            (
                avSf > VSMALL
             && mag(magSf[facei] - nmagSf)/avSf > areaMatchTol
            )
            {
                FatalErrorIn
                (
                    "processorPolyPatch::calcGeometry()"
                )   << "face " << facei << " area does not match neighbour by "
                    << mag(magSf[facei] - nmagSf)/avSf
                    << " with tolerance " << areaMatchTol
                    << ".  Possible face ordering problem.  " << nl
                    << " patch: " << name() << " mesh face: " << start() + facei
                    << exit(FatalError);
            }
        }

        calcTransformTensors
        (
            faceCentres(),
            neighbFaceCentres_,
            faceNormals(),
            neighbFaceAreas_/(mag(neighbFaceAreas_) + VSMALL)
        );
    }
}


void processorPolyPatch::initMovePoints(const pointField& p)
{
    polyPatch::movePoints(p);
    initGeometry();
}


void processorPolyPatch::movePoints(const pointField&)
{
    calcGeometry();
}


void processorPolyPatch::initUpdateMesh()
{
    // For completeness
    polyPatch::initUpdateMesh();

    deleteDemandDrivenData(neighbPointsPtr_);
    deleteDemandDrivenData(neighbEdgesPtr_);

    if (Pstream::parRun())
    {
        // Express all points as patch face and index in face.
        labelList patchFace(nPoints());
        labelList indexInFace(nPoints());

        for (label patchPointI = 0; patchPointI < nPoints(); patchPointI++)
        {
            label faceI = pointFaces()[patchPointI][0];

            patchFace[patchPointI] = faceI;

            const face& f = localFaces()[faceI];

            indexInFace[patchPointI] = findIndex(f, patchPointI);
        }

        OPstream toNeighbProc
        (
            neighbProcNo(),
            3*sizeof(label)
          + 2*nPoints()*sizeof(label)
          + nEdges()*sizeof(edge)
        );

        toNeighbProc
            << patchFace
            << indexInFace
            << edges();
    }
}


void processorPolyPatch::updateMesh()
{
    // For completeness
    polyPatch::updateMesh();

    if (Pstream::parRun())
    {
        labelList nbrPatchFace(nPoints());
        labelList nbrIndexInFace(nPoints());
        edgeList nbrEdges(nEdges());

        {
            // Note cannot predict exact size since edgeList not (yet) sent as
            // binary entity but as List of edges.
            IPstream fromNeighbProc(neighbProcNo());

            fromNeighbProc
                >> nbrPatchFace
                >> nbrIndexInFace
                >> nbrEdges;
        }

        if (nbrPatchFace.size() == nPoints() && nbrEdges.size() == nEdges())
        {
            // Convert neighbour edges and indices into face back into
            // my edges and points.
            neighbPointsPtr_ = new labelList(nPoints());
            labelList& neighbPoints = *neighbPointsPtr_;

            // Inverse of neighbPoints so from neighbour point to current point.
            labelList nbrToThis(nPoints(), -1);

            forAll(nbrPatchFace, nbrPointI)
            {
                // Find face and index in face on this side.
                const face& f = localFaces()[nbrPatchFace[nbrPointI]];
                label index = (f.size() - nbrIndexInFace[nbrPointI]) % f.size();
                label patchPointI = f[index];

                neighbPoints[patchPointI] = nbrPointI;
                nbrToThis[nbrPointI] = patchPointI;
            }

            // Convert edges.
            neighbEdgesPtr_ = new labelList(nEdges());
            labelList& neighbEdges = *neighbEdgesPtr_;

            forAll(nbrEdges, nbrEdgeI)
            {
                const edge& nbrEdge = nbrEdges[nbrEdgeI];

                // Get edge in local point numbering
                edge e(nbrToThis[nbrEdge[0]], nbrToThis[nbrEdge[1]]);

                // Find the edge.
                const labelList& pEdges = pointEdges()[e[0]];

                label edgeI = -1;

                forAll(pEdges, i)
                {
                    if (edges()[pEdges[i]] == e)
                    {
                        edgeI = pEdges[i];
                        break;
                    }
                }

                if (edgeI == -1)
                {
                    if (debug)
                    {
                        WarningIn("processorPolyPatch::updateMesh()")
                            << "Cannot find patch edge with vertices " << e
                            << " coords:"
                            << localPoints()[e[0]]<< localPoints()[e[1]]
                            << " on patch " << name() << nl
                            << "Can only find edges "
                            << IndirectList<edge>(edges(), pEdges)()
                            << " connected to first vertex" << nl
                            << "Either your mesh is incorrect or this patch"
                            << " was constructed from part of a cyclic patch."
                            << nl
                            << "Not calculating edge neighbour addressing."
                            << endl;
                    }
                    deleteDemandDrivenData(neighbEdgesPtr_);
                    break;
                }

                neighbEdges[edgeI] = nbrEdgeI;
            }
        }
        else
        {
            // Differing number of points or edges. Probably patch includes
            // part of a cyclic.
            neighbPointsPtr_ = NULL;
            neighbEdgesPtr_ = NULL;
        }
    }
}


const labelList& processorPolyPatch::neighbPoints() const
{
    if (!neighbPointsPtr_)
    {
        // Was probably created from cyclic patch and hence the
        // number of edges or points might differ on both
        // sides of the processor patch since one side might have
        // it merged with another bit of geometry

        FatalErrorIn("processorPolyPatch::neighbPoints() const")
            << "No extended addressing calculated for patch " << name()
            << nl
            << "This can happen if the number of points or edges on both"
            << " sides of the two coupled patches differ." << nl
            << "This happens if the processorPatch was constructed from"
            << " part of a cyclic patch."
            << abort(FatalError);
    }
    return *neighbPointsPtr_;
}


const labelList& processorPolyPatch::neighbEdges() const
{
    if (!neighbEdgesPtr_)
    {
        // Was probably created from cyclic patch and hence the
        // number of edges or points might differ on both
        // sides of the processor patch since one side might have
        // it merged with another bit of geometry

        FatalErrorIn("processorPolyPatch::neighbEdges() const")
            << "No extended addressing calculated for patch " << name()
            << nl
            << "This can happen if the number of points or edges on both"
            << " sides of the two coupled patches differ." << nl
            << "This happens if the processorPatch was constructed from"
            << " part of a cyclic patch."
            << abort(FatalError);
    }
    return *neighbEdgesPtr_;
}


//- Initialize ordering (on new mesh)
void processorPolyPatch::initOrder(const primitivePatch& pp) const
{
    if (!Pstream::parRun())
    {
        return;
    }

    const bool isMaster = Pstream::myProcNo() < neighbProcNo();

    // Check (on old patch!) for weirdness.
    if (separated() || !parallel())
    {
        WarningIn
        (
            "processorPolyPatch::initOrder(const primitivePatch&) const"
        )   << "in patch:" << name() << " : "
            << "using geometric matching on this processor patch might fail"
            << " since it has 'separated' faces or is not 'parallel'"
            << endl;
    }


    if (isMaster)
    {
        pointField ctrs(calcFaceCentres(pp, pp.points()));

        pointField anchorPoints(getAnchorPoints(pp, pp.points()));

        // Now send all info over to the neighbour
        {
            if (debug)
            {
                Pout<< "processorPolyPatch::initOrder :"
                    << " sending face centres:" << ctrs.size()
                    << " sending anchors:" << anchorPoints.size()
                    << endl;
            }
            OPstream toNeighbour(neighbProcNo());
            toNeighbour << ctrs << anchorPoints;
        }
    }
}


//- Return new ordering. Ordering is -faceMap: for every face index
//  the new face -rotation:for every new face the clockwise shift
//  of the original face. Return false if nothing changes (faceMap
//  is identity, rotation is 0)
bool processorPolyPatch::order
(
    const primitivePatch& pp,
    labelList& faceMap,
    labelList& rotation
) const
{
    if (!Pstream::parRun())
    {
        return false;
    }

    faceMap.setSize(pp.size());
    faceMap = -1;

    rotation.setSize(pp.size());
    rotation = 0;

    const bool isMaster = Pstream::myProcNo() < neighbProcNo();

    if (isMaster)
    {
        // Do nothing (i.e. identical mapping, zero rotation).
        // See comment at top.
        forAll(faceMap, patchFaceI)
        {
            faceMap[patchFaceI] = patchFaceI;
        }

        return false;
    }
    else
    {
        vectorField masterCtrs;
        vectorField masterAnchorPoints;

        // Receive data from neighbour
        {
            IPstream fromNeighbour(neighbProcNo());
            fromNeighbour >> masterCtrs >> masterAnchorPoints;
        }

        // Calculate my face centres
        pointField ctrs(calcFaceCentres(pp, pp.points()));

        // Calculate typical distance from face centre
        scalarField tols(calcFaceTol(pp, pp.points(), ctrs));

        // Geometric match of face centre vectors
        bool matchedAll = matchPoints(ctrs, masterCtrs, tols, false, faceMap);

        if (debug)
        {
            fileName ccName(name() + "_faceCentres.obj");

            Pout<< "processorPolyPatch::order : "
                << "Dumping newly found match as lines between"
                << " corresponding face centres to OBJ file " << ccName
                << endl;

            OFstream ccStr(ccName);

            label vertI = 0;

            forAll(ctrs, faceI)
            {
                label masterFaceI = faceMap[faceI];

                if (masterFaceI != -1)
                {
                    const point& c0 = masterCtrs[masterFaceI];
                    const point& c1 = ctrs[faceI];
                    writeOBJ(ccStr, c0, c1, vertI);
                }
            }
        }

        if (!matchedAll)
        {
            SeriousErrorIn
            (
                "processorPolyPatch::order(const primitivePatch&"
                ", labelList&, labelList&) const"
            )   << "in patch:" << name() << " : "
                << "Cannot match vectors to faces on both sides of patch"
                << endl
                << "masterCtrs[0]:" << masterCtrs[0] << endl
                << "ctrs[0]:" << ctrs[0] << endl
                << "Please check your topology changes or maybe you have"
                << " multiple separated (from cyclics) processor patches"
                << endl
                << "Continuing with incorrect face ordering from now on!"
                << endl;

            return false;
        }

        // Set rotation.
        forAll(faceMap, oldFaceI)
        {
            // The face f will be at newFaceI (after morphing) and we want its
            // anchorPoint (= f[0]) to align with the anchorpoint for the
            // corresponding face on the other side.

            label newFaceI = faceMap[oldFaceI];

            const point& wantedAnchor = masterAnchorPoints[newFaceI];

            rotation[newFaceI] = getRotation
            (
                pp.points(),
                pp[oldFaceI],
                wantedAnchor,
                tols[oldFaceI]
            );

            if (rotation[newFaceI] == -1)
            {
                SeriousErrorIn
                (
                    "processorPolyPatch::order(const primitivePatch&"
                    ", labelList&, labelList&) const"
                )   << "in patch:" << name()
                    << " : "
                    << "Cannot find point on face " << pp[oldFaceI]
                    << " with vertices:"
                    << IndirectList<point>(pp.points(), pp[oldFaceI])()
                    << " that matches point " << wantedAnchor
                    << " when matching the halves of processor patch " << name()
                    << "Continuing with incorrect face ordering from now on!"
                    << endl;

                return false;
            }
        }

        forAll(faceMap, faceI)
        {
            if (faceMap[faceI] != faceI || rotation[faceI] != 0)
            {
                return true;
            }
        }

        return false;
    }
}


// Write
void processorPolyPatch::write(Ostream& os) const
{
    polyPatch::write(os);

    os  << nl << myProcNo_ << token::SPACE << neighbProcNo_;
}


void processorPolyPatch::writeDict(Ostream& os) const
{
    os  << nl << name() << nl << token::BEGIN_BLOCK << nl
        << "    type " << type() << token::END_STATEMENT << nl;
    patchIdentifier::writeDict(os);
    os  << "    nFaces " << size() << token::END_STATEMENT << nl
        << "    startFace " << start() << token::END_STATEMENT << nl
        << "    myProcNo " << myProcNo_ << token::END_STATEMENT << nl
        << "    neighbProcNo " << neighbProcNo_ << token::END_STATEMENT << nl
        << token::END_BLOCK << endl;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
