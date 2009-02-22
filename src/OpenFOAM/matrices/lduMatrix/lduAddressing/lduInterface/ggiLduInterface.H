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
    ggiLduInterface

Description
    An abstract base class for ggi coupled interfaces.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

SourceFiles
    ggiLduInterface.C

\*---------------------------------------------------------------------------*/

#ifndef ggiLduInterface_H
#define ggiLduInterface_H

#include "lduInterface.H"
#include "primitiveFieldsFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                       Class ggiLduInterface Declaration
\*---------------------------------------------------------------------------*/

class ggiLduInterface
{

public:

    //- Runtime type information
    TypeName("ggiLduInterface");

    // Constructors

    // Destructor

        virtual ~ggiLduInterface();


    // Member Functions

        //- Is this the master side?
        virtual bool master() const = 0;

        //- Is this the fine level?
        virtual bool fineLevel() const = 0;

        //- Return shadow patch index
        virtual label shadowIndex() const = 0;

        //- Return fine level addressing
        virtual const labelListList& addressing() const = 0;

        //- Return weights
        virtual const scalarListList& weights() const = 0;

        //- Return face transformation tensor
        virtual const tensorField& forwardT() const = 0;

        //- Return face reverse transformation tensor
        virtual const tensorField& reverseT() const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
