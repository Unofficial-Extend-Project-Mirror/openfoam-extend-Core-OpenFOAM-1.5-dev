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
    Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

\*---------------------------------------------------------------------------*/

#include "tractionDisplacementIncrementFvPatchVectorField.H"
#include "addToRunTimeSelectionTable.H"
#include "volFields.H"
#include "rheologyModel.H"
#include "plasticityModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

tractionDisplacementIncrementFvPatchVectorField::
tractionDisplacementIncrementFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedGradientFvPatchVectorField(p, iF),
    DUName_("undefined"),
    rheologyName_("undefined"),
    DTraction_(p.size(), vector::zero),
    DPressure_(p.size(), 0.0)
{
    fvPatchVectorField::operator=(patchInternalField());
    gradient() = vector::zero;
}


tractionDisplacementIncrementFvPatchVectorField::
tractionDisplacementIncrementFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fixedGradientFvPatchVectorField(p, iF),
    DUName_(dict.lookup("DU")),
    rheologyName_(dict.lookup("rheology")),
    DTraction_("DTraction", dict, p.size()),
    DPressure_("DPressure", dict, p.size())
{
    fvPatchVectorField::operator=(patchInternalField());
    gradient() = vector::zero;
    Info << "rf: " << rheologyName_ << endl;
}


tractionDisplacementIncrementFvPatchVectorField::
tractionDisplacementIncrementFvPatchVectorField
(
    const tractionDisplacementIncrementFvPatchVectorField& tdpvf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedGradientFvPatchVectorField(tdpvf, p, iF, mapper),
    DUName_(tdpvf.DUName_),
    rheologyName_(tdpvf.rheologyName_),
    DTraction_(tdpvf.DTraction_, mapper),
    DPressure_(tdpvf.DPressure_, mapper)
{}


tractionDisplacementIncrementFvPatchVectorField::
tractionDisplacementIncrementFvPatchVectorField
(
    const tractionDisplacementIncrementFvPatchVectorField& tdpvf
)
:
    fixedGradientFvPatchVectorField(tdpvf),
    DUName_(tdpvf.DUName_),
    rheologyName_(tdpvf.rheologyName_),
    DTraction_(tdpvf.DTraction_),
    DPressure_(tdpvf.DPressure_)
{}


tractionDisplacementIncrementFvPatchVectorField::
tractionDisplacementIncrementFvPatchVectorField
(
    const tractionDisplacementIncrementFvPatchVectorField& tdpvf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedGradientFvPatchVectorField(tdpvf, iF),
    DUName_(tdpvf.DUName_),
    rheologyName_(tdpvf.rheologyName_),
    DTraction_(tdpvf.DTraction_),
    DPressure_(tdpvf.DPressure_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void tractionDisplacementIncrementFvPatchVectorField::autoMap
(
    const fvPatchFieldMapper& m
)
{
    fixedGradientFvPatchVectorField::autoMap(m);
    DTraction_.autoMap(m);
    DPressure_.autoMap(m);
}


// Reverse-map the given fvPatchField onto this fvPatchField
void tractionDisplacementIncrementFvPatchVectorField::rmap
(
    const fvPatchVectorField& ptf,
    const labelList& addr
)
{
    fixedGradientFvPatchVectorField::rmap(ptf, addr);

    const tractionDisplacementIncrementFvPatchVectorField& dmptf =
        refCast<const tractionDisplacementIncrementFvPatchVectorField>(ptf);

    DTraction_.rmap(dmptf.DTraction_, addr);
    DPressure_.rmap(dmptf.DPressure_, addr);
}


// Update the coefficients associated with the patch field
void tractionDisplacementIncrementFvPatchVectorField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    // Looking up rheology
    const rheologyModel& rheology =
        this->db().objectRegistry::lookupObject<rheologyModel>(rheologyName_);

    const scalarField mu = rheology.mu()().boundaryField()[patch().index()];
    const scalarField lambda =
        rheology.lambda()().boundaryField()[patch().index()];

    vectorField n = patch().nf();

    const fvPatchField<tensor>& gradU =
        patch().lookupPatchField<volTensorField, tensor>
        (
            "grad(" +DUName_ + ")"
        );

    gradient() =
    (
        (DTraction_ - (DPressure_)*n)
      - (n & (mu*gradU.T() - (mu + lambda)*gradU))
      - n*lambda*tr(gradU)
    )/(2.0*mu + lambda);

    if(rheology.type() == plasticityModel::typeName)
    {
        const plasticityModel& plasticity = 
            refCast<const plasticityModel>(rheology);

        gradient() +=
            2*mu*(n & plasticity.DEpsilonP().boundaryField()[patch().index()])
           /(2.0*mu + lambda);
    }

    fixedGradientFvPatchVectorField::updateCoeffs();
}


// Write
void tractionDisplacementIncrementFvPatchVectorField::write(Ostream& os) const
{
    fvPatchVectorField::write(os);
    os.writeKeyword("DU") << DUName_ << token::END_STATEMENT << nl;
    os.writeKeyword("rheology") << rheologyName_ << token::END_STATEMENT << nl;
    DTraction_.writeEntry("DTraction", os);
    DPressure_.writeEntry("DPressure", os);
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField(fvPatchVectorField, tractionDisplacementIncrementFvPatchVectorField);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
