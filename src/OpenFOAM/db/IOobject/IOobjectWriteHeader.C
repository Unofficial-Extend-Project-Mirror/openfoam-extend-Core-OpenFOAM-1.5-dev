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
    Writes the header description of the File to the stream
    associated with the File.

\*---------------------------------------------------------------------------*/

#include "IOobject.H"
#include "token.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

bool IOobject::writeHeader(Ostream& os) const
{
    if (!os.good())
    {
        Info<< "IOobject::writeHeader(Ostream&) : "
            << "no stream open for write"
            << endl << os.info() << endl;

        return false;
    }

    writeBanner(os);
    os  << endl;
    os  << "FoamFile\n{\n";
    os  << "    version " << os.version() << ";\n";
    os  << "    format " << os.format() << ";\n\n";

    os  << "    root " << rootPath() << ";\n";
    os  << "    case " << caseName() << ";\n";
    os  << "    instance " << instance() << ";\n";
    os  << "    local " << local() << ";\n\n";

    os  << "    class " << type() << ";\n";

    if (note().size())
    {
        os << "\n    note " << note() << ";\n\n";
    }

    os  << "    object " << name() << ";\n";
    os  << "}\n\n";
    writeDivider(os);
    os  << endl;

    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
