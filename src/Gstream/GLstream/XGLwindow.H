#ifndef XGLwindow_H
#define XGLwindow_H

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

GLenum XGLopenWindow
(
    Display **xDisplay,
    char *title,
    const int winX,
    const int winY,
    const int winWidth,
    const int winHeight
);

void XGLcloseWindow();

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

#endif

// ************************************************************************* //
