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

\*---------------------------------------------------------------------------*/

#include "IOField.H"
#include "Time.H"

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class Type>
Foam::tmp<Foam::IOField<Type> > Foam::reconstructLagrangianField
(
    const polyMesh& mesh,
    PtrList<polyMesh>& meshes,
    const IOobject& fieldIoObject
)
{
    tmp<IOField<Type> > tfield
    (
        new IOField<Type>
        (
            IOobject
            (
                fieldIoObject.name(),
                mesh.time().timeName(),
                "lagrangian",
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            Field<Type>(0)
        )
    );
    Field<Type>& field = tfield();

    forAll(meshes, i)
    {
        IOobject fieldIOobject
        ( 
            fieldIoObject.name(),
            meshes[i].time().timeName(),
            "lagrangian",
            meshes[i],
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );

        if (fieldIOobject.headerOk())
        {
            IOField<Type> fieldi(fieldIOobject);

            label offset = field.size();
            field.setSize(offset + fieldi.size());
            
            forAll(fieldi, j)
            {
                field[offset + j] = fieldi[j];
            }
        }
    }

    return tfield;
}


template<class Type>
void Foam::reconstructLagrangianFields
(
    const polyMesh& mesh,
    PtrList<polyMesh>& meshes,
    const IOobjectList& objects
)
{
    word fieldClassName(IOField<Type>::typeName);

    IOobjectList fields = objects.lookupClass(fieldClassName);

    if (fields.size())
    {
        Info<< "    Reconstructing lagrangian "
            << fieldClassName << "s\n" << endl;

        for
        (
            IOobjectList::iterator fieldIter = fields.begin();
            fieldIter != fields.end();
            ++fieldIter
        )
        {
            Info<< "        " << fieldIter()->name() << endl;
            reconstructLagrangianField<Type>
            (
                mesh,
                meshes,
                *fieldIter()
            )().write();
        }

        Info<< endl;
    }
}


// ************************************************************************* //
