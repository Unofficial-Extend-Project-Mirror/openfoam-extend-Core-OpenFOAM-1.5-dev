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

#include "probes.H"
#include "volFields.H"
#include "IOmanip.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class T>
class isNotEqOp
{
public:

    void operator()(T& x, const T& y) const
    {
        const T unsetVal(-VGREAT*pTraits<T>::one);

        if (x != unsetVal)
        {
            // Keep x.

            // Note:chould check for y != unsetVal but multiple sample cells
            // already handled in read().
        }
        else
        {
            // x is not set. y might be.
            x = y;
        }
    }
};


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class T>
void probes::findFields(wordList& typeFieldNames, boolList& foundFields)
{
    typeFieldNames.setSize(fieldNames_.size());
    label typeFieldI = 0;

    forAll(fieldNames_, fieldI)
    {
        const word& fldName = fieldNames_[fieldI];

        if (obr_.foundObject<T>(fldName))
        {
            typeFieldNames[typeFieldI++] = fldName;
            foundFields[fieldI] = true;
        }
    }

    typeFieldNames.setSize(typeFieldI);
}


template <class T>
void probes::sampleAndWrite(const word& fieldName)
{
    const GeometricField<T, fvPatchField, volMesh>& fld =
        obr_.lookupObject<GeometricField<T, fvPatchField, volMesh> >
        (
            fieldName
        );

    // Make sure all processors call sample
    Field<T> vals(sample<T>(fieldName));

    if (Pstream::master())
    {
        unsigned int w = IOstream::defaultPrecision() + 7;

        OFstream& probeStream = *probeFilePtrs_[fieldName];

        probeStream << setw(w) << fld.time().value();

        forAll(vals, probeI)
        {
            probeStream << setw(w) << vals[probeI];
        }
        probeStream << nl;
    }
}


template <class T>
void probes::sampleAndWrite(const wordList& typeFields)
{
    forAll(typeFields, i)
    {
        sampleAndWrite<T>(typeFields[i]);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <class T>
Field<T> probes::sample(const word& fieldName) const
{
    const GeometricField<T, fvPatchField, volMesh>& fld =
        obr_.lookupObject<GeometricField<T, fvPatchField, volMesh> >
        (
            fieldName
        );

    const T unsetVal(-VGREAT*pTraits<T>::one);

    Field<T> vals(probeLocations_.size(), unsetVal);

    forAll(probeLocations_, probeI)
    {
        if (cellList_[probeI] >= 0)
        {
            vals[probeI] = fld[cellList_[probeI]];
        }
    }

    Pstream::listCombineGather(vals, isNotEqOp<T>());
    Pstream::listCombineScatter(vals);

    return vals;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
