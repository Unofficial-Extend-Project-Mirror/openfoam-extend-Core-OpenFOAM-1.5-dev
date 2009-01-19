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

#include "gaussLaplacianScheme.H"
#include "surfaceInterpolate.H"
#include "fvcDiv.H"
#include "fvcGrad.H"
#include "fvMatrices.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<fvMatrix<Type> >
gaussLaplacianScheme<Type>::fvmLaplacianUncorrected
(
    const surfaceScalarField& gammaMagSf,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<surfaceScalarField> tdeltaCoeffs =
        this->tsnGradScheme_().deltaCoeffs(vf);
    const surfaceScalarField& deltaCoeffs = tdeltaCoeffs();

    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            deltaCoeffs.dimensions()*gammaMagSf.dimensions()*vf.dimensions()
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    fvm.upper() = deltaCoeffs.internalField()*gammaMagSf.internalField();
    fvm.negSumDiag();

    forAll(fvm.psi().boundaryField(), patchI)
    {
        const fvPatchField<Type>& psf = fvm.psi().boundaryField()[patchI];
        const fvsPatchScalarField& patchGamma =
            gammaMagSf.boundaryField()[patchI];

        fvm.internalCoeffs()[patchI] = patchGamma*psf.gradientInternalCoeffs();
        fvm.boundaryCoeffs()[patchI] = -patchGamma*psf.gradientBoundaryCoeffs();
    }

    return tfvm;
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
gaussLaplacianScheme<Type>::gammaSnGradCorr
(
    const surfaceVectorField& SfGammaCorr,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = this->mesh();

    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tgammaSnGradCorr
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                "gammaSnGradCorr("+vf.name()+')',
                vf.instance(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            SfGammaCorr.dimensions()
           *vf.dimensions()*mesh.deltaCoeffs().dimensions()
        )
    );

    for (direction cmpt = 0; cmpt < pTraits<Type>::nComponents; cmpt++)
    {
        tgammaSnGradCorr().replace
        (
            cmpt,
            SfGammaCorr & fvc::interpolate(fvc::grad(vf.component(cmpt)))
        );
    }

    return tgammaSnGradCorr;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<fvMatrix<Type> >
gaussLaplacianScheme<Type>::fvmLaplacian
(
    const surfaceScalarField& gamma,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = this->mesh();

    surfaceScalarField gammaMagSf = gamma*mesh.magSf();

    tmp<fvMatrix<Type> > tfvm = fvmLaplacianUncorrected(gammaMagSf, vf);
    fvMatrix<Type>& fvm = tfvm();

    if (this->tsnGradScheme_().corrected())
    {
        if (mesh.fluxRequired(vf.name()))
        {
            fvm.faceFluxCorrectionPtr() = new
            GeometricField<Type, fvsPatchField, surfaceMesh>
            (
                gammaMagSf*this->tsnGradScheme_().correction(vf)
            );

            fvm.source() -=
                mesh.V()*
                fvc::div
                (
                    *fvm.faceFluxCorrectionPtr()
                )().internalField();
        }
        else
        {
            fvm.source() -=
                mesh.V()*
                fvc::div
                (
                    gammaMagSf*this->tsnGradScheme_().correction(vf)
                )().internalField();
        }
    }

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
gaussLaplacianScheme<Type>::fvmLaplacian
(
    const surfaceTensorField& gamma,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = this->mesh();

    surfaceVectorField Sn = mesh.Sf()/mesh.magSf();

    surfaceVectorField SfGamma = mesh.Sf() & gamma;
    surfaceScalarField SfGammaSn = SfGamma & Sn;
    surfaceVectorField SfGammaCorr = SfGamma - SfGammaSn*Sn;

    tmp<fvMatrix<Type> > tfvm = fvmLaplacianUncorrected(SfGammaSn, vf);
    fvMatrix<Type>& fvm = tfvm();

    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tfaceFluxCorrection
        = gammaSnGradCorr(SfGammaCorr, vf);

    if (this->tsnGradScheme_().corrected())
    {
        tfaceFluxCorrection() +=
            SfGammaSn*this->tsnGradScheme_().correction(vf);

    }

    fvm.source() -= mesh.V()*fvc::div(tfaceFluxCorrection())().internalField();

    if (mesh.fluxRequired(vf.name()))
    {
        fvm.faceFluxCorrectionPtr() = tfaceFluxCorrection.ptr();
    }

    return tfvm;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
gaussLaplacianScheme<Type>::fvcLaplacian
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = this->mesh();

    tmp<GeometricField<Type, fvPatchField, volMesh> > tLaplacian
    (
        fvc::div(this->tsnGradScheme_().snGrad(vf)*mesh.magSf())
    );

    tLaplacian().rename("laplacian(" + vf.name() + ')');

    return tLaplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
gaussLaplacianScheme<Type>::fvcLaplacian
(
    const surfaceScalarField& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = this->mesh();

    tmp<GeometricField<Type, fvPatchField, volMesh> > tLaplacian
    (
        fvc::div(gamma*this->tsnGradScheme_().snGrad(vf)*mesh.magSf())
    );

    tLaplacian().rename("laplacian(" + gamma.name() + ',' + vf.name() + ')');

    return tLaplacian;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
gaussLaplacianScheme<Type>::fvcLaplacian
(
    const surfaceTensorField& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = this->mesh();

    surfaceVectorField Sn = mesh.Sf()/mesh.magSf();

    surfaceVectorField SfGamma = mesh.Sf() & gamma;
    surfaceScalarField SfGammaSn = SfGamma & Sn;
    surfaceVectorField SfGammaCorr = SfGamma - SfGammaSn*Sn;

    tmp<GeometricField<Type, fvPatchField, volMesh> > tLaplacian
    (
        fvc::div
        (
            SfGammaSn*this->tsnGradScheme_().snGrad(vf)
          + gammaSnGradCorr(SfGammaCorr, vf)
        )
    );

    tLaplacian().rename("laplacian(" + gamma.name() + ',' + vf.name() + ')');

    return tLaplacian;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
