/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2004-2007 Hrvoje Jasak
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

Class
    cohesiveZoneIncrementalFvPatchVectorField

Description

\*---------------------------------------------------------------------------*/

#include "cohesiveZoneIncrementalFvPatchVectorField.H"
#include "addToRunTimeSelectionTable.H"
#include "transformField.H"
#include "volFields.H"
#include "rheologyModel.H"
#include "plasticityModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

cohesiveZoneIncrementalFvPatchVectorField::cohesiveZoneIncrementalFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    directionMixedFvPatchVectorField(p, iF),
    UName_("undefined"),
    DUName_("undefined"),
    rheologyName_("undefined"),
    cohesiveLawPtr_(NULL),
    crackIndicator_(p.size(), 0.0),
    crazeIndicator_(p.size(), 0.0),
    relaxationFactor_(1.0)
{}


cohesiveZoneIncrementalFvPatchVectorField::cohesiveZoneIncrementalFvPatchVectorField
(
    const cohesiveZoneIncrementalFvPatchVectorField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    directionMixedFvPatchVectorField(ptf, p, iF, mapper),
    UName_(ptf.UName_),
    DUName_(ptf.DUName_),
    rheologyName_(ptf.rheologyName_),
    cohesiveLawPtr_(ptf.cohesiveLawPtr_),
    crackIndicator_(ptf.crackIndicator_),
    crazeIndicator_(ptf.crazeIndicator_),
    relaxationFactor_(ptf.relaxationFactor_)
{}


cohesiveZoneIncrementalFvPatchVectorField::cohesiveZoneIncrementalFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    directionMixedFvPatchVectorField(p, iF),
    UName_(dict.lookup("U")),
    DUName_(dict.lookup("DU")),
    rheologyName_(dict.lookup("rheology")),
    cohesiveLawPtr_
    (
        cohesiveLaw::New(dict.lookup("cohesiveLaw"), dict).ptr()
    ),
    crackIndicator_(p.size(), 0.0),    
    crazeIndicator_(p.size(), 0.0),    
    relaxationFactor_(readScalar(dict.lookup("relaxationFactor")))
{
    if (dict.found("refValue"))
    {
        this->refValue() = vectorField("refValue", dict, p.size());
    }
    else
    {
        this->refValue() = vector::zero;
    }
    
    if (dict.found("refGradient"))
    {
        this->refGrad() = vectorField("refGradient", dict, p.size());
    }
    else
    {
        this->refGrad() = vector::zero;
    }

    if (dict.found("valueFraction"))
    {
        this->valueFraction() = 
            symmTensorField("valueFraction", dict, p.size());
    }
    else
    {
        vectorField n = patch().nf();

        this->valueFraction() = sqr(n);
    }

    if (dict.found("value"))
    {
        Field<vector>::operator=(vectorField("value", dict, p.size()));
    }
    else
    {
        Field<vector> normalValue = transform(valueFraction(), refValue());

        Field<vector> gradValue =
            this->patchInternalField() + refGrad()/this->patch().deltaCoeffs();

        Field<vector> transformGradValue =
            transform(I - valueFraction(), gradValue);

        Field<vector>::operator=(normalValue + transformGradValue);
    }

    if (dict.found("crackIndicator"))
    {
        this->refValue() = vectorField("crackIndicator", dict, p.size());
    }

    if (dict.found("crazeIndicator"))
    {
        this->refValue() = vectorField("crazeIndicator", dict, p.size());
    }
}


cohesiveZoneIncrementalFvPatchVectorField::cohesiveZoneIncrementalFvPatchVectorField
(
    const cohesiveZoneIncrementalFvPatchVectorField& ptf,
    const DimensionedField<vector, volMesh>& iF
)
:
    directionMixedFvPatchVectorField(ptf, iF),
    UName_(ptf.UName_),
    DUName_(ptf.DUName_),
    rheologyName_(ptf.rheologyName_),
    cohesiveLawPtr_(ptf.cohesiveLawPtr_),
    crackIndicator_(ptf.crackIndicator_),
    crazeIndicator_(ptf.crazeIndicator_),
    relaxationFactor_(ptf.relaxationFactor_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Map from self
void cohesiveZoneIncrementalFvPatchVectorField::autoMap
(
    const fvPatchFieldMapper& m
)
{
    if (cohesiveLawPtr_ == NULL)
    {
        FatalErrorIn("cohesiveZoneIncrementalFvPatchVectorField::autoMap")
            << "NULL cohesive law"
            << abort(FatalError);
    }

    directionMixedFvPatchVectorField::autoMap(m);
    crackIndicator_.autoMap(m);
    crazeIndicator_.autoMap(m);
}


// Reverse-map the given fvPatchField onto this fvPatchField
void cohesiveZoneIncrementalFvPatchVectorField::rmap
(
    const fvPatchField<vector>& ptf,
    const labelList& addr
)
{
    directionMixedFvPatchVectorField::rmap(ptf, addr);

    const cohesiveZoneIncrementalFvPatchVectorField& dmptf =
        refCast<const cohesiveZoneIncrementalFvPatchVectorField>(ptf);

    // No need to grab the cohesive zone pointer more than once
    if (!cohesiveLawPtr_)
    {
        cohesiveLawPtr_ = dmptf.cohesiveLawPtr_->clone().ptr();
    }

    crackIndicator_ = dmptf.crackIndicator_;
    crazeIndicator_ = dmptf.crazeIndicator_;
    relaxationFactor_ = dmptf.relaxationFactor_;
}


void cohesiveZoneIncrementalFvPatchVectorField::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    // Looking up rheology
    const rheologyModel& rheology =
        this->db().objectRegistry::lookupObject<rheologyModel>(rheologyName_);

    const scalarField mu = 
        rheology.mu()().boundaryField()[patch().index()];

    const scalarField lambda =
        rheology.lambda()().boundaryField()[patch().index()];

    const fvPatchField<tensor>& gradDU =
        patch().lookupPatchField<volTensorField, tensor>
        (
            "grad(" + DUName_ + ")"
        );

    const fvPatchField<vector>& oldU =
        patch().lookupPatchField<volVectorField, vector>
        (
            UName_
        );

    const fvPatchField<symmTensor>& oldSigma =
        patch().lookupPatchField<volSymmTensorField, symmTensor>
        (
            "sigma"
        );

    // Patch displacement increment
    const vectorField& DU = *this;

    symmTensorField DEpsilon = symm(gradDU);

    symmTensorField DEpsilonP(size(), symmTensor::zero);
    if(rheology.type() == plasticityModel::typeName)
    {
        const plasticityModel& plasticity = 
            refCast<const plasticityModel>(rheology);

        DEpsilonP = 
            plasticity.DEpsilonP().boundaryField()[patch().index()];
    }

    // Patch stress increment
    symmTensorField DSigma = 
        2*mu*(DEpsilon - DEpsilonP) + I*(lambda*tr(DEpsilon));


    // Patch stress
    symmTensorField curSigma = oldSigma + DSigma;

    // Patch normal
    vectorField n = patch().nf();

    // Normal stress component
    scalarField oldSigmaN = (n&(n&oldSigma));

    // Normal stress component
    scalarField curSigmaN = (n&(n&curSigma));

    // Normal stress component
    scalarField DSigmaN = (n&(n&DSigma));

    // Chech crack propagation
    forAll(curSigmaN, faceI)
    {
        vector cohesiveTractionIncrement = vector::zero;

        if
        (
            (magSqr(valueFraction()[faceI]) > 1-SMALL)
         && (curSigmaN[faceI] >= law().sigmaMax().value())
        )
        {
            // Switch to full traction boundary condition
            valueFraction()[faceI] = symmTensor::zero;
            crazeIndicator_[faceI] = 1;
            crackIndicator_[faceI] = 0;
            
            Pout << "Crack started at face: " << faceI << endl;

            // Cohesive traction
            cohesiveTractionIncrement = 
                n[faceI]*law().sigmaMax().value()
              - n[faceI]*oldSigmaN[faceI];
        }
        else if(magSqr(valueFraction()[faceI]) < SMALL)
        {
            // Normal displacement
            scalar Un = -(n[faceI]&(oldU[faceI] + DU[faceI]));

            if(Un < -0.001*law().deltaC().value()/2)
            {
                // Return from traction to symmetryPlane
                refValue()[faceI] = vector::zero;
                refGrad()[faceI] = vector::zero;
                valueFraction()[faceI] = sqr(n[faceI]);
                crazeIndicator_[faceI] = 0;
                crackIndicator_[faceI] = 0;

                Pout << "Face removed from crack: " << faceI << endl;
            }
            else if(Un > law().deltaC().value()/2)
            {
                // Traction free
                cohesiveTractionIncrement =
                    vector::zero
                  - n[faceI]*oldSigmaN[faceI];

                crazeIndicator_[faceI] = 0;
                crackIndicator_[faceI] = 1;
            }
            else
            {
                // Calculate cohesive traction from cohesive zone model
                cohesiveTractionIncrement = 
                    law().traction(2*Un)*n[faceI]
                  - n[faceI]*oldSigmaN[faceI];

                crazeIndicator_[faceI] = 1;
                crackIndicator_[faceI] = 0;
            }
        }

        if(magSqr(valueFraction()[faceI]) < SMALL)
        {
            cohesiveTractionIncrement = 
                relaxationFactor_*cohesiveTractionIncrement 
              + (1.0 - relaxationFactor_)*DSigmaN[faceI]*n[faceI];

            refGrad()[faceI] =
            (
                cohesiveTractionIncrement
              - (
                    n[faceI] 
                  & (
                        mu[faceI]*gradDU[faceI].T() 
                      - (mu[faceI] + lambda[faceI])*gradDU[faceI]
                    )
                )
              - n[faceI]*lambda[faceI]*tr(gradDU[faceI])
              + 2*mu[faceI]*(n[faceI] & DEpsilonP[faceI])
            )
           /(2.0*mu[faceI] + lambda[faceI]);
        }
    }

    directionMixedFvPatchVectorField::updateCoeffs();
}

// void cohesiveZoneIncrementalFvPatchVectorField::evaluate()
// {
//     if (!this->updated())
//     {
//         this->updateCoeffs();
//     }

//     Field<vector> normalValue = transform(valueFraction(), refValue());

//     const fvPatchField<tensor>& gradU =
//         patch().lookupPatchField<volTensorField, tensor>
//         (
//             "grad(" + DUName_ + ")"
//         );

//     const vectorField n = patch().nf();

//     vectorField nGradUp = (n&gradU.patchInternalField());

//     Field<vector> gradValue =
//         this->patchInternalField() 
//       + 0.5*nGradUp/this->patch().deltaCoeffs()
//       + 0.5*refGrad()/this->patch().deltaCoeffs();

//     Field<vector> transformGradValue =
//         transform(I - valueFraction(), gradValue);

//     Field<vector>::operator=(normalValue + transformGradValue);

//     transformFvPatchField<vector>::evaluate();
// }


// Write
void cohesiveZoneIncrementalFvPatchVectorField::write(Ostream& os) const
{
    directionMixedFvPatchVectorField::write(os);
    os.writeKeyword("U") << UName_ << token::END_STATEMENT << nl;
    os.writeKeyword("DU") << DUName_ << token::END_STATEMENT << nl;
    os.writeKeyword("rheology") << rheologyName_ << token::END_STATEMENT << nl;
    os.writeKeyword("cohesiveLaw") << law().type() 
        << token::END_STATEMENT << nl;
    crazeIndicator_.writeEntry("crazeIndicator", os);
    crackIndicator_.writeEntry("crackIndicator", os);
    os.writeKeyword("relaxationFactor") << relaxationFactor_
        << token::END_STATEMENT << nl;
    law().writeDict(os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchVectorField, 
    cohesiveZoneIncrementalFvPatchVectorField
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
