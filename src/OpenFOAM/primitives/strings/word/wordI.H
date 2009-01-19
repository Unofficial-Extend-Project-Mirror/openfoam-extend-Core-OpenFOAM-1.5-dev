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

#include <cctype>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline void word::stripInvalid()
{
    if (debug && string::stripInvalid<word>(*this))
    {
        std::cerr
            << "word::stripInvalid() called for string "
            << this->c_str() << std::endl;

        if (debug > 1)
        {
            std::abort();
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline word::word(const word& w)
:
    string(w)
{}


inline word::word()
:
    string()
{}


inline word::word(const string& str)
:
    string(str)
{
    stripInvalid();
}


inline word::word(const std::string& stdStr)
:
    string(stdStr)
{
    stripInvalid();
}


inline word::word(const char* chars)
:
    string(chars)
{
    stripInvalid();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool word::valid(char c)
{
    return
    (
          !isspace(c)
     && c != '"'
     && c != '/'
     && c != ';'
     && c != '{'
     && c != '}'
    );
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void word::operator=(const word& q)
{
    string::operator=(q);
}


inline void word::operator=(const string& q)
{
    string::operator=(q);
    stripInvalid();
}


inline void word::operator=(const std::string& q)
{
    string::operator=(q);
    stripInvalid();
}


inline void word::operator=(const char* q)
{
    string::operator=(q);
    stripInvalid();
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline word operator&(const word& a, const word& b)
{
    if (!b.size())
    {
        return a;
    }
    else
    {
        string ub = b;
        ub.string::operator[](0) = char(toupper(ub.string::operator[](0)));

        return a + ub;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
