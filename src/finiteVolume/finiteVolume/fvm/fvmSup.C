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

#include "volFields.H"
#include "surfaceFields.H"
#include "fvMatrix.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fvm
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<fvMatrix<Type> >
Su
(
    const GeometricField<Type, fvPatchField, volMesh>& su,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = vf.mesh();

    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            dimVol*su.dimensions()
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    fvm.source() -= mesh.V()*su.internalField();

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
Su
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm = fvm::Su(tsu(), vf);
    tsu.clear();
    return tfvm;
}


template<class Type>
Zero
Su
(
    const Zero&,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return zero;
}


template<class Type>
tmp<fvMatrix<Type> >
Sp
(
    const volScalarField& sp,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = vf.mesh();

    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            dimVol*sp.dimensions()*vf.dimensions()
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    fvm.diag() += mesh.V()*sp.internalField();

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
Sp
(
    const tmp<volScalarField>& tsp,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm = fvm::Sp(tsp(), vf);
    tsp.clear();
    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
Sp
(
    const dimensionedScalar& sp,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = vf.mesh();

    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            dimVol*sp.dimensions()*vf.dimensions()
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    fvm.diag() += mesh.V()*sp.value();

    return tfvm;
}


template<class Type>
Zero
Sp
(
    const Zero&,
    GeometricField<Type, fvPatchField, volMesh>&
)
{
    return zero;
}


template<class Type>
tmp<fvMatrix<Type> >
SuSp
(
    const volScalarField& sp,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = vf.mesh();

    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            dimVol*sp.dimensions()*vf.dimensions()
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    fvm.diag() += mesh.V()*max(sp.internalField(), scalar(0));

    fvm.source() -= mesh.V()*min(sp.internalField(), scalar(0))
        *vf.internalField();

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
SuSp
(
    const tmp<volScalarField>& tsp,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm = fvm::SuSp(tsp(), vf);
    tsp.clear();
    return tfvm;
}


template<class Type>
Zero
SuSp
(
    const Zero&,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return zero;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvm

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
