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
    pointMapper

Description
    This object provides mapping and fill-in information for point data
    between the two meshes after the topological change.  It is
    constructed from mapPolyMesh.

SourceFiles
    pointMapper.C

\*---------------------------------------------------------------------------*/

#ifndef pointMapper_H
#define pointMapper_H

#include "morphFieldMapper.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Class forward declarations
class polyMesh;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                           Class pointMapper Declaration
\*---------------------------------------------------------------------------*/

class pointMapper
:
    public morphFieldMapper
{
    // Private data

        //- Reference to polyMesh
        const polyMesh& mesh_;

        //- Reference to mapPolyMesh
        const mapPolyMesh& mpm_;

        //- Are there any inserted (unmapped) points
        bool insertedPoints_;


    // Demand-driven private data

        //- Direct addressing (only one for of addressing is used)
        mutable labelList* directAddrPtr_;

        //- Inserted points
        mutable labelList* insertedPointLabelsPtr_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        pointMapper(const pointMapper&);

        //- Disallow default bitwise assignment
        void operator=(const pointMapper&);


        //- Calculate addressing for mapping with inserted points
        void calcAddressing() const;

        //- Clear out local storage
        void clearOut();


public:

    // Static data members

    // Constructors

        //- Construct from mapPolyMesh
        pointMapper(const mapPolyMesh& mpm);


    // Destructor

        virtual ~pointMapper();


    // Member Functions

        //- Return size
        virtual label size() const;

        //- Return size of field before mapping
        virtual label sizeBeforeMapping() const;

        //- Is the mapping direct  (points can only be added from points)
        virtual bool direct() const
        {
            return true;
        }

        //- Return direct addressing.
        //  For inserted points, the master point is set to zero to
        //  avoid checking during the mapping.  The values should be
        //  over-ridden later using insertedPointLabels.
        //  
        virtual const unallocLabelList& directAddressing() const;

        //- Are there any inserted objects
        virtual bool insertedObjects() const
        {
            return insertedPoints_;
        }

        //- Return list of inserted objects
        virtual const labelList& insertedObjectLabels() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
