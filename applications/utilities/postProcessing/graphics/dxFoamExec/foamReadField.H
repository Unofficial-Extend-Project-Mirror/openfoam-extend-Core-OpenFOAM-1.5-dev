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
    foamReadField

Description

SourceFiles
    foamReadField.C
    foamReadVolField.C
    foamReadVolScalarField.C
    foamReadSurfaceField.C
    foamReadSurfaceScalarField.C
    foamReadPointField.C
    foamReadPointScalarField.C
    foamReadTetPointField.C
    foamReadTetPointScalarField.C
    foamReadLagrangianField.C
    foamReadLagrangianScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef foamReadField_H
#define foamReadField_H

#include "dxFoamExec.H"
#include "dxFvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
inline int* shape()
{
    static int typeShape[Type::rank];

    for (direction d=0; d<Type::rank; d++)
    {
        typeShape[d] = vector::nComponents;
    }

    return typeShape;
}


template<class Type>
Error foamReadVolField
(
    const IOobject& fieldObject,
    const dxFvMesh& mesh,
    const DXArray dxPositions,
    const DXArray dxConnections,
    DXField& dxVolField,
    DXField& dxPointField
);

template<>
Error foamReadVolField<scalar>
(
    const IOobject& fieldObject,
    const dxFvMesh& mesh,
    const DXArray dxPositions,
    const DXArray dxConnections,
    DXField& dxVolField,
    DXField& dxPointField
);


template<class Type>
Error foamReadSurfaceField
(
    const IOobject& fieldObject,
    const dxFvMesh& mesh,
    DXField& dxPointField
);

template<>
Error foamReadSurfaceField<scalar>
(
    const IOobject& fieldObject,
    const dxFvMesh& mesh,
    DXField& dxPointField
);


template<class Type>
Error foamReadPointField
(
    const IOobject& fieldObject,
    const dxFvMesh& mesh,
    const DXArray dxPositions,
    const DXArray dxConnections,
    DXField& dxPointField
);

template<>
Error foamReadPointField<scalar>
(
    const IOobject& fieldObject,
    const dxFvMesh& mesh,
    const DXArray dxPositions,
    const DXArray dxConnections,
    DXField& dxPointField
);


template<class Type>
Error foamReadTetPointField
(
    const IOobject& fieldObject,
    const dxFvMesh& mesh,
    const DXArray dxPositions,
    const DXArray dxConnections,
    DXField& dxPointField
);

template<>
Error foamReadTetPointField<scalar>
(
    const IOobject& fieldObject,
    const dxFvMesh& mesh,
    const DXArray dxPositions,
    const DXArray dxConnections,
    DXField& dxPointField
);


template<class Type>
Error foamReadLagrangianField
(
    const IOobject& fieldObject,
    const DXArray dxLagrangianPositions,
    DXField& dxPointField
);

template<>
Error foamReadLagrangianField<scalar>
(
    const IOobject& fieldObject,
    const DXArray dxLagrangianPositions,
    DXField& dxPointField
);


//- Set the DX positions and connections on the DX field
DXField createDxField
(
    const word& fieldName,
    const DXArray dxArray,
    const DXArray dxPositions,
    const DXArray dxConnections,
    const char* dependency
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "foamReadVolField.C"
#   include "foamReadSurfaceField.C"
#   include "foamReadPointField.C"
#   include "foamReadTetPointField.C"
#   include "foamReadLagrangianField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
