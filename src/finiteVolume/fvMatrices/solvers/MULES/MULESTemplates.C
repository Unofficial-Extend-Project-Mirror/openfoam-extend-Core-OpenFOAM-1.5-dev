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

#include "MULES.H"
#include "upwind.H"
#include "uncorrectedSnGrad.H"
#include "gaussConvectionScheme.H"
#include "gaussLaplacianScheme.H"
#include "uncorrectedSnGrad.H"
#include "surfaceInterpolate.H"
#include "fvcSurfaceIntegrate.H"
#include "slicedSurfaceFields.H"
#include "syncTools.H"

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class SpType, class SuType>
void Foam::MULES::explicitSolve
(
    volScalarField& psi,
    const surfaceScalarField& phi,
    surfaceScalarField& phiPsi,
    const SpType& Sp,
    const SuType& Su,
    const scalar psiMax,
    const scalar psiMin
)
{
    Info<< "MULES: Solving for " << psi.name() << endl;

    const fvMesh& mesh = psi.mesh();

    surfaceScalarField phiBD = upwind<scalar>(psi.mesh(), phi).flux(psi);

    surfaceScalarField& phiCorr = phiPsi;
    phiCorr -= phiBD;

    scalarField allLambda(mesh.nFaces(), 1.0);

    slicedSurfaceScalarField lambda
    (
        IOobject
        (
            "lambda",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        mesh,
        dimless,
        allLambda
    );

    limiter
    (
        allLambda,
        psi,
        phiBD,
        phiCorr,
        Sp.field(),
        Su.field(),
        psiMax,
        psiMin
    );

    phiPsi = phiBD + lambda*phiCorr;

    scalarField& psiIf = psi;
    const scalarField& psi0 = psi.oldTime();
    const scalar deltaT = mesh.time().deltaT().value();

    psiIf = 0.0;
    fvc::surfaceIntegrate(psiIf, phiPsi);
    psiIf = (psi0/deltaT + Su.field() - psiIf)/(1.0/deltaT - Sp.field());
    psi.correctBoundaryConditions();
}


template<class SpType, class SuType>
void Foam::MULES::explicitSolve01
(
    volScalarField& psi,
    const surfaceScalarField& phi,
    surfaceScalarField& phiPsi,
    const SpType& Sp,
    const SuType& Su
)
{
    const scalar psiMax = 1.0;
    const scalar psiMin = 0.0;

    // This is sometimes beneficial but in other cases this will limit the
    // removal of unboundedness to the level in the old-time field
    //psi.oldTime();
    //psi.max(psiMin);
    //psi.min(psiMax);

    explicitSolve(psi, phi, phiPsi, Sp, Su, psiMax, psiMin);
}


template<class SpType, class SuType>
void Foam::MULES::implicitSolve01
(
    volScalarField& psi,
    const surfaceScalarField& phi,
    const dimensionedScalar& Dpsi,
    surfaceScalarField& phiPsi,
    const SpType& Sp,
    const SuType& Su
)
{
    const scalar psiMax = 1.0;
    const scalar psiMin = 0.0;

    const fvMesh& mesh = psi.mesh();

    scalarField allLambda(mesh.nFaces(), 1.0);

    slicedSurfaceScalarField lambda
    (
        IOobject
        (
            "lambda",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        mesh,
        dimless,
        allLambda
    );

    linear<scalar> CDs(mesh);
    upwind<scalar> UDs(mesh, phi);
    fv::uncorrectedSnGrad<scalar> snGrads(mesh);

    surfaceScalarField Dpsif
    (
        IOobject
        (
            Dpsi.name(),
            psi.instance(),
            mesh
        ),
        mesh,
        Dpsi
    );

    //HJ, bug? Wrong sign on the Sp term  18/Jul/2007
    fvScalarMatrix psiConvectionDiffusion
    (
        fvm::ddt(psi)
      + fv::gaussConvectionScheme<scalar>(mesh, phi, UDs).fvmDiv(phi, psi)
      - fv::gaussLaplacianScheme<scalar>(mesh, CDs, snGrads)
       .fvmLaplacian(Dpsif, psi)
      - fvm::Sp(Sp, psi)
      - Su
    );

    // This is sometimes beneficial but in other cases this will limit the
    // removal of unboundedness to the level in the old-time field
    //psi.oldTime();
    //psi.max(psiMin);
    //psi.min(psiMax);

    surfaceScalarField phiBD = psiConvectionDiffusion.flux();

    surfaceScalarField& phiCorr = phiPsi;
    phiCorr -= phiBD;

    for (int i = 0; i < 4; i++)
    {
        if (i == 1)
        {
            allLambda = 1.0;
        }

        if (i > 0)
        {
            psi.max(psiMin);
            psi.min(psiMax);

            phiBD = psiConvectionDiffusion.flux();
        }

        limiter
        (
            allLambda,
            psi,
            phiBD,
            phiCorr,
            Sp.field(),
            Su.field(),
            psiMax,
            psiMin
        );

        solve(psiConvectionDiffusion + fvc::div(lambda*phiCorr));

        Info<< "max(psi) - 1 = " << max(psi.internalField()) - 1.0 << endl;
        Info<< "min(psi) = " << min(psi.internalField()) << endl;
    }

    phiPsi = psiConvectionDiffusion.flux() + lambda*phiCorr;
}


template<class SpType, class SuType>
void Foam::MULES::limiter
(
    scalarField& allLambda,
    const volScalarField& psi,
    const surfaceScalarField& phiBD,
    const surfaceScalarField& phiCorr,
    const SpType& Sp,
    const SuType& Su,
    const scalar psiMax,
    const scalar psiMin
)
{
    const scalarField& psiIf = psi;
    const volScalarField::GeometricBoundaryField& psiBf = psi.boundaryField();

    const scalarField& psi0 = psi.oldTime();

    const fvMesh& mesh = psi.mesh();

    const unallocLabelList& owner = mesh.owner();
    const unallocLabelList& neighb = mesh.neighbour();
    const scalarField& V = mesh.V();
    const scalar deltaT = mesh.time().deltaT().value();

    const scalarField& phiBDIf = phiBD;
    const surfaceScalarField::GeometricBoundaryField& phiBDBf = 
        phiBD.boundaryField();

    const scalarField& phiCorrIf = phiCorr;
    const surfaceScalarField::GeometricBoundaryField& phiCorrBf = 
        phiCorr.boundaryField();

    slicedSurfaceScalarField lambda
    (
        IOobject
        (
            "lambda",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        mesh,
        dimless,
        allLambda
    );

    scalarField& lambdaIf = lambda;
    surfaceScalarField::GeometricBoundaryField& lambdaBf = 
        lambda.boundaryField();

    scalarField psiMaxn(psiIf.size(), psiMin);
    scalarField psiMinn(psiIf.size(), psiMax);

    scalarField sumPhiBD(psiIf.size(), 0.0);

    scalarField sumPhip(psiIf.size(), VSMALL);
    scalarField mSumPhim(psiIf.size(), VSMALL);

    forAll(phiCorrIf, facei)
    {
        label own = owner[facei];
        label nei = neighb[facei];

        psiMaxn[own] = max(psiMaxn[own], psiIf[nei]);
        psiMinn[own] = min(psiMinn[own], psiIf[nei]);

        psiMaxn[nei] = max(psiMaxn[nei], psiIf[own]);
        psiMinn[nei] = min(psiMinn[nei], psiIf[own]);

        sumPhiBD[own] += phiBDIf[facei];
        sumPhiBD[nei] -= phiBDIf[facei];

        scalar phiCorrf = phiCorrIf[facei];

        if (phiCorrf > 0.0)
        {
            sumPhip[own] += phiCorrf;
            mSumPhim[nei] += phiCorrf;
        }
        else
        {
            mSumPhim[own] -= phiCorrf;
            sumPhip[nei] -= phiCorrf;
        }
    }

    forAll(phiCorrBf, patchi)
    {
        const fvPatchScalarField& psiPf = psiBf[patchi];
        const scalarField& phiBDPf = phiBDBf[patchi];
        const scalarField& phiCorrPf = phiCorrBf[patchi];

        const labelList& pFaceCells = mesh.boundary()[patchi].faceCells();

        if (psiPf.coupled())
        {
            scalarField psiPNf = psiPf.patchNeighbourField();

            forAll(phiCorrPf, pFacei)
            {
                label pfCelli = pFaceCells[pFacei];
                
                psiMaxn[pfCelli] = max(psiMaxn[pfCelli], psiPNf[pFacei]);
                psiMinn[pfCelli] = min(psiMinn[pfCelli], psiPNf[pFacei]);
            }
        }
        else
        {
            forAll(phiCorrPf, pFacei)
            {
                label pfCelli = pFaceCells[pFacei];
                
                psiMaxn[pfCelli] = max(psiMaxn[pfCelli], psiPf[pFacei]);
                psiMinn[pfCelli] = min(psiMinn[pfCelli], psiPf[pFacei]);
            }
        }

        forAll(phiCorrPf, pFacei)
        {
            label pfCelli = pFaceCells[pFacei];

            sumPhiBD[pfCelli] += phiBDPf[pFacei];

            scalar phiCorrf = phiCorrPf[pFacei];

            if (phiCorrf > 0.0)
            {
                sumPhip[pfCelli] += phiCorrf;
            }
            else
            {
                mSumPhim[pfCelli] -= phiCorrf;
            }
        }
    }

    psiMaxn = min(psiMaxn, psiMax);
    psiMinn = max(psiMinn, psiMin);

    //scalar smooth = 0.5;
    //psiMaxn = min((1.0 - smooth)*psiIf + smooth*psiMaxn, psiMax);
    //psiMinn = max((1.0 - smooth)*psiIf + smooth*psiMinn, psiMin);

    psiMaxn = 
        V*((1.0/deltaT - Sp)*psiMaxn - (1.0/deltaT)*psi0 - Su)
      + sumPhiBD;

    psiMinn = 
        V*((1.0/deltaT)*psi0 - (1.0/deltaT - Sp)*psiMinn + Su)
      - sumPhiBD;

    scalarField sumlPhip(psiIf.size());
    scalarField mSumlPhim(psiIf.size());

    for(int j=0; j<3; j++)
    {
        sumlPhip = 0.0;
        mSumlPhim = 0.0;

        forAll(lambdaIf, facei)
        {
            label own = owner[facei];
            label nei = neighb[facei];

            scalar lambdaPhiCorrf = lambdaIf[facei]*phiCorrIf[facei];

            if (lambdaPhiCorrf > 0.0)
            {
                sumlPhip[own] += lambdaPhiCorrf;
                mSumlPhim[nei] += lambdaPhiCorrf;
            }
            else
            {
                mSumlPhim[own] -= lambdaPhiCorrf;
                sumlPhip[nei] -= lambdaPhiCorrf;
            }
        }

        forAll(lambdaBf, patchi)
        {
            scalarField& lambdaPf = lambdaBf[patchi];
            const scalarField& phiCorrfPf = phiCorrBf[patchi];

            const labelList& pFaceCells = mesh.boundary()[patchi].faceCells();

            forAll(lambdaPf, pFacei)
            {
                label pfCelli = pFaceCells[pFacei];
                
                scalar lambdaPhiCorrf = lambdaPf[pFacei]*phiCorrfPf[pFacei];

                if (lambdaPhiCorrf > 0.0)
                {
                    sumlPhip[pfCelli] += lambdaPhiCorrf;
                }
                else
                {
                    mSumlPhim[pfCelli] -= lambdaPhiCorrf;
                }
            }
        }

        forAll (sumlPhip, celli)
        {
            sumlPhip[celli] =
                max(min
                (
                    (sumlPhip[celli] + psiMaxn[celli])/mSumPhim[celli],
                    1.0), 0.0
                );

            mSumlPhim[celli] =
                max(min
                (
                    (mSumlPhim[celli] + psiMinn[celli])/sumPhip[celli],
                    1.0), 0.0
                );
        }

        const scalarField& lambdam = sumlPhip;
        const scalarField& lambdap = mSumlPhim;

        forAll(lambdaIf, facei)
        {
            if (phiCorrIf[facei] > 0.0)
            {
                lambdaIf[facei] = min
                (
                    lambdaIf[facei],
                    min(lambdap[owner[facei]], lambdam[neighb[facei]])
                );
            }
            else
            {
                lambdaIf[facei] = min
                (
                    lambdaIf[facei],
                    min(lambdam[owner[facei]], lambdap[neighb[facei]])
                );
            }
        }


        forAll(lambdaBf, patchi)
        {
            fvsPatchScalarField& lambdaPf = lambdaBf[patchi];
            const scalarField& phiCorrfPf = phiCorrBf[patchi];

            const labelList& pFaceCells = mesh.boundary()[patchi].faceCells();

            forAll(lambdaPf, pFacei)
            {
                label pfCelli = pFaceCells[pFacei];

                if (phiCorrfPf[pFacei] > 0.0)
                {
                    lambdaPf[pFacei] = min(lambdaPf[pFacei], lambdap[pfCelli]);
                }
                else
                {
                    lambdaPf[pFacei] = min(lambdaPf[pFacei], lambdam[pfCelli]);
                }
            }
        }

        syncTools::syncFaceList(mesh, allLambda, minEqOp<scalar>(), false);
    }
}


// ************************************************************************* //
