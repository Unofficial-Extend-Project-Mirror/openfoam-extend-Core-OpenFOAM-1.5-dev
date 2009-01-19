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

#include "ReadFields.H"
#include "HashSet.H"
#include "Pstream.H"
#include "IOobjectList.H"

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

// Read all fields of type. Returns names of fields read. Guarantees all
// processors to read fields in same order.
template<class GeoField, class Mesh>
Foam::wordList Foam::ReadFields
(
    const Mesh& mesh,
    const IOobjectList& objects,
    PtrList<GeoField>& fields
)
{
    wordList localNames(objects.names(GeoField::typeName));

    wordList masterNames(localNames);
    Pstream::scatter(masterNames);

    // Check that I have the same fields as the master
    {
        HashSet<word> localNamesSet(localNames);

        forAll(masterNames, i)
        {
            const word& masterFld = masterNames[i];

            HashSet<word>::iterator iter = localNamesSet.find(masterFld);

            if (iter == localNamesSet.end())
            {
                FatalErrorIn
                (
                    "readFields<class GeoField, class Mesh>"
                    "(const Mesh&, const IOobjectList&, PtrList<GeoField>&)"
                )   << "Fields not synchronised across processors." << endl
                    << "Master has field " << masterFld
                    << " which processor " << Pstream::myProcNo()
                    << " does not have." << exit(FatalError);
            }
            else
            {
                localNamesSet.erase(iter);
            }
        }

        forAllConstIter(HashSet<word>, localNamesSet, iter)
        {
            FatalErrorIn
            (
                "readFields<class GeoField, class Mesh>"
                "(const Mesh&, const IOobjectList&, PtrList<GeoField>&)"
            )   << "Fields not synchronised across processors." << endl
                << "Processor " << Pstream::myProcNo()
                << " has field " << iter.key()
                << " which the master processor does not have."
                << exit(FatalError);
        }
    }


    fields.setSize(masterNames.size());

    forAll(masterNames, i)
    {
        const word& fieldName = masterNames[i];

        Info<< "Reading field " << fieldName << endl;

        fields.set
        (
            i,
            new GeoField
            (
                IOobject
                (
                    fieldName,
                    mesh.time().timeName(),
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh
            )
        );
    }
    return masterNames;    
}


// ************************************************************************* //
