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
#include "Particle.H"
#include "Time.H"
#include "IOPosition.H"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class ParticleType>
void Foam::Cloud<ParticleType>::initCloud(const bool checkClass)
{
    IOPosition<ParticleType> ioP(*this);

    if (ioP.headerOk())
    {
        ioP.readData(*this, checkClass);
        ioP.close();

        if (this->size())
        {
            readFields();
        }
    }
    else
    {
        WarningIn("Cloud<ParticleType>::initCloud(const bool checkClass)")
            << "Cannot read particle positions file " << nl
            << "    " << ioP.path() << nl
            << "    assuming the initial cloud contains 0 particles." << endl;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParticleType>
Foam::Cloud<ParticleType>::Cloud
(
    const polyMesh& pMesh,
    const bool checkClass
)
:
    cloud(pMesh),
    polyMesh_(pMesh),
    allFaces_(pMesh.faces()),
    points_(pMesh.points()),
    cellFaces_(pMesh.cells()),
    allFaceCentres_(pMesh.faceCentres()),
    owner_(pMesh.faceOwner()),
    neighbour_(pMesh.faceNeighbour()),
    meshInfo_(polyMesh_)
{
    initCloud(checkClass);
}


template<class ParticleType>
Foam::Cloud<ParticleType>::Cloud
(
    const polyMesh& pMesh,
    const word& cloudName,
    const bool checkClass
)
:
    cloud(pMesh, cloudName),
    polyMesh_(pMesh),
    allFaces_(pMesh.faces()),
    points_(pMesh.points()),
    cellFaces_(pMesh.cells()),
    allFaceCentres_(pMesh.faceCentres()),
    owner_(pMesh.faceOwner()),
    neighbour_(pMesh.faceNeighbour()),
    meshInfo_(polyMesh_)
{
    initCloud(checkClass);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ParticleType>
template<class DataType>
void Foam::Cloud<ParticleType>::checkFieldIOobject
(
    const Cloud<ParticleType>& c,
    const IOField<DataType>& data
) const
{
    if (data.size() != c.size())
    {
        FatalErrorIn
        (
            "void Cloud<ParticleType>::checkFieldIOobject"
            "(Cloud<ParticleType>, IOField<DataType>)"
        )   << "Size of " << data.name()
            << " field does not match the number of particles"
            << abort(FatalError);
    }
}


template<class ParticleType>
Foam::IOobject Foam::Cloud<ParticleType>::fieldIOobject
(
    const word& fieldName
) const
{
    return IOobject
    (
        fieldName,
        time().timeName(),
        *this,
        IOobject::MUST_READ,
        IOobject::NO_WRITE,
        false
    );
}


template<class ParticleType>
template<class Type>
Foam::tmp<Foam::IOField<Type> > Foam::Cloud<ParticleType>::readField
(
    const word& fieldName
) const
{
    return tmp<IOField<Type> >(new IOField<Type>(fieldIOobject(fieldName)));
}


template<class ParticleType>
void Foam::Cloud<ParticleType>::readFields()
{}


template<class ParticleType>
void Foam::Cloud<ParticleType>::writeFields() const
{}


template<class ParticleType>
bool Foam::Cloud<ParticleType>::writeObject
(
    IOstream::streamFormat fmt,
    IOstream::versionNumber ver,
    IOstream::compressionType cmp
) const
{
    if (this->size())
    {
        writeFields();
        return cloud::writeObject(fmt, ver, cmp);
    }
    else
    {
        return true;
    }
}


// * * * * * * * * * * * * * * * Ostream Operators * * * * * * * * * * * * * //

template<class ParticleType>
Foam::Ostream& Foam::operator<<(Ostream& os, const Cloud<ParticleType>& pc)
{
    pc.writeData(os);

    // Check state of Ostream
    os.check("Ostream& operator<<(Ostream&, const Cloud<ParticleType>&)");

    return os;
}


// ************************************************************************* //
