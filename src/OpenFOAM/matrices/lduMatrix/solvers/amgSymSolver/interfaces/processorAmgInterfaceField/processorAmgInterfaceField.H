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
    processorAmgInterfaceField

Description
    AMG agglomerated processor interface.

SourceFiles
    processorAmgInterfaceField.C

\*---------------------------------------------------------------------------*/

#ifndef processorAmgInterfaceField_H
#define processorAmgInterfaceField_H

#include "amgInterfaceField.H"
#include "processorLduInterface.H"
#include "processorLduInterfaceField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class processorAmgInterfaceField Declaration
\*---------------------------------------------------------------------------*/

class processorAmgInterfaceField
:
    public amgInterfaceField,
    public processorLduInterface,
    public processorLduInterfaceField
{
    // Private data

        //- Local processor number
        int myProcNo_;

        //- Neighbour processor number
        int neighbProcNo_;

        //- Is the transform required
        bool doTransform_;

        // Face transformation tensor
        tensorField forwardT_;

        //- Rank of component for transformation
        int rank_;

        //- Pointer to addressing
        mutable labelField* addrPtr_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        processorAmgInterfaceField(const processorAmgInterfaceField&);

        //- Disallow default bitwise assignment
        void operator=(const processorAmgInterfaceField&);

        //- Assemble a interface internal field
        template<class Type>
        tmp<Field<Type> > interfaceInternalField(const UList<Type>& iF) const;
    

public:

    //- Runtime type information
    TypeName("processor");

    // Constructors

        //- Construct from fine-level interface
        processorAmgInterfaceField
        (
            const lduInterfaceField* fineInterfacePtr
        );


    // Destructor

        virtual ~processorAmgInterfaceField();


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

            //- Initialise interface data transfer
            virtual void initTransfer
            (
                const unallocLabelList& interfaceData,
                const bool bufferedTransfer
            ) const;

            //- Transfer and return neighbour field
            virtual tmp<labelField> transfer
            (
                const unallocLabelList& interfaceData
            ) const;

            //- Initialise transfer of internal field adjacent to the interface
            virtual void initInternalFieldTransfer
            (
                const unallocLabelList& iF,
                const bool bufferedTransfer
            ) const;

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

            //- Initialise neighbour matrix update
            virtual void initInterfaceMatrixUpdate
            (
                const scalarField& psiInternal,
                scalarField& result,
                const lduMatrix& m,
                const scalarField& coeffs,
                const direction cmpt,
                const bool bufferedTransfer
            ) const;

            //- Update result field based on interface functionality
            virtual void updateInterfaceMatrix
            (
                const scalarField& psiInternal,
                scalarField& result,
                const lduMatrix&,
                const scalarField& coeffs,
                const direction cmpt
            ) const;


        //- Processor interface functions

            //- Return processor number
            virtual int myProcNo() const
            {
                return myProcNo_;
            }

            //- Return neigbour processor number
            virtual int neighbProcNo() const
            {
                return neighbProcNo_;
            }

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
