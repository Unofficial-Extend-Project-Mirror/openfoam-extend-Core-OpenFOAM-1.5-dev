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
    error

Description
    Class to handle errors and exceptions in a simple, consistent stream-based
    manner.  The error class is globaly instantiated with a title string.
    Errors, messages and other data are piped to the messageStream class in the
    standard manner.   Manipulators are supplied for exit and abort which may
    terminate the program or throw an exception depending of if the exception
     handling has beed switched on (off by default).

Usage
    error << "message1" << "message2" << FoamDataType << exit(errNo);
    error << "message1" << "message2" << FoamDataType << abort();

SourceFiles
    error.C

\*---------------------------------------------------------------------------*/

#ifndef error_H
#define error_H

#include "messageStream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class error Declaration
\*---------------------------------------------------------------------------*/

class error
:
    public messageStream
{

protected:

    // Protected data

        string functionName_;
        string sourceFileName_;
        label sourceFileLineNumber_;

        bool abort_;

        bool throwExceptions_;
        OStringStream* messageStreamPtr_;

public:

    // Constructors

        //- Construct from title string
        error(const string& title);

        //- Construct from dictionary
        error(const dictionary& errDict);

        //- Construct as copy
        error(const error& err);


    // Destructor

        ~error();


    // Member functions

        string message() const;

        const string& functionName() const
        {
            return functionName_;
        }

        const string& sourceFileName() const
        {
            return sourceFileName_;
        }

        label sourceFileLineNumber() const
        {
            return sourceFileLineNumber_;
        }

        void throwExceptions()
        {
            throwExceptions_ = true;
        }

        void dontThrowExceptions()
        {
            throwExceptions_ = false;
        }

        //- Convert to Ostream
        //  Prints basic message and then returns Ostream for further info.
        OSstream& operator()
        (
            const char* functionName,
            const char* sourceFileName,
            const int sourceFileLineNumber = 0
        );

        OSstream& operator()
        (
            const string& functionName,
            const char* sourceFileName,
            const int sourceFileLineNumber = 0
        );

        //- Convert to Ostream
        //  Prints basic message and then returns Ostream for further info.
        operator OSstream&();

        //- Create and return a dictionary
        operator dictionary() const;


        //- Helper function to print a stack
        static void printStack(Ostream& os);

        //- Exit : can be called for any error to exit program. Prints stack
        //  before exiting.
        void exit(const int errNo = 1);

        //- Abort : used to stop code for fatal errors. Prints stack before
        //  exiting.
        void abort();


    // Ostream operator

        friend Ostream& operator<<(Ostream&, const error&);
};


/*---------------------------------------------------------------------------*\
                           Class IOerror Declaration
\*---------------------------------------------------------------------------*/

class IOerror
:
    public error
{
    // Private data

        string ioFileName_;
        label ioStartLineNumber_;
        label ioEndLineNumber_;


public:

    // Constructors

        //- Construct from title string
        IOerror(const string& title);

        //- Construct from dictionary
        IOerror(const dictionary& errDict);


    // Member functions

        const string& ioFileName() const
        {
            return ioFileName_;
        }

        label ioStartLineNumber() const
        {
            return ioStartLineNumber_;
        }

        label ioEndLineNumber() const
        {
            return ioEndLineNumber_;
        }

        //- Convert to Ostream
        //  Prints basic message and then returns Ostream for further info.
        OSstream& operator()
        (
            const char* functionName,
            const char* sourceFileName,
            const int sourceFileLineNumber,
            const string& ioFileName,
            const label ioStartLineNumber = -1,
            const label ioEndLineNumber = -1
        );

        //- Convert to Ostream
        //  Prints basic message and then returns Ostream for further info.
        OSstream& operator()
        (
            const char* functionName,
            const char* sourceFileName,
            const int sourceFileLineNumber,
            const IOstream&
        );

        //- Convert to Ostream
        //  Prints basic message and then returns Ostream for further info.
        OSstream& operator()
        (
            const char* functionName,
            const char* sourceFileName,
            const int sourceFileLineNumber,
            const dictionary&
        );

        //- Create and return a dictionary
        operator dictionary() const;


        //- Exit : can be called for any error to exit program
        void exit(const int errNo = 1);

        //- Abort : used to stop code for fatal errors
        void abort();


    // Ostream operator

        friend Ostream& operator<<(Ostream&, const IOerror&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Global error declarations: defined in error.C

extern error         FatalError;
extern IOerror       FatalIOError;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Convienient macros to add the file name and line number to the function name

#define FatalErrorIn(fn) FatalError(fn, __FILE__, __LINE__)
#define FatalIOErrorIn(fn, ios) FatalIOError(fn, __FILE__, __LINE__, ios)

// Call for functions which are not currently implemented.
// The functionName is printed and then abort is called.
#define notImplemented(fn) \
    FatalErrorIn(fn) << "Not implemented" << Foam::abort(FatalError);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "errorManip.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
