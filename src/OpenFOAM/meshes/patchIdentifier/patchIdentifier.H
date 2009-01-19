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
    patchIdentifier

Description

SourceFiles
    patchIdentifier.C

\*---------------------------------------------------------------------------*/

#ifndef patchIdentifier_H
#define patchIdentifier_H

#include "word.H"
#include "label.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class dictionary;

/*---------------------------------------------------------------------------*\
                           Class patchIdentifier Declaration
\*---------------------------------------------------------------------------*/

class patchIdentifier
{
    // Private data

        //- Name of patch
        word name_;

        //- Index of patch in boundary
        label boundaryIndex_;

        //- Optional physical type
        mutable word physicalType_;


public:

    // Constructors

        //- Construct from components
        patchIdentifier
        (
            const word& name,
            const label index,
            const word& physicalType = word::null
        );

        //- Construct from Istream
        patchIdentifier(Istream&, const label index);

        //- Construct from dictionary
        patchIdentifier
        (
            const word& name,
            const dictionary& dict,
            const label index
        );

        //- Construct from geometric patch, resetting the index
        patchIdentifier
        (
            const patchIdentifier& p,
            const label index
        );


    // Destructor

        virtual ~patchIdentifier();


    // Member Functions

        //- Return name
        const word& name() const
        {
            return name_;
        }

        //- Return the optional physical type of the patch
        const word& physicalType() const
        {
            return physicalType_;
        }

        //- Return the optional physical type of the patch for modification
        word& physicalType()
        {
            return physicalType_;
        }

        //- Return the index of this patch in the boundaryMesh
        label index() const
        {
            return boundaryIndex_;
        }

        //- Return the index of this patch in the boundaryMesh for modification
        label& index()
        {
            return boundaryIndex_;
        }

        //- Write
        void write(Ostream&) const;

        //- Write dictionary
        void writeDict(Ostream&) const;


    // Ostream Operator

        friend Ostream& operator<<(Ostream&, const patchIdentifier&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
