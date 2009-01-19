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

#include "simpleFilter.H"
#include "addToRunTimeSelectionTable.H"
#include "fvc.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(simpleFilter, 0);
addToRunTimeSelectionTable(LESfilter, simpleFilter, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// from components
simpleFilter::simpleFilter
(
    const fvMesh& mesh
)
:
    LESfilter(mesh)
{}


simpleFilter::simpleFilter(const fvMesh& mesh, const dictionary&)
:
    LESfilter(mesh)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void simpleFilter::read(const dictionary&)
{}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

tmp<volScalarField> simpleFilter::operator()
(
    const tmp<volScalarField>& unFilteredField
) const
{
    tmp<volScalarField> filteredField = fvc::surfaceSum
    (
        mesh().magSf()*fvc::interpolate(unFilteredField)
    )/fvc::surfaceSum(mesh().magSf());

    unFilteredField.clear();

    return filteredField;
}


tmp<volVectorField> simpleFilter::operator()
(
    const tmp<volVectorField>& unFilteredField
) const
{
    tmp<volVectorField> filteredField = fvc::surfaceSum
    (
        mesh().magSf()*fvc::interpolate(unFilteredField)
    )/fvc::surfaceSum(mesh().magSf());

    unFilteredField.clear();

    return filteredField;
}


tmp<volSymmTensorField> simpleFilter::operator()
(
    const tmp<volSymmTensorField>& unFilteredField
) const
{
    tmp<volSymmTensorField> filteredField = fvc::surfaceSum
    (
        mesh().magSf()*fvc::interpolate(unFilteredField)
    )/fvc::surfaceSum(mesh().magSf());

    unFilteredField.clear();

    return filteredField;
}


tmp<volTensorField> simpleFilter::operator()
(
    const tmp<volTensorField>& unFilteredField
) const
{
    tmp<volTensorField> filteredField = fvc::surfaceSum
    (
        mesh().magSf()*fvc::interpolate(unFilteredField)
    )/fvc::surfaceSum(mesh().magSf());

    unFilteredField.clear();

    return filteredField;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
