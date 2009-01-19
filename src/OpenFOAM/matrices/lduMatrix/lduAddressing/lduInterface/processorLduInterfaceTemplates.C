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

#include "processorLduInterface.H"
#include "IPstream.H"
#include "OPstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

template<class Type>
void processorLduInterface::send
(
    const UList<Type>& f,
    const bool bufferedTransfer
) const
{
    OPstream::write
    (
        neighbProcNo(),
        reinterpret_cast<const char*>(f.begin()),
        f.byteSize(),
        bufferedTransfer
    );
}

template<class Type>
void processorLduInterface::receive
(
    UList<Type>& f
) const
{
    IPstream::read
    (
        neighbProcNo(), 
        reinterpret_cast<char*>(f.begin()),
        f.byteSize()
    );
}

template<class Type>
tmp<Field<Type> > processorLduInterface::receive
(
    const label size
) const
{
    tmp<Field<Type> > tf(new Field<Type>(size));
    receive(tf());
    return tf;
}


template<class Type>
void processorLduInterface::compressedSend
(
    const UList<Type>& f,
    const bool bufferedTransfer
) const
{
    if (sizeof(scalar) != sizeof(float) && Pstream::floatTransfer)
    {
        static const label nCmpts = sizeof(Type)/sizeof(scalar);
        label nm1 = (f.size() - 1)*nCmpts;
        label nlast = sizeof(Type)/sizeof(float);
        label nFloats = nm1 + nlast;
        label nBytes = nFloats*sizeof(float);

        const scalar *sArray = reinterpret_cast<const scalar*>(f.begin());
        const scalar *slast = &sArray[nm1];
        float fArray[nFloats];

        for (register label i=0; i<nm1; i++)
        {
            fArray[i] = sArray[i] - slast[i%nCmpts];
        }

        reinterpret_cast<Type&>(fArray[nm1]) = f[f.size() - 1];

        OPstream::write
        (
            neighbProcNo(),
            reinterpret_cast<const char*>(fArray),
            nBytes,
            bufferedTransfer
        );
    }
    else
    {
        this->send(f, bufferedTransfer);
    }
}

template<class Type>
void processorLduInterface::compressedReceive
(
     UList<Type>& f
) const
{
    if (sizeof(scalar) != sizeof(float) && Pstream::floatTransfer)
    {
        static const label nCmpts = sizeof(Type)/sizeof(scalar);
        label nm1 = (f.size() - 1)*nCmpts;
        label nlast = sizeof(Type)/sizeof(float);
        label nFloats = nm1 + nlast;
        label nBytes = nFloats*sizeof(float);

        float fArray[nFloats];

        IPstream::read(neighbProcNo(), reinterpret_cast<char*>(fArray), nBytes);

        f[f.size() - 1] = reinterpret_cast<const Type&>(fArray[nm1]);
        scalar *sArray = reinterpret_cast<scalar*>(f.begin());
        const scalar *slast = &sArray[nm1];

        for (register label i=0; i<nm1; i++)
        {
            sArray[i] = fArray[i] + slast[i%nCmpts];
        }
    }
    else
    {
        this->receive<Type>(f);
    }
}

template<class Type>
tmp<Field<Type> > processorLduInterface::compressedReceive
(
    const label size
) const
{
    tmp<Field<Type> > tf(new Field<Type>(size));
    compressedReceive(tf());
    return tf;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
