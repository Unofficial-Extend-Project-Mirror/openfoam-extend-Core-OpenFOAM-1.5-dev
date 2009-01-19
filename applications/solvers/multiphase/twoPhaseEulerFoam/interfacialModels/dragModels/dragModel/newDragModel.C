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

\*---------------------------------------------------------------------------*/

#include "dragModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::dragModel> Foam::dragModel::New
(
    const dictionary& interfaceDict,
    const volScalarField& alpha,
    const phaseModel& phasea,
    const phaseModel& phaseb
)
{
    word dragModelType
    (
        interfaceDict.lookup("dragModel" + phasea.name())
    );

    Info << "Selecting dragModel for phase " 
        << phasea.name()
        << ": "
        << dragModelType << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(dragModelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalError
            << "dragModel::New : " << endl
                << "    unknown dragModelType type "
                << dragModelType
                << ", constructor not in hash table" << endl << endl
                << "    Valid dragModel types are : " << endl;
        Info << dictionaryConstructorTablePtr_->toc() << abort(FatalError);
    }

    return cstrIter()(interfaceDict, alpha, phasea, phaseb);
}


// ************************************************************************* //
