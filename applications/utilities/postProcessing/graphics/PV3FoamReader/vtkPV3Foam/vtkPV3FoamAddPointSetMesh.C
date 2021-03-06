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

#include "vtkPV3Foam.H"

// Foam includes
#include "pointSet.H"
#include "vtkPV3FoamInsertNextPoint.H"

// VTK includes
#include "vtkPoints.h"
#include "vtkPolyData.h"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::vtkPV3Foam::addPointSetMesh
(
    const fvMesh& mesh,
    const pointSet& pSet,
    vtkPolyData* vtkmesh
)
{
    if (debug)
    {
        Info<< "entered add point set mesh" << endl;
    }

    // Use all points: support for inactive points and faces.
    // HJ, 28/Mar/2009
    const pointField& meshPoints = mesh.allPoints();

    vtkPoints *vtkpoints = vtkPoints::New();
    vtkpoints->Allocate(pSet.size());

    forAllConstIter(pointSet, pSet, iter)
    {
        vtkPV3FoamInsertNextPoint(vtkpoints, meshPoints[iter.key()]);
    }

    vtkmesh->SetPoints(vtkpoints);
    vtkpoints->Delete();
}


// ************************************************************************* //
