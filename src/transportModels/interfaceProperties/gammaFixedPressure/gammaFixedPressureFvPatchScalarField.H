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

Class
    Foam::gammaFixedPressureFvPatchScalarField

Description
    A fixed-pressure gammaContactAngle boundary

SourceFiles
    gammaFixedPressureFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef gammaFixedPressureFvPatchScalarField_H
#define gammaFixedPressureFvPatchScalarField_H

#include "fixedValueFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                 Class gammaFixedPressureFvPatch Declaration
\*---------------------------------------------------------------------------*/

class gammaFixedPressureFvPatchScalarField
:
    public fixedValueFvPatchScalarField
{
    // Private data

        //- Fixed pressure
        scalarField p_;


public:

    //- Runtime type information
    TypeName("gammaFixedPressure");


    // Constructors

        //- Construct from patch and internal field
        gammaFixedPressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        gammaFixedPressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given gammaFixedPressureFvPatchScalarField
        //  onto a new patch
        gammaFixedPressureFvPatchScalarField
        (
            const gammaFixedPressureFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        gammaFixedPressureFvPatchScalarField
        (
            const gammaFixedPressureFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new gammaFixedPressureFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        gammaFixedPressureFvPatchScalarField
        (
            const gammaFixedPressureFvPatchScalarField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new gammaFixedPressureFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Return the gammaFixed pressure
            const scalarField& p() const
            {
                return p_;
            }

            //- Return reference to the gammaFixed pressure to allow adjustment
            scalarField& p()
            {
                return p_;
            }


        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchScalarField&,
                const labelList&
            );


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
