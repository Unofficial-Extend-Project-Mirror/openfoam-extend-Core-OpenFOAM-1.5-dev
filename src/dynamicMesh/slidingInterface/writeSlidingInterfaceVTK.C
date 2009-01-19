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

Class
    slidingInterface

Description
    Sliding interface visual debugging tools

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved.  Copyright Hrvoje Jasak.

\*---------------------------------------------------------------------------*/

#include "slidingInterface.H"
#include "polyTopoChanger.H"
#include "Time.H"
#include "OFstream.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::slidingInterface::writeVTK
(
    const word& name,
    const pointField& points,
    const faceList& faces
) const
{
    // Write patch and points into VTK
    const polyMesh& mesh = topoChanger().mesh();

    fileName fvPath(mesh.time().path()/"VTK");
    mkDir(fvPath);

    OFstream mps(fvPath/name + ".vtk");

    mps << "# vtk DataFile Version 2.0" << nl
        << name << ".vtk" << nl
        << "ASCII" << nl
        << "DATASET POLYDATA" << nl
        << "POINTS " << points.size() << " float" << nl;

    // Write points
    List<float> mlpBuffer(3*points.size());

    label counter = 0;
    forAll (points, i)
    {
        mlpBuffer[counter++] = float(points[i].x());
        mlpBuffer[counter++] = float(points[i].y());
        mlpBuffer[counter++] = float(points[i].z());
    }

    forAll (mlpBuffer, i)
    {
        mps << mlpBuffer[i] << ' ';

        if (i > 0 && (i % 10) == 0)
        {
            mps << nl;
        }
    }

    // Write faces
    label nFaceVerts = 0;

    forAll (faces, faceI)
    {
        nFaceVerts += faces[faceI].size() + 1;
    }
    labelList mlfBuffer(nFaceVerts);

    counter = 0;
    forAll (faces, faceI)
    {
        const face& f = faces[faceI];

        mlfBuffer[counter++] = f.size();

        forAll (f, fpI)
        {
            mlfBuffer[counter++] = f[fpI];
        }
    }
    mps << nl;

    mps << "POLYGONS " << faces.size() << ' ' << nFaceVerts << endl;

    forAll (mlfBuffer, i)
    {
        mps << mlfBuffer[i] << ' ';

        if (i > 0 && (i % 10) == 0)
        {
            mps << nl;
        }
    }
    mps << nl;
}


void Foam::slidingInterface::writeVTKnormals
(
    const word& name,
    const pointField& points,
    const faceList& faces
) const
{
    // Write patch and points into VTK
    const polyMesh& mesh = topoChanger().mesh();

    fileName fvPath(mesh.time().path()/"VTK");
    mkDir(fvPath);

    OFstream mps(fvPath/name + "Normals.vtk");

    mps << "# vtk DataFile Version 2.0" << nl
        << name << ".vtk" << nl
        << "ASCII" << nl
        << "DATASET POLYDATA" << nl
        << "POINTS " << faces.size() << " float" << nl;

    // Write points
    List<float> mlPointBuffer(3*faces.size());

    label counter = 0;
    forAll (faces, i)
    {
        const vector c = faces[i].centre(points);

        mlPointBuffer[counter++] = float(c.x());
        mlPointBuffer[counter++] = float(c.y());
        mlPointBuffer[counter++] = float(c.z());
    }

    forAll (mlPointBuffer, i)
    {
        mps << mlPointBuffer[i] << ' ';

        if (i > 0 && (i % 10) == 0)
        {
            mps << nl;
        }
    }
    mps << nl;

    // Write normals
    mps << "POINT_DATA " << faces.size() << nl
        << "FIELD attributes " << 1 << nl
        << "normals" << " 3 "
        << faces.size() << " float" << nl;

    List<float> mlNormalBuffer(3*faces.size());

    counter = 0;
    forAll (faces, i)
    {
        const vector n = faces[i].normal(points);

        mlNormalBuffer[counter++] = float(n.x());
        mlNormalBuffer[counter++] = float(n.y());
        mlNormalBuffer[counter++] = float(n.z());
    }

    forAll (mlNormalBuffer, i)
    {
        mps << mlNormalBuffer[i] << ' ';

        if (i > 0 && (i % 10) == 0)
        {
            mps << nl;
        }
    }
    mps << nl;
}


// ************************************************************************* //
