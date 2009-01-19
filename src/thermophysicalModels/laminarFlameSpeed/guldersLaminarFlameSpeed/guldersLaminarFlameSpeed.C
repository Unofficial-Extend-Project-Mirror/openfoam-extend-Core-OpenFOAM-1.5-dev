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

#include "guldersLaminarFlameSpeed.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(guldersLaminarFlameSpeed, 0);

addToRunTimeSelectionTable
(
    laminarFlameSpeed,
    guldersLaminarFlameSpeed,
    dictionary
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

guldersLaminarFlameSpeed::guldersLaminarFlameSpeed
(
    const dictionary& dict,
    const hhuCombustionThermo& ct
)
:
    laminarFlameSpeed(dict, ct),

    coeffsDict_(dict.subDict(typeName + "Coeffs").subDict(fuel_)),
    W_(dimensionedScalar(coeffsDict_.lookup("W")).value()),
    eta_(dimensionedScalar(coeffsDict_.lookup("eta")).value()),
    xi_(dimensionedScalar(coeffsDict_.lookup("xi")).value()),
    f_(dimensionedScalar(coeffsDict_.lookup("f")).value()),
    alpha_(dimensionedScalar(coeffsDict_.lookup("alpha")).value()),
    beta_(dimensionedScalar(coeffsDict_.lookup("beta")).value())
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

guldersLaminarFlameSpeed::~guldersLaminarFlameSpeed()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

inline scalar guldersLaminarFlameSpeed::SuRef
(
    scalar phi
) const
{
    if (phi > SMALL)
    {
        return W_*pow(phi, eta_)*exp(-xi_*sqr(phi - 1.075));
    }
    else
    {
        return 0.0;
    }
}


inline scalar guldersLaminarFlameSpeed::Su0pTphi
(
    scalar p,
    scalar Tu,
    scalar phi,
    scalar Yres
) const
{
    static const scalar Tref = 300.0;
    static const scalar pRef = 1.013e5;

    return SuRef(phi)*pow((Tu/Tref), alpha_)*pow((p/pRef), beta_)*(1 - f_*Yres);
}


tmp<volScalarField> guldersLaminarFlameSpeed::Su0pTphi
(
    const volScalarField& p,
    const volScalarField& Tu,
    scalar phi
) const
{
    tmp<volScalarField> tSu0
    (
        new volScalarField
        (
            IOobject
            (
                "Su0",
                p.time().timeName(),
                p.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            p.mesh(),
            dimensionedScalar("Su0", dimVelocity, 0.0)
        )
    );

    volScalarField& Su0 = tSu0();

    forAll(Su0, celli)
    {
        Su0[celli] = Su0pTphi(p[celli], Tu[celli], phi, 0.0);
    }

    forAll(Su0.boundaryField(), patchi)
    {
        forAll(Su0.boundaryField()[patchi], facei)
        {
            Su0.boundaryField()[patchi][facei] =
                Su0pTphi
                (
                    p.boundaryField()[patchi][facei],
                    Tu.boundaryField()[patchi][facei],
                    phi,
                    0.0
                );
        }
    }

    return tSu0;
}


tmp<volScalarField> guldersLaminarFlameSpeed::Su0pTphi
(
    const volScalarField& p,
    const volScalarField& Tu,
    const volScalarField& phi
) const
{
    tmp<volScalarField> tSu0
    (
        new volScalarField
        (
            IOobject
            (
                "Su0",
                p.time().timeName(),
                p.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            p.mesh(),
            dimensionedScalar("Su0", dimVelocity, 0.0)
        )
    );

    volScalarField& Su0 = tSu0();

    forAll(Su0, celli)
    {
        Su0[celli] = Su0pTphi(p[celli], Tu[celli], phi[celli], 0.0);
    }

    forAll(Su0.boundaryField(), patchi)
    {
        forAll(Su0.boundaryField()[patchi], facei)
        {
            Su0.boundaryField()[patchi][facei] =
                Su0pTphi
                (
                    p.boundaryField()[patchi][facei],
                    Tu.boundaryField()[patchi][facei],
                    phi.boundaryField()[patchi][facei],
                    0.0
                );
        }
    }

    return tSu0;
}


tmp<volScalarField> guldersLaminarFlameSpeed::operator()() const
{
    if (hhuCombustionThermo_.composition().contains("ft"))
    {
        const volScalarField& ft = hhuCombustionThermo_.composition().Y("ft");

        return Su0pTphi
        (
            hhuCombustionThermo_.p(),
            hhuCombustionThermo_.Tu(),
            dimensionedScalar
            (
                hhuCombustionThermo_.lookup("stoichiometricAirFuelMassRatio")
            )*ft/(scalar(1) - ft)
        );
    }
    else
    {
        return Su0pTphi
        (
            hhuCombustionThermo_.p(),
            hhuCombustionThermo_.Tu(),
            equivalenceRatio_
        );
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
