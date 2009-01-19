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

#include "spaceVaryingGammaContactAngleFvPatchScalarField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

spaceVaryingGammaContactAngleFvPatchScalarField::
spaceVaryingGammaContactAngleFvPatchScalarField
(
    const fvPatch& p,
    const scalarField& iF
)
:
    gammaContactAngleFvPatchScalarField(p, iF),
    theta0_(p.size(), 0.0)
{}


spaceVaryingGammaContactAngleFvPatchScalarField::
spaceVaryingGammaContactAngleFvPatchScalarField
(
    const spaceVaryingGammaContactAngleFvPatchScalarField& gcpsf,
    const fvPatch& p,
    const scalarField& iF,
    const fvPatchFieldMapper& mapper
)
:
    gammaContactAngleFvPatchScalarField(gcpsf, p, iF, mapper),
    theta0_(gcpsf.theta0_, mapper)
{}


spaceVaryingGammaContactAngleFvPatchScalarField::
spaceVaryingGammaContactAngleFvPatchScalarField
(
    const fvPatch& p,
    const scalarField& iF,
    const dictionary& dict
)
:
    gammaContactAngleFvPatchScalarField(p, iF),
    theta0_("theta0", dict, p.size())
{
    evaluate();
}


spaceVaryingGammaContactAngleFvPatchScalarField::
spaceVaryingGammaContactAngleFvPatchScalarField
(
    const spaceVaryingGammaContactAngleFvPatchScalarField& gcpsf
)
:
    gammaContactAngleFvPatchScalarField(gcpsf),
    theta0_(gcpsf.theta0_)
{}


spaceVaryingGammaContactAngleFvPatchScalarField::
spaceVaryingGammaContactAngleFvPatchScalarField
(
    const spaceVaryingGammaContactAngleFvPatchScalarField& gcpsf,
    const scalarField& iF
)
:
    gammaContactAngleFvPatchScalarField(gcpsf, iF),
    theta0_(gcpsf.theta0_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void spaceVaryingGammaContactAngleFvPatchScalarField::autoMap
(
    const fvPatchFieldMapper& m
)
{
    scalarField::autoMap(m);
    theta0_.autoMap(m);
}


void spaceVaryingGammaContactAngleFvPatchScalarField::rmap
(
    const fvPatchScalarField& ptf,
    const labelList& addr
)
{
    zeroGradientFvPatchScalarField::rmap(ptf, addr);

    const spaceVaryingGammaContactAngleFvPatchScalarField& tiptf =
        refCast<const spaceVaryingGammaContactAngleFvPatchScalarField>(ptf);

    theta0_.rmap(tiptf.theta0_, addr);
}


// Return equilibrium contact-angle
tmp<scalarField> spaceVaryingGammaContactAngleFvPatchScalarField::theta
(
    const fvPatchVectorField&,
    const fvPatchVectorField&
) const
{
    return theta0_;
}


// Write
void spaceVaryingGammaContactAngleFvPatchScalarField::write(Ostream& os) const
{
    fvPatchScalarField::write(os);
    theta0_.writeEntry("theta0", os);
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchScalarField,
    spaceVaryingGammaContactAngleFvPatchScalarField
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
