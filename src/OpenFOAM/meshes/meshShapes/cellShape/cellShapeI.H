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

#include "cellShapeList.H"
#include "IOstreams.H"
#include "cell.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct null
inline cellShape::cellShape()
:
    m_(NULL)
{}


// Construct from components
inline cellShape::cellShape
(
    const cellModel& M,
    const labelList& l,
    const bool doCollapse
)
:
    labelList(l),
    m_(&M)
{
    if (doCollapse)
    {
        collapse();
    }
}


// Construct from Istream
inline cellShape::cellShape(Istream& is)
{
    is >> *this;
}


// Clone
inline autoPtr<cellShape> cellShape::clone() const
{
    return autoPtr<cellShape>(new cellShape(*this));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Return the points associated with this cellShape
inline pointField cellShape::points(const pointField& meshPoints) const
{
    // There are as many points as there labels for them
    pointField p(size());

    // For each point in list, set it to the point in 'pnts' addressed
    // by 'labs'
    forAll(p, i)
    {
        p[i] = meshPoints[operator[](i)];
    }

    // Return list
    return p;
}


// Return model reference
inline const cellModel& cellShape::model() const
{
    if (!m_)
    {
        FatalErrorIn("const cellModel& cellShape::model() const")
            << "No model" << endl;
    }

    return *m_;
}


// Mesh face labels of this cell (in order of model)
inline labelList cellShape::meshFaces
(
    const faceList& allFaces,
    const cell& cFaces
) const
{
    // Faces in model order
    faceList localFaces(faces());

    // Do linear match (usually cell shape is low complexity)

    labelList modelToMesh(localFaces.size(), -1);

    forAll(localFaces, i)
    {
        const face& localF = localFaces[i];

        forAll(cFaces, j)
        {
            label meshFaceI = cFaces[j];

            if (allFaces[meshFaceI] == localF)
            {
                modelToMesh[i] = meshFaceI;

                break;
            }
        }
    }

    return modelToMesh;
}


// Mesh edge labels of this cell (in order of model)
inline labelList cellShape::meshEdges
(
    const edgeList& allEdges,
    const labelList& cEdges
) const
{
    // Edges in model order
    edgeList localEdges(edges());

    // Do linear match (usually cell shape is low complexity)

    labelList modelToMesh(localEdges.size(), -1);

    forAll(localEdges, i)
    {
        const edge& e = localEdges[i];

        forAll(cEdges, j)
        {
            label edgeI = cEdges[j];

            if (allEdges[edgeI] == e)
            {
                modelToMesh[i] = edgeI;

                break;
            }
        }
    }

    return modelToMesh;
}


// Return faces of cell
inline faceList cellShape::faces() const
{
    return model().faces(*this);
}


inline faceList cellShape::collapsedFaces() const
{
    faceList oldFaces(faces());

    faceList newFaces(oldFaces.size());
    label newFaceI = 0;

    forAll(oldFaces, oldFaceI)
    {
        const face& f = oldFaces[oldFaceI];

        face& newF = newFaces[newFaceI];

        newF.setSize(f.size());

        label newFp = 0;
        label prevVertI = -1;

        forAll(f, fp)
        {
            label vertI = f[fp];

            if (vertI != prevVertI)
            {
                newF[newFp++] = vertI;

                prevVertI = vertI;
            }
        }

        if ((newFp > 1) && (newF[newFp-1] == newF[0]))
        {
            --newFp;
        }

        if (newFp > 2)
        {
            // Size face and go to next one
            newF.setSize(newFp);

            newFaceI++;
        }
    }
    newFaces.setSize(newFaceI);

    return newFaces;    
}


// Return number of faces
inline label cellShape::nFaces() const
{
    return model().nFaces();
}


// Return edges of cell
inline edgeList cellShape::edges() const
{
    return model().edges(*this);
}


// Return number of edges
inline label cellShape::nEdges() const
{
    return model().nEdges();
}


// Return number of points
inline label cellShape::nPoints() const
{
    return size();
}


// Centre point of cellShape
inline point cellShape::centre(const pointField& points) const
{
    return model().centre(*this, points);
}


// Return scalar magnitude
inline scalar cellShape::mag(const pointField& points) const
{
    return model().mag(*this, points);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
