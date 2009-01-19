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
    MapGeometricFields

Description
    Generic Geometric field mapper.  For "real" mapping, add template
    specialisations for mapping of internal fields depending on mesh
    type.

\*---------------------------------------------------------------------------*/

#ifndef MapGeometricFields_H
#define MapGeometricFields_H

#include "polyMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

template<class Type, class MeshMapper, class GeoMesh>
class MapInternalField
{
public:
    
    MapInternalField()
    {}

    void operator()
    (
        Field<Type>& field,
        const MeshMapper& mapper
    ) const;
};


template
<
    class Type,
    template<class> class PatchField,
    class MeshMapper,
    class GeoMesh
>
void MapGeometricFields
(
    const MeshMapper& mapper
)
{
    HashTable<const GeometricField<Type, PatchField, GeoMesh>*> fields
    (
        mapper.mesh().objectRegistry::lookupClass
            <GeometricField<Type, PatchField, GeoMesh> >()
    );

    // It is necessary to enforce that all old-time fields are stored
    // before the mapping is performed.  Otherwise, if the
    // old-time-level field is mapped before the field itself, sizes
    // will not match.  

    for
    (
        typename HashTable<const GeometricField<Type, PatchField, GeoMesh>*>::
            iterator fieldIter = fields.begin();
        fieldIter != fields.end();
        ++fieldIter
    )
    {
        const_cast<GeometricField<Type, PatchField, GeoMesh>*>(fieldIter())
            ->storeOldTimes();
    }

    for
    (
        typename HashTable<const GeometricField<Type, PatchField, GeoMesh>*>::
            iterator fieldIter = fields.begin();
        fieldIter != fields.end();
        ++fieldIter
    )
    {
        GeometricField<Type, PatchField, GeoMesh>& field = 
            const_cast<GeometricField<Type, PatchField, GeoMesh>&>
            (*fieldIter());

        if (polyMesh::debug)
        {
            Info<< "Mapping field " << field.name() << endl;
        }

        // Map the internal field
        MapInternalField<Type, MeshMapper, GeoMesh>()
        (
            field.internalField(),
            mapper
        );

        // Map the patch fields
        forAll(field.boundaryField(), patchi)
        {
            // Cannot check sizes for patch fields because of
            // empty fields in FV and because point fields get their size
            // from the patch which has already been resized
            // 

            field.boundaryField()[patchi].autoMap
            (
                mapper.boundaryMap()[patchi]
            );
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
