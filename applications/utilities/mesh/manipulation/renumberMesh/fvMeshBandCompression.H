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
    fvMeshBandCompression

Description
    the class holds the information necessary to renumber the mesh in such a way
    that the band of the matrix is reduced. The algorithm uses a simple search
    through the neighbour list

SourceFiles
    fvMeshBandCompression.C
    fvMeshBandCompressionRenumberField.C

\*---------------------------------------------------------------------------*/

#ifndef fvMeshBandCompression_H
#define fvMeshBandCompression_H

#include "fvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class fvMeshBandCompression Declaration
\*---------------------------------------------------------------------------*/

class fvMeshBandCompression
:
    public fvMesh
{
    // Private data

        //- New cell order for the compressed band
        labelList cellOrder_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        fvMeshBandCompression(const fvMeshBandCompression&);

        //- Disallow default bitwise assignment
        void operator=(const fvMeshBandCompression&);


public:


    // Constructors

        //- Construct from IOobject
        fvMeshBandCompression(const IOobject& io);


    // Member Functions

        //- Return new cell order.
        // This gives the order in which the cells in the original mesh
        // are to be addressed to create a mesh with a compressed band
        const labelList& cellOrder() const
        {
            return cellOrder_;
        }

        //- Return pointer to renumbered mesh created on freestore
        fvMesh* renumberedMesh() const;

        //- Renumber intenal field
        template<class Type>
        tmp<Field<Type> > renumberInternalField
        (
            const Field<Type>&
        ) const;

        //- Renumber a volume field
        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh> > renumber
        (
            const fvMesh& targetMesh,
            const GeometricField<Type, fvPatchField, volMesh>& f
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "fvMeshBandCompressionRenumberField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
