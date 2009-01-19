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
    
\*---------------------------------------------------------------------------*/

#include "gaussFaGrad.H"
#include "facGrad.H"
#include "areaFields.H"
#include "edgeFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fa
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp
<
    GeometricField
    <
        typename outerProduct<vector, Type>::type, faPatchField, areaMesh
    >
>
gaussGrad<Type>::grad
(
    const GeometricField<Type, faPatchField, areaMesh>& vsf
)
{
    typedef typename outerProduct<vector, Type>::type GradType;

    tmp<GeometricField<GradType, faPatchField, areaMesh> > tGrad
    (
        fac::edgeIntegrate
        (
            vsf.mesh().Le()
           *tinterpScheme_().interpolate(vsf)
        )
    );


    // Remove component of gradient normal to surface (area)
    const areaVectorField& n = vsf.mesh().faceAreaNormals();

    tGrad() -= n*(n&tGrad());


    tGrad().rename("grad(" + vsf.name() + ')');

    forAll(vsf.boundaryField(), patchI)
    {
        if (!vsf.boundaryField()[patchI].coupled())
        {
            vectorField m =
                this->mesh().Le().boundaryField()[patchI]
                /this->mesh().magLe().boundaryField()[patchI];

            tGrad().boundaryField()[patchI] = 
                m*vsf.boundaryField()[patchI].snGrad();

//             tGrad().boundaryField()[patchI] += m *
//             (
//                 vsf.boundaryField()[patchI].snGrad()
//               - (n & tGrad().boundaryField()[patchI])
//             );
        }
    }

    return tGrad;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fa

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
