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

#include "porousZone.H"
#include "fvMesh.H"
#include "fvMatrices.H"
#include "OneField.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::porousZone::porousZone(const fvMesh& mesh, Istream& is)
:
    mesh_(mesh),
    name_(is),
    dict_(is),
    cellZoneID_(mesh_.cellZones().findZoneID(name_)),
    C0_("C0", dimensionSet(0, 1, 0, 0, 0), scalar(0)),
    C1_("C1", dimensionSet(0, 1, 0, 0, 0), scalar(0)),
    D_("D", dimensionSet(0, -2, 0, 0, 0), tensor::zero),
    F_("F", dimensionSet(0, -1, 0, 0, 0), tensor::zero)
{
    if (cellZoneID_ == -1 && !Pstream::parRun())
    {
        FatalErrorIn
        (
            "Foam::porousZone::porousZone(const fvMesh& , const dictionary&)"
        )   << "cannot find porous cellZone " << name_
            << exit(FatalError);
    }

    tensor E;

    if (dict_.found("e1") && dict_.found("e2"))
    {
        vector e1(dict_.lookup("e1"));
        vector e2(dict_.lookup("e2"));

        // Absorb minor nonorthogonality in e2
        e1 = e1/mag(e1);
        e2 = e2 - (e1 & e2)*e1;
        e2 = e2/mag(e2);
        vector e3 = e1 ^ e2;

        E.x() = e1;
        E.y() = e2;
        E.z() = e3;
        E = E.T();
    }
    else if (dict_.found("e2") && dict_.found("e3"))
    {
        vector e2(dict_.lookup("e2"));
        vector e3(dict_.lookup("e3"));

        // Absorb minor nonorthogonality in e3
        e2 = e2/mag(e2);
        e3 = e3 - (e2 & e3)*e2;
        e3 = e3/mag(e3);
        vector e1 = e2 ^ e3;

        E.x() = e1;
        E.y() = e2;
        E.z() = e3;
        E = E.T();
    }
    else if (dict_.found("e3") && dict_.found("e1"))
    {
        vector e3(dict_.lookup("e3"));
        vector e1(dict_.lookup("e1"));

        // Absorb minor nonorthogonality in e1
        e3 = e3/mag(e3);
        e1 = e1 - (e1 & e3)*e3;
        e1 = e1/mag(e1);
        vector e2 = e3 ^ e1;

        E.x() = e1;
        E.y() = e2;
        E.z() = e3;
        E = E.T();
    }
    else
    {
        FatalIOErrorIn
        (
            "porousZone::porousZone(const fvMesh&, const Istream&)",
            dict_
        )
            << "orientation as e1/e2 or e2/e3 or e3/e1 not specified"
            << exit(FatalIOError);
    }


    // powerLaw coefficients
    if (dict_.found("powerLaw"))
    {
        dictionary const & subDict = dict_.subDict("powerLaw");
        if (subDict.found("C0"))
        {
            subDict.lookup("C0") >> C0_;
        }
        if (subDict.found("C1"))
        {
            subDict.lookup("C1") >> C1_;
        }
    }


    // Darcy-Forchheimer coefficients
    if (dict_.found("Darcy"))
    {
        const dictionary& subDict = dict_.subDict("Darcy");

        dimensionedVector d("d", D_.dimensions(), vector::zero);
        dimensionedVector f("f", F_.dimensions(), vector::zero);

        if (subDict.found("d"))
        {
            subDict.lookup("d") >> d;
        }
        if (subDict.found("f"))
        {
            subDict.lookup("f") >> f;
        }

        if (D_.dimensions() != d.dimensions())
        {
            FatalIOErrorIn
            (
                "Foam::porousZone::porousZone"
                "(const fvMesh& , const dictionary&)",
                dict_
            )   << "incorrect dimensions for d: " << d.dimensions()
                << " should be " << D_.dimensions()
                << exit(FatalIOError);
        }

        if (F_.dimensions() != f.dimensions())
        {
            FatalIOErrorIn
                (
                    "Foam::porousZone::porousZone"
                    "(const fvMesh& , const dictionary&)",
                    dict_
                )   << "incorrect dimensions for f: " << f.dimensions()
                    << " should be " << F_.dimensions()
                    << exit(FatalIOError);
        }

        D_.value().xx() = d.value().x();
        D_.value().yy() = d.value().y();
        D_.value().zz() = d.value().z();
        D_.value() = (E & D_ & E.T()).value();

        F_.value().xx() = 0.5*f.value().x();
        F_.value().yy() = 0.5*f.value().y();
        F_.value().zz() = 0.5*f.value().z();
        F_.value() = (E & F_ & E.T()).value();
    }

    // provide some feedback for the user
    //writeDict(Info, false);

    // it is an error not to define anything
    if
    (
        C0_.value() <= VSMALL
     && magSqr(D_.value()) <= VSMALL
     && magSqr(F_.value()) <= VSMALL
    )
    {
        FatalIOErrorIn
        (
            "porousZone::porousZone(const fvMesh&, const Istream&)",
            dict_
        )   << "neither powerLaw (C0/C1) "
               "nor Darcy-Forchheimer law (d/f) specified"
            << exit(FatalIOError);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::porousZone::addResistance(fvVectorMatrix& UEqn) const
{
    if (cellZoneID_ == -1)
    {
        return;
    }

    bool compressible = false;
    if (UEqn.dimensions() == dimensionSet(1, 1, -2, 0, 0))
    {
        compressible = true;
    }

    const labelList& cells = mesh_.cellZones()[cellZoneID_];
    const scalarField& V = mesh_.V();
    scalarField& Udiag = UEqn.diag();
    vectorField& Usource = UEqn.source();
    const vectorField& U = UEqn.psi();

    const scalar C0 = C0_.value();

    if (C0 > VSMALL)
    {
        if (compressible)
        {
            addPowerLawResistance
            (
                Udiag,
                cells,
                V,
                mesh_.lookupObject<volScalarField>("rho"),
                U
            );
        }
        else
        {
            addPowerLawResistance
            (
                Udiag,
                cells,
                V,
                OneField<scalar>(),
                U
            );
        }
    }

    const tensor& D = D_.value();
    const tensor& F = F_.value();

    if (magSqr(D) > VSMALL || magSqr(F) > VSMALL)
    {
        if (compressible)
        {
            addViscousInertialResistance
            (
                Udiag,
                Usource,
                cells,
                V,
                mesh_.lookupObject<volScalarField>("rho"),
                mesh_.lookupObject<volScalarField>("mu"),
                U
            );
        }
        else
        {
            addViscousInertialResistance
            (
                Udiag,
                Usource,
                cells,
                V,
                OneField<scalar>(),
                mesh_.lookupObject<volScalarField>("nu"),
                U
            );
        }
    }
}


void Foam::porousZone::addResistance
(
    const fvVectorMatrix& UEqn,
    volTensorField& AU,
    bool correctAUpBC
) const
{
    if (cellZoneID_ == -1)
    {
        return;
    }

    bool compressible = false;
    if (UEqn.dimensions() == dimensionSet(1, 1, -2, 0, 0))
    {
        compressible = true;
    }

    const labelList& cells = mesh_.cellZones()[cellZoneID_];
    const vectorField& U = UEqn.psi();

    const scalar C0 = C0_.value();

    if (C0 > VSMALL)
    {
        if (compressible)
        {
            addPowerLawResistance
            (
                AU,
                cells,
                mesh_.lookupObject<volScalarField>("rho"),
                U
            );
        }
        else
        {
            addPowerLawResistance
            (
                AU,
                cells,
                OneField<scalar>(),
                U
            );
        }
    }

    const tensor& D = D_.value();
    const tensor& F = F_.value();

    if (magSqr(D) > VSMALL || magSqr(F) > VSMALL)
    {
        if (compressible)
        {
            addViscousInertialResistance
            (
                AU,
                cells,
                mesh_.lookupObject<volScalarField>("rho"),
                mesh_.lookupObject<volScalarField>("mu"),
                U
            );
        }
        else
        {
            addViscousInertialResistance
            (
                AU,
                cells,
                OneField<scalar>(),
                mesh_.lookupObject<volScalarField>("nu"),
                U
            );
        }
    }

    if (correctAUpBC)
    {
        // Correct the doundary conditions of the tensorial diagonal to ensure
        // processor bounaries are correctly handled when AU^-1 is interpolated
        // for the pressure equation.
        AU.correctBoundaryConditions();
    }
}


void Foam::porousZone::writeDict(Ostream& os, bool subDict) const
{
    if (subDict)
    {
        os  << indent << token::BEGIN_BLOCK << incrIndent << nl;
        os.writeKeyword("name") << zoneName() << token::END_STATEMENT << nl;
    }
    else
    {
        os  << indent << zoneName() << nl
            << indent << token::BEGIN_BLOCK << incrIndent << nl;
    }

    // could write orientation e1/e2 or axis/direction if needed

    if (dict_.found("powerLaw"))
    {
        dictionary const & subDict = dict_.subDict("powerLaw");

        os << indent << "powerLaw";
        subDict.write(os);
    }

    if (dict_.found("Darcy"))
    {
        dictionary const & subDict = dict_.subDict("Darcy");

        os << indent << "Darcy";
        subDict.write(os);
    }

    os << decrIndent << indent << token::END_BLOCK << endl;
}


Foam::Ostream& Foam::operator<<(Ostream& os, const porousZone& pZone)
{
    pZone.writeDict(os);
    return os;
}


// ************************************************************************* //
