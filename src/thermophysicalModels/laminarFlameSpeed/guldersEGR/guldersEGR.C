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

#include "guldersEGR.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(guldersEGR, 0);

addToRunTimeSelectionTable
(
    laminarFlameSpeed,
    guldersEGR,
    dictionary
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

guldersEGR::guldersEGR
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

guldersEGR::~guldersEGR()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

inline scalar guldersEGR::SuRef
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


inline scalar guldersEGR::Su0pTphi
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


tmp<volScalarField> guldersEGR::Su0pTphi
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


tmp<volScalarField> guldersEGR::Su0pTphi
(
    const volScalarField& p,
    const volScalarField& Tu,
    const volScalarField& phi,
    const volScalarField& egr
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
        Su0[celli] = Su0pTphi(p[celli], Tu[celli], phi[celli], egr[celli]);
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
                    egr.boundaryField()[patchi][facei]
                );
        }
    }

    return tSu0;
}


tmp<volScalarField> guldersEGR::operator()() const
{
    if 
    (
        hhuCombustionThermo_.composition().contains("ft")
     && hhuCombustionThermo_.composition().contains("egr")
    )
    {
        return Su0pTphi
        (
            hhuCombustionThermo_.p(),
            hhuCombustionThermo_.Tu(),
            dimensionedScalar
            (
                hhuCombustionThermo_.lookup("stoichiometricAirFuelMassRatio")
            )/
            (
                scalar(1)/hhuCombustionThermo_.composition().Y("ft")
              - scalar(1)
            ),
            hhuCombustionThermo_.composition().Y("egr")
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
