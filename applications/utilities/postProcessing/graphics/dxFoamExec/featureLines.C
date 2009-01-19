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

#include "featureLines.H"
#include "mathematicalConstants.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
featureLines::featureLines(const polyPatch& p)
:
    patch_(p)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

edgeList featureLines::extractEdges(const scalar featureAngle)
{
    if ((featureAngle < 0.0) || (featureAngle > 180.0))
    {
        FatalErrorIn("featureLines::extractEdges(const scalar featureAngle)")
            << "Feature angle outside [0 .. 180]"
            << exit(FatalError);
    }

    scalar featureCos(Foam::cos(featureAngle*mathematicalConstant::pi/180.0));

    const labelListList& eFaces = patch_.edgeFaces();
    const edgeList& edges = patch_.edges();

    // Container for edges with correct featureangle
    edgeList subEdges(edges.size());

    label internalEdgesi = 0;
    label boundaryEdgesi = edges.size();

    // Collect correct edges into subEdges

    if (featureCos < (-1 + 1E-5))
    {
        // All edges.
        forAll (edges, edgei)
        {
            if (eFaces[edgei].size() == 1)
            {
                subEdges[--boundaryEdgesi] = edges[edgei];
            }
            else
            {
              subEdges[internalEdgesi++] = edges[edgei];
            }
        }
    }
    else if (featureCos > (1 - 1E-5))
    {
        // Only boundary edges
        forAll (edges, edgei)
        {
            if (eFaces[edgei].size() == 1)
            {
                subEdges[--boundaryEdgesi] = edges[edgei];
            }
        }
    }
    else
    {
        // Calculate feature angle.
        forAll (edges, edgei)
        {
            const labelList& faces = eFaces[edgei];

            if (faces.size() == 1)
            {
                subEdges[--boundaryEdgesi] = edges[edgei];
            }
            else if (faces.size() == 2)
            {
                const vectorField& normals = patch_.faceNormals();

                scalar cosAngle = -(normals[faces[0]] & normals[faces[1]]);

                if (cosAngle >= featureCos)
                {
                  subEdges[internalEdgesi++] = edges[edgei];
                }
            }
        }
    }


    // Compact subEdges:
    //   0              .. boundaryEdgesi-1 : internal edges
    //   boundaryEdgesi .. size             : boundary edges
    for (label i=boundaryEdgesi; i<edges.size(); i++)
    {
        subEdges[internalEdgesi++] = subEdges[i];
    }
    subEdges.setSize(internalEdgesi);

    return subEdges;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
