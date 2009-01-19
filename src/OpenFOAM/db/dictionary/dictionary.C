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

#include "dictionary.H"
#include "primitiveEntry.H"
#include "dictionaryEntry.H"

/* * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * */

defineTypeNameAndDebug(Foam::dictionary, 0);

//- Null dictionary
const Foam::dictionary Foam::dictionary::null;


// * * * * * * * * * * * * * Private member functions  * * * * * * * * * * * //

void Foam::dictionary::add(entry* ePtr)
{
    if (!hashedEntries_.found(ePtr->keyword()))
    {
        ePtr->name() = name_ + "::" + ePtr->keyword();
        append(ePtr);
        hashedEntries_.insert(ePtr->keyword(), ePtr);
    }
    else
    {
        IOWarningIn("dictionary::add(entry* ePtr)", (*this))
            << "attempt to add an entry already in dictionary " << name()
            << endl;

        delete ePtr;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::dictionary::dictionary()
{}


Foam::dictionary::dictionary(const dictionary& dict)
:
    IDLList<entry>(dict),
    name_(dict.name())
{
    for
    (
        IDLList<entry>::iterator iter = begin();
        iter != end();
        ++iter
    )
    {
        hashedEntries_.insert((*iter).keyword(), &(*iter));
    }
}


Foam::autoPtr<Foam::dictionary> Foam::dictionary::clone() const
{
    return autoPtr<dictionary>(new dictionary(*this));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::dictionary::~dictionary()
{
    //cerr<< "~dictionary() " << name() << " " << long(this) << std::endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
    
// Return line number of first token in dictionary
Foam::label Foam::dictionary::startLineNumber() const
{
    if (size())
    {
        return first()->startLineNumber();
    }
    else
    {
        return -1;
    }
}


// Return line number of last token in dictionary
Foam::label Foam::dictionary::endLineNumber() const
{
    if (size())
    {
        return last()->endLineNumber();
    }
    else
    {
        return -1;
    }
}


// Clear the Dictionary
void Foam::dictionary::clear()
{
    IDLList<entry>::clear();
    hashedEntries_.clear();
}


// Find and return entry
bool Foam::dictionary::found(const word& keyword) const
{
    return hashedEntries_.found(keyword);
}


// Find and return an entry
const Foam::entry& Foam::dictionary::lookupEntry(const word& keyword) const
{
    HashTable<entry*>::const_iterator iter = hashedEntries_.find(keyword);

    if (iter == hashedEntries_.end())
    {
        // If keyword not found print error message ...
        FatalIOErrorIn
        (
            "dictionary::lookupEntry(const word& keyword) const",
            *this
        )   << " keyword " << keyword << " is undefined in dictionary "
            << name()
            << exit(FatalIOError);
    }

    return *(*iter);
}


// Find and return an entry
Foam::ITstream& Foam::dictionary::lookup(const word& keyword) const
{
    return lookupEntry(keyword).stream();
}


// Check if entry is a sub-dictionary
bool Foam::dictionary::isDict(const word& keyword) const
{
    return lookupEntry(keyword).isDict();
}


// Find and return a sub-dictionary
const Foam::dictionary& Foam::dictionary::subDict(const word& keyword) const
{
    return lookupEntry(keyword).dict();
}


// Return the table of contents
Foam::wordList Foam::dictionary::toc() const
{
    wordList keywords(size());

    label i = 0;
    for
    (
        IDLList<entry>::const_iterator iter = begin();
        iter != end();
        ++iter
    )
    {
        keywords[i++] = iter().keyword();
    }

    return keywords;
}


// Add an entry
void Foam::dictionary::add(const entry& e)
{
    add(e.clone().ptr());
}


// Add a token entry
void Foam::dictionary::add(const word& keyword, const token& t)
{
    add(new primitiveEntry(keyword, t));
}

// Add a word entry
void Foam::dictionary::add(const word& keyword, const word& w)
{
    add(new primitiveEntry(keyword, token(w)));
}

// Add a string entry
void Foam::dictionary::add(const word& keyword, const Foam::string& s)
{
    add(new primitiveEntry(keyword, token(s)));
}

// Add a label entry
void Foam::dictionary::add(const word& keyword, const label l)
{
    add(new primitiveEntry(keyword, token(l)));
}

// Add a word entry
void Foam::dictionary::add(const word& keyword, const scalar s)
{
    add(new primitiveEntry(keyword, token(s)));
}

// Add an entry constructed from ITstream
void Foam::dictionary::add(const word& keyword, const ITstream& tokens)
{
    add(new primitiveEntry(keyword, tokens));
}

// Add an entry constructed from tokenList
void Foam::dictionary::add(const word& keyword, const tokenList& tokens)
{
    add(new primitiveEntry(keyword, tokens));
}

// Add a dictionary entry
void Foam::dictionary::add(const word& keyword, const dictionary& dict)
{
    add(new dictionaryEntry(keyword, dict));
}


bool Foam::dictionary::remove(const word& Keyword)
{
    HashTable<entry*>::iterator iter = hashedEntries_.find(Keyword);

    if (iter != hashedEntries_.end())
    {
        IDLList<entry>::remove(iter());
        delete iter();
        hashedEntries_.erase(iter);

        return true;
    }
    else
    {
        return false;
    }
}


// Change the keyword for an entry,
// optionally forcing overwrite of an existing entry
bool Foam::dictionary::changeKeyword
(
    const word& oldKeyword,
    const word& newKeyword,
    bool forceOverwrite
)
{
    // no change
    if (oldKeyword == newKeyword)
    {
        return false;
    }

    HashTable<entry*>::iterator iter = hashedEntries_.find(oldKeyword);

    // oldKeyword not found - do nothing
    if (iter == hashedEntries_.end())
    {
        return false;
    }

    HashTable<entry*>::iterator iter2 = hashedEntries_.find(newKeyword);

    // newKeyword already exists
    if (iter2 != hashedEntries_.end())
    {
        if (forceOverwrite)
        {
            IDLList<entry>::remove(iter2());
            delete iter2();
            hashedEntries_.erase(iter2);
        }
        else
        {
            // could issue warning if desired
            return false;
        }
    }

    // change name and HashTable, but leave DL-List untouched
    iter()->keyword() = newKeyword;
    iter()->name() = name_ + "::" + newKeyword;
    hashedEntries_.erase(oldKeyword);
    hashedEntries_.insert(newKeyword, iter());

    return true;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

Foam::ITstream& Foam::dictionary::operator[](const word& keyword) const
{
    return lookup(keyword);
}


void Foam::dictionary::operator=(const dictionary& dict)
{
    // Check for assignment to self
    if (this == &dict)
    {
        FatalErrorIn("dictionary::operator=(const dictionary&)")
            << "attempted assignment to self for dictionary " << name()
            << abort(FatalError);
    }

    IDLList<entry>::operator=(dict);
    name_ = dict.name();

    hashedEntries_.clear();

    for
    (
        IDLList<entry>::iterator iter = begin();
        iter != end();
        ++iter
    )
    {
        hashedEntries_.insert((*iter).keyword(), &(*iter));
    }
}


void Foam::dictionary::operator+=(const dictionary& dict)
{
    // Check for assignment to self
    if (this == &dict)
    {
        FatalErrorIn("dictionary::operator+=(const dictionary&)")
            << "attempted addition assignment to self for dictionary " << name()
            << abort(FatalError);
    }

    for
    (
        IDLList<entry>::const_iterator iter = dict.begin();
        iter != dict.end();
        ++iter
    )
    {
        add((*iter).clone()());
    }
}


void Foam::dictionary::operator|=(const dictionary& dict)
{
    // Check for assignment to self
    if (this == &dict)
    {
        FatalErrorIn("dictionary::operator|=(const dictionary&)")
            << "attempted assignment to self for dictionary " << name()
            << abort(FatalError);
    }

    for
    (
        IDLList<entry>::const_iterator iter = dict.begin();
        iter != dict.end();
        ++iter
    )
    {
        if (!found(iter().keyword()))
        {
            add((*iter).clone()());
        }
    }
}


void Foam::dictionary::operator<<=(const dictionary& dict)
{
    // Check for assignment to self
    if (this == &dict)
    {
        FatalErrorIn("dictionary::operator<<=(const dictionary&)")
            << "attempted assignment to self for dictionary " << name()
            << abort(FatalError);
    }

    for
    (
        IDLList<entry>::const_iterator iter = dict.begin();
        iter != dict.end();
        ++iter
    )
    {
        remove(iter().keyword());
        add((*iter).clone()());
    }
}


/* * * * * * * * * * * * * * * * Global operators  * * * * * * * * * * * * * */

Foam::dictionary Foam::operator+
(
    const dictionary& dict1,
    const dictionary& dict2
)
{
    dictionary sum(dict1);
    sum += dict2;
    return sum;
}


Foam::dictionary Foam::operator|
(
    const dictionary& dict1,
    const dictionary& dict2
)
{
    dictionary sum(dict1);
    sum |= dict2;
    return sum;
}


// ************************************************************************* //
