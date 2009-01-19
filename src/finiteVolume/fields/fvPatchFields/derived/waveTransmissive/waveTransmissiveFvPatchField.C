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

#include "waveTransmissiveFvPatchField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "EulerDdtScheme.H"
#include "CrankNicholsonDdtScheme.H"
#include "backwardDdtScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    mixedFvPatchField<Type>(p, iF),
    fieldName_("Undefined"),
    phiName_("Undefined"),
    rhoName_("Undefined"),
    psiName_("Undefined"),
    gamma_(0.0),
    fieldInf_(pTraits<Type>::zero),
    lInf_(0.0)
{
    this->refValue() = pTraits<Type>::zero;
    this->refGrad() = pTraits<Type>::zero;
    this->valueFraction() = 0.0;
}


template<class Type>
waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
    const waveTransmissiveFvPatchField& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchField<Type>(ptf, p, iF, mapper),
    fieldName_(ptf.fieldName_),
    phiName_(ptf.phiName_),
    rhoName_(ptf.rhoName_),
    psiName_(ptf.psiName_),
    gamma_(ptf.gamma_),
    fieldInf_(ptf.fieldInf_),
    lInf_(ptf.lInf_)
{}


template<class Type>
waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchField<Type>(p, iF),
    fieldName_(dict.lookup("field")),
    phiName_(dict.lookup("phi")),
    rhoName_(dict.lookup("rho")),
    psiName_(dict.lookup("psi")),
    gamma_(readScalar(dict.lookup("gamma"))),
    fieldInf_(pTraits<Type>(dict.lookup("fieldInf"))),
    lInf_(readScalar(dict.lookup("lInf")))
{
    if (dict.found("value"))
    {
        fvPatchField<Type>::operator=
        (
            Field<Type>("value", dict, p.size())
        );
    }
    else
    {
        fvPatchField<Type>::operator=(this->patchInternalField());
    }

    this->refValue() = *this;
    this->refGrad() = pTraits<Type>::zero;
    this->valueFraction() = 0.0;

    if (mag(fieldInf_) < SMALL)
    {
        FatalIOErrorIn
        (
            "waveTransmissiveFvPatchField<Type>::"
            "waveTransmissiveFvPatchField"
            "(const fvPatch&, const Field<Type>&, const dictionary&)",
            dict
        )   << "unphysical fieldInf_ specified (fieldInf_ = 0)" << endl
            << "    To switch of field relaxation specify fieldInf_ = -1"
            << "\n    on patch " << this->patch().name()
            << " of field " << this->dimensionedInternalField().name()
            << " in file " << this->dimensionedInternalField().objectPath()
            << exit(FatalIOError);
    }

    if (mag(lInf_) < SMALL)
    {
        FatalIOErrorIn
        (
            "waveTransmissiveFvPatchField<Type>::"
            "waveTransmissiveFvPatchField"
            "(const fvPatch&, const Field<Type>&, const dictionary&)",
            dict
        )   << "unphysical lInf_ specified (lInf_ = 0)"
            << "\n    on patch " << this->patch().name()
            << " of field " << this->dimensionedInternalField().name()
            << " in file " << this->dimensionedInternalField().objectPath()
            << exit(FatalIOError);
    }
}


template<class Type>
waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
    const waveTransmissiveFvPatchField& ptpsf
)
:
    mixedFvPatchField<Type>(ptpsf),
    fieldName_(ptpsf.fieldName_),
    phiName_(ptpsf.phiName_),
    rhoName_(ptpsf.rhoName_),
    psiName_(ptpsf.psiName_),
    gamma_(ptpsf.gamma_),
    fieldInf_(ptpsf.fieldInf_),
    lInf_(ptpsf.lInf_)
{}


