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
    GEvent

Description


SourceFiles
    GEvent.C

\*---------------------------------------------------------------------------*/

#ifndef GEvent_H
#define GEvent_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "coord2D.H"
#include "word.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class GEvent Declaration
\*---------------------------------------------------------------------------*/

class GEvent
{

public:

    enum eventType {NONE, BPRESS, KPRESS};


private:

    // Private data

        eventType event_;
        bool happening_;

        unsigned int buttonNo_;
        unsigned int keyNo_;

        coord2D location_;
        word key_;


public:

    // Constructors

        //- Construct null
        GEvent();

        //- Construct from components
        GEvent(XEvent&);

        //- Construct as copy
        GEvent(const GEvent&);


    // Destructor

        ~GEvent();


    // Member Functions

        // Access

            inline bool didItHappen() const;
            inline eventType whatHappened() const;

            inline label whichButton() const;
            inline word whichKey() const;


        // Edit

            void setEvent(XEvent&);    // set the event from an Xevent


    // Member Operators

        void operator=(const GEvent&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline bool GEvent::didItHappen() const
{
    return happening_;
}

inline GEvent::eventType GEvent::whatHappened() const
{
    return event_;
}

inline label GEvent::whichButton() const
{
    return buttonNo_;
}

inline word  GEvent::whichKey() const
{
    return key_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
