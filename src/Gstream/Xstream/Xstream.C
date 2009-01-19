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
    Xstream

Description
    Xstream provides code to implement a simple plotting xwindow
    by declaring it to be an object.

\*---------------------------------------------------------------------------*/

#include "error.H"
#include "Xstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Xstream::openSesame(const colour& forecol, const colour& backcol)
{
    mydisplay = XOpenDisplay("");

    myscreen = DefaultScreen(mydisplay);
    myscreenp = XDefaultScreenOfDisplay(mydisplay);

    myxgcv.foreground = BlackPixel(mydisplay, myscreen);
    myxgcv.background = WhitePixel(mydisplay, myscreen);

    myxgcv.line_style = LineSolid;

    // Load font
    myxgcv.font = XLoadFont(mydisplay, font().c_str());

    myhint.x = 0;
    myhint.y = 0;
    myhint.width = width().x();
    myhint.height = width().y();
    myhint.flags = PPosition | PSize;

    mywindow = XCreateSimpleWindow
    (
        mydisplay,
        DefaultRootWindow(mydisplay),
        myhint.x,
        myhint.y,
        myhint.width,
        myhint.height,
        5,
        myxgcv.foreground,
        myxgcv.background
    );

    int argv(0);
    char *argc("");

    XTextProperty wn;
    char* winNames[2];
    winNames[0] = const_cast<char*>(winName().c_str());
    winNames[1] = NULL;

    XStringListToTextProperty(winNames, 1, &wn);

    XSetWMProperties
    (
        mydisplay,
        mywindow,
        &wn,
        &wn,
        &argc,
        argv,
        &myhint,
        None,
        None
    );

    // set up the colour table

    XColor foreground_color;
    foreground_color.red = int(forecol.r()*65535);
    foreground_color.green = int(forecol.g()*65535);
    foreground_color.blue = int(forecol.b()*65535);

    XColor background_color;
    background_color.red = int(backcol.r()*65535);
    background_color.green = int(backcol.g()*65535);
    background_color.blue = int(backcol.b()*65535);

    if
    (
        XAllocColor
        (
            mydisplay,
            DefaultColormapOfScreen(myscreenp),
            &foreground_color
        )
      &&
        XAllocColor
        (
            mydisplay,
            DefaultColormapOfScreen(myscreenp),
            &background_color
        )
    )
    {
        myxgcv.foreground = foreground_color.pixel;
        myxgcv.background = background_color.pixel;

        mygc = XCreateGC
        (
            mydisplay,
            mywindow,
            GCForeground | GCBackground,
            &myxgcv
        );
    }
    else
    {
        FatalErrorIn
        (
            "Xstream::openSesame(const colour& forecol, const colour& backcol)"
        )   << "Colour doesn't exist"
            << abort(FatalError);
    }


    XSelectInput
    (
        mydisplay,
        mywindow,
        ButtonPressMask | KeyPressMask | ExposureMask | StructureNotifyMask
    );

    /* map window to the screen */

    XMapRaised(mydisplay, mywindow);

    XEvent myevent;

    XNextEvent(mydisplay, &myevent);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Xstream() : create an Xstream with window name *fname, X and Y scales
// given by s, with the centre at (cX,cY) and with width given by
// width (optional parameters)

Xstream::Xstream
(
    const word& winName,
    const colour& fore,
    const colour& back,
    double sX, double sY,
    double cX, double cY,
    int widthX, int widthY
)
:
    Gstream(winName, sX, sY, cX, cY, widthX, widthY)
{
    openSesame(fore, back);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

// destruction operator : clear up afterwards

Xstream::~Xstream()
{
    /* Free up and clean up the window created */

    XFreeGC(mydisplay, mygc);

    XDestroyWindow(mydisplay, mywindow);
    XCloseDisplay(mydisplay);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Hold the screen in place : remap when an Expose event occurs, and
// stop the program execution. When a key is pressed (if the pointer
// is inside the window) continue execution

int Xstream::waitForEvent(GEvent& event)
{
    int notDone(1);
    int leave(1);
    XEvent flag;

    while (notDone)
    {
        XNextEvent(mydisplay,&flag);

        switch (flag.type)
        {

        case Expose:
            if (flag.xexpose.count==0)
            {
                notDone=0;
                leave = 1;
            }
            break;

        case ButtonPress:
            notDone=0;
            leave = 1;
            break;

        case KeyPress:
            notDone=0;
            leave = 1;
            break;

        case ConfigureNotify:
            notDone=0;
            leave = 1;

            Gstream::scale_ = point2D
            (
                double(flag.xconfigure.width)*scale().x()/double(width().x()),
                double(flag.xconfigure.height)*scale().y()/double(width().y())
            );

            Gstream::width_ = coord2D
            (
                flag.xconfigure.width,
                flag.xconfigure.height
            );
            break;

        default:
            notDone=1;
            break;
        }
    }

    event.setEvent(flag);

    return leave;
}


// draw functions : overloaded functions to draw various things

void Xstream::draw(const line2D& l)
{
    XDrawLine
    (
        mydisplay, mywindow, mygc,
        x(l.start()), y(l.start()),
        x(l.end()), y(l.end())
    );

    XFlush(mydisplay);
}


void Xstream::draw(const rectangle2D& r)
{
    XFillRectangle
    (
        mydisplay, mywindow, mygc,
        x(r.first()),
        y(r.second()),
        x(r.second()) - x(r.first()),
        y(r.first()) - y(r.second())

    );

    XFlush(mydisplay);
}


void Xstream::draw(const polygon2D& p)
{
    const List <point2D> & vertex = p.verts();
    int nVertex(p.verts().size());
    List <XPoint> xvertex(nVertex);

    forAll(xvertex, i)
    {
        xvertex[i].x = short(x(vertex[i]));
        xvertex[i].y = short(y(vertex[i]));
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
}


void Xstream::draw(const string2D& s2D)
{
    if (s2D.horizontal())
    {
        XDrawString
        (
            mydisplay,
            mywindow,
            mygc,
            x(s2D.startPt()),
            y(s2D.startPt()),
            s2D.text().c_str(),
            s2D.text().size()
        );
    }
}


void Xstream::draw(const structuredScalars2D&)
{}


// reset foreground and background colours

void Xstream::setColour(const colour& v)
{
    XColor screen_color;
    screen_color.red = int(v.r()*65535);
    screen_color.green = int(v.g()*65535);
    screen_color.blue = int(v.b()*65535);

    if
    (
        XAllocColor
        (
            mydisplay,
            DefaultColormapOfScreen(myscreenp),
            &screen_color
        )
    )
    {
        myxgcv.foreground = screen_color.pixel;
        XChangeGC(mydisplay, mygc, GCForeground, &myxgcv);
    }
}


void Xstream::setLineStyle(const lineStyle& ls)
{
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
            (unsigned int)(thick),
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
            dash_list[j] = int(pat[j]*ls.patternLength());
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
            (unsigned int)(thick),
            LineOnOffDash,
            CapButt,
            JoinMiter
        );
    }
}


void Xstream::clear()
{
    XClearWindow(mydisplay,mywindow);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
