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
#include "cellModeller.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{


// Add 'simple' face to faces arrays(starting at bi). Returns success status
bool dxFvMesh::addFace
(
    const labelList& faceLabels,
    label& bi,
    int *facesArray
)
{
    label nFacePoints = faceLabels.size();

    switch(nFacePoints)
    {
        case nQuadPoints:  // quad
        {
            facesArray[bi++] = faceLabels[0];
            facesArray[bi++] = faceLabels[1];
            facesArray[bi++] = faceLabels[3];
            facesArray[bi++] = faceLabels[2];
        }
        break;

        case 3:  // tri
        {
            facesArray[bi++] = faceLabels[0];
            facesArray[bi++] = faceLabels[1];
            facesArray[bi++] = faceLabels[0];
            facesArray[bi++] = faceLabels[2];
        }
        break;

        default:
        {
            SeriousError<< "Unknown face shape " << faceLabels << endl;

            return false;
        }
    }
    return true;            
}


// Add 'simple' face to cells (starting at i), faces arrays(starting at bi).
// Returns success status
bool dxFvMesh::addFace
(
    const labelList& faceLabels,

    label& bi,
    int *facesArray,

    label& i,
    int *cellsArray
)
{
    label nFacePoints = faceLabels.size();

    switch(nFacePoints)
    {
        case nQuadPoints:  // quad
        {
            facesArray[bi++] = faceLabels[0];
            facesArray[bi++] = faceLabels[1];
            facesArray[bi++] = faceLabels[3];
            facesArray[bi++] = faceLabels[2];

            cellsArray[i++] = faceLabels[0];
            cellsArray[i++] = faceLabels[1];
            cellsArray[i++] = faceLabels[3];
            cellsArray[i++] = faceLabels[2];
            cellsArray[i++] = faceLabels[0];
            cellsArray[i++] = faceLabels[1];
            cellsArray[i++] = faceLabels[3];
            cellsArray[i++] = faceLabels[2];
        }
        break;

        case 3:  // tri
        {
            facesArray[bi++] = faceLabels[0];
            facesArray[bi++] = faceLabels[1];
            facesArray[bi++] = faceLabels[0];
            facesArray[bi++] = faceLabels[2];

            cellsArray[i++] = faceLabels[0];
            cellsArray[i++] = faceLabels[1];
            cellsArray[i++] = faceLabels[0];
            cellsArray[i++] = faceLabels[2];
            cellsArray[i++] = faceLabels[0];
            cellsArray[i++] = faceLabels[1];
            cellsArray[i++] = faceLabels[0];
            cellsArray[i++] = faceLabels[2];
        }
        break;

        default:
        {
            SeriousError<< "Unknown face shape " << faceLabels << endl;

            return false;
        }
    }
    return true;            
}




void dxFvMesh::createDxPatchFaces
(
    const faceList& faces,
    const label patchi,

    int *facesArray,
    int *cellsArray,

    // Indices for original cells/faces
    label& i,
    label& bi,

    // Indices for additional cells/faces
    label& ni,
    label& nbi,

    // Additional faces
    label& afi
)
{
    forAll(faces, facei)
    {
        const face& faceLabels = faces[facei];
        label nFacePoints = faceLabels.size();

        if (nFacePoints <= nQuadPoints)
        {
            addFace(faceLabels, bi, facesArray, i, cellsArray);
        }
        else
        {
            // Decomposition from vertex 0. Results in one or more quads
            // and zero or one triangles

            label nQuads = (nFacePoints - 2)/2;
            label nTris = (nFacePoints - 2)%2;

            // Enter first quad in place of original cell
            labelList quad(4);
            quad[0] = faceLabels[0];
            quad[1] = faceLabels[1];
            quad[2] = faceLabels[2];
            quad[3] = faceLabels[3];
            addFace(quad, bi, facesArray, i, cellsArray);

            // Current face vertex; edge from 0 to qpi is starting edge
            label qpi = 2;

            // Remaining (if any) quads
            for (label quadi=1; quadi<nQuads; quadi++)
            {
                superFacePatches_[afi] = patchi;
                superFaces_[afi++] = facei;

                quad[0] = faceLabels[0];
                quad[1] = faceLabels[qpi + 1];
                quad[2] = faceLabels[qpi + 2];
                quad[3] = faceLabels[qpi + 3];
                addFace(quad, nbi, facesArray, ni, cellsArray);

                qpi += 2;
            }

            // Remaining (if any) single triangle
            if (nTris)
            {
                superFacePatches_[afi] = patchi;
                superFaces_[afi++] = facei;

                labelList tri(3);
                tri[0] = faceLabels[0];
                tri[1] = faceLabels[qpi + 1];
                tri[2] = faceLabels[qpi + 2];
                addFace(tri, nbi, facesArray, ni, cellsArray);
            }
        }
    }
}


