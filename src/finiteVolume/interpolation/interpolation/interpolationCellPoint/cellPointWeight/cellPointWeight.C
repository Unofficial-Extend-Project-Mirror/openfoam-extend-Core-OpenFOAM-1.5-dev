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

#include "cellPointWeight.H"
#include "polyMesh.H"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

/*
    find the tetrahedron in which the position is.
    while searching for the tet, store the tet
    closest to the position.
    This way, if position is not inside any tet,
    choose the closest one.
*/
bool Foam::cellPointWeight::findTet
(
    const polyMesh& mesh,
    const label nFace,
    vector tetPoints[],
    label tetLabelCandidate[],
    scalar phiCandidate[],
    label& closestFace,
    scalar& minDistance
) 
{
    bool foundTet = false;

    const labelList& thisFacePoints = mesh.allFaces()[nFace];

    // NN.
    // brief descripion of interpolation algorithm:
    //
    // construct the face triangles using facePoint 0 and the
    // edges (from edge 1 to numEdges-1),
    // which for the 'square' below is edge1 to edge2
    //
    //            edge1
    //     p2 ---------------- p1
    //        |              |
    //  edge2 |              | edge0
    //        |              |
    //     p3 ---------------- p0
    //          edge3
    //
    // hence, triangle0 will be p0 and p1-p2 (edge1)
    // and triangle1; p0 and p2-p3 (edge2)
    // and so on...
    // these points, together with the cell centre defines the tet.

    label pointi = 1;

    while ((pointi+1) < thisFacePoints.size() && !foundTet)
    {
        label nextPointLabel = pointi;
        label lastPointLabel = pointi + 1;

        faceVertices_[0] = thisFacePoints[0];
        faceVertices_[1] = thisFacePoints[nextPointLabel];
        faceVertices_[2] = thisFacePoints[lastPointLabel];

        tetPoints[0] = mesh.points()[faceVertices_[0]];
        tetPoints[1] = mesh.points()[faceVertices_[1]];
        tetPoints[2] = mesh.points()[faceVertices_[2]];

        bool inside = true;
        scalar dist = 0.0;

        // check if position is inside tet
        for (label n=0; n<4; n++)
        {
            label p1 = (n + 1) % 4;
            label p2 = (n + 2) % 4;
            label p3 = (n + 3) % 4;

            vector referencePoint, faceNormal;

            // form the face normal by the cross-product of the opposite face
            // vectors or use the area vector.
            if (n != 3)
            {
                referencePoint = tetPoints[p1];

                faceNormal =
                    (tetPoints[p3] - tetPoints[p1])
                  ^ (tetPoints[p2] - tetPoints[p1]);
            }
            else
            {
                faceNormal = mesh.faceAreas()[nFace];
                referencePoint = mesh.faceCentres()[nFace];
            }

            faceNormal /= mag(faceNormal) + VSMALL;

            // correct normal to point inwards the tet
            vector v0 = tetPoints[n] - referencePoint;
            scalar correct = v0 & faceNormal;
            if (correct < 0)
            {
                faceNormal = -faceNormal;
            }

            vector v1 = position_ - referencePoint + SMALL*faceNormal;
            scalar rightSide = v1 & faceNormal;

            // since normal is inwards, inside the tet
            // is defined as all dot-products being positive
            inside = inside && (rightSide >= 0);

            scalar phiLength = (position_ - referencePoint) & faceNormal;

            scalar maxLength =
                max(VSMALL, (tetPoints[n] - referencePoint) & faceNormal);

            weights_[n] = phiLength/maxLength;

            dist += weights_[n];
        }

        // due to roundoff it's possible for the position to be 'just'
        // outside the tet. If no tet contains the position, get
        // the 'closest' candidate and store it for later use.
        if (!inside)
        {
            // the 'closeness' is defined by dist
            // (if dist == 1.0, the position is inside the tet)
            if (mag(dist - 1.0) < minDistance)
            {
                minDistance = mag(dist - 1.0);
                closestFace = nFace;

                for (label i=0; i<4; i++)
                {
                    phiCandidate[i] = weights_[i];
                }

                tetLabelCandidate[0] = faceVertices_[0];
                tetLabelCandidate[1] = faceVertices_[1];
                tetLabelCandidate[2] = faceVertices_[2];
            }
        }

        foundTet = inside;

        pointi++;
    }

    if (foundTet)
    {
        closestFace = nFace;
    }

    return foundTet;
}


