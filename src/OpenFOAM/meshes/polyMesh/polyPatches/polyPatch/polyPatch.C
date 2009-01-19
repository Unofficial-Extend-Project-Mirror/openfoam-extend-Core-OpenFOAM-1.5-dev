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

#include "polyPatch.H"
#include "addToRunTimeSelectionTable.H"
#include "polyBoundaryMesh.H"
#include "polyMesh.H"
#include "primitiveMesh.H"
#include "SubField.H"
#include "entry.H"
#include "dictionary.H"
#include "pointPatchField.H"
#include "OFstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(polyPatch, 0);

defineRunTimeSelectionTable(polyPatch, word);
defineRunTimeSelectionTable(polyPatch, Istream);
defineRunTimeSelectionTable(polyPatch, dictionary);

addToRunTimeSelectionTable(polyPatch, polyPatch, word);
addToRunTimeSelectionTable(polyPatch, polyPatch, Istream);
addToRunTimeSelectionTable(polyPatch, polyPatch, dictionary);


const scalar polyPatch::matchTol_
(
    debug::tolerances("patchFaceMatchTol", 1e-3)
);


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void polyPatch::movePoints(const pointField& p)
{
    primitivePatch::movePoints(p);
}

void polyPatch::updateMesh()
{
    clearAddressing();
}


// Is old face now in the current patch?
bool polyPatch::inPatch
(
    const labelList& oldToNew,
    const label oldFaceI
) const
{
    label faceI = oldToNew[oldFaceI];

    return faceI >= start() && faceI < start() + size();
}


void polyPatch::writeOBJ(Ostream& os, const point& pt)
{
    os << "v " << pt.x() << ' ' << pt.y() << ' ' << pt.z() << endl;
}


void polyPatch::writeOBJ
(
    Ostream& os,
    const pointField& points,
    const labelList& pointLabels
)
{
    forAll(pointLabels, i)
    {
        writeOBJ(os, points[pointLabels[i]]);
    }
}


// Write edge
void polyPatch::writeOBJ
(
    Ostream& os,
    const point& p0,
    const point& p1,
    label& vertI
)
{
    writeOBJ(os, p0);
    vertI++;

    writeOBJ(os, p1);
    vertI++;

    os<< "l " << vertI-1 << ' ' << vertI << nl;
}


// Write patch
void polyPatch::writeOBJ
(
    const fileName& fName,
    const faceList& faces,
    const pointField& points
)
{
    OFstream os(fName);

    Map<label> foamToObj(4*faces.size());

    label vertI = 0;

    forAll(faces, i)
    {
        const face& f = faces[i];

        forAll(f, fp)
        {
            if (foamToObj.insert(f[fp], vertI))
            {
                writeOBJ(os, points[f[fp]]);
                vertI++;
            }
        }

        os << 'l';
        forAll(f, fp)
        {
            os << ' ' << foamToObj[f[fp]]+1;
        }
        os << ' ' << foamToObj[f[0]]+1 << nl;
    }
}


// Get face centre with specified points
// (can't use points() since might not be set yet in morphing; usually called
//  with mapPolyMesh.premotionPoints())
tmp<pointField> polyPatch::calcFaceCentres
(
    const faceList& faces,
    const pointField& points
)
{
    tmp<pointField> tctrs(new pointField(faces.size()));

    pointField& ctrs = tctrs();

    forAll(faces, faceI)
    {
        ctrs[faceI] = faces[faceI].centre(points);
    }

    return tctrs;
}


// Get coordinate of f[0] for every face
tmp<pointField> polyPatch::getAnchorPoints
(
    const faceList& faces,
    const pointField& points
)
{
    tmp<pointField> tanchors(new pointField(faces.size()));

    pointField& anchors = tanchors();

    forAll(faces, faceI)
    {
        anchors[faceI] = points[faces[faceI][0]];
    }

    return tanchors;
}