template<class Type>
waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
    const waveTransmissiveFvPatchField& ptpsf,
    const DimensionedField<Type, volMesh>& iF
)
:
    mixedFvPatchField<Type>(ptpsf, iF),
    fieldName_(ptpsf.fieldName_),
    phiName_(ptpsf.phiName_),
    rhoName_(ptpsf.rhoName_),
    psiName_(ptpsf.psiName_),
    gamma_(ptpsf.gamma_),
    fieldInf_(ptpsf.fieldInf_),
    lInf_(ptpsf.lInf_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void waveTransmissiveFvPatchField<Type>::autoMap
(
    const fvPatchFieldMapper& m
)
{
    mixedFvPatchField<Type>::autoMap(m);
}


template<class Type>
void waveTransmissiveFvPatchField<Type>::rmap
(
    const fvPatchField<Type>& ptf,
    const labelList& addr
)
{
    mixedFvPatchField<Type>::rmap(ptf, addr);
}


template<class Type>
void waveTransmissiveFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    word ddtScheme
    (
        this->dimensionedInternalField().mesh().ddtScheme(fieldName_)
    );
    scalar deltaT = this->db().time().deltaT().value();

    const GeometricField<Type, fvPatchField, volMesh>& field = 
        this->db().objectRegistry::
        lookupObject<GeometricField<Type, fvPatchField, volMesh> >(fieldName_);

    // Lookup the velocity and compressibility of the patch
    const fvPatchField<scalar>& psip = this->patch().lookupPatchField
    (
        psiName_,
        reinterpret_cast<const volScalarField*>(NULL),
        reinterpret_cast<const scalar*>(NULL)
    );

    const surfaceScalarField& phi = 
        this->db().objectRegistry::lookupObject<surfaceScalarField>(phiName_);

    fvsPatchField<scalar> phip = this->patch().lookupPatchField
    (
        phiName_,
        reinterpret_cast<const surfaceScalarField*>(NULL),
        reinterpret_cast<const scalar*>(NULL)
    );

    if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
    {
        const fvPatchScalarField& rhop = this->patch().lookupPatchField
        (
            rhoName_,
            reinterpret_cast<const volScalarField*>(NULL),
            reinterpret_cast<const scalar*>(NULL)
        );

        phip /= rhop;
    }

    // Calculate the speed of the field wave w
    // by summing the component of the velocity normal to the boundary
    // and the speed of sound (sqrt(gamma_/psi)).
    // If the wave is incoming set the speed to 0.
    scalarField w = 
        Foam::max(phip/this->patch().magSf() + sqrt(gamma_/psip), scalar(0));

    // Calculate the field wave coefficient alpha (See notes)
    scalarField alpha = w*deltaT*this->patch().deltaCoeffs();

    // Calculate the field relaxation coefficient k (See notes)
    scalarField k = w*deltaT/lInf_;

    label patchi = this->patch().index();

    // Non-reflecting outflow boundary
    // If fieldInf_ defined setup relaxation to that value.
    // If it is 0 or negative
    if (mag(fieldInf_) > SMALL)
    {
        if
        (
            ddtScheme == fv::EulerDdtScheme<scalar>::typeName
         || ddtScheme == fv::CrankNicholsonDdtScheme<scalar>::typeName
        )
        {
            this->refValue() = 
            (
                field.oldTime().boundaryField()[patchi] + k*fieldInf_
            )/(1.0 + k);

            this->valueFraction() = (1.0 + k)/(1.0 + alpha + k);
        }
        else if (ddtScheme == fv::backwardDdtScheme<scalar>::typeName)
        {
            this->refValue() =
            (
                2.0*field.oldTime().boundaryField()[patchi]
              - 0.5*field.oldTime().oldTime().boundaryField()[patchi]
              + k*fieldInf_
            )/(1.5 + k);

            this->valueFraction() = (1.5 + k)/(1.5 + alpha + k);
        }
        else
        {
            FatalErrorIn
            (
                "waveTransmissiveFvPatchField<Type>::updateCoeffs()"
            )   << "    Unsupported temporal differencing scheme : "
                << ddtScheme
                << "\n    on patch " << this->patch().name()
                << " of field " << this->dimensionedInternalField().name()
                << " in file " << this->dimensionedInternalField().objectPath()
                << exit(FatalError);
        }
    }
    else
    {
        if
        (
            ddtScheme == fv::EulerDdtScheme<scalar>::typeName
         || ddtScheme == fv::CrankNicholsonDdtScheme<scalar>::typeName
        )
        {
            this->refValue() = field.oldTime().boundaryField()[patchi];

            this->valueFraction() = 1.0/(1.0 + alpha);
        }
        else if (ddtScheme == fv::backwardDdtScheme<scalar>::typeName)
        {
            this->refValue() =
            (
                2.0*field.oldTime().boundaryField()[patchi]
              - 0.5*field.oldTime().oldTime().boundaryField()[patchi]
            )/1.5;

            this->valueFraction() = 1.5/(1.5 + alpha);
        }
        else
        {
            FatalErrorIn
            (
                "waveTransmissiveFvPatchField<Type>::updateCoeffs()"
            )   << "    Unsupported temporal differencing scheme : "
                << ddtScheme
                << "\n    on patch " << this->patch().name()
                << " of field " << this->dimensionedInternalField().name()
                << " in file " << this->dimensionedInternalField().objectPath()
                << exit(FatalError);
        }
    }

    mixedFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void waveTransmissiveFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    os.writeKeyword("field") << fieldName_ << token::END_STATEMENT << nl;
    os.writeKeyword("phi") << phiName_ << token::END_STATEMENT << nl;
    os.writeKeyword("rho") << rhoName_ << token::END_STATEMENT << nl;
    os.writeKeyword("psi") << psiName_ << token::END_STATEMENT << nl;
    os.writeKeyword("gamma") << gamma_ << token::END_STATEMENT << endl;
    os.writeKeyword("fieldInf") << fieldInf_ << token::END_STATEMENT << endl;
    os.writeKeyword("lInf") << lInf_ << token::END_STATEMENT << endl;
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
