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
    Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

Class
    faceCoupleInfo

Description
    Container for information needed to couple to meshes. When constructed from
    two meshes and a geometric tolerance finds the corresponding boundary faces.

    The information it keeps is the set of faces&points (cutFaces, cutPoints)
    that should replace a set of faces on the master (masterPatch) and
    a set of faces on the slave (slavePatch)


    Uses same tolerance to match faces and points on matched faces since
    they both originate from the same points and the tolerance usually
    comes from writing these points with limited precision (6 by default)

    Can be constructed from slave being subdivision of master with the
    polyPatch constructor:

    Notes on multiple slave faces per master:

    As long as
    - all master edges are present in slave
    - slave can have extra edges/points
    So master:
    +-------+
    |       |
    |       |
    |       |
    |       |
    |       |
    |       |
    |       |
    +-------+

    slave:
    +---+---+
    |\  |  /|
    | \ | / |
    |  \|/  |
    +---+---+
    |  /|\  |
    | / | \ |
    |/  |  \|
    +---+---+

    is ok.

    For this kind of matching the order is:
    - match cutpoint to masterpoint
    - find those cutEdges that align with a master edge. This gives two sets
      of cut edges: those that have a master equivalent ('border edges') and
      those that don't ('internal edges'). The border edges now divide the
      cutFaces into regions with the same masterFace correspondence.
    - find cutFaces that are fully determined by the border edges they use.
    - all cutFaces that are connected through an internal edge have the same
      master face.


    Note: say decomposition into 4

    +--+--+
    | C| D|
    +--+--+
    | A| B|
    +--+--+

    If the original case gets reconstructed by addition in order:

    A = A+D
    A = A+B
    A = A+C

    the shared mid point between A,B,C,D is not on the cut when doing the
    first addition. So explicit provision is for points which are shared but
    not on a face that is shared. (sharedToMasterPoints, sharedToSlavePoints)


SourceFiles
    faceCoupleInfo.C


\*---------------------------------------------------------------------------*/

#ifndef faceCoupleInfo_H
#define faceCoupleInfo_H

#include "pointField.H"
#include "indirectPrimitivePatch.H"
#include "primitiveFacePatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Class forward declarations
class face;
class primitiveMesh;
class polyPatch;
class polyMesh;

/*---------------------------------------------------------------------------*\
                           Class faceCoupleInfo Declaration
\*---------------------------------------------------------------------------*/

class faceCoupleInfo
{
    // Private data

        //- Angle matching tolerance.
        static const scalar angleTol_;

        //- Master patch
        autoPtr<indirectPrimitivePatch> masterPatchPtr_;

        //- Slave patch
        autoPtr<indirectPrimitivePatch> slavePatchPtr_;


        //- Description of cut. Cut is the matching area between the slave
        //  and the master. All of the slave faces/edge/points will be present
        //  in the cut, but master faces might be subdivided.
        //  Cut consists of faces and points (note: could be expressed as some
        //  kind of PrimitivePatch which holds points instead of reference to
        //  them)
        //  Orientation of cutFaces should be same as masterFaces!
        pointField cutPoints_;
        autoPtr<primitiveFacePatch> cutFacesPtr_;

        //- Additional point coupling information. Is between points on
        //  boundary of both meshes.

        // Addressing from cut

            //- map from cutFaces to masterPatch
            labelList cutToMasterFaces_;
            labelList cutToMasterPoints_;
            labelList cutToMasterEdges_;

            //- map from cutFaces to slavePatch
            labelList cutToSlaveFaces_;
            labelList cutToSlavePoints_;
            labelList cutToSlaveEdges_;

            //- map from master to cut (unique points)
            labelList masterToCutPoints_;
            labelList masterToCutEdges_;
            labelList slaveToCutPoints_;
            labelList slaveToCutEdges_;


        // Shared points (but not on shared faces i.e. not on cut)

            //- pointlabels of master mesh that are shared with slave
            labelList sharedToMasterPoints_;
            //- pointlabels of slave mesh that are shared with master
            labelList sharedToSlavePoints_;


    // Private Member Functions

