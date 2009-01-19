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

\*---------------------------------------------------------------------------*/

// Foam header files.
#include "dictionary.H"
#include "argList.H"
#include "OSspecific.H"

// FoamX header files.
#include "FoamXErrors.H"
#include "IPatchToolServerImpl.H"
#include "LogManager.H"
#include "LogEntry.H"
#include "Orb.H"

// Declare all the namespaces used by FoamX
#include "FoamXNameSpaces.H"

using namespace FoamX;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char **argv)
{
    static const char* functionName =
        "patchToolServer::main(int argc, char **argv)";

    try
    {
//        FatalError.throwExceptions();
//        FatalIOError.throwExceptions();

        // Construct the ORB, remove ORB arguments
        Orb orb(argc, argv);

        argList args(argc, argv);

        // Initialise the log manager object. 
        // Write the log into the case directory.
        LogManager logMgr(args.path()/"CasePreCaseServerLog.xml");
        //LogManager logMgr("/dev/tty");

        // Register the root function call.
        LogEntry log(functionName, __FILE__, __LINE__);


        // Allocate the PatchToolServerImpl object on the heap.
        // This is a reference counted object and will be deleted by
        // the POA when it is no longer needed.
        IPatchToolServerImpl* casePreServerPtr = new IPatchToolServerImpl
        (
            orb,
            args.rootPath(),
            args.caseName()
        );

        if (casePreServerPtr == NULL)
        {
            FatalErrorIn(functionName)
                << "Failed to instantiate PatchToolServerImpl object."
                << exit(FatalError);
        }

        log << "Created PatchToolServerImpl object." << endl;

        // Get the object IOR
        Foam::string ior = orb.ior(casePreServerPtr->_this());

        log << "IOR : " << ior << "." << endl;

        log << "PatchToolServer running....." << endl;
        Info << "PatchToolServer running....." << endl;

        // Go!
        orb.run();

        log << "Shutting down Case Pre Server." << endl;
    }
    catch (Foam::IOerror& fIOErr)
    {
        Serr<< "Caught Foam IOerror in " << functionName << " : "
            << nl << fIOErr << endl;
    }
    catch (Foam::error& fErr)
    {
        Serr<< "Caught Foam error in " << functionName << " : "
            << nl << fErr << endl;
    }
    catch (FoamXServer::FoamXIOError& fIOErr)
    {
        Serr<< "Caught FoamXIOError exception in " << functionName << " : "
            << nl << fIOErr << endl;
    }
    catch (FoamXServer::FoamXError& ex)
    {
        Serr<< "Caught FoamXError exception in " << functionName << " : "
            << nl << ex << endl;
    }
    catch (CORBA::COMM_FAILURE& ex)
    {
        Serr<< "Caught CORBA::COMM_FAILURE exception in "
            << functionName << endl;
    }
    catch (CORBA::SystemException& ex)
    {
        Serr<< "Caught CORBA::SystemException exception in "
            << functionName << endl;
    }
    catch (CORBA::Exception& ex)
    {
        Serr<< "Caught CORBA::Exception exception in "
            << functionName << endl;
    }
    catch (...)
    {
        Serr<< "Caught system exception in "
            << functionName << endl;
    }

    Info<< "Finishing " << functionName << endl;

    return 0;
}


// ************************************************************************* //