// Given list of starts of patches and a face label determine the patch.
label polyPatch::whichPatch
(
    const labelList& patchStarts,
    const label faceI
)
{
    forAll(patchStarts, patchI)
    {
        if (patchStarts[patchI] <= faceI)
        {
            if (patchI == patchStarts.size() - 1)
            {
                return patchI;
            }
            else if (patchStarts[patchI+1] > faceI)
            {
                return patchI;
            }
        }
    }

    return -1;
}


// Get local typical dimension and tolerance from that. Currently max of
// distance from centre to any of the face points.
scalarField polyPatch::calcFaceTol
(
    const faceList& faces,
    const pointField& points,
    const pointField& faceCentres
)
{
    // Calculate typical distance per face
    scalarField tols(faces.size());

    forAll(faces, faceI)
    {
        const point& cc = faceCentres[faceI];

        const face& f = faces[faceI];

        scalar maxLen = -GREAT;

        forAll(f, fp)
        {
            maxLen = max(maxLen, mag(points[f[fp]] - cc));
        }

        tols[faceI] = matchTol_*maxLen;
    }
    return tols;
}


label polyPatch::getRotation
(
    const pointField& points,
    const face& f,
    const point& anchor,
    const scalar tol
)
{
    label anchorFp = -1;
    scalar minDistSqr = GREAT;

    forAll(f, fp)
    {
        scalar distSqr = magSqr(anchor - points[f[fp]]);

        if (distSqr < minDistSqr)
        {
            minDistSqr = distSqr;
            anchorFp = fp;
        }
    }

    if (anchorFp == -1 || mag(minDistSqr) > tol)
    {
        return -1;
    }
    else
    {
        // Positive rotation
        return (f.size() - anchorFp) % f.size();
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

polyPatch::polyPatch
(
    const word& name,
    const label size,
    const label start,
    const label index,
    const polyBoundaryMesh& bm
)
:
    patchIdentifier(name, index),
    primitivePatch
    (
        faceSubList(bm.mesh().allFaces(), size, start),
        bm.mesh().allPoints()
    ),
    start_(start),
    boundaryMesh_(bm),
    faceCellsPtr_(NULL),
    mePtr_(NULL)
{}


polyPatch::polyPatch
(
    Istream& is,
    const label index,
    const polyBoundaryMesh& bm
)
:
    patchIdentifier(is, index),
    primitivePatch
    (
        faceSubList(bm.mesh().allFaces(), 0, 0),
        bm.mesh().allPoints()
    ),
    start_(-1),
    boundaryMesh_(bm),
    faceCellsPtr_(NULL),
    mePtr_(NULL)
{
    label size = readLabel(is);
    start_ = readLabel(is);
    UList<face>::operator=
    (
        faceSubList(bm.mesh().allFaces(), size, start_)
    );
}


polyPatch::polyPatch
(
    const word& name,
    const dictionary& dict,
    const label index,
    const polyBoundaryMesh& bm
)
:
    patchIdentifier(name, dict, index),
    primitivePatch
    (
        faceSubList
        (
            bm.mesh().allFaces(),
            readLabel(dict.lookup("nFaces")),
            readLabel(dict.lookup("startFace"))
        ),
        bm.mesh().allPoints()
    ),
    start_(readLabel(dict.lookup("startFace"))),
    boundaryMesh_(bm),
    faceCellsPtr_(NULL),
    mePtr_(NULL)
{}


polyPatch::polyPatch
(
    const polyPatch& pp,
    const polyBoundaryMesh& bm
)
:
    patchIdentifier(pp),
    primitivePatch
    (
        faceSubList
        (
            bm.mesh().allFaces(),
            pp.size(),
            pp.start()
        ),
        bm.mesh().allPoints()
    ),
    start_(pp.start()),
    boundaryMesh_(bm),
    faceCellsPtr_(NULL),
    mePtr_(NULL)
{}


polyPatch::polyPatch
(
    const polyPatch& pp,
    const polyBoundaryMesh& bm,
    const label index,
    const label newSize,
    const label newStart
)
:
    patchIdentifier(pp, index),
    primitivePatch
    (
        faceSubList
        (
            bm.mesh().allFaces(),
            newSize,
            newStart
        ),
        bm.mesh().allPoints()
    ),
    start_(newStart),
    boundaryMesh_(bm),
    faceCellsPtr_(NULL),
    mePtr_(NULL)
{}


polyPatch::polyPatch(const polyPatch& p)
:
    patchIdentifier(p),
    primitivePatch(p),
    start_(p.start_),
    boundaryMesh_(p.boundaryMesh_),
    faceCellsPtr_(NULL),
    mePtr_(NULL)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

polyPatch::~polyPatch()
{
    clearAddressing();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool polyPatch::constraintType(const word& pt)
{
    return pointPatchField<scalar>::PointPatchConstructorTablePtr_->found(pt);
}


wordList polyPatch::constraintTypes()
{
    wordList cTypes(dictionaryConstructorTablePtr_->size());

    label i = 0;

    for
    (
        dictionaryConstructorTable::iterator cstrIter =
            dictionaryConstructorTablePtr_->begin();
        cstrIter != dictionaryConstructorTablePtr_->end();
        ++cstrIter
    )
    {
        if (constraintType(cstrIter.key()))
        {
            cTypes[i++] = cstrIter.key();
        }
    }

    cTypes.setSize(i);

    return cTypes;
}


const polyBoundaryMesh& polyPatch::boundaryMesh() const
{
    return boundaryMesh_;
}


const pointField& polyPatch::allPoints() const
{
    return boundaryMesh_.mesh().allPoints();
}


const vectorField::subField polyPatch::faceCentres() const
{
    return patchSlice(boundaryMesh().mesh().faceCentres());
}


const vectorField::subField polyPatch::faceAreas() const
{
    return patchSlice(boundaryMesh().mesh().faceAreas());
}


// Return the patch face neighbour cell centres
tmp<vectorField> polyPatch::faceCellCentres() const
{
    tmp<vectorField> tcc(new vectorField(size()));
    vectorField& cc = tcc();

    // get reference to global cell centres
    const vectorField& gcc = boundaryMesh_.mesh().cellCentres();

    const unallocLabelList& faceCells = this->faceCells();

    forAll (faceCells, facei)
    {
        cc[facei] = gcc[faceCells[facei]];
    }

    return tcc;
}


const unallocLabelList& polyPatch::faceCells() const
{
    if (!faceCellsPtr_)
    {
        faceCellsPtr_ = new labelList::subList
        (
            patchSlice(boundaryMesh().mesh().faceOwner())
        );
    }

    return *faceCellsPtr_;
}


const labelList& polyPatch::meshEdges() const
{
    if (!mePtr_)
    {
        mePtr_ =
            new labelList
            (
                primitivePatch::meshEdges
                (
                    boundaryMesh().mesh().edges(),
                    boundaryMesh().mesh().cellEdges(),
                    faceCells()
                )
            );
    }

    return *mePtr_;
}


void polyPatch::clearAddressing()
{
    deleteDemandDrivenData(faceCellsPtr_);
    deleteDemandDrivenData(mePtr_);
}


void polyPatch::write(Ostream& os) const
{
    os  << nl << type()
        << static_cast<const patchIdentifier&>(*this) << endl
        << this->size() << tab << start_;
}


void polyPatch::writeDict(Ostream& os) const
{
    os  << nl << name() << nl << token::BEGIN_BLOCK << nl
        << "    type " << type() << token::END_STATEMENT << nl;

    patchIdentifier::writeDict(os);

    os  << "    nFaces " << this->size() << token::END_STATEMENT << nl
        << "    startFace " << start() << token::END_STATEMENT << nl
        << token::END_BLOCK << endl;
}


void polyPatch::initOrder(const primitivePatch&) const
{}


bool polyPatch::order
(
    const primitivePatch&,
    labelList& faceMap,
    labelList& rotation
) const
{
    // Nothing changed.
    return false;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void polyPatch::operator=(const polyPatch& p)
{
    clearAddressing();

    patchIdentifier::operator=(p);
    primitivePatch::operator=(p);
    start_ = p.start_;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

Ostream& operator<<(Ostream& os, const polyPatch& p)
{
    p.write(os);
    os.check("Ostream& operator<<(Ostream& f, const polyPatch& p");
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