        // Debugging

            //- Calculate face centres from (subset of) faces.
            template<template<class> class FaceList>
            static pointField calcFaceCentres
            (
                const FaceList<face>&,
                const pointField&,
                const label start,
                const label size
            );

            //- Write edges
            static void writeOBJ
            (
                const fileName& fName,
                const edgeList& edges,
                const pointField& points,
                const bool compact = true
            );

            //- Write edges
            static void writeOBJ
            (
                const fileName& fName,
                const pointField& points0,
                const pointField& points1
            );

            //- Write connections between corresponding points and faces
            //  as .obj files.
            void writePointsFaces() const;

            //- Write connections between corresponding edges as .obj files.
            void writeEdges() const;


        // Edge handling/matching

            //- Find corresponding edges on patch when having only a map for
            //  the points.
            labelList findMappedEdges
            (
                const edgeList& edges,
                const labelList& pointMap,
                const indirectPrimitivePatch&
            );

            //- Check if edge on slavePatch corresponds to an edge between faces
            //  in two different polyPatches on the mesh.
            bool regionEdge(const polyMesh&, const label slaveEdgeI) const;

            //- Finds edge connected to point most aligned with master edge.
            label mostAlignedCutEdge
            (
                const bool report,
                const polyMesh& slaveMesh,
                const bool patchDivision,
                const label pointI,
                const label edgeStart,
                const label edgeEnd
            ) const;


        // Face matching

            //- Returns max distance to masterF of any point on cutF.
            static scalar maxDistance
            (
                const face& cutF,
                const pointField& cutPoints,
                const face& masterF,
                const pointField& masterPoints
            );

            //- Finds matching (boundary)face centres.
            //  Since faces identical uses geometric match on face centres.
            static void findPerfectMatchingFaces
            (
                const primitiveMesh& mesh0,
                const primitiveMesh& mesh1,
                const scalar absTol,

                labelList& mesh0Faces,
                labelList& mesh1Faces
            );

            //- Find matching (boundary)faces. Matching if slave is on top of
            //  master face (slaves is subdivision of master)
            static void findSlavesCoveringMaster
            (
                const primitiveMesh& mesh0,
                const primitiveMesh& mesh1,
                const scalar absTol,

                labelList& mesh0Faces,
                labelList& mesh1Faces
            );

            //- Grow cutToMasterFace across 'internal' edges.
            label growCutFaces(Map<labelList>&);

            void checkMatch() const;

            //- Gets a list of cutFaces (that use a master edge) and the
            // candidate master faces.
            // Checks among these master faces if there is only one remaining
            // unmatched one.
            label matchEdgeFaces(Map<labelList>& candidates);

            //- Gets a list of cutFaces (that use a master edge) and the
            //  candidate master faces.
            //  Finds most aligned master face.
            label geometricMatchEdgeFaces(Map<labelList>& candidates);


        //- Find point and edge correspondence for perfect matching faces
        void perfectPointMatch(const scalar absTol, const bool);

        //- Find point and edge correspondence for slaves being subdivision of
        //  master.
        void subDivisionMatch
        (
            const polyMesh& slaveMesh,
            const bool patchDivision,
            const scalar absTol
        );

        //- Get all boundary points not in excludePoints.
        static labelList getBoundaryPoints
        (
            const primitiveMesh&,
            const labelList& excludePoints
        );

        //- Set sharedMasterPoints_/sharedSlavePoints_ to all the points
        //  that are shared but not on a cut. Uses geometric matching with
        //  absTol on all buondary points that are not on the cut.
        void findNonCutSharedPoints
        (
            const primitiveMesh& masterMesh,
            const primitiveMesh& slaveMesh,
            const scalar absTol
        );


public:

    //- Runtime type information
    ClassName("faceCoupleInfo");


    // Constructors

        //- Construct from two meshes and absolute tolerance.
        //  Finds out matches geometrically. No checking for nonsense match.
        //  Tolerance is absolute one so use with care.
        //  perfectMatch : each point/edge/face has corresponding point on other
        //                 side
        faceCoupleInfo
        (
            const polyMesh& mesh0,
            const polyMesh& mesh1,
            const scalar absTol,
            const bool perfectMatch
        );

