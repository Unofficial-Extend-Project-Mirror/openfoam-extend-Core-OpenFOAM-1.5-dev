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
    Simple central-difference lnGrad scheme with non-orthogonal correction.

\*---------------------------------------------------------------------------*/

#include "correctedLnGrad.H"
#include "areaFields.H"
#include "edgeFields.H"
#include "linearEdgeInterpolation.H"
#include "gaussFaGrad.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fa
{

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
correctedLnGrad<Type>::~correctedLnGrad()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, faePatchField, edgeMesh> >
correctedLnGrad<Type>::correction
(
    const GeometricField<Type, faPatchField, areaMesh>& vf
) const
{
    // construct GeometricField<Type, faePatchField, edgeMesh>
    tmp<GeometricField<Type, faePatchField, edgeMesh> > tssf
    (
        new GeometricField<Type, faePatchField, edgeMesh>
        (
            IOobject
            (
                "lnGradCorr("+vf.name()+')',
                vf.instance(),
                vf.db()
            ),
            this->mesh(),
            vf.dimensions()*this->mesh().deltaCoeffs().dimensions()
        )
    );
    GeometricField<Type, faePatchField, edgeMesh>& ssf = tssf();

    for (direction cmpt = 0; cmpt < pTraits<Type>::nComponents; cmpt++)
    {
        ssf.replace
        (
            cmpt,
            this->mesh().correctionVectors()
          & linearEdgeInterpolation
            <
                typename 
                outerProduct<vector, typename pTraits<Type>::cmptType>::type
            >(this->mesh()).interpolate
            (
                gaussGrad<typename pTraits<Type>::cmptType>(this->mesh())
               .grad(vf.component(cmpt))
            )
        );
    }

// #   ifdef NEW_NON_ORTH
//     // Set reference to difference factors array
//     const scalarField& DeltaCoeffs = mesh().deltaCoeffs().internalField();

//     // Owner/neighbour addressing
//     const areaVectorField& cellCentres = mesh().centres();
//     const unallocLabelList& owner = mesh().owner();
//     const unallocLabelList& neighbour = mesh().neighbour();
//     const edgeVectorField& areas = mesh().Sf();
//     const edgeScalarField& magAreas = mesh().magSf();
//     const edgeVectorField& corrVecs = mesh().correctionVectors();

//     forAll(owner, faceI)
//     {
//         vector delta =
//             cellCentres[neighbour[faceI]] - cellCentres[owner[faceI]];

//         vector unitArea = areas[faceI]/magAreas[faceI];

//         scalar orthCorrCoeff =
//             (delta & (unitArea - corrVecs[faceI]))/mag(delta);

//         Type orthCorr =
//             (orthCorrCoeff/mag(delta) - DeltaCoeffs[faceI])
//            *(vf[neighbour[faceI]] - vf[owner[faceI]]);

//         ssf[faceI] += orthCorr;
//     }
// #   endif

    return tssf;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fa

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
