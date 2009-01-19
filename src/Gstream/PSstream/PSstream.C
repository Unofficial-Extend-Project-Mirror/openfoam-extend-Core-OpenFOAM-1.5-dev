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
    PSstream

Description
    PSstream provides code to implement a simple plotting postscript
    by declaring it to be an object.

\*---------------------------------------------------------------------------*/

#include "PSstream.H"
#include "GEvent.H"
#include "shapes2D.H"
#include "colour.H"
#include "lineStyle.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
// create a PSstream with window name, X and Y scales
// given by s, with the centre at (cX,cY) and with width given by
// width (optional parameters)

PSstream::PSstream
(
    const word& winName,
    const colour&,
    const colour&,
    double sX, double sY,
    double cX, double cY,
    int widthX, int widthY
)
:
    Gstream(winName, sX, sY, cX, cY, widthX, widthY),
    postFileName((winName + ".ps").c_str()),
    postFile(postFileName)
{
    Info<< "writing postscript to file " << postFileName << nl;

    if (!postFile.good())
    {
        FatalErrorIn
        (
            "PSstream::PSstream(const word& winName, const colour&, "
            "const colour&, double sX, double sY, double cX, double cY, "
            "int widthX, int widthY)"
        )   << "cannot open file " << postFileName
            << abort(FatalError);
    }

    // write header to postscript file

    postFile
        << "%!\n"
        << "%!PS-Adobe-2.0 EPSF-1.2\n"
        << "%%Creator: graph\n"
        << "%%graph written by PSstream\n"
        << "%%Date: 2/10/90\n"
        << "%%EndComments\n"
        << "%%BoundingBox: "
        << "0.0 0.0 "
        << scalar(width().x()) << ' '
        << scalar(width().y()) << nl;

     // write rotate plot by 90 degrees
     postFile << "%90 rotate\n";

     // write translation in file, to put graph in lower left corner
     postFile << "%40 -570 translate\n";

     // Set default font to Times
     postFile << "/Times-Roman findfont\n12 scalefont\nsetfont\n";
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

PSstream::~PSstream()
{
    // write the showpage command into the postscript file

    postFile << "showpage\n";

    // write message

    Info<< "closing postscript file " << postFileName << nl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

int PSstream::waitForEvent(GEvent&)
{
    return 0;
}


void PSstream::draw(const line2D& l)
{
    postFile
        << "newpath\n"
        << x(l.start()) << ' ' << y(l.start()) << " moveto\n"
        << x(l.end()) << ' ' << y(l.end()) << " lineto\n"
        << "stroke\n";
}


void PSstream::draw(const rectangle2D&)
{
/*
    XFillRectangle
    (
        mydisplay, mywindow, mygc,
        x(r.first()),
        y(r.second()),
        x(r.second()) - x(r.first()),
        y(r.first()) - y(r.second())

    );

    XFlush(mydisplay);
    */
}


void PSstream::draw(const polygon2D&)
{
/*
    const List <point2D> & vertex = p.verts();
    int nVertex(p.verts().size());
    List <XPoint> xvertex(nVertex);

    forAll(xvertex, i)
    {
        xvertex[i].x = (short) x(vertex[i]);
        xvertex[i].y = (short) y(vertex[i]);
    }

    XFillPolygon
    (
        mydisplay, mywindow, mygc,
        xvertex.begin(),
        nVertex,
        Complex,
        CoordModeOrigin
    );

    XFlush(mydisplay);
    */
}


void PSstream::draw(const string2D& s2D)
{
    postFile
        << x(s2D.startPt()) << ' ' << y(s2D.startPt()) << " moveto\n"
        << '(' << s2D.text().c_str() << ") show\n";
}


void PSstream::draw(const structuredScalars2D&)
{}


// reset foreground colour
void PSstream::setColour(const colour& c)
{
    postFile
        << c.r() << ' ' << c.g() << ' ' << c.b() << " setrgbcolor\n";
}


void PSstream::setLineStyle(const lineStyle&)
{
/*
    // determine thickness
    scalar thick(ls.thickness());

    // dashed style :
    // number of elements in pattern

    int n(ls.pattern().size());

    if (n==1)
    {
        XSetLineAttributes
        (
            mydisplay,
            mygc,
            (unsigned int) thick,
            LineSolid,
            CapButt,
            JoinMiter
        );
    }
    else
    {
        // now, create the dash list
        char * dash_list = new char[n];
        const scalarList& pat = ls.pattern();

        for (label j=0; j<n; j++)
        {
            dash_list[j] = (int)(pat[j]*ls.patternLength());
        }

        XSetDashes
        (
            mydisplay,
            mygc,
            1,
            dash_list,
            n
        );

        XSetLineAttributes
        (
            mydisplay,
            mygc,
            (unsigned int) thick,
            LineOnOffDash,
            CapButt,
            JoinMiter
        );
    }
    */
}


void PSstream::clear()
{
    postFile << "showpage\n";
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
