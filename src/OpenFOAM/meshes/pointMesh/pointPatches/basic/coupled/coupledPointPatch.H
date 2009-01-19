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
    coupledPointPatch

Description
    Coupled point patch.  Used as the base class for processor
    and cyclic pointPatches

SourceFiles
    coupledPointPatch.C

\*---------------------------------------------------------------------------*/

#ifndef coupledPointPatch_H
#define coupledPointPatch_H

#include "coupledPolyPatch.H"
#include "lduInterface.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class pointBoundaryMesh;

/*---------------------------------------------------------------------------*\
                      Class coupledPointPatch Declaration
\*---------------------------------------------------------------------------*/

class coupledPointPatch
:
    public lduInterface
{
    // Private Member Functions

        //- Disallow default construct as copy
        coupledPointPatch(const coupledPointPatch&);

        //- Disallow default assignment
        void operator=(const coupledPointPatch&);


public:

    typedef pointBoundaryMesh BoundaryMesh;


    //- Runtime type information
    TypeName(coupledPolyPatch::typeName_());


    // Constructors

        //- Construct from components
        coupledPointPatch(const pointBoundaryMesh& bm);


    // Destructor

        virtual ~coupledPointPatch();


    // Member Functions

        // Access

            //- Return true because this patch is coupled
            virtual bool coupled() const
            {
                return true;
            }


        // Dummy functionality for new lduInteface base class
        // To be removed.  HJ, 2/Nov/2007

        // Access

            //- Return faceCell addressing
            virtual const unallocLabelList& faceCells() const
            {
                return labelList::null();
            }


        // Interface transfer functions

            //- Return the values of the given internal data adjacent to
            //  the interface as a field
            virtual tmp<labelField> interfaceInternalField
            (
                const unallocLabelList& internalData
            ) const
            {
                return tmp<labelField>(NULL);
            }

            //- Initialise interface data transfer
            virtual void initTransfer
            (
                const unallocLabelList& interfaceData,
                const bool bufferedTransfer
            ) const
            {}

            //- Transfer and return neighbour field
            virtual tmp<labelField> transfer
            (
                const unallocLabelList& interfaceData
            ) const
            {
                return tmp<labelField>(NULL);
            }

            //- Initialise transfer of internal field adjacent to the interface
            virtual void initInternalFieldTransfer
            (
                const unallocLabelList& iF,
                const bool bufferedTransfer
            ) const
            {}

            //- Transfer and return internal field adjacent to the interface
            virtual tmp<labelField> internalFieldTransfer
            (
                const unallocLabelList& iF
            ) const
            {
                return tmp<labelField>(NULL);
            }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
