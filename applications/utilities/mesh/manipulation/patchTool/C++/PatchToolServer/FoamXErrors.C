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

#include "long.H"
#include "string.H"
#include "fileName.H"
#include "dictionary.H"
#include "LogEntry.H"

// Project header files.
#include "PatchTool.H"
#include "FoamXErrors.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(FoamX::FoamXError, 0);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

FoamX::FoamXError::FoamXError
(
    FoamXServer::ErrorCode errCode,
    const Foam::string& message,
    const char* function,
    const char* fName, 
    Foam::label lineNo
)
:
    FoamXServer::FoamXError
    (
        errCode,
        message.c_str(),
        function,
        fName, 
        lineNo
    )
{
    static const char* functionName =
        "FoamX::FoamXError::FoamXError"
        "(FoamXServer::ErrorCode errCode, const Foam::string& message,"
        "const char* functionName, const char* fName, Foam::label lineNo)";

    Foam::LogEntry log(functionName, __FILE__, __LINE__);
    log << *this << Foam::endl;
}

FoamX::FoamXError::FoamXError(const FoamXServer::FoamXError& fErr)
:
    FoamXServer::FoamXError(fErr)
{
    static const char* functionName =
        "FoamX::FoamXError::FoamXError(const FoamXServer::FoamXError& fErr)";

    Foam::LogEntry log(functionName, __FILE__, __LINE__);
    log << *this << Foam::endl;
}

FoamX::FoamXError::FoamXError(const Foam::error& fErr)
:
    FoamXServer::FoamXError
    (
        FoamXServer::E_FOAM,
        fErr.message().c_str(),
        fErr.functionName().c_str(),
        fErr.sourceFileName().c_str(),
        fErr.sourceFileLineNumber()
    )
{
    static const char* functionName =
        "FoamX::FoamXError::FoamXError(const Foam::error& fErr)";

    Foam::LogEntry log(functionName, __FILE__, __LINE__);
    log << *this << Foam::endl;
}


Foam::Ostream& FoamX::operator<<
(
    Foam::Ostream& os,
    const FoamXServer::FoamXError& fxErr
)
{
    os  << "FoamXError " << Foam::string(fxErr.errorMessage) << Foam::nl
        << "In function " << Foam::string(fxErr.methodName) << Foam::nl
        << "in file " << Foam::fileName(fxErr.fileName)
        << " at line " << fxErr.lineNo;

    return os;
}


FoamX::FoamXSYSError::FoamXSYSError
(
    FoamXServer::ErrorCode errCode,
    const Foam::string& message,
    const Foam::string& hostName,
    const char* function,
    const char* fName, 
    Foam::label lineNo
)
:
    FoamXServer::FoamXSYSError
    (
        errCode,
        message.c_str(),
        hostName.c_str(),
        function,
        fName, 
        lineNo
    )
{
    static const char* functionName =
        "FoamX::FoamXError::FoamXSYSError"
        "(FoamXServer::ErrorCode errCode, const Foam::string& messsage, "
        "const Foam::string& hostName, "
        "const char* functionName, const char* fName, Foam::label lineNo)";

    Foam::LogEntry log(functionName, __FILE__, __LINE__);
    log << *this << Foam::endl;
}

FoamX::FoamXSYSError::FoamXSYSError(const FoamXServer::FoamXSYSError& sysErr)
:
    FoamXServer::FoamXSYSError(sysErr)
{
    static const char* functionName =
        "FoamX::FoamXError::FoamXError"
        "(const FoamXServer::FoamXSYSError& sysErr)";

    Foam::LogEntry log(functionName, __FILE__, __LINE__);
    log << *this << Foam::endl;
}

Foam::Ostream& FoamX::operator<<
(
    Foam::Ostream& os,
    const FoamXServer::FoamXSYSError& sysErr
)
{
    os  << "FoamXSYSError " << Foam::string(sysErr.errorMessage) << Foam::nl
        << "Problem with machine " << Foam::string(sysErr.hostName) << Foam::nl
        << "In function " << Foam::string(sysErr.methodName) << Foam::nl
        << "in file " << Foam::fileName(sysErr.fileName)
        << " at line " << sysErr.lineNo;

    return os;
}


FoamX::FoamXIOError::FoamXIOError
(
    const Foam::string& message,
    const Foam::string& ioFileName,
    Foam::label ioStartLineNumber,
    Foam::label ioEndLineNumber,
    const char* function,
    const char* fName, 
    Foam::label lineNo
)
:
    FoamXServer::FoamXIOError
    (
        message.c_str(),
        ioFileName.c_str(),
        ioStartLineNumber,
        ioEndLineNumber,
        function,
        fName, 
        lineNo
    )
{
    static const char* functionName =
        "FoamX::FoamXError::FoamXError"
        "(const Foam::string& message, const Foam::string& ioFileName, "
        "Foam::label ioStartLineNumber, Foam::label ioEndLineNumber, "
        "const char* functionName, const char* fName, Foam::label lineNo)";

    Foam::LogEntry log(functionName, __FILE__, __LINE__);
    log << *this << Foam::endl;
}

FoamX::FoamXIOError::FoamXIOError(const FoamXServer::FoamXIOError& fErr)
:
    FoamXServer::FoamXIOError(fErr)
{
    static const char* functionName =
        "FoamX::FoamXIOError::FoamXIOError"
        "(const FoamXServer::FoamXIOError& fErr)";

    Foam::LogEntry log(functionName, __FILE__, __LINE__);
    log << *this << Foam::endl;
}

