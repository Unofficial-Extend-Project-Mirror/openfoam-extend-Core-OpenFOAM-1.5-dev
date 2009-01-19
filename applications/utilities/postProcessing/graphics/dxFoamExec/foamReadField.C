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

#include "foamReadField.H"
#include "IFstream.H"
#include "dxDatabase.H"
#include "tetFemMatrices.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

extern "C"
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Error m_foamReadField(Object *in, Object *out)
{
    Info<< "dxFoamExec:foamReadField : reading Field" << endl;

    char *fieldNameChars;

    if (!in[0])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamReadField : missing fieldName"
        );
        return DXERROR;
    }
    else if (!DXExtractString(in[0], &fieldNameChars))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamReadField : fieldName must be a string"
        );
        return DXERROR;
    }

    word fieldName = fieldNameChars;

    char *fieldTypeString = NULL;

    if (!in[1])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamReadField : missing fieldType"
        );
        return DXERROR;
    }
    else if (!DXExtractString(in[1], &fieldTypeString))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamReadField : fieldType must be a string"
        );
        return DXERROR;
    }

    word fieldType(fieldTypeString);

    Info<< "dxFoamExec:foamReadField : reading Field " << fieldName
        << " of type " << fieldType << endl;


    dxDatabase* runTimePtr = NULL;
    if (!in[2])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamReadField : missing foamMesh"
        );
        return DXERROR;
    }
    else if (!(runTimePtr = (dxDatabase*)DXGetPrivateData((Private)in[2])))
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamReadField : cannot get mesh from input"
        );
        return DXERROR;
    }
    dxFvMesh& mesh = runTimePtr->mesh();



    DXArray dxPositions;
    DXArray dxConnections;

    if (!in[3])
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamReadField : missing mesh"
        );
        return DXERROR;
    }
    else if
    (
        !(dxPositions = (DXArray)DXExtract(in[3], "positions"))
     || !(dxConnections = (DXArray)DXExtract(in[3], "connections"))
    )
    {
        DXSetError
        (
            ERROR_BAD_PARAMETER,
            "dxFoamExec:foamReadField : "
            "missing positions or connections in mesh"
        );
        return DXERROR;
    }

    DXField dxVolField = NULL;
    DXField dxPointField = NULL;

    if
    (
        fieldType == volScalarField::typeName
     || fieldType == volVectorField::typeName
     || fieldType == volTensorField::typeName
     || fieldType == surfaceScalarField::typeName
     || fieldType == surfaceVectorField::typeName
     || fieldType == surfaceTensorField::typeName
     || fieldType == pointScalarField::typeName
     || fieldType == pointVectorField::typeName
     || fieldType == pointTensorField::typeName
    )
    {
        IOobject fieldObject
        (
            fieldName,
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );

        if (!fieldObject.headerOk())
        {
            DXSetError
            (
                ERROR_MISSING_DATA,
                (char*)
                (
                    "dxFoamExec:foamReadField : Field " + fieldObject.name()
                  + " header is not OK"
                ).c_str()
            );
            return DXERROR;
        }

        if (fieldObject.headerClassName() != fieldType)
        {
            DXSetError
            (
                ERROR_MISSING_DATA,
                (char*)
                (
                    "dxFoamExec:foamReadField : Field " + fieldObject.name()
                  + " has unexpected field type "
                  + fieldObject.headerClassName()
                  + ", expected " + fieldType
                ).c_str()
            );
            return DXERROR;
        }


        if (fieldType == volScalarField::typeName)
        {
            if
            (
                !foamReadVolField<scalar>
                (
                    fieldObject,
                    mesh,
                    dxPositions,
                    dxConnections,
                    dxVolField,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
        else if (fieldType == volVectorField::typeName)
        {
            if
            (
                !foamReadVolField<vector>
                (
                    fieldObject,
                    mesh,
                    dxPositions,
                    dxConnections,
                    dxVolField,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
        else if (fieldType == volTensorField::typeName)
        {
            if
            (
                !foamReadVolField<tensor>
                (
                    fieldObject,
                    mesh,
                    dxPositions,
                    dxConnections,
                    dxVolField,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
        else if (fieldType == surfaceScalarField::typeName)
        {
            if
            (
                !foamReadSurfaceField<scalar>
                (
                    fieldObject,
                    mesh,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
        else if (fieldType == surfaceVectorField::typeName)
        {
            if
            (
                !foamReadSurfaceField<vector>
                (
                    fieldObject,
                    mesh,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
        else if (fieldType == surfaceTensorField::typeName)
        {
            if
            (
                !foamReadSurfaceField<tensor>
                (
                    fieldObject,
                    mesh,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
        else if (fieldType == pointScalarField::typeName)
        {
            if
            (
                !foamReadPointField<scalar>
                (
                    fieldObject,
                    mesh,
                    dxPositions,
                    dxConnections,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
        else if (fieldType == pointVectorField::typeName)
        {
            if
            (
                !foamReadPointField<vector>
                (
                    fieldObject,
                    mesh,
                    dxPositions,
                    dxConnections,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
        else if (fieldType == pointTensorField::typeName)
        {
            if
            (
                !foamReadPointField<tensor>
                (
                    fieldObject,
                    mesh,
                    dxPositions,
                    dxConnections,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
    }
    else if
    (
        fieldType == tetPointScalarField::typeName
     || fieldType == tetPointVectorField::typeName
     || fieldType == tetPointTensorField::typeName
    )
    {
        IOobject fieldObject
        (
            fieldName,
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );

        if (!fieldObject.headerOk())
        {
            DXSetError
            (
                ERROR_MISSING_DATA,
                (char*)
                (
                    "dxFoamExec:foamReadField : Field " + fieldObject.name()
                  + " header is not OK"
                ).c_str()
            );
            return DXERROR;
        }

        if (fieldObject.headerClassName() != fieldType)
        {
            DXSetError
            (
                ERROR_MISSING_DATA,
                (char*)
                (
                    "dxFoamExec:foamReadField : Field " + fieldObject.name()
                  + " has unexpected field type "
                  + fieldObject.headerClassName()
                  + ", expected " + fieldType
                ).c_str()
            );
            return DXERROR;
        }

        // Construct tetPolyMesh
        mesh.setTetMesh();

        if (fieldType == tetPointScalarField::typeName)
        {
            if
            (
                !foamReadTetPointField<scalar>
                (
                    fieldObject,
                    mesh,
                    dxPositions,
                    dxConnections,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
        else if (fieldType == tetPointVectorField::typeName)
        {
            if
            (
                !foamReadTetPointField<vector>
                (
                    fieldObject,
                    mesh,
                    dxPositions,
                    dxConnections,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
        else if (fieldType == tetPointTensorField::typeName)
        {
            if
            (
                !foamReadTetPointField<tensor>
                (
                    fieldObject,
                    mesh,
                    dxPositions,
                    dxConnections,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
    }
    else if
    (
        fieldType == "scalarField"
     || fieldType == "vectorField"
     || fieldType == "tensorField"
    )
    {
        DXArray dxLagrangianPositions = NULL;

        if (!in[4])
        {
            DXSetError
            (
                ERROR_BAD_PARAMETER,
                "dxFoamExec:foamReadField : missing lagrangianPositions"
            );
            return DXERROR;
        }
        else if
        (
            !(dxLagrangianPositions = (DXArray)DXExtract(in[4], "positions"))
        )
        {
            DXSetError
            (
                ERROR_BAD_PARAMETER,
                "dxFoamExec:foamReadField : "
                "missing positions in lagrangianPositions"
            );
            return DXERROR;
        }


        IOobject fieldObject
        (
            fieldName,
            mesh.time().timeName(),
            "lagrangian",
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );

        if (!fieldObject.headerOk())
        {
            DXSetError
            (
                ERROR_MISSING_DATA,
                (char*)
                (
                    "dxFoamExec:foamReadField : Field " + fieldObject.name()
                  + " header is not OK"
                ).c_str()
            );
            return DXERROR;
        }

        if (fieldObject.headerClassName() != fieldType)
        {
            DXSetError
            (
                ERROR_MISSING_DATA,
                (char*)
                (
                    "dxFoamExec:foamReadField : Field " + fieldObject.name()
                  + " has unexpected field type "
                  + fieldObject.headerClassName()
                  + ", expected " + fieldType
                ).c_str()
            );
            return DXERROR;
        }


        if (fieldType == "scalarField")
        {
            if
            (
                !foamReadLagrangianField<scalar>
                (
                    fieldObject,
                    dxLagrangianPositions,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
        else if (fieldType == "vectorField")
        {
            if
            (
                !foamReadLagrangianField<vector>
                (
                    fieldObject,
                    dxLagrangianPositions,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
        else if (fieldType == "tensorField")
        {
            if
            (
                !foamReadLagrangianField<tensor>
                (
                    fieldObject,
                    dxLagrangianPositions,
                    dxPointField
                )
            )
            {
                return DXERROR;
            }
        }
    }
    else
    {
        DXSetError
        (
            ERROR_NOT_IMPLEMENTED,
            (char*)
            (
                "dxFoamExec:foamReadField : Field type " + fieldType
              + " is not supported"
            ).c_str()
        );
        return DXERROR;
    }

    if (dxVolField)
    {
        if (!DXEndField(dxVolField))
        {
            DXSetError
            (
                ERROR_BAD_PARAMETER,
                "dxFoamExec:foamReadField : "
                "cannot add bounding box to dxVolField"
            );
            return DXERROR;
        }
    }

    if (dxPointField)
    {
        if (!DXEndField(dxPointField))
        {
            DXSetError
            (
                ERROR_BAD_PARAMETER,
                "dxFoamExec:foamReadField : "
                "cannot add bounding box to dxPointField"
            );
            return DXERROR;
        }
    }

    out[0] = (Object)dxVolField;
    out[1] = (Object)dxPointField;

    return OK;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // end extern "C"

// ************************************************************************* //
