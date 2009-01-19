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

Description
    Read token and binary block from IPstream

\*---------------------------------------------------------------------------*/

#include <mpi.h>

#include "IPstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * //

IPstream::IPstream
(
    const int fromProcNo,
    const label bufSize,
    streamFormat format,
    versionNumber version
)
:
    Pstream(bufSize),
    Istream(format, version),
    fromProcNo_(fromProcNo),
    messageSize_(0)
{
    setOpened();
    setGood();

    MPI_Status status;

    // If the buffer size is not specified probe the incomming message
    // and set it
    if (!bufSize)
    {
        MPI_Probe(procID(fromProcNo_), msgType(), MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_BYTE, &messageSize_);

        buf_.setSize(messageSize_);
    }

    messageSize_ = read(fromProcNo_, buf_.begin(), buf_.size());

    if (!messageSize_)
    {
        FatalErrorIn
        (
            "IPstream::IPstream(const int fromProcNo, "
            "const label bufSize, streamFormat format, versionNumber version)"
        )   << "read failed" << Foam::abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

label IPstream::read
(
    const int fromProcNo,
    char* buf,
    const std::streamsize bufSize
)
{
    MPI_Status status;

    if
    (
        MPI_Recv
        (
            buf,
            bufSize,
            MPI_PACKED,
            procID(fromProcNo),
            msgType(),
            MPI_COMM_WORLD,
            &status
        )
    )
    {
        FatalErrorIn
        (
            "IPstream::read"
            "(const int fromProcNo, char* buf, std::streamsize bufSize)"
        )   << "MPI_Recv cannot receive incomming message"
            << Foam::abort(FatalError);

        return 0;
    }


    // Check size of message read

    label messageSize;
    MPI_Get_count(&status, MPI_BYTE, &messageSize);

    if (messageSize > bufSize)
    {
        FatalErrorIn
        (
            "IPstream::read"
            "(const int fromProcNo, char* buf, std::streamsize bufSize)"
        )   << "buffer (" << label(bufSize)
            << ") not large enough for incomming message ("
            << messageSize << ')'
            << Foam::abort(FatalError);
    }

    return messageSize;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
