// Header file shapes2D.H
//
//     this file contains a list of include files for the shapes2D
//     library. This is a set of classes implementing basic 2D drawing
//     objects such as lines, polylines, rectangles, plus derived
//     objects -- arrows, axes -- and manipulators to change colour,
//     linestyle, etc.
//
//     Include anywhere that needs access to any of the routines
//
//     14/4/96     Gavin Tabor

#ifndef SHAPES2D_H
#define SHAPES2D_H

// declare the base types

#include "shape2D.H"
#include "shape2D.H"
#include "shape2D.H"

// First declare the fundamental header files :

#include "point2D.H"
#include "coord2D.H"
#include "line2D.H"
#include "rectangle2D.H"
#include "polygon2D.H"
#include "structuredScalars2D.H"
#include "string2D.H"

// Now derived types

#include "arrow2D.H"
#include "structuredVectors2D.H"

// Finally, manipulators. Start by including the fundamental classes : palettes

#include "colourPalette.H"

// Now 2D classes for manipulating the streams

#include "changecolour.H"

// and finally the standard colour palettes, standard linestyles, etc.

#include "stdColourPalette.H"

#endif

