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

#include "timeVaryingUniformFixedValueFvPatchFields.H"
#include "addToRunTimeSelectionTable.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

makePatchFields(timeVaryingUniformFixedValue);

// Update the coefficients associated with the patch field
template<>
void timeVaryingUniformFixedValueFvPatchField<scalar>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    checkTable();

    operator==
    (
        interpolateXY
        (
            this->db().time().value(),
            timeDataPtr_().x(),
            timeDataPtr_().y()
        )
    );

    fixedValueFvPatchField<scalar>::updateCoeffs();
}


template<>
void timeVaryingUniformFixedValueFvPatchField<vector>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    checkTable();

    operator==
    (
       -interpolateXY
        (
            this->db().time().value(),
            timeDataPtr_().x(),
            timeDataPtr_().y()
        )*patch().nf()
    );

    fixedValueFvPatchField<vector>::updateCoeffs();
}


template<>
void timeVaryingUniformFixedValueFvPatchField<sphericalTensor>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    checkTable();

    operator==
    (
        interpolateXY
        (
            this->db().time().value(),
            timeDataPtr_().x(),
            timeDataPtr_().y()
        )*sphericalTensor::I
    );

    fixedValueFvPatchField<sphericalTensor>::updateCoeffs();
}


template<>
void timeVaryingUniformFixedValueFvPatchField<symmTensor>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    checkTable();

    operator==
    (
        interpolateXY
        (
            this->db().time().value(),
            timeDataPtr_().x(),
            timeDataPtr_().y()
        )*sqr(patch().nf())
    );

    fixedValueFvPatchField<symmTensor>::updateCoeffs();
}


template<>
void timeVaryingUniformFixedValueFvPatchField<tensor>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    checkTable();

    operator==
    (
        interpolateXY
        (
            this->db().time().value(),
            timeDataPtr_().x(),
            timeDataPtr_().y()
        )*(patch().nf()*patch().nf())
    );

    fixedValueFvPatchField<tensor>::updateCoeffs();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