void dxFvMesh::createDxZoneFaces
(
    const labelList& indices,
    const label zonei,

    int *facesArray,

    // Index for original faces
    label& bi,

    // Indices for additional faces
    label& nbi,

    // Additional faces
    label& afi
)
{
    forAll(indices, indexi)
    {
        label facei = indices[indexi];

        if (facei >= nFaces())
        {
            continue;
        }

        const face& faceLabels = allFaces()[facei];
        label nFacePoints = faceLabels.size();

        if (nFacePoints <= nQuadPoints)
        {
            addFace(faceLabels, bi, facesArray);
        }
        else
        {
            // Decomposition from vertex 0. Results in one or more quads
            // and zero or one triangles

            label nQuads = (nFacePoints - 2)/2;
            label nTris = (nFacePoints - 2)%2;

            // Enter first quad in place of original cell
            labelList quad(4);
            quad[0] = faceLabels[0];
            quad[1] = faceLabels[1];
            quad[2] = faceLabels[2];
            quad[3] = faceLabels[3];
            addFace(quad, bi, facesArray);

            // Current face vertex; edge from 0 to qpi is starting edge
            label qpi = 2;

            // Remaining (if any) quads
            for (int quadi=1; quadi<nQuads; quadi++)
            {
                superZFaces_[afi] = facei;
                superZFaceZones_[afi++] = zonei;

                quad[0] = faceLabels[0];
                quad[1] = faceLabels[qpi + 1];
                quad[2] = faceLabels[qpi + 2];
                quad[3] = faceLabels[qpi + 3];
                addFace(quad, nbi, facesArray);

                qpi += 2;
            }

            // Remaining (if any) single triangle
            if (nTris)
            {
                superZFaces_[afi] = facei;
                superZFaceZones_[afi++] = zonei;

                labelList tri(3);
                tri[0] = faceLabels[0];
                tri[1] = faceLabels[qpi + 1];
                tri[2] = faceLabels[qpi + 2];
                addFace(tri, nbi, facesArray);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

DXArray dxFvMesh::createDxConnections()
{
    Info<< "dxFoamExec:dxFvMesh::createDxConnections : create DX connections"
        << endl;

    const cellModel& hex = *(cellModeller::lookup("hex"));
    const cellModel& wedge = *(cellModeller::lookup("wedge"));
    const cellModel& prism = *(cellModeller::lookup("prism"));
    const cellModel& pyr = *(cellModeller::lookup("pyr"));
    const cellModel& tet = *(cellModeller::lookup("tet"));
    const cellModel& tetWedge = *(cellModeller::lookup("tetWedge"));

    const cellList& cls = cells();
    const faceList& fcs = faces();
    const cellShapeList& cellShps = cellShapes();

    // Number of original cells in FOAM mesh
    label nOCells = nCells();

    // Number of additional points needed by the decomposition of polyhedra
    label nAddPoints = 0;

    // Number of additional cells generated by the decomposition of polyhedra
    label nAddCells = 0;

    // Scan for cells which need to be decomposed and count additional points
    // and cells

    forAll(cellShps, celli)
    {
        const cellModel& model = cellShps[celli].model();

        if 
        (
            model != hex
         && model != wedge
         && model != prism
         && model != pyr
         && model != tet
         && model != tetWedge
        )
        {
            forAll(cls[celli], facei)
            {
                label nFacePoints = fcs[cls[celli][facei]].size();

                label nQuads = (nFacePoints - 2)/2;
                label nTris = (nFacePoints - 2)%2;
                nAddCells += nQuads + nTris;
            }

            nAddCells--;
            nAddPoints++;
        }
    }


    // Scan boundary patches for the number of faces in the valid patches
    // and for additional faces generated by cell decomposition
    
    nBfaces_ = 0;
    label nAddBfaces = 0;

    const polyBoundaryMesh& patches = boundaryMesh();

    forAll (patches, patchi)
    {
        const polyPatch& faces = patches[patchi];

        if (valid(faces))
        {
            nBfaces_ += faces.size();

            forAll(faces, facei)
            {
                label nFacePoints = faces[facei].size();

                if (nFacePoints > nQuadPoints)
                {
                    label nQuads = (nFacePoints - 2)/2;
                    label nTris = (nFacePoints - 2)%2;

                    nAddBfaces += nQuads + nTris - 1;
                }
            }
        }
    }

    // Treat faceZones like boundary patches

    nZFaces_ = 0;
    label nAddZFaces = 0;

    forAll(faceZones(), zonei)
    {
        const labelList& indices = faceZones()[zonei];

        forAll(indices, indexi)
        {
            label facei = indices[indexi];

            // Skip inactive faces for now.
            if (facei >= nFaces())
            {
                //Info<< "Skipping facei:" << facei
                //    << "  nFaces:" << nFaces() << endl;

                continue;
            }

            nZFaces_++;

            // Could use faces() here since inactive faces already skipped.
            label nFacePoints = allFaces()[facei].size();

            //Info<< "Using facei:" << facei
            //    << "  nFaces:" << nFaces()
            //    << "  nFacePoints:" << nFacePoints << endl;

            if (nFacePoints > nQuadPoints)
            {
                label nQuads = (nFacePoints - 2)/2;
                label nTris = (nFacePoints - 2)%2;

                nAddZFaces += nQuads + nTris - 1;
            }
        }
    }


    // Total number of boundaryFaces and faceZones in DX mesh
    label totalNBfaces = nBfaces_ + nAddBfaces;
    label totalNZFaces = nZFaces_ + nAddZFaces;


    //Info<< "After Scanning" << endl
    //    << "    nAddCells:" << nAddCells << endl
    //    << "    nAddPoints:" << nAddPoints << endl
    //    << "    nBfaces_:" << nBfaces_ << endl
    //    << "    nAddBfaces:" << nAddBfaces << endl
    //    << "    nZFaces_:" << nZFaces_ << endl
    //    << "    nAddZFaces:" << nAddZFaces << endl
    //    << "    totalNBfaces:" << totalNBfaces << endl
    //    << "    totalNZFaces:" << totalNZFaces << endl;

    // Total number of cells in DX mesh
    totalNCells_ = nOCells + nAddCells + totalNBfaces;

    // Set size of additional point addressing array
    addPointCellLabels_.setSize(nAddPoints);

    // Set size of additional cells mapping array
    superCells_.setSize(nAddCells);

    // Set size of additional faces patch mapping array
    superFacePatches_.setSize(nAddBfaces);

    // Set size of additional faces face mapping array
    superFaces_.setSize(nAddBfaces);

    // faceZones

    // Set size of additional zone faces mapping array
    superZFaceZones_.setSize(nAddZFaces);

    // Set size of additional zone faces face mapping array
    superZFaces_.setSize(nAddZFaces);


    // Create temporary storage for DX connections array vector
    int *bFacesArray = new int[nQuadPoints*(totalNBfaces+totalNZFaces)];

    // Create temporary storage for DX connections array vector
    int *cellsArray = new int[nHexPoints*totalNCells_];

    // Set counters for connections, additional points and cells
    label i = 0, api = 0, aci = 0;

    // Additional points start at the end of the points array
    label addPointi = nPoints();

    // Scan cells and build connections array
    forAll(cellShps, celli)
    {
        const cellModel& model = cellShps[celli].model();
        const labelList& shapeLabels = cellShps[celli];

        if (model == hex)
        {
            cellsArray[i++] = shapeLabels[0];
            cellsArray[i++] = shapeLabels[1];
            cellsArray[i++] = shapeLabels[3];
            cellsArray[i++] = shapeLabels[2];
            cellsArray[i++] = shapeLabels[4];
            cellsArray[i++] = shapeLabels[5];
            cellsArray[i++] = shapeLabels[7];
            cellsArray[i++] = shapeLabels[6];
        }
        else if (model == wedge)
        {
            cellsArray[i++] = shapeLabels[0];
            cellsArray[i++] = shapeLabels[1];
            cellsArray[i++] = shapeLabels[0];
            cellsArray[i++] = shapeLabels[2];
            cellsArray[i++] = shapeLabels[3];
            cellsArray[i++] = shapeLabels[4];
            cellsArray[i++] = shapeLabels[6];
            cellsArray[i++] = shapeLabels[5];
        }
        else if (model == prism)
        {
            cellsArray[i++] = shapeLabels[0];
            cellsArray[i++] = shapeLabels[1];
            cellsArray[i++] = shapeLabels[0];
            cellsArray[i++] = shapeLabels[2];
            cellsArray[i++] = shapeLabels[3];
            cellsArray[i++] = shapeLabels[4];
            cellsArray[i++] = shapeLabels[3];
            cellsArray[i++] = shapeLabels[5];
        }
        else if (model == pyr)
        {
            cellsArray[i++] = shapeLabels[0];
            cellsArray[i++] = shapeLabels[1];
            cellsArray[i++] = shapeLabels[3];
            cellsArray[i++] = shapeLabels[2];
            cellsArray[i++] = shapeLabels[4];
            cellsArray[i++] = shapeLabels[4];
            cellsArray[i++] = shapeLabels[4];
            cellsArray[i++] = shapeLabels[4];
        }
        else if (model == tet)
        {
            cellsArray[i++] = shapeLabels[0];
            cellsArray[i++] = shapeLabels[1];
            cellsArray[i++] = shapeLabels[0];
            cellsArray[i++] = shapeLabels[2];
            cellsArray[i++] = shapeLabels[3];
            cellsArray[i++] = shapeLabels[3];
            cellsArray[i++] = shapeLabels[3];
            cellsArray[i++] = shapeLabels[3];
        }
        else if (model == tetWedge)
        {
            cellsArray[i++] = shapeLabels[0];
            cellsArray[i++] = shapeLabels[1];
            cellsArray[i++] = shapeLabels[2];
            cellsArray[i++] = shapeLabels[2];
            cellsArray[i++] = shapeLabels[3];
            cellsArray[i++] = shapeLabels[3];
            cellsArray[i++] = shapeLabels[4];
            cellsArray[i++] = shapeLabels[4];
        }
        else
        {
            addPointCellLabels_[api++] = celli;

            bool substituteCell = true;

            forAll(cls[celli], facei)
            {
                const labelList& faceLabels = fcs[cls[celli][facei]];
                label nFacePoints = faceLabels.size();

                label nQuads = (nFacePoints - 2)/2;
                label nTris = (nFacePoints - 2)%2;

                label qpi = 0;

                for (int quadi=0; quadi<nQuads; quadi++)
                {
                    label ni = i;

                    if (substituteCell)
                    {
                        i += nHexPoints;
                        substituteCell = false;
                    }
                    else
                    {
                        superCells_[aci] = celli;
                        ni = nHexPoints*(nOCells + aci++);
                    }

                    cellsArray[ni++] = faceLabels[0];
                    cellsArray[ni++] = faceLabels[qpi + 1];
                    cellsArray[ni++] = faceLabels[qpi + 3];
                    cellsArray[ni++] = faceLabels[qpi + 2];
                    cellsArray[ni++] = addPointi;
                    cellsArray[ni++] = addPointi;
                    cellsArray[ni++] = addPointi;
                    cellsArray[ni++] = addPointi;

                    qpi += 2;
                }

                if (nTris)
                {
                     label ni = i;

                    if (substituteCell)
                    {
                        i += nHexPoints;
                        substituteCell = false;
                    }
                    else
                    {
                        superCells_[aci] = celli;
                        ni = nHexPoints*(nOCells + aci++);
                    }

                    cellsArray[ni++] = faceLabels[0];
                    cellsArray[ni++] = faceLabels[qpi + 1];
                    cellsArray[ni++] = faceLabels[0];
                    cellsArray[ni++] = faceLabels[qpi + 2];
                    cellsArray[ni++] = addPointi;
                    cellsArray[ni++] = addPointi;
                    cellsArray[ni++] = addPointi;
                    cellsArray[ni++] = addPointi;
                }
            }

            addPointi++;
        }
    }


    // Reset cells connection counter to start after the additional cells
    i = nHexPoints*(nOCells + nAddCells);

    // Create a boundary face connection counter
    label bi = 0;

    // Set additional face counter
    label afi = 0;

    // Cells from decomposed patch faces start after storage for undecomposed
    // ones
    label ni = nHexPoints*(nOCells + nAddCells + nBfaces_);

    // Additional boundary face connection counter. Faces from decomposed
    // faces start after undecomposed ones
    label nbi = nQuadPoints*nBfaces_;


    // Scan patches and build connections (Note: faces are converted into hexes)
    forAll (patches, patchi)
    {
        const polyPatch& faces = patches[patchi];

        if (valid(faces))
        {
            createDxPatchFaces
            (
                faces,
                patchi,
                bFacesArray,
                cellsArray,
                i,
                bi,
                ni,
                nbi,
                afi
            );
        }
    }

    // Add faceZones to facesArray

    // Face zones stored after all boundary faces
    bi = nQuadPoints*totalNBfaces;


    // Additional faces from zones stored after undecomposed ones
    nbi = bi + nQuadPoints*nZFaces_;

    // Additional faces counter
    afi = 0;

    forAll(faceZones(), zonei)
    {
        createDxZoneFaces
        (
            faceZones()[zonei],
            zonei,
            bFacesArray,
            bi,
            nbi,
            afi
        );
    }


    // Some checks
    if 
    (
        (bi > nQuadPoints*(totalNBfaces+totalNZFaces))
     || (nbi != nQuadPoints*(totalNBfaces+totalNZFaces))
    )
    {
        SeriousError
            << "dxFoamExec:dxFvMesh::createDxConnections : "
            << "bFacesArray over/underflow:" << endl
            << "    i:" << i << endl
            << "    ni:" << ni << endl
            << "    bi:" << bi << endl
            << "    nbi:" << nbi << endl
            << "    allocated:" << nQuadPoints*(totalNBfaces+totalNZFaces)
            << endl;

        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxConnections : "
            "bFacesArray over/underflow"
        );

        delete[] cellsArray;
        delete[] bFacesArray;

        return DXERROR;
    }

    if 
    (
        (i > nHexPoints*totalNCells_)
     || (ni != nHexPoints*totalNCells_)
    )
    {
        SeriousError
            << "dxFoamExec:dxFvMesh::createDxConnections : "
            << "cellsArray over/underflow:" << endl
            << "    i:" << i << endl
            << "    ni:" << ni << endl
            << "    bi:" << bi << endl
            << "    nbi:" << nbi << endl
            << "    allocated:" << nQuadPoints*(totalNBfaces+totalNZFaces)
            << endl;

        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxConnections : "
            "cellsArray over/underflow"
        );

        delete[] cellsArray;
        delete[] bFacesArray;

        return DXERROR;
    }



    // Create array objects for cell connections
    // -----------------------------------------
    DXArray dxCellCons = DXNewArray(TYPE_INT, CATEGORY_REAL, 1, nHexPoints);

    if (!DXAddArrayData(dxCellCons, 0, totalNCells_, cellsArray))
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxConnections : Error in DXAddArrayData"
        );

        delete[] cellsArray;
        delete[] bFacesArray;

        return DXERROR;
    }

    delete[] cellsArray;

    if
    (
        !DXSetStringAttribute
        (
            reinterpret_cast<Object>(dxCellCons),
            "element type",
            "cubes"
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxConnections : "
            "Error in DXSetStringAttribute element type"
        );

        delete[] bFacesArray;

        return DXERROR;
    }

    if
    (
        !DXSetStringAttribute
        (
            reinterpret_cast<Object>(dxCellCons),
            "ref",
            "positions"
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxConnections : "
            "Error in DXSetStringAttribute ref"
        );

        delete[] bFacesArray;

        return DXERROR;
    }

    if
    (
        !DXSetStringAttribute
        (
            reinterpret_cast<Object>(dxCellCons),
            "dep",
            "connections"
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxConnections : "
            "Error in DXSetStringAttribute dep"
        );

        delete[] bFacesArray;

        return DXERROR;
    }


    // Create array objects for boundary face connections
    // --------------------------------------------------
    dxBfaceCons_ = DXNewArray(TYPE_INT, CATEGORY_REAL, 1, nQuadPoints);

    if
    (
        !DXAddArrayData
        (
            dxBfaceCons_,
            0,
            totalNBfaces+totalNZFaces,
            bFacesArray
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxConnections : Error in DXAddArrayData"
        );

        delete[] bFacesArray;

        return DXERROR;
    }

    delete[] bFacesArray;

    if
    (
        !DXSetStringAttribute
        (
            reinterpret_cast<Object>(dxBfaceCons_),
            "element type",
            "quads"
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxConnections : "
            "Error in DXSetStringAttribute element type"
        );
        return DXERROR;
    }

    if
    (
        !DXSetStringAttribute
        (
            reinterpret_cast<Object>(dxBfaceCons_),
            "ref",
            "positions"
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxConnections : "
            "Error in DXSetStringAttribute ref"
        );
        return DXERROR;
    }

    if
    (
        !DXSetStringAttribute
        (
            reinterpret_cast<Object>(dxBfaceCons_),
            "dep",
            "BFconnections"
        )
    )
    {
        DXSetError
        (
            ERROR_INTERNAL,
            "dxFoamExec:dxFvMesh::createDxConnections : "
            "Error in DXSetStringAttribute dep"
        );
        return DXERROR;
    }


    return dxCellCons;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
