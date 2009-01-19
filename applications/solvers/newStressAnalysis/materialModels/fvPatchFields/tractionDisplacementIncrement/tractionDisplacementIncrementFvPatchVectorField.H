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

Class
    tractionDisplacementIncrementFvPatchVectorField

Description
    Fixed traction increment boundary condition for the standard 
    linear elastic, fixed coefficient displacement increment 
    equation (stressedFoam).

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

SourceFiles
    tractionDisplacementIncrementFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef tractionDisplacementIncrementFvPatchVectorField_H
#define tractionDisplacementIncrementFvPatchVectorField_H

#include "fvPatchFields.H"
#include "fixedGradientFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
             Class tractionDisplacementIncrementFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class tractionDisplacementIncrementFvPatchVectorField
:
    public fixedGradientFvPatchVectorField
{

    // Private Data

        //- Name of the displacement increment field
        const word DUName_;

        //- Name of rheology model
        const word rheologyName_;

        //- Traction increment
        vectorField DTraction_;

        //- Pressure increment
        scalarField DPressure_;


public:

    //- Runtime type information
    TypeName("tractionDisplacementIncrement");


    // Constructors

        //- Construct from patch and internal field
        tractionDisplacementIncrementFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        tractionDisplacementIncrementFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  tractionDisplacementIncrementFvPatchVectorField onto a new patch
        tractionDisplacementIncrementFvPatchVectorField
        (
            const tractionDisplacementIncrementFvPatchVectorField&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        tractionDisplacementIncrementFvPatchVectorField
        (
            const tractionDisplacementIncrementFvPatchVectorField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchVectorField> clone() const
        {
            return tmp<fvPatchVectorField>
            (
                new tractionDisplacementIncrementFvPatchVectorField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        tractionDisplacementIncrementFvPatchVectorField
        (
            const tractionDisplacementIncrementFvPatchVectorField&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchVectorField> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return tmp<fvPatchVectorField>
            (
                new tractionDisplacementIncrementFvPatchVectorField(*this, iF)
            );
        }


    // Member functions

        // Access

            virtual const vectorField& DTraction() const
            {
                return DTraction_;
            }

            virtual vectorField& DTraction()
            {
                return DTraction_;
            }

            virtual const scalarField& DPressure() const
            {
                return DPressure_;
            }

            virtual  scalarField& DPressure()
            {
                return DPressure_;
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
                const fvPatchVectorField&,
                const labelList&
            );


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
