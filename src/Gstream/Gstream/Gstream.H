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
    Gstream

Description

SourceFiles
    Gstream.C


\*---------------------------------------------------------------------------*/

#ifndef Gstream_H
#define Gstream_H

#include "coord2D.H"
#include "point2D.H"
#include "string.H"
#include "word.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class colour;
class lineStyle;

class line2D;
class rectangle2D;
class polygon2D;
class string2D;
class structuredScalars2D;

/*---------------------------------------------------------------------------*\
                           Class Gstream Declaration
\*---------------------------------------------------------------------------*/

class Gstream
{
    // Private data

        word winName_;       // Name of window
        string font_;       // font name for text


    // Private Member Functions

        //- Disallow default bitwise copy construct and assignment
        Gstream(const Gstream&);
        void operator=(const Gstream&);


protected:

    // Protected data

        point2D scale_;      // X,Y scaling ratios
        point2D center_;     // ratio of centre position to widths
        coord2D width_;      // absolute width of window in pixels


public:

    // Constructors

        // from components

        Gstream
        (
            const word&,
            const double,
            const double,
            const double = 0.5,
            const double = 0.5,
            const int widthX = 800,
            const int widthY = 800
        );


    // Destructor

        virtual ~Gstream();


    // Member Functions

        // Access

            const word& winName()
            {
                return winName_;
            }

            string font()
            {
                return font_;
            }

            //- X,Y scaling ratios
            const point2D& scale() const
            {
                return scale_;
            }

            //- ratio of centre position to widths
            const point2D& centre() const
            {
                return center_;
            }

            //- absolute width of window in pixels
            const coord2D& width() const
            {
                return width_;
            }


        // Change Context

            virtual void setColour(const colour&) = 0;
            virtual void setLineStyle(const lineStyle&) = 0;
            virtual void clear() = 0;


        // Draw

            virtual void draw(const line2D&) = 0;
            virtual void draw(const rectangle2D&) = 0;
            virtual void draw(const polygon2D&) = 0;
            virtual void draw(const string2D&) = 0;
            virtual void draw(const structuredScalars2D&) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
