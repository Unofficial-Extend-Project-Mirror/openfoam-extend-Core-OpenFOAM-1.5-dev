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

#include "anisotropicFilter.H"
#include "addToRunTimeSelectionTable.H"
#include "zeroGradientFvPatchFields.H"
#include "wallFvPatch.H"
#include "fvc.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(anisotropicFilter, 0);
addToRunTimeSelectionTable(LESfilter, anisotropicFilter, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

anisotropicFilter::anisotropicFilter(const fvMesh& mesh, scalar widthCoeff)
:
    LESfilter(mesh),
    widthCoeff_(widthCoeff),
    coeff_
    (
        IOobject
        (
            "anisotropicFilterCoeff",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimLength*dimLength,
        zeroGradientFvPatchScalarField::typeName
    )
{
    for (direction d=0; d<vector::nComponents; d++)
    {
        coeff_.internalField().replace
        (
            d,
            (2.0/widthCoeff_)*mesh.V()
           /fvc::surfaceSum(mag(mesh.Sf().component(d)))().internalField()
        );
    }

    coeff_.correctBoundaryConditions();
}


anisotropicFilter::anisotropicFilter(const fvMesh& mesh, const dictionary& bd)
:
    LESfilter(mesh),
    widthCoeff_(readScalar(bd.subDict(type() + "Coeffs").lookup("widthCoeff"))),
    coeff_
    (
        IOobject
        (
            "anisotropicFilterCoeff",
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimLength*dimLength,
        zeroGradientFvPatchScalarField::typeName
    )
{
    for (direction d=0; d<vector::nComponents; d++)
    {
        coeff_.internalField().replace
        (
            d,
            (2.0/widthCoeff_)*mesh.V()
            /fvc::surfaceSum(mag(mesh.Sf().component(d)))().internalField()
        );
    }

    coeff_.correctBoundaryConditions();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void anisotropicFilter::read(const dictionary& bd)
{
    bd.subDict(type() + "Coeffs").lookup("widthCoeff") >> widthCoeff_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

tmp<volScalarField> anisotropicFilter::operator()
(
    const tmp<volScalarField>& unFilteredField
) const
{
    tmp<volScalarField> tmpFilteredField =
        unFilteredField
      + (
           coeff_
         & fvc::surfaceIntegrate
           (
               mesh().Sf()
              *fvc::snGrad(unFilteredField())
           )
        );

    volScalarField& filteredField = tmpFilteredField();

    forAll(mesh().boundary(), patchI)
    {
        if (isType<wallFvPatch>(mesh().boundary()[patchI]))
        {
            filteredField.boundaryField()[patchI] = 0.0;
        }
    }

    unFilteredField.clear();

    return tmpFilteredField;
}


tmp<volVectorField> anisotropicFilter::operator()
(
    const tmp<volVectorField>& unFilteredField
) const
{
    tmp<volVectorField> tmpFilteredField =
        unFilteredField
      + (
           coeff_
         & fvc::surfaceIntegrate
           (
               mesh().Sf()
              *fvc::snGrad(unFilteredField())
           )
        );

    volVectorField& filteredField = tmpFilteredField();

    forAll(mesh().boundary(), patchI)
    {
        if (isType<wallFvPatch>(mesh().boundary()[patchI]))
        {
            filteredField.boundaryField()[patchI] = vector::zero;
        }
    }

    unFilteredField.clear();

    return tmpFilteredField;
}


tmp<volSymmTensorField> anisotropicFilter::operator()
(
    const tmp<volSymmTensorField>& unFilteredField
) const
{
    tmp<volSymmTensorField> tmpFilteredField
    (
        new volSymmTensorField
        (
            IOobject
            (
                "anisotropicFilteredSymmTensorField",
                mesh().time().timeName(),
                mesh(),
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            mesh(),
            dimLength
        )
    );

    for (direction d=0; d<symmTensor::nComponents; d++)
    {
        tmpFilteredField().replace
        (
            d, anisotropicFilter::operator()(unFilteredField().component(d))
        );
    }

    unFilteredField.clear();

    return tmpFilteredField;
}


tmp<volTensorField> anisotropicFilter::operator()
(
    const tmp<volTensorField>& unFilteredField
) const
{
    tmp<volTensorField> tmpFilteredField
    (
        new volTensorField
        (
            IOobject
            (
                "anisotropicFilteredTensorField",
                mesh().time().timeName(),
                mesh(),
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            mesh(),
            dimLength
        )
    );

    for (direction d=0; d<tensor::nComponents; d++)
    {
        tmpFilteredField().replace
        (
            d, anisotropicFilter::operator()(unFilteredField().component(d))
        );
    }

    unFilteredField.clear();

    return tmpFilteredField;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
