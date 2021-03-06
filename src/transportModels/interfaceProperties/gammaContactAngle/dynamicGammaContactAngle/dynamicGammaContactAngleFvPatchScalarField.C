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

#include "dynamicGammaContactAngleFvPatchScalarField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volMesh.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::dynamicGammaContactAngleFvPatchScalarField::
dynamicGammaContactAngleFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    gammaContactAngleFvPatchScalarField(p, iF),
    theta0_(0.0),
    uTheta_(0.0),
    thetaA_(0.0),
    thetaR_(0.0)
{}


Foam::dynamicGammaContactAngleFvPatchScalarField::
dynamicGammaContactAngleFvPatchScalarField
(
    const dynamicGammaContactAngleFvPatchScalarField& gcpsf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    gammaContactAngleFvPatchScalarField(gcpsf, p, iF, mapper),
    theta0_(gcpsf.theta0_),
    uTheta_(gcpsf.uTheta_),
    thetaA_(gcpsf.thetaA_),
    thetaR_(gcpsf.thetaR_)
{}


Foam::dynamicGammaContactAngleFvPatchScalarField::
dynamicGammaContactAngleFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    gammaContactAngleFvPatchScalarField(p, iF),
    theta0_(readScalar(dict.lookup("theta0"))),
    uTheta_(readScalar(dict.lookup("uTheta"))),
    thetaA_(readScalar(dict.lookup("thetaA"))),
    thetaR_(readScalar(dict.lookup("thetaR")))
{
    evaluate();
}


Foam::dynamicGammaContactAngleFvPatchScalarField::
dynamicGammaContactAngleFvPatchScalarField
(
    const dynamicGammaContactAngleFvPatchScalarField& gcpsf
)
:
    gammaContactAngleFvPatchScalarField(gcpsf),
    theta0_(gcpsf.theta0_),
    uTheta_(gcpsf.uTheta_),
    thetaA_(gcpsf.thetaA_),
    thetaR_(gcpsf.thetaR_)
{}


Foam::dynamicGammaContactAngleFvPatchScalarField::
dynamicGammaContactAngleFvPatchScalarField
(
    const dynamicGammaContactAngleFvPatchScalarField& gcpsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    gammaContactAngleFvPatchScalarField(gcpsf, iF),
    theta0_(gcpsf.theta0_),
    uTheta_(gcpsf.uTheta_),
    thetaA_(gcpsf.thetaA_),
    thetaR_(gcpsf.thetaR_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::tmp<Foam::scalarField>
Foam::dynamicGammaContactAngleFvPatchScalarField::theta
(
    const fvPatchVectorField& Up,
    const fvsPatchVectorField& nHat
) const
{
    if (uTheta_ < SMALL)
    {
        return tmp<scalarField>(new scalarField(size(), theta0_));
    }

    vectorField nf = patch().nf();

    // Calculated the component of the velocity parallel to the wall
    vectorField Uwall = Up.patchInternalField() - Up;
    Uwall -= (nf & Uwall)*nf;

    // Find the direction of the interface parallel to the wall
    vectorField nWall = nHat - (nf & nHat)*nf;

    // Normalise nWall
    nWall /= (mag(nWall) + SMALL);

    // Calculate Uwall resolved normal to the interface parallel to
    // the interface
    scalarField uwall = nWall & Uwall;

    return theta0_ + (thetaA_ - thetaR_)*tanh(uwall/uTheta_);
}


void Foam::dynamicGammaContactAngleFvPatchScalarField::write(Ostream& os) const
{
    fvPatchScalarField::write(os);
    os.writeKeyword("theta0") << theta0_ << token::END_STATEMENT << nl;
    os.writeKeyword("uTheta") << uTheta_ << token::END_STATEMENT << nl;
    os.writeKeyword("thetaA") << thetaA_ << token::END_STATEMENT << nl;
    os.writeKeyword("thetaR") << thetaR_ << token::END_STATEMENT << nl;
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        dynamicGammaContactAngleFvPatchScalarField
    );
}


// ************************************************************************* //
