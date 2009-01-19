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
    GLstream

Description

SourceFiles
    Gstream.C
    GstreamIO.C


\*---------------------------------------------------------------------------*/

#ifndef GLstream_H
#define GLstream_H

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "List.H"
#include "Gstream.H"
#include "lineStyle.H"
#include "shapes2D.H"
#include "GEvent.H"
#include "colour.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class GLstream Declaration
\*---------------------------------------------------------------------------*/

class GLstream
:
    public virtual Gstream
{
    // Private data

        Display *mydisplay;


    List<short> dashSty;
    GLuint base;      // something to do with plotting characters on the screen

    // X() and Y() : return the raw GLstream coordinates associated
    // with particular points

    double x(const point2D&);
    double y(const point2D&);

    // and the X and Y values of the point associated with a particular
    // coord

    double x(const coord2D&);
    double y(const coord2D&);

    // convert a point to the appropriate coord

    coord2D P2C(const point2D&);

    // and the other way around

    point2D C2P(const coord2D&);

    //- open a window
    void openSesame(const colour&, const colour&);

    short convertLinestyleToGL(const scalarList&);

    // deal with characters for fonts

    void makeRasterFont();
    void printString(char *s);


public:

    // Constructors

        // Construct from components
        GLstream
        (
            const word&,
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

        ~GLstream();


    // Member Functions

        // Event Handlers

            //- Wait for any keypress then return true
            int waitForEvent();

            //- Wait for any keypress then return true
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


    // Friend Functions

        friend colour structuredScalars2D::newColour(const scalar&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "GLstreamI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