        //- Construct from meshes and subset of mesh faces
        //  (i.e. indirectPrimitivePatch addressing)
        //  All faces in patch are considered matched (but don't have to be 
        //  ordered)
        //  perfectMatch : each point/edge/face has corresponding point on other
        //                 side
        //  orderedFaces : faces in patch are ordered (so masterAddressing[i]
        //  matches slaveAddressing[i])
        //  patchDivision: faces in slave mesh that originate from the
        //  same master face have the same patch. Used by some triangulation
        //  methods.
        faceCoupleInfo
        (
            const polyMesh& masterMesh,
            const labelList& masterAddressing,
            const polyMesh& slaveMesh,
            const labelList& slaveAddressing,
            const scalar absTol,
            const bool perfectMatch,
            const bool orderedFaces,
            const bool patchDivision
        );


    // Destructor

        ~faceCoupleInfo();



    // Member Functions

        //- Utility functions

            //- Get patch face labels
            static labelList faceLabels(const polyPatch&);

            //- Create Map from List
            static Map<label> makeMap(const labelList&);
            static Map<labelList> makeMap(const labelListList&);


        // Access

            //- Addressing engine for coupled faces on mesh0
            const indirectPrimitivePatch& masterPatch() const
            {
                return masterPatchPtr_();
            }

            //- Addressing engine for coupled faces on mesh1
            const indirectPrimitivePatch& slavePatch() const
            {
                return slavePatchPtr_();
            }

            //- Addressing engine for combined set of faces.
            const primitiveFacePatch& cutFaces() const
            {
                return cutFacesPtr_();
            }

            //- Points for combined set of faces.
            const pointField& cutPoints() const
            {
                return cutPoints_;
            }


            //- Points which are not on cut but are shared between
            //  master and slave.
            const labelList& sharedToMasterPoints() const
            {
                return sharedToMasterPoints_;
            }
            const labelList& sharedToSlavePoints() const
            {
                return sharedToSlavePoints_;
            }


        // Addressing from meshes to cut and vice versa.

            //- Master face for every face on cut. Will always be at least
            //  one but there might be multiple cut faces pointing to the same
            //  master
            const labelList& cutToMasterFaces() const
            {
                return cutToMasterFaces_;
            }

            //- Master point for every point on cut. -1 means no equivalent
            //  master point. All master points will be present though in cut.
            const labelList& cutToMasterPoints() const
            {
                return cutToMasterPoints_;
            }

            //- Master edge for every edge on cut. -1 means no equivalent
            //  master edge. All master edges will be present though in cut.
            const labelList& cutToMasterEdges() const
            {
                return cutToMasterEdges_;
            }

            //- Slave face for every face on cut. Will always be at least
            //  one but there might be multiple cut faces pointing to the same
            //  master
            const labelList& cutToSlaveFaces() const
            {
                return cutToSlaveFaces_;
            }

            //- Slave point for every point on cut. All slave points will
            //  be present in cut.
            const labelList& cutToSlavePoints() const
            {
                return cutToSlavePoints_;
            }

            //- Slave edge for every edge on cut. -1 means no equivalent
            //  slave edge. All slave edges will be present though in cut.
            const labelList& cutToSlaveEdges() const
            {
                return cutToSlaveEdges_;
            }
 
            //- Cut point for master point. All master points will be present.
            const labelList& masterToCutPoints() const
            {
                return masterToCutPoints_;
            }
            //- Cut edge for master edge. Might be -1 since not all
            //  master edges will be present in cut.
            const labelList& masterToCutEdges() const
            {
                return masterToCutEdges_;
            }
            //- Cut point for slave point. All slave points will be present in
            //  cut.
            const labelList& slaveToCutPoints() const
            {
                return slaveToCutPoints_;
            }
            //- Cut edge for slave edge. All slave edges will be present in
            //  cut.
            const labelList& slaveToCutEdges() const
            {
                return slaveToCutEdges_;
            }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "faceCoupleInfoTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
