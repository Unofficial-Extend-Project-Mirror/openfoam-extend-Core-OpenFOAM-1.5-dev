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

    See README.

\*---------------------------------------------------------------------------*/

#include "dxFvMesh.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

DXArray dxFvMesh::createDxPositions()
{
    Info<< "dxFoamExec:dxFvMesh::createDxPositions : create DX positions"
        << endl;

    // Create vertex array
    float *pointVector = new float[3*totalNPoints()];

    // Set vertices
    label i = 0;
    const pointField& pts = points();
    forAll(pts, pointi)
    {
        pointVector[i++] = pts[pointi].x();
        pointVector[i++] = pts[pointi].y();
        pointVector[i++] = pts[pointi].z();
    }

    const vectorField& cellCentres = C().internalField();

    forAll(addPointCellLabels_, j)
    {
        const vector& c = cellCentres[addPointCellLabels_[j]];

        pointVector[i++] = c.x();
        pointVector[i++] = c.y();
        pointVector[i++] = c.z();
    }


    // Check for overflow
    if (i != 3*totalNPoints())
    {
        SeriousError
            << "dxFoamExec:dxFvMesh::createDxPositions : "
            << "pointVector over/underflow:" << endl
            << "    i:" << i << endl
            << "    allocated:" << 3*totalNPoints()
            << endl;

        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxPositions : "
            "pointVector over/underflow"
        );

        delete[] pointVector;

        return DXERROR;
    }


    // Create array objects for points (positions)
    // -------------------------------------------

    DXArray dxPositions = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 1, 3);

    if (!DXAddArrayData(dxPositions, 0, totalNPoints(), pointVector))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxPositions : Error in DXAddArrayData"
        );

        delete[] pointVector;

        return DXERROR;
    }

    delete[] pointVector;

    if
    (
        !DXSetStringAttribute
        (
            reinterpret_cast<Object>(dxPositions),
            "dep",
            "positions"
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxPositions : "
            "Error in DXSetStringAttribute dep"
        );
        return DXERROR;
    }

    return dxPositions;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
