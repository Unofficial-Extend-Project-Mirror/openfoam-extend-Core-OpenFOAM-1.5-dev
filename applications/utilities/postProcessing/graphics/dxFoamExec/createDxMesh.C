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
#include "foamValid.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

DXField dxFvMesh::createDxMesh()
{
    DXArray dxPatchNames = NULL;

    if (boundaryMesh().size())
    {
        const char** patchNameStringsConst =
            new const char*
            [
                boundaryMesh().size()
              + faceZones().size()
            ];

        label nValidPatches = 0;
        forAll (boundaryMesh(), patchi)
        {
            if (valid(boundaryMesh()[patchi]))
            {
                patchNameStringsConst[nValidPatches++] =
                    boundaryMesh()[patchi].name().c_str();
            }
        }
        forAll(faceZones(), zoneI)
        {
            patchNameStringsConst[nValidPatches++] =
                faceZones()[zoneI].name().c_str();
        }

        char** patchNameStrings = const_cast<char**>(patchNameStringsConst);

        dxPatchNames =
            DXMakeStringListV
            (
                nValidPatches,
                patchNameStrings
            );
        delete[] patchNameStringsConst;

        if (!dxPatchNames)
        {
            DXSetError
            (
                ERROR_INTERNAL,
                "dxFoamExec:dxFvMesh::createDxMesh : Error in DXMakeStringListV"
            );
            return DXERROR;
        }
    }
    else
    {
        Info<< "dxFoamExec:dxFvMesh::createDxMesh : "
            << "no valid patches" << endl;
    }
    
    DXField dxMesh = DXNewField();

    if (!dxMesh)
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxMesh : Error creating dxMesh"
        );
        return DXERROR;
    }

    if
    (
        !DXSetStringAttribute
        (
            reinterpret_cast<Object>(dxMesh),
            "name",
            "mesh"
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxMesh : "
            "Error in DXSetStringAttribute name"
        );
        return DXERROR;
    }

    if
    (
        !DXSetComponentValue
        (
            dxMesh,
            "positions",
            reinterpret_cast<Object>(dxPositions_)
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxMesh : "
            "Error in DXSetComponentValue positions"
        );
        return DXERROR;
    }
    
    if
    (
        !DXSetComponentValue
        (
            dxMesh,
            "connections",
            reinterpret_cast<Object>(dxConnections_)
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxMesh : "
            "Error in DXSetComponentValue connections"
        );
        return DXERROR;
    }

    if
    (
        !DXSetComponentValue
        (
            dxMesh,
            "BFconnections",
            reinterpret_cast<Object>(dxBfaceCons_)
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxMesh : "
            "Error in DXSetComponentValue connections"
        );
        return DXERROR;
    }

    if
    (
        !DXSetComponentValue
        (
            dxMesh,
            "patchNames",
            reinterpret_cast<Object>(dxPatchNames)
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxMesh : "
            "Error in DXSetComponentValue patchNames"
        );
        return DXERROR;
    }

    if (!DXEndField(dxMesh))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxMesh : Error in DXEndField"
        );
        return DXERROR;
    }

    return dxMesh;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
