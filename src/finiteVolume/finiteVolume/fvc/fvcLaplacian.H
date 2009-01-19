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

Namespace
    fvc

Description
    Calculate the laplacian of the given field.

SourceFiles
    fvcLaplacian.C

\*---------------------------------------------------------------------------*/


#ifndef fvcLaplacian_H
#define fvcLaplacian_H

#include "volFieldsFwd.H"
#include "surfaceFieldsFwd.H"
#include "dimensionedTypes.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Namespace fvc functions Declaration
\*---------------------------------------------------------------------------*/

namespace fvc
{
    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );


    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const dimensionedScalar&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const dimensionedScalar&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const dimensionedScalar&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const dimensionedScalar&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );


    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const volScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<volScalarField>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const volScalarField&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<volScalarField>&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const volScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<volScalarField>&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const volScalarField&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<volScalarField>&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );


    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const surfaceScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<surfaceScalarField>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const surfaceScalarField&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<surfaceScalarField>&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const surfaceScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<surfaceScalarField>&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const surfaceScalarField&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<surfaceScalarField>&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );


    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const volTensorField&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<volTensorField>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const volTensorField&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<volTensorField>&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const volTensorField&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<volTensorField>&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const volTensorField&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<volTensorField>&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );


    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const surfaceTensorField&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<surfaceTensorField>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const surfaceTensorField&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<surfaceTensorField>&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const surfaceTensorField&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<surfaceTensorField>&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const surfaceTensorField&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
    (
        const tmp<surfaceTensorField>&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "fvcLaplacian.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
