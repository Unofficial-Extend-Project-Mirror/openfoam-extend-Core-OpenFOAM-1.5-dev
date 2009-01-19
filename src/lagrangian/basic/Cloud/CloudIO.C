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

#include "Cloud.H"
#include "particle.H"
#include "Time.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class particleType>
Cloud<particleType>::Cloud
(
    const polyMesh& pMesh
)
:
    cloud(pMesh),
    polyMesh_(pMesh),
    allFaces_(pMesh.faces()),
    points_(pMesh.points()),
    cellFaces_(pMesh.cells()),
    allFaceCentres_(pMesh.faceCentres()),
    owner_(pMesh.faceOwner()),
    neighbour_(pMesh.faceNeighbour())
{
    readOpt() = IOobject::MUST_READ;

    if (headerOk())
    {
        Istream& is = readStream(typeName);

        token firstToken(is);

        if (firstToken.isLabel())
        {
            label s = firstToken.labelToken();

            // Read beginning of contents
            is.readBeginList("Cloud<particleType>");

            for (label i=0; i<s; i++)
            {
                append(new particleType(*this, is, false));
            }

            // Read end of contents
            is.readEndList("Cloud<particleType>");
        }
        else if (firstToken.isPunctuation())
        {
            if (firstToken.pToken() != token::BEGIN_LIST)
            {
                FatalIOErrorIn
                (
                    "Cloud<particleType>::Cloud(const IOobject&, "
                    "const polyMesh&)",
                    is
                )   << "incorrect first token, '(', found "
                    << firstToken.info()
                    << exit(FatalIOError);
            }

            token lastToken(is);
            while
            (
                !(
                    lastToken.isPunctuation()
                 && lastToken.pToken() == token::END_LIST
                )
            )
            {
                is.putBack(lastToken);
                append(new particleType(*this, is, false));
                is >> lastToken;
            }
        }
        else
        {
            FatalIOErrorIn
            (
                "Cloud<particleType>::Cloud(const IOobject&, "
                "const polyMesh&)",
                is
            )   << "incorrect first token, expected <int> or '(', found "
                << firstToken.info()
                << exit(FatalIOError);
        }

        // Check state of IOstream
        is.check
        (
            "Cloud<particleType>::Cloud(const IOobject&, const polyMesh&)"
        );

        close();

        if (this->size())
        {
            readFields();
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class particleType>
IOobject Cloud<particleType>::fieldIOobject(const word& fieldName) const
{
    return IOobject
    (
        fieldName,
        polyMesh_.time().timeName(),
        this->lagrangianSubDir,
        polyMesh_,
        IOobject::MUST_READ,
        IOobject::NO_WRITE,
        false
    );
}


template<class particleType>
template<class Type>
tmp<IOField<Type> > Cloud<particleType>::readField(const word& fieldName) const
{
    return tmp<IOField<Type> >(new IOField<Type>(fieldIOobject(fieldName)));
}


template<class particleType>
void Cloud<particleType>::readFields()
{}


template<class particleType>
bool Cloud<particleType>::writeData(Ostream& os) const
{
    os  << this->size() << nl << token::BEGIN_LIST << nl;

    for
    (
        const_iterator iter = this->begin();
        iter != this->end();
        ++iter
    )
    {
        os << static_cast<const particle<particleType>&>(iter()) << nl;
    }

    os  << token::END_LIST << endl;

    if (this->size())
    {
        writeFields();
    }

    return os.good();
}


template<class particleType>
void Cloud<particleType>::writeFields() const
{}


template<class particleType>
bool Cloud<particleType>::writeObject
(
    IOstream::streamFormat fmt,
    IOstream::versionNumber ver,
    IOstream::compressionType cmp
) const
{
    if (this->size())
    {
        return regIOobject::writeObject(fmt, ver, cmp);
    }
    else
    {
        return true;
    }
}


// * * * * * * * * * * * * * * * Ostream Operators * * * * * * * * * * * * * //

template<class particleType>
Ostream& operator<<(Ostream& os, const Cloud<particleType>& pc)
{
    pc.writeData(os);

    // Check state of Ostream
    os.check("Ostream& operator<<(Ostream&, const Cloud<particleType>&)");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
