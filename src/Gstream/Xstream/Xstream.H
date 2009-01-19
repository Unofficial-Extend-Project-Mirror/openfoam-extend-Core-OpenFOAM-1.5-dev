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

SourceFiles
    Xstream.C
    XstreamIO.C

\*---------------------------------------------------------------------------*/

#ifndef Xstream_H
#define Xstream_H


#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "GEvent.H"
#include "shapes2D.H"
#include "Gstream.H"
#include "lineStyle.H"
#include "List.H"
#include "word.H"
#include "colour.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class Xstream Declaration
\*---------------------------------------------------------------------------*/

class Xstream
:
    public virtual Gstream
{

    // Private data

        Display *mydisplay;
        Window mywindow;
        GC mygc;
        XEvent myevent;
        XSizeHints myhint;
        XGCValues myxgcv;

        int myscreen;
        Screen *myscreenp;

    // Private Member Functions

        //- Disallow default bitwise copy construct and assignment
        Xstream(const Xstream&);
        void operator=(const Xstream&);


        // x() and y() : return the raw Xstream coordinates associated
        // with particular points

        int x(const point2D&);
        int y(const point2D&);

        // and the X and Y values of the point associated with a particular
        // coord

        double x(const coord2D&);
        double y(const coord2D&);

        //- open a window
        void openSesame(const colour&, const colour&);


public:

    // Constructors

        //- Construct from components
        Xstream
        (
            const word& winName,
            const colour&,
            const colour&,
            double,
            double,
            double = 0.5,
            double = 0.5,
            int widthX = 800,
            int widthY = 800
        );


    // Destructor

        virtual ~Xstream();


    // Member Functions

        // Event Handlers

            //- Wait for any keypress then return true
            int waitForEvent();
            int waitForEvent(GEvent &);

        // Change Context

            virtual void setColour(const colour&);
            virtual void setLineStyle(const lineStyle&);
            virtual void clear();

        // Draw

            virtual void draw(const line2D&);
            virtual void draw(const rectangle2D&);
            virtual void draw(const polygon2D&);
            virtual void draw(const string2D&);
            virtual void draw(const structuredScalars2D&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// include inline implementaions

#include "XstreamI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
