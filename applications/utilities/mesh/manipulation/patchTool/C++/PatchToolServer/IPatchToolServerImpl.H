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
    IPatchToolServerImpl

Description

SourceFiles
    IPatchToolServerImpl.C

\*---------------------------------------------------------------------------*/

#ifndef IPatchToolServerImpl_H
#define IPatchToolServerImpl_H

// Foam header files.
#include "word.H"
#include "stringList.H"
#include "fileName.H"
#include "Dictionary.H"
#include "boundaryMesh.H"
#include "Time.H"

// FoamX header files.
#include "PatchTool.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace FoamX
{

// Class forward declarations.
class Orb;

/*---------------------------------------------------------------------------*\
                         Class IPatchToolServerImpl Declaration
\*---------------------------------------------------------------------------*/

class IPatchToolServerImpl
:
    public POA_FoamXServer::PatchToolServer::IPatchToolServer,
    public PortableServer::RefCountServantBase
{
    // Private data

        //- Case root directory.
        Foam::fileName rootDir_;

        //- Case name.
        Foam::fileName caseName_;

        Foam::Time db_;

        Foam::boundaryMesh bMesh_;

        //- Context name for name server registration.
        Foam::fileName hostContext_; 

        //- Context name for name server registration.
        Foam::fileName userContext_; 

        //- Object name for name server registration.
        Foam::fileName objectName_; 

        //- Case dictionary filename.
        Foam::fileName caseDictName_;

        //- Reference to the ORB
        Orb& orb_;


    // Static Functions

        //- Copy boolList into Corba BoolList
        static void copy(const Foam::boolList&, FoamXServer::BoolList_out&);

        //- Copy labelList into Corba LongList
        static void copy(const Foam::labelList&, FoamXServer::LongList_out&);

        //- Copy scalarField into Corba FloatList
        static void copy(const Foam::scalarField&, FoamXServer::FloatList_out&);

        //- Copy vectorField into Corba FloatList
        static void copy(const Foam::vectorField&, FoamXServer::FloatList_out&);

        //- Copy LongList into labelList
        static Foam::labelList copy(const FoamXServer::LongList& elems);


public:

    // Constructors

        IPatchToolServerImpl
        (
            Orb& orb,
            const Foam::fileName& rootDir,
            const Foam::fileName& caseName
        );


    // Destructor

        virtual ~IPatchToolServerImpl();


    // Member Functions

            virtual char* caseRoot();

            virtual char* caseName();
 
        // Database

            //- Time-steps.
            virtual FoamXServer::StringList* availableTimeSteps();

            virtual char* getTime();

            virtual void setTime(const char*, const CORBA::Long);

        // I/O

            //- Read current mesh from objectRegistry and construct boundaryMesh
            virtual void read();

            //- Read already triangulated surface from file
            virtual void readTriSurface(const char*);

            //- Apply to current mesh and write mesh
            virtual void write();

            //- Write to file
            virtual void writeTriSurface(const char*);

        // Access

            virtual void getPoints(FoamXServer::FloatList_out allPoints);

            virtual CORBA::Long getNFaces();

            virtual CORBA::Long getNEdges();

            virtual void getEdges(FoamXServer::LongList_out verts);

        // Feature edges and extra edges

            virtual void setFeatureEdges(const CORBA::Double minCos);

            virtual void getFeaturePoints(FoamXServer::FloatList_out points);

            virtual void getFeatureEdges(FoamXServer::LongList_out verts);

            virtual void getFeatureToEdge(FoamXServer::LongList_out edgeLabels);

            virtual void getEdgeToFeature(FoamXServer::LongList_out featLabels);

            virtual void getFeatureSegments
            (
                FoamXServer::LongLongList_out featLabels
            );

            virtual void setExtraEdges(const CORBA::Long edgeI);

            virtual void getExtraEdges(FoamXServer::LongList_out edgeLabels);

        // Patches

            virtual CORBA::Long whichPatch(const CORBA::Long faceI);

            virtual CORBA::Long findPatchID(const char* patchName);

            virtual void getPatches(FoamXServer::SurfacePatchList_out patches);

            virtual void addPatch(const char* patchName);

            virtual void deletePatch(const char* patchName);

            virtual void changePatchType(const char* patchName, const char*);

            virtual void changeFaces
            (
                const FoamXServer::LongList& patchIDs,
                FoamXServer::LongList_out oldToNew
            );

        // Face handling

            virtual CORBA::Long getNTris
            (
                const CORBA::Long startFaceI,
                const CORBA::Long nFaces,
                FoamXServer::LongList_out nTris
            );

            virtual void triangulate
            (
                const CORBA::Long startFaceI,
                const CORBA::Long nFaces,
                const CORBA::Long totalNtris,
                FoamXServer::LongList_out triVerts
            );

            virtual CORBA::Long getNPoints
            (
                const CORBA::Long startFaceI,
                const CORBA::Long nFaces
            );

            virtual void triangulateLocal
            (
                const CORBA::Long startFaceI,
                const CORBA::Long nFaces,
                const CORBA::Long totalNtris,
                FoamXServer::LongList_out triVerts,
                FoamXServer::LongList_out localToGlobal
            );

        // Utilities

            // Flood filling
            virtual void markFaces
            (   
                const FoamXServer::LongList& protectedEdges,
                const CORBA::Long faceI,
                FoamXServer::BoolList_out visited
            );


        //- Lifetime management.
        virtual void close();
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace FoamX

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
