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
    Write primitive and binary block from OPstream

\*---------------------------------------------------------------------------*/

#include "mpi.h"

#include "OPstream.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// Outstanding non-blocking operations.
//! @cond fileScope
Foam::DynamicList<MPI_Request> OPstream_outstandingRequests_;
//! @endcond fileScope

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::OPstream::~OPstream()
{
    if
    (
       !write
        (
            commsType_,
            toProcNo_,
            buf_.begin(),
            bufPosition_
        )
    )
    {
        FatalErrorIn("OPstream::~OPstream()")
            << "MPI_Bsend cannot send outgoing message"
            << Foam::abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::OPstream::write
(
    const commsTypes commsType,
    const int toProcNo,
    const char* buf,
    const std::streamsize bufSize
)
{
    bool transferFailed = true;

    if (commsType == blocking)
    {
        transferFailed = MPI_Bsend
        (
            const_cast<char*>(buf),
            bufSize,
            MPI_PACKED,
            procID(toProcNo),
            msgType(),
            MPI_COMM_WORLD
        );
    }
    else if (commsType == scheduled)
    {
        transferFailed = MPI_Send
        (
            const_cast<char*>(buf),
            bufSize,
            MPI_PACKED,
            procID(toProcNo),
            msgType(),
            MPI_COMM_WORLD
        );
    }
    else if (commsType == nonBlocking)
    {
        MPI_Request request;

        transferFailed = MPI_Isend
        (
            const_cast<char*>(buf),
            bufSize,
            MPI_PACKED,
            procID(toProcNo),
            msgType(),
            MPI_COMM_WORLD,
            &request
        );

        OPstream_outstandingRequests_.append(request);
    }
    else
    {
        FatalErrorIn
        (
            "OPstream::write"
            "(const int fromProcNo, char* buf, std::streamsize bufSize)"
        )   << "Unsupported communications type " << commsType
            << Foam::abort(FatalError);
    }

    return !transferFailed;
}


void Foam::OPstream::waitRequests()
{
    if (OPstream_outstandingRequests_.size() > 0)
    {
        List<MPI_Status> status(OPstream_outstandingRequests_.size());

        if
        (
            MPI_Waitall
            (
                OPstream_outstandingRequests_.size(),
                OPstream_outstandingRequests_.begin(),
                status.begin()
            )
        )
        {
            FatalErrorIn
            (
                "OPstream::waitRequests()"
            )   << "MPI_Waitall returned with error" << Foam::endl;
        }

        OPstream_outstandingRequests_.clear();
    }
}


bool Foam::OPstream::finishedRequest(const label i)
{
    if (i >= OPstream_outstandingRequests_.size())
    {
        FatalErrorIn
        (
            "OPstream::finishedRequest(const label)"
        )   << "There are " << OPstream_outstandingRequests_.size()
            << " outstanding send requests and you are asking for i=" << i
            << nl
            << "Maybe you are mixing blocking/non-blocking comms?"
            << Foam::abort(FatalError);
    }

    int flag;
    MPI_Status status;

    MPI_Test(&OPstream_outstandingRequests_[i], &flag, &status);

    return flag != 0;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// ************************************************************************* //
