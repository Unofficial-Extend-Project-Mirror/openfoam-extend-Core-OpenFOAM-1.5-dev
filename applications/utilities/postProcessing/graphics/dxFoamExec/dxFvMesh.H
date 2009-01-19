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
    dxFvMesh

Description
    fvMesh + decomposed mesh for dx usage.

    The additional mesh from decomposition is accessed through
    - superCells : gives original cell label for additional cells
    - superFacePatches: gives original patch number for decomposed patch faces
    - superFaces: gives original patch face number for decomposed patch
        faces
    - superZFaceZones: original zone number for decomposed faceZone faces
    - superZFaces: original mesh face number for decomposed faceZone faces

    Storage is

    dxPositions:
        - mesh.allPoints()
        - mesh.cellCentres()

    dxConnections:
        - mesh.cells() with first cell of decomposition replacing the original
        - remaining cells from decomposition
        - patch faces extruded into cell. First cell of face decomposition
          replacing the original
        - remaining cells from patch face decomposition

    dxBfaceCons:
    (needed to be able to pick up patches separately)
        - patch faces as quads. First face of face decomposition replaces
          original
        - remaining faces from patch face decomposition
        - faceZone faces as quads. First face of decomposition replaces original
        - remaining faces from zone face decomposition.

SourceFiles
    dxFvMesh.C

\*---------------------------------------------------------------------------*/

#ifndef dxFvMesh_H
#define dxFvMesh_H

#include "fvMesh.H"
#include "tetPolyMesh.H"
#include "volPointInterpolation.H"
#include "dxFoamExec.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
class dxDatabase;
class Ostream;

/*---------------------------------------------------------------------------*\
                           Class dxFvMesh Declaration
\*---------------------------------------------------------------------------*/

class dxFvMesh
:
    public fvMesh
{
    // Private data

        pointMesh pointMesh_;
        volPointInterpolation volPointInterpolation_;

        //- Directory containing the mesh faces
        fileName facesDir_;

        //- Directory containing the mesh points
        fileName pointsDir_;

        //- Additional points (cell centres) from polyhedral cell decomposition
        labelList addPointCellLabels_;

        //- Original cell labels of the cells generated from the 
        //  polyhedral cell decomposition
        labelList superCells_;

        //- Number of valid boundary faces
        label nBfaces_;

        //- Original face patch labels of the faces generated from the 
        //  polyhedral cell decomposition
        labelList superFacePatches_;

        //- Original face labels of the faces generated from the 
        //  polyhedral cell decomposition
        labelList superFaces_;

        //- Number of faceZone faces
        label nZFaces_;

        //- Original zone number of the faces generated from the 
        //  faceZone face decomposition
        labelList superZFaceZones_;

        //- Original face labels of the faces generated from the 
        //  faceZone face decomposition
        labelList superZFaces_;

        //- Total number of cells
        label totalNCells_;

        //- Demand driven pointer to tetPolyMesh
        tetPolyMesh* tetMeshPtr_;

        //- Number of points in a DX boundary face (they are all quads!)
        static const int nQuadPoints = 4;

        //- Number of points in a DX cell (they are all hexes!)
        static const int nHexPoints = 8;

        //- The DX boundary face connections array
        DXArray dxBfaceCons_;

        //- The DX cell connections array
        DXArray dxConnections_;

        //- The DX positions array
        DXArray dxPositions_;

        //- The DX mesh (combines dxPositions and dxConnections)
        DXField dxMesh_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        dxFvMesh(const dxFvMesh&);

        //- Disallow default bitwise assignment
        void operator=(const dxFvMesh&);

        //- Add single face to Dx connections faces array
        static bool addFace
        (
            const labelList& faceLabels,
            label& bi,
            int *facesArray
        );

        //- Add single face to Dx connections faces and cells arrays
        static bool addFace
        (
            const labelList& faceLabels,
            label& bi,
            int *facesArray,
            label& i,
            int *cellsArray
        );


        //- Add patchfaces to bFacesArray and cellsArray
        void createDxPatchFaces
        (
            const faceList& faces,
            const label patchi,
            int *facesArray,
            int *cellsArray,
            label& i,
            label& bi,
            label& ni,
            label& nbi,
            label& afi
        );

        //- Add zone faces to bFacesArray
        void createDxZoneFaces
        (
            const labelList& indices,
            const label zonei,
            int *facesArray,
            label& bi,
            label& nbi,
            label& afi
        );

        DXArray createDxPositions();
        DXArray createDxConnections();
        DXField createDxMesh();

public:

    // Constructors

        //- Construct from components
        dxFvMesh
        (
            dxDatabase&,
            IOobject::readOption r=IOobject::MUST_READ,
            IOobject::writeOption w=IOobject::NO_WRITE
        );


    // Destructor

        ~dxFvMesh();


    // Member Functions

        // Access

            const fileName& facesDir() const
            {
                return facesDir_;
            }

            label totalNPoints() const
            {
                return nPoints() + addPointCellLabels_.size();
            }

            const labelList& addPointCellLabels() const
            {
                return addPointCellLabels_;
            }

            const labelList& superCells() const
            {
                return superCells_;
            }

            const labelList& superFacePatches() const
            {
                return superFacePatches_;
            }

            const labelList& superFaces() const
            {
                return superFaces_;
            }

            const labelList& superZFaceZones() const
            {
                return superZFaceZones_;
            }

            const labelList& superZFaces() const
            {
                return superZFaces_;
            }

            label totalNCells() const
            {
                return totalNCells_;
            }

            label totalBfaces() const
            {
                return nBfaces_ + superFaces_.size();
            }

            label totalZFaces() const
            {
                return nZFaces_ + superZFaces_.size();
            }

            const tetPolyMesh& tetMesh() const
            {
                if (!tetMeshPtr_)
                {
                    FatalErrorIn("dxFvMesh::tetMesh()")
                        << "tetMesh not set" << abort(FatalError);
                }
                return *tetMeshPtr_;
            }

            DXField DXMesh() const
            {
                return dxMesh_;
            }

            //- Interpolate volField returning pointField
            //  using inverse distance weighting
            template<class Type>
            tmp<GeometricField<Type, pointPatchField, pointMesh> > interpolate
            (
                const GeometricField<Type, fvPatchField, volMesh>& vf
            ) const
            {
                return volPointInterpolation_.interpolate(vf);
            }


        // Edit

            //- Create tetMesh
            void setTetMesh();

            //- meshMoved set if mesh has moved. Returns DX error code.
            Error move(bool& meshMoved);


        // Write

            //- Print some statistics on mesh
            void writeStats(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//extern "C"
//{
//    Error deleteMeshRef(Pointer dxFoamMeshPtr);
//}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