FoamX::FoamXIOError::FoamXIOError(const Foam::IOerror& fIOErr)
:
    FoamXServer::FoamXIOError
    (
        fIOErr.message().c_str(),
        fIOErr.ioFileName().c_str(),
        fIOErr.ioStartLineNumber(),
        fIOErr.ioEndLineNumber(),
        fIOErr.functionName().c_str(),
        fIOErr.sourceFileName().c_str(),
        fIOErr.sourceFileLineNumber()
    )
{
    static const char* functionName =
        "FoamX::foamXError(const Foam::IOerror& fIOErr)";

    Foam::LogEntry log(functionName, __FILE__, __LINE__);
    log << *this << Foam::endl;
}


Foam::Ostream& FoamX::operator<<
(
    Foam::Ostream& os,
    const FoamXServer::FoamXIOError& fIOErr
)
{
    os  << "FoamXIOError " << Foam::string(fIOErr.errorMessage) << Foam::nl
        << "File " << Foam::fileName(fIOErr.ioFileName)
        << " starting at line " << fIOErr.ioStartLineNo
        << " ending at line " << fIOErr.ioEndLineNo << Foam::nl
        << "In function " << Foam::string(fIOErr.methodName) << Foam::nl
        << "in file " << Foam::fileName(fIOErr.fileName)
        << " at line " << fIOErr.lineNo;

    return os;
}



Foam::dictionary FoamX::dict(const Foam::error& fErr)
{
    return fErr;
}

Foam::dictionary FoamX::dict(const Foam::IOerror& fIOErr)
{
    return fIOErr;
}

Foam::dictionary FoamX::dict(const FoamXServer::FoamXError& fxErr)
{
    Foam::dictionary fxErrDict;
    fxErrDict.add("type", Foam::word("FoamXServer::FoamXError"));
    fxErrDict.add("errorCode", Foam::label(fxErr.errorCode));
    fxErrDict.add("message", Foam::string(fxErr.errorMessage));
    fxErrDict.add("function", Foam::string(fxErr.methodName));
    fxErrDict.add("sourceFile", Foam::fileName(fxErr.fileName));
    fxErrDict.add("sourceFileLineNumber", fxErr.lineNo);

    return fxErrDict;
}

Foam::dictionary FoamX::dict(const FoamXServer::FoamXIOError& fIOErr)
{
    Foam::dictionary fIOErrDict;
    fIOErrDict.add("type", Foam::word("FoamXServer::FoamXIOError"));
    fIOErrDict.add("message", Foam::string(fIOErr.errorMessage));
    fIOErrDict.add("ioFileName", Foam::fileName(fIOErr.ioFileName));
    fIOErrDict.add("ioStartLineNumber", fIOErr.ioStartLineNo);
    fIOErrDict.add("ioEndLineNumber", fIOErr.ioEndLineNo);
    fIOErrDict.add("function", Foam::string(fIOErr.methodName));
    fIOErrDict.add("sourceFile", Foam::fileName(fIOErr.fileName));
    fIOErrDict.add("sourceFileLineNumber", fIOErr.lineNo);

    return fIOErrDict;
}

Foam::dictionary FoamX::dict(const CORBA::COMM_FAILURE& ex)
{
    Foam::dictionary exDict;
    exDict.add("type", Foam::word("CORBA::COMM_FAILURE"));
    return exDict;
}

Foam::dictionary FoamX::dict(const CORBA::SystemException& ex)
{
    Foam::dictionary exDict;
    exDict.add("type", Foam::word("CORBA::SystemException"));
    return exDict;
}

Foam::dictionary FoamX::dict(const CORBA::Exception& ex)
{
    Foam::dictionary exDict;
    exDict.add("type", Foam::word("CORBA::Exception"));
    return exDict;
}

Foam::dictionary FoamX::dict(const FoamX::systemError& ex)
{
    Foam::dictionary exDict;
    exDict.add("type", Foam::word("::systemError"));
    return exDict;
}


void FoamX::reThrow(const Foam::dictionary& errorDict)
{
    Foam::word errorType(errorDict.lookup("type"));

    if (errorType == "Foam::error")
    {
        throw FoamXError(Foam::error(errorDict));
    }
    else if (errorType == "Foam::IOerror")
    {
        throw FoamXIOError(Foam::IOerror(errorDict));
    }
    else if (errorType == "FoamXServer::FoamXError")
    {
        throw FoamXError
        (
            FoamXServer::ErrorCode
            (
                Foam::readLabel(errorDict.lookup("errorCode"))
            ),
            Foam::string(errorDict.lookup("message")),
            Foam::string(errorDict.lookup("function")).c_str(),
            Foam::string(errorDict.lookup("sourceFile")).c_str(),
            Foam::readLabel(errorDict.lookup("sourceFileLineNumber"))
        );
    }
    else if (errorType == "FoamXServer::FoamXIOError")
    {
        throw FoamXIOError
        (
            Foam::string(errorDict.lookup("message")),
            Foam::string(errorDict.lookup("ioFileName")),
            Foam::readLabel(errorDict.lookup("ioStartLineNumber")),
            Foam::readLabel(errorDict.lookup("ioEndLineNumber")),
            Foam::string(errorDict.lookup("function")).c_str(),
            Foam::string(errorDict.lookup("sourceFile")).c_str(),
            Foam::readLabel(errorDict.lookup("sourceFileLineNumber"))
        );
    }
    else
    {
        throw FoamXError
        (
            FoamXServer::E_UNEXPECTED,
            "Unexpected error: " + errorType,
            "FoamX::reThrow(const Foam::dictionary& errorDict)",
            __FILE__, __LINE__
        );
    }
}


// ************************************************************************* //
