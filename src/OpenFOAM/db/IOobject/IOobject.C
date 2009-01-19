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
    Constructors & destructor for IOobject.

\*---------------------------------------------------------------------------*/

#include "IOobject.H"
#include "Time.H"
#include "IFstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(IOobject, 0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

IOobject::IOobject
(
    const word& name,
    const fileName& instance,
    const objectRegistry& registry,
    readOption ro,
    writeOption wo,
    bool registerObject
)
:
    name_(name),
    headerClassName_(typeName),
    note_(),
    instance_(instance),
    local_(),
    db_(registry),
    rOpt_(ro),
    wOpt_(wo),
    registerObject_(registerObject),
    objState_(GOOD)
{
    if (objectRegistry::debug)
    {
        Info<< "Constructing IOobject called " << name_
            << " of type " << headerClassName_
            << endl;
    }
}


IOobject::IOobject
(
    const word& name,
    const fileName& instance,
    const fileName& local,
    const objectRegistry& registry,
    readOption ro,
    writeOption wo,
    bool registerObject
)
:
    name_(name),
    headerClassName_(typeName),
    note_(),
    instance_(instance),
    local_(local),
    db_(registry),
    rOpt_(ro),
    wOpt_(wo),
    registerObject_(registerObject),
    objState_(GOOD)
{
    if (objectRegistry::debug)
    {
        Info<< "Constructing IOobject called " << name_
            << " of type " << headerClassName_
            << endl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const objectRegistry& IOobject::db() const
{
    return db_;
}


const Time& IOobject::time() const
{
    return db_.time();
}


const fileName& IOobject::caseName() const
{
    return time().caseName();
}


const fileName& IOobject::rootPath() const
{
    return time().rootPath();
}


fileName IOobject::path() const
{
    return rootPath()/caseName()/instance()/db_.dbDir()/local();
}


fileName IOobject::path(const word& instance, const fileName& local) const
{
    return rootPath()/caseName()/instance/db_.dbDir()/local;
}


fileName IOobject::filePath() const
{
    if (file(objectPath()))
    {
        return objectPath();
    }
    else if
    (
        time().processorCase()
     && (
            instance() == time().system()
         || instance() == time().constant()
        )
     && !db_.dbDir().size()
     && file(rootPath()/caseName()/".."/instance()/local()/name())
    )
    {
        return rootPath()/caseName()/".."/instance()/local()/name();
    }
    else if (!dir(path()))
    {
        word newInstancePath = time().findInstancePath(instant(instance()));

        if (newInstancePath.size())
        {
            fileName fName
            (
                rootPath()/caseName()/newInstancePath/db_.dbDir()/local()/name()
            );

            if (file(fName))
            {
                return fName;
            }
        }
    }

    return fileName::null;
}


Istream* IOobject::objectStream()
{
    fileName fName = filePath();

    if (fName != fileName::null)
    {
        IFstream* isPtr = new IFstream(fName);
    
        if (isPtr->good())
        {
            return isPtr;
        }
        else
        {
            delete isPtr;
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}


bool IOobject::headerOk()
{
    bool ok = true;

    Istream* isPtr = objectStream();

    // If the stream has failed return
    if (!isPtr)
    {
        if (objectRegistry::debug)
        {
            Info
                << "IOobject::headerOk() : "
                << "file " << objectPath() << " could not be opened"
                << endl;
        }

        ok = false;
    }
    else
    {
        // Try reading header
        if (!readHeader(*isPtr))
        {
            if (objectRegistry::debug)
            {
                IOWarningIn("IOobject::headerOk()", (*isPtr))
                    << "failed to read header of file " << objectPath()
                    << endl;
            }

            ok = false;
        }
    }

    delete isPtr;

    return ok;
}


// Set the IOobject to bad: say why
void IOobject::setBad(const string& s)
{
    if (objState_ != GOOD)
    {
        FatalErrorIn("IOobject::setBad(const string&)")
            << "recurrent failure for object " << s
            << exit(FatalError);
    }

    if (error::level)
    {
        Info<< "IOobject::setBad(const string&) : "
            << "broken object " << s << info() << endl;
    }

    objState_ = BAD;
}


// Assign to IOobject
void IOobject::operator=(const IOobject& io)
{
    name_ = io.name_;
    headerClassName_ = io.headerClassName_;
    note_ = io.note_;
    instance_ = io.instance_;
    local_ = io.local_;
    rOpt_ = io.rOpt_;
    wOpt_ = io.wOpt_;
    objState_ = io.objState_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
