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

Class
    messageStream

\*---------------------------------------------------------------------------*/

#include "error.H"
#include "dictionary.H"
#include "Pstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

int messageStream::level(debug::debugSwitch("level", 2));


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Construct from components
messageStream::messageStream
(
    const string& title,
    errorSeverity sev,
    const int maxErrors
)
:
    title_(title),
    severity_(sev),
    maxErrors_(maxErrors),
    errorCount_(0)
{}


//- Construct from dictionary
messageStream::messageStream(const dictionary& dict)
:
    title_(dict.lookup("title")),
    severity_(FATAL),
    maxErrors_(0),
    errorCount_(0)
{}


OSstream& messageStream::operator()
(
    const char* functionName,
    const char* sourceFileName,
    const int sourceFileLineNumber
)
{
    OSstream& os = operator OSstream&();

    os  << endl
        << "    From function " << functionName << endl
        << "    in file " << sourceFileName
        << " at line " << sourceFileLineNumber << endl
        << "    ";

    return os;
}


OSstream& messageStream::operator()
(
    const string& functionName,
    const char* sourceFileName,
    const int sourceFileLineNumber
)
{
    return operator()
    (
        functionName.c_str(),
        sourceFileName,
        sourceFileLineNumber
    );
}


OSstream& messageStream::operator()
(
    const char* functionName,
    const char* sourceFileName,
    const int sourceFileLineNumber,
    const string& ioFileName,
    const label ioStartLineNumber,
    const label ioEndLineNumber
)
{
    OSstream& os = operator OSstream&();

    os  << endl
        << "    From function " << functionName << endl
        << "    in file " << sourceFileName
        << " at line " << sourceFileLineNumber << endl
        << "    Reading " << ioFileName;

    if (ioStartLineNumber >= 0 && ioEndLineNumber >= 0)
    {
        os  << " from line " << ioStartLineNumber
            << " to line " << ioEndLineNumber;
    }
    else if (ioStartLineNumber >= 0)
    {
        os  << " at line " << ioStartLineNumber;
    }

    os << endl  << "    ";

    return os;
}


OSstream& messageStream::operator()
(
    const char* functionName,
    const char* sourceFileName,
    const int sourceFileLineNumber,
    const IOstream& ioStream
)
{
    return operator()
    (
        functionName,
        sourceFileName,
        sourceFileLineNumber,
        ioStream.name(),
        ioStream.lineNumber(),
        -1
    );
}


OSstream& messageStream::operator()
(
    const char* functionName,
    const char* sourceFileName,
    const int sourceFileLineNumber,
    const dictionary& dict
)
{
    return operator()
    (
        functionName,
        sourceFileName,
        sourceFileLineNumber,
        dict.name(),
        dict.startLineNumber(),
        dict.endLineNumber()
    );
}


messageStream::operator OSstream&()
{
    if (level)
    {
        // Report the error
        if (severity_ == INFO && !Pstream::master())
        {
            return Snull;
        }
        else
        {
            if (title().size())
            {
                if (Pstream::parRun() && severity_ != INFO)
                {
                    Pout<< title().c_str();
                }
                else
                {
                    Sout<< title().c_str();
                }
            }
        
            if (maxErrors_)
            {
                errorCount_++;
        
                if (errorCount_ >= maxErrors_)
                {
                    FatalErrorIn("messageStream::operator OSstream&()")
                        << "Too many errors"
                        << abort(FatalError);
                }
            }

            if (Pstream::parRun() && severity_ != INFO)
            {
                return Pout;
            }
            else
            {
                return Sout;
            }
        }
    }

    return Snull;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Global messageStream definitions

messageStream SeriousError 
(
    "--> FOAM Serious Error : ", messageStream::SERIOUS, 100
);

messageStream Warning("--> FOAM Warning : ", messageStream::WARNING);
messageStream Info("", messageStream::INFO);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
