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
    Namespace of functions to calculate explicit derivatives.
    Time derivatives are calculated using Euler-implicit, backward differencing
    or Crank-Nicholson. Spatial derivatives are calculated using Gauss' Theorem.

\*---------------------------------------------------------------------------*/

#include "fvcLaplacian.H"
#include "fvMesh.H"
#include "laplacianScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fvc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    return fv::laplacianScheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().laplacianScheme(name)
    )().fvcLaplacian(vf);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tvf(), name)
    );
    tvf.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fvc::laplacian(vf, "laplacian(" + vf.name() + ')');
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tvf())
    );
    tvf.clear();
    return Laplacian;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const dimensionedScalar& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    return gamma*fvc::laplacian(vf, name);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const dimensionedScalar& gamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(gamma, tvf(), name)
    );
    tvf.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const dimensionedScalar& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return gamma*fvc::laplacian
    (
        vf, "laplacian(" + gamma.name() + ',' + vf.name() + ')'
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const dimensionedScalar& gamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(gamma, tvf())
    );
    tvf.clear();
    return Laplacian;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const volScalarField& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    return fv::laplacianScheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().laplacianScheme(name)
    )().fvcLaplacian(gamma, vf);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<volScalarField>& tgamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tgamma(), vf, name)
    );
    tgamma.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const volScalarField& gamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(gamma, tvf(), name)
    );
    tvf.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<volScalarField>& tgamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tgamma(), tvf(), name)
    );
    tgamma.clear();
    tvf.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const volScalarField& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fvc::laplacian
    (
        gamma,
        vf,
        "laplacian(" + gamma.name() + ',' + vf.name() + ')'
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<volScalarField>& tgamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fvc::laplacian
    (
        tgamma,
        vf,
        "laplacian(" + tgamma().name() + ',' + vf.name() + ')'
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const volScalarField& gamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    return fvc::laplacian
    (
        gamma,
        tvf,
        "laplacian(" + gamma.name() + ',' + tvf().name() + ')'
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<volScalarField>& tgamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    return fvc::laplacian
    (
        tgamma,
        tvf,
        "laplacian(" + tgamma().name() + ',' + tvf().name() + ')'
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const surfaceScalarField& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    return fv::laplacianScheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().laplacianScheme(name)
    )().fvcLaplacian(gamma, vf);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<surfaceScalarField>& tgamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tgamma(), vf, name)
    );
    tgamma.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const surfaceScalarField& gamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(gamma, tvf(), name)
    );
    tvf.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
(
    const tmp<surfaceScalarField>& tgamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tgamma(), tvf(), name)
    );
    tgamma.clear();
    tvf.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const surfaceScalarField& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fvc::laplacian
    (
        gamma,
        vf,
        "laplacian(" + gamma.name() + ',' + vf.name() + ')'
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<surfaceScalarField>& tgamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tgamma(), vf)
    );
    tgamma.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const surfaceScalarField& gamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(gamma, tvf())
    );
    tvf.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
(
    const tmp<surfaceScalarField>& tgamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tgamma(), tvf())
    );
    tgamma.clear();
    tvf.clear();
    return Laplacian;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const volTensorField& Gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    const fvMesh& mesh = vf.mesh();

    return fvc::laplacian
    (
        (mesh.Sf() & fvc::interpolate(Gamma) & mesh.Sf())/sqr(mesh.magSf()),
        vf,
        name
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const volTensorField& Gamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(Gamma, tvf(), name)
    );
    tvf.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<volTensorField>& tGamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tGamma(), vf, name)
    );
    tGamma.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<volTensorField>& tGamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tGamma(), tvf(), name)
    );
    tGamma.clear();
    tvf.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const volTensorField& Gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return laplacian
    (
        Gamma, vf, "laplacian(" + Gamma.name() + ',' + vf.name() + ')'
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const volTensorField& Gamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(Gamma, tvf())
    );
    tvf.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<volTensorField>& tGamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tGamma(), vf)
    );
    tGamma.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<volTensorField>& tGamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tGamma(), tvf())
    );
    tGamma.clear();
    tvf.clear();
    return Laplacian;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
(
    const surfaceTensorField& Gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    const fvMesh& mesh = vf.mesh();

    return fvc::laplacian
    (
        (mesh.Sf() & Gamma & mesh.Sf())/sqr(mesh.magSf()),
        vf,
        name
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const surfaceTensorField& Gamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(Gamma, tvf(), name)
    );
    tvf.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<surfaceTensorField>& tGamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tGamma(), vf, name)
    );
    tGamma.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<surfaceTensorField>& tGamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tGamma(), tvf(), name)
    );
    tGamma.clear();
    tvf.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
(
    const surfaceTensorField& Gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = vf.mesh();

    return fvc::laplacian
    (
        Gamma,
        vf,
        "laplacian(" + Gamma.name() + ',' + vf.name() + ')'
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const surfaceTensorField& Gamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(Gamma, tvf())
    );
    tvf.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<surfaceTensorField>& tGamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tGamma(), vf)
    );
    tGamma.clear();
    return Laplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacian
(
    const tmp<surfaceTensorField>& tGamma,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Laplacian
    (
        fvc::laplacian(tGamma(), tvf())
    );
    tGamma.clear();
    tvf.clear();
    return Laplacian;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