/*
    find the triangle in which the position is,
    when the position is known to be on the face
*/
bool Foam::cellPointWeight::findTriangle
(
    const polyMesh& mesh,
    const label nFace
) 
{
    bool foundTriangle = false;
    vector tetPoints[3];
    const labelList& facePoints = mesh.allFaces()[nFace];

    label pointi = 0;

    while (pointi < facePoints.size() && !foundTriangle)
    {
        // The triangle is constructed from face center and one
        // face edge
        label p1 = (pointi + 1) % facePoints.size();
        label p2 = (pointi + 2) % facePoints.size();

        faceVertices_[0] = facePoints[pointi];
        faceVertices_[1] = facePoints[p1];
        faceVertices_[2] = facePoints[p2];

        tetPoints[0] = mesh.points()[faceVertices_[0]];
        tetPoints[1] = mesh.points()[faceVertices_[1]];
        tetPoints[2] = mesh.points()[faceVertices_[2]];

        vector fc = (tetPoints[0] + tetPoints[1] + tetPoints[2])/3.0;
        // calculate triangle edge vectors and triangle face normal
        // the 'i':th edge is opposite node i
        vector edge[3], normal[3];
        for(label i=0; i<3; i++)
        {
            label ip0 = (i+1) % 3;
            label ipp = (i+2) % 3;
            edge[i] = tetPoints[ipp]-tetPoints[ip0];
        }

        vector triangleFaceNormal = edge[1] ^ edge[2];
        
        // calculate edge normal (pointing inwards)
        for(label i=0; i<3; i++)
        {
            normal[i] = triangleFaceNormal ^ edge[i];
            normal[i] /= mag(normal[i]) + VSMALL;
        }

        // check if position is inside triangle
        bool inside = true;
        vector newPos = position_ + SMALL*(fc - position_);
        for(label i=0; i<3; i++)
        {
            label ip = (i+1) % 3;
            inside = inside && (((newPos - tetPoints[ip]) & edge[i]) >= 0);
        }

        if (inside)
        {
            foundTriangle = true;

            // calculate phi-values
            for(label i=0; i<3; i++)
            {
                label ip = (i+1) % 3;
                scalar phiMax = max(VSMALL, normal[i] & edge[ip]);
                scalar phiLength = (position_-tetPoints[ip]) & normal[i];
                weights_[i] = phiLength/phiMax;
            }
        }

        pointi++;
    }

    return foundTriangle;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
Foam::cellPointWeight::cellPointWeight
(
    const polyMesh& mesh,
    const vector& position, 
    const label nCell,
    const label facei
)
:
    position_(position),
    cellNo_(nCell),
    face_(facei)
{
    vector tetPoints[4];
    scalar phiCandidate[4];
    label tetLabelCandidate[3];

    // only use face information when the position is on a face
    if (face_ < 0)
    {
        const vector& cellCentre = mesh.cellCentres()[cellNo_];
        const labelList& cellFaces = mesh.cells()[cellNo_];

        vector projection = position_ - cellCentre;
        tetPoints[3] = cellCentre;

        // *********************************************************************
        // project the cell-center through the point onto the face
        // and get the closest face, ...
        // *********************************************************************

        bool foundTet = false;
        label closestFace = -1;
        scalar minDistance = GREAT;

        forAll(cellFaces, facei)
        {
            label nFace = cellFaces[facei];

            vector normal = mesh.faceAreas()[nFace];
            normal /= mag(normal) + VSMALL;

            const vector& faceCentreTmp = mesh.faceCentres()[nFace];
            
            scalar multiplierNumerator = (faceCentreTmp - cellCentre) & normal;
            scalar multiplierDenominator = projection & normal;

            // if normal and projection are not orthogonal
            // this could be the one...
            if (mag(multiplierDenominator) > SMALL)
            {
                scalar multiplier = multiplierNumerator/multiplierDenominator;
                vector iPoint = cellCentre + multiplier*projection;
                scalar dist = mag(position_ - iPoint);

                if (dist < minDistance)
                {
                    closestFace = nFace;
                    minDistance = dist;
                }
            }
        }

        // *********************************************************************
        // find the tetrahedron containing 'position'
        // from the cell center, and
        // the other points on the face
        // *********************************************************************

        minDistance = GREAT;
        if (closestFace != -1)
        {
            label nFace = closestFace;
            foundTet = findTet
            (
                mesh,
                nFace,
                tetPoints,
                tetLabelCandidate,
                phiCandidate,
                closestFace,
                minDistance
            );
        }

        if (!foundTet)
        {
            // check if the position is 'just' outside a tet
            if (minDistance < 1.0e-5)
            {
                foundTet = true;
                for (label i=0; i<4; i++)
                {
                    weights_[i] = phiCandidate[i];
                }
                faceVertices_[0] = tetLabelCandidate[0];
                faceVertices_[1] = tetLabelCandidate[1];
                faceVertices_[2] = tetLabelCandidate[2];
            }
        }

        // *********************************************************************
        // if the search failed check all the cell-faces
        // *********************************************************************

        if (!foundTet)
        {
            minDistance = GREAT;

            label facei = 0;
            while (facei < cellFaces.size() && !foundTet)
            {
                label nFace = cellFaces[facei];
                if (nFace < mesh.nFaces())
                {
                    foundTet = findTet
                    (
                        mesh,
                        nFace,
                        tetPoints,
                        tetLabelCandidate,
                        phiCandidate,
                        closestFace,
                        minDistance
                    );
                }
                facei++;
            }
        }

        // ... and if the tet was still not found, then ...
        if (!foundTet)
        {
            // ... check if the position is 'just' outside a tet
            // this time with a more tolerant limit
            if (minDistance < 1.0e-3)
            {
                foundTet = true;
                for (label i=0; i<4; i++)
                {
                    weights_[i] = phiCandidate[i];
                }
                faceVertices_[0] = tetLabelCandidate[0];
                faceVertices_[1] = tetLabelCandidate[1];
                faceVertices_[2] = tetLabelCandidate[2];
            }
        }

        // *********************************************************************
        // if the tet was found do the interpolation,
        // otherwise use the closest face value
        // *********************************************************************

        if (foundTet)
        {
            for (label n=0; n<4; n++)
            {
                weights_[n] = min(1.0, weights_[n]);
                weights_[n] = max(0.0, weights_[n]);
            }
        }
        else
        {
            WarningIn
            (
                "interpolationCellPoint<Type>::interpolate(const vector&, "
                "const label celli) const"
            )   << "search failed; using cell value" << endl
                << "cellnumber " << cellNo_ << endl;
            
            for (label n=0; n<3; n++)
            {
                weights_[n] = 0.0;
                faceVertices_[n] = mesh.allFaces()[mesh.cells()[cellNo_][0]][0];
            }
            weights_[3] = 1.0;
        }
        
        if(closestFace != -1)
        {
            forAll(mesh.cells()[cellNo_], facei)
            {
                if(mesh.cells()[cellNo_][facei] == closestFace)
                {
                    cellFace_ = facei;
                }
            }
        }
        else
        {
            cellFace_ = 0;
        }
    }
    else
    {
        forAll(mesh.cells()[cellNo_], facei)
        {
            if(mesh.cells()[cellNo_][facei] == face_)
            {
                cellFace_ = facei;
            }
        }
        
        bool foundTriangle = findTriangle
        (
            mesh,
            face_
        );
        
        if (foundTriangle)
        {
            weights_[3] = 0;
        }
        else
        {
            WarningIn
            (
                "interpolationCellPoint<Type>::interpolate(const vector&, "
                "const label celli) const"
            )   << "face search failed; using first 3 point values" << endl
                << "cellnumber " << cellNo_ << endl;

            // use first three points on face
            faceVertices_[0] = mesh.allFaces()[face_][0];
            faceVertices_[1] = mesh.allFaces()[face_][1];
            faceVertices_[2] = mesh.allFaces()[face_][2];
            weights_[0] = 1.0/3.0;
            weights_[1] = 1.0/3.0;
            weights_[2] = 1.0/3.0;
            weights_[3] = 0.0;
        }
    }
}


// ************************************************************************* //
