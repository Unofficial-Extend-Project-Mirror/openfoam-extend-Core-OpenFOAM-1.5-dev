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
    spaceVaryingGammaContactAngleFvPatchScalarField

Description
    Constant contact angle varying along the patch in space

SourceFiles
    spaceVaryingGammaContactAngleFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef spaceVaryingGammaContactAngleFvPatchScalarField_H
#define spaceVaryingGammaContactAngleFvPatchScalarField_H

#include "gammaContactAngleFvPatchScalarField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
            Class spaceVaryingGammaContactAngleFvPatch Declaration
\*---------------------------------------------------------------------------*/

class spaceVaryingGammaContactAngleFvPatchScalarField
:
    public gammaContactAngleFvPatchScalarField
{
    // Private data

        //- Equilibrium contact angle
        scalarField theta0_;


public:

    //- Runtime type information
    TypeName("spaceVaryingGammaContactAngle");


    // Constructors

        //- Construct from patch and internal field
        spaceVaryingGammaContactAngleFvPatchScalarField
        (
            const fvPatch&,
            const scalarField&
        );

        //- Construct from patch, internal field and dictionary
        spaceVaryingGammaContactAngleFvPatchScalarField
        (
            const fvPatch&,
            const scalarField&,
            const dictionary&
        );

        //- Construct by mapping given
        //  spaceVaryingGammaContactAngleFvPatchScalarField
        //  onto a new patch
        spaceVaryingGammaContactAngleFvPatchScalarField
        (
            const spaceVaryingGammaContactAngleFvPatchScalarField&,
            const fvPatch&,
            const scalarField&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        spaceVaryingGammaContactAngleFvPatchScalarField
        (
            const spaceVaryingGammaContactAngleFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new spaceVaryingGammaContactAngleFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        spaceVaryingGammaContactAngleFvPatchScalarField
        (
            const spaceVaryingGammaContactAngleFvPatchScalarField&,
            const scalarField&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone(const scalarField& iF) const
        {
            return tmp<fvPatchScalarField>
            (
                new spaceVaryingGammaContactAngleFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Return contact angle field
            const scalarField& theta0() const
            {
                return theta0_;
            }

            //- Return access to contact angle field
            scalarField& theta0()
            {
                return theta0_;
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


        //- Return the equilibrium contact-angle
        virtual tmp<scalarField> theta
        (
            const fvPatchVectorField& Up,
            const fvPatchVectorField& nHat
        ) const;

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
