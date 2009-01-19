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
    cyclicAmgInterfaceField

Description
    AMG agglomerated cyclic interface.

SourceFiles
    cyclicAmgInterfaceField.C

\*---------------------------------------------------------------------------*/

#ifndef cyclicAmgInterfaceField_H
#define cyclicAmgInterfaceField_H

#include "amgInterfaceField.H"
#include "cyclicLduInterface.H"
#include "cyclicLduInterfaceField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class cyclicAmgInterfaceField Declaration
\*---------------------------------------------------------------------------*/

class cyclicAmgInterfaceField
:
    public amgInterfaceField,
    public cyclicLduInterface,
    public cyclicLduInterfaceField
{
    // Private data

        //- Is the transform required
        bool doTransform_;

        // Face transformation tensor
        tensorField forwardT_;

        // Neighbour-cell transformation tensor
        tensorField reverseT_;

        //- Rank of component for transformation
        int rank_;

        //- Pointer to addressing
        mutable labelField* addrPtr_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        cyclicAmgInterfaceField(const cyclicAmgInterfaceField&);

        //- Disallow default bitwise assignment
        void operator=(const cyclicAmgInterfaceField&);

        //- Assemble a interface internal field
        template<class Type>
        tmp<Field<Type> > interfaceInternalField(const UList<Type>& iF) const;
    

public:

    //- Runtime type information
    TypeName("cyclic");

    // Constructors

        //- Construct from fine-level interface
        cyclicAmgInterfaceField
        (
            const lduInterfaceField* fineInterfacePtr
        );


    // Destructor

        virtual ~cyclicAmgInterfaceField();


    // Member Functions

        // Access

            //- Return size
            label size() const;

            //- Return faceCell addressing
            virtual const unallocLabelList& faceCells() const;


        // Interface transfer functions

            //- Return the values of the given internal data adjacent to
            //  the interface as a field
            virtual tmp<labelField> interfaceInternalField
            (
                const unallocLabelList& internalData
            ) const;

            //- Transfer and return neighbour field
            virtual tmp<labelField> transfer(const unallocLabelList&) const;

            //- Transfer and return internal field adjacent to the interface
            virtual tmp<labelField> internalFieldTransfer
            (
                const unallocLabelList& iF
            ) const;

            //- Return coarse level coefficients.
            //  Because of the poor programming this also calculates the
            //  addressing.
            virtual tmp<scalarField> coeffs
            (
                const labelField& localRestrictAddressing,
                const labelField& neighbourRestrictAddressing,
                const scalarField& fineCoeffs
            ) const;


        // Interface matrix update

            //- Update result field based on interface functionality
            virtual void updateInterfaceMatrix
            (
                const scalarField& psiInternal,
                scalarField& result,
                const lduMatrix&,
                const scalarField& coeffs,
                const direction cmpt
            ) const;


        //- Cyclic interface functions

            //- Does the interface field perform the transfromation
            virtual bool doTransform() const
            {
                return doTransform_;
            }

            //- Return face transformation tensor
            virtual const tensorField& forwardT() const
            {
                return forwardT_;
            }

            //- Return neighbour-cell transformation tensor
            virtual const tensorField& reverseT() const
            {
                return reverseT_;
            }

            //- Return rank of component for transform
            virtual int rank() const
            {
                return rank_;
            }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
