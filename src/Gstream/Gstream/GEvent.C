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


\*---------------------------------------------------------------------------*/

#include "GEvent.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct null
GEvent::GEvent()
:
    event_(NONE),
    happening_(False),
    buttonNo_(0),
    keyNo_(0),
    location_(0, 0),
    key_()
{}


// Construct from XEvent
GEvent::GEvent(XEvent& event)
:
    event_(NONE),
    happening_(False),
    buttonNo_(0),
    keyNo_(0),
    location_(0, 0),
    key_()
{
    setEvent(event);
}


// Construct as copy
GEvent::GEvent(const GEvent& event)
:
    event_(event.event_),
    happening_(event.happening_),
    buttonNo_(event.buttonNo_),
    keyNo_(event.keyNo_),
    location_(event.location_),
    key_(event.key_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

GEvent::~GEvent()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void GEvent::setEvent(XEvent& nevent)
{
    char *cc;

    switch (nevent.type)
    {

    case KeyPress:
        happening_ = True;
        event_ = KPRESS;
        keyNo_ = nevent.xkey.keycode;
        location_ = coord2D(nevent.xkey.x, nevent.xkey.y);
        cc = new char[20];
        XComposeStatus compose;
        XLookupString(&nevent.xkey, cc, 20, NULL, &compose);
        key_ = word(cc);
        break;

    case ButtonPress:
        happening_ = True;
        event_ = BPRESS;
        buttonNo_ = nevent.xbutton.button;
        location_ = coord2D(nevent.xbutton.x, nevent.xbutton.y);
        break;

    default:
        happening_ = False;
        break;
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void GEvent::operator=(const GEvent& event)
{
    event_ = event.event_;
    happening_ = event.happening_;
    buttonNo_ = event.buttonNo_;
    keyNo_ = event.keyNo_;
    location_ = event.location_;
    key_ = event.key_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
