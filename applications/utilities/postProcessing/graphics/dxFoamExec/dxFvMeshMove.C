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

#include "dxFoamExec.H"
#include "dxFvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Error dxFvMesh::move(bool& meshMoved)
{
    try
    {
        //Info<< "dxFoamExec::dxFvMesh::move : move mesh if required" << endl;

        meshMoved = false;

        fileName pointsDir(time().findInstance(meshDir(), "points"));

        if (pointsDir != pointsDir_)
        {
            // points exists for time step, let's read them
            Info<< "dxFoamExec::dxFvMesh::move : "
                   "Point position changed. - updating points." << endl;

            pointsDir_ = pointsDir;

            // Reading new points
            pointIOField newPoints
            (
                IOobject
                (
                    "points",
                    pointsDir_,
                    meshDir(),
                    time(),
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            );

            movePoints(newPoints);
            volPointInterpolation_.movePoints();

            meshMoved = true;

            if (!(dxPositions_ = createDxPositions()))
            {
                return DXERROR;
            }

            if (!DXDeleteComponent(dxMesh_, "positions"))
            {
                DXSetError
                (
                    ERROR_INTERNAL,
                    "dxFoamExec::dxFvMesh::move : "
                    "failed to delete positions"
                );
                return DXERROR;
            }

            if
            (
               !DXSetComponentValue
                (
                    dxMesh_,
                    "positions",
                    reinterpret_cast<Object>(dxPositions_)
                )
            )
            {
                DXSetError
                (
                    ERROR_INTERNAL,
                    "dxFoamExec::dxFvMesh::move : "
                    "Error in DXSetComponentValue positions"
                );
                return DXERROR;
            }


            if (!DXDeleteComponent(dxMesh_, "box"))
            {
                DXSetError
                (
                    ERROR_INTERNAL,
                    "dxFoamExec::dxFvMesh::move : "
                    "failed to delete bounding box"
                );
                return DXERROR;
            }

            Point bb[8];
            if (!DXBoundingBox(Object(dxMesh_), bb))
            {
                DXSetError
                (
                    ERROR_INTERNAL,
                    "dxFoamExec::dxFvMesh::move : "
                    "Error in DXBoundingBox"
                );
                return DXERROR;
            }
        }

        return OK;
    }
    catch (IOerror& ioErr) 
    {
        Info << endl << ioErr << endl << endl;

        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:dxFvMesh::move : could not move points"
        );

        return DXERROR;
    }
    catch (error& fErr) 
    {
        Info << endl << fErr << endl << endl;

        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:dxFvMesh::move : could not move points"
        );

        return DXERROR;
    }
    catch (...) 
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:dxFvMesh::move : could not move points"
        );

        return DXERROR;
    }
}


// ************************************************************************* //
