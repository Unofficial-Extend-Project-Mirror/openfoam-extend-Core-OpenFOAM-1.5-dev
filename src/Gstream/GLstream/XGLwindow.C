#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "XGLwindow.H"
#include "tk.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define AUX_SINGLE 0
#define AUX_RGB    0

int xScreen = 0;
Window wRoot = 0;

Atom deleteWindowAtom;

typedef struct _WINDOW_REC
{
    int x, y, w, h;
    GLenum type;
    Display *xDisplay;
    Window wMain;
    XVisualInfo *vInfoMain;
    Colormap cMapMain;
    GLXContext cMain;
} WINDOW_REC;


WINDOW_REC w = 
{
    0, 0, 300, 300, GLenum(TK_RGB|TK_SINGLE|TK_DIRECT), 0, 0, 0, 0, 0
};

GLenum drawAllowFlag;

float colorMaps[] = 
{
    0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 1.000000,
    0.000000, 1.000000, 0.333333, 0.776471, 0.443137, 0.556863,
    0.443137, 0.556863, 0.219608, 0.666667, 0.666667, 0.333333,
    0.666667, 0.333333, 0.666667, 0.333333, 0.666667, 0.333333,
    0.666667, 0.333333, 0.666667, 0.333333, 0.666667, 0.333333,
    0.666667, 0.333333, 0.039216, 0.078431, 0.117647, 0.156863,
    0.200000, 0.239216, 0.278431, 0.317647, 0.356863, 0.400000,
    0.439216, 0.478431, 0.517647, 0.556863, 0.600000, 0.639216,
    0.678431, 0.717647, 0.756863, 0.800000, 0.839216, 0.878431,
    0.917647, 0.956863, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 1.000000, 0.000000, 0.000000, 1.000000, 1.000000,
    0.333333, 0.443137, 0.776471, 0.556863, 0.443137, 0.219608,
    0.556863, 0.666667, 0.666667, 0.333333, 0.666667, 0.333333,
    0.666667, 0.333333, 0.666667, 0.333333, 0.666667, 0.333333,
    0.666667, 0.333333, 0.666667, 0.333333, 0.666667, 0.333333,
    0.039216, 0.078431, 0.117647, 0.156863, 0.200000, 0.239216,
    0.278431, 0.317647, 0.356863, 0.400000, 0.439216, 0.478431,
    0.517647, 0.556863, 0.600000, 0.639216, 0.678431, 0.717647,
    0.756863, 0.800000, 0.839216, 0.878431, 0.917647, 0.956863,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 0.333333, 0.443137,
    0.443137, 0.219608, 0.776471, 0.556863, 0.556863, 0.666667,
    0.666667, 0.333333, 0.666667, 0.333333, 0.666667, 0.333333,
    0.666667, 0.333333, 0.666667, 0.333333, 0.666667, 0.333333,
    0.666667, 0.333333, 0.666667, 0.333333, 0.039216, 0.078431,
    0.117647, 0.156863, 0.200000, 0.239216, 0.278431, 0.317647,
    0.356863, 0.400000, 0.439216, 0.478431, 0.517647, 0.556863,
    0.600000, 0.639216, 0.678431, 0.717647, 0.756863, 0.800000,
    0.839216, 0.878431, 0.917647, 0.956863, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
};


static int ErrorHandler(Display *xDisplay, XErrorEvent *event)
{
    char buf[80];

    printf("\nReceived X error!\n");
    printf("\tError code   : %d\n", event->error_code);
    printf("\tRequest code : %d\n", event->request_code);
    printf("\tMinor code   : %d\n\n", event->minor_code);
    XGetErrorText(xDisplay, event->error_code, buf, 80);
    printf("\tError text : '%s'\n\n", buf);
    return 0;
}


static int WaitForMainWindow(Display*, XEvent *e, char*)
{
    if (e->type == MapNotify && e->xmap.window == w.wMain)
    {
        return GL_TRUE;
    }
    else
    {
        return GL_FALSE;
    }
}


static XVisualInfo *FindMainVisual(GLenum type)
{
    int list[32], i;

    i = 0;

    list[i++] = GLX_LEVEL;
    list[i++] = 0;

    if (TK_IS_DOUBLE(type))
    {
        list[i++] = GLX_DOUBLEBUFFER;
    }

    if (TK_IS_RGB(type))
    {
        list[i++] = GLX_RGBA;
        list[i++] = GLX_RED_SIZE;
        list[i++] = 1;
        list[i++] = GLX_GREEN_SIZE;
        list[i++] = 1;
        list[i++] = GLX_BLUE_SIZE;
        list[i++] = 1;

        if (TK_HAS_ALPHA(type))
        {
            list[i++] = GLX_ALPHA_SIZE;
            list[i++] = 1;
        }

        if (TK_HAS_ACCUM(type))
        {
            list[i++] = GLX_ACCUM_RED_SIZE;
            list[i++] = 1;
            list[i++] = GLX_ACCUM_GREEN_SIZE;
            list[i++] = 1;
            list[i++] = GLX_ACCUM_BLUE_SIZE;
            list[i++] = 1;

            if (TK_HAS_ALPHA(type))
            {
                list[i++] = GLX_ACCUM_ALPHA_SIZE;
                list[i++] = 1;
            }
        }
    }
    else if (TK_IS_INDEX(type))
    {
        list[i++] = GLX_BUFFER_SIZE;
        list[i++] = 1;
    }

    if (TK_HAS_DEPTH(type))
    {
        list[i++] = GLX_DEPTH_SIZE;
        list[i++] = 1;
    }

    if (TK_HAS_STENCIL(type))
    {
        list[i++] = GLX_STENCIL_SIZE;
        list[i++] = 1;
    }

    list[i] = int(None);

    return glXChooseVisual(w.xDisplay, xScreen, list);
}


static GLenum GetMainWindowType(XVisualInfo *vi)
{
    int x, y, z;
    int mask = 0;

    glXGetConfig(w.xDisplay, vi, GLX_DOUBLEBUFFER, &x);

    if (x)
    {
        mask |= TK_DOUBLE;
    }
    else
    {
        mask |= TK_SINGLE;
    }

    glXGetConfig(w.xDisplay, vi, GLX_RGBA, &x);

    if (x)
    {
        mask |= TK_RGB;
        glXGetConfig(w.xDisplay, vi, GLX_ALPHA_SIZE, &x);

        if (x > 0)
        {
            mask |= TK_ALPHA;
        }

        glXGetConfig(w.xDisplay, vi, GLX_ACCUM_RED_SIZE, &x);
        glXGetConfig(w.xDisplay, vi, GLX_ACCUM_GREEN_SIZE, &y);
        glXGetConfig(w.xDisplay, vi, GLX_ACCUM_BLUE_SIZE, &z);

        if (x > 0 && y > 0 && z > 0)
        {
            mask |= TK_ACCUM;
        }
    }
    else
    {
        mask |= TK_INDEX;
    }

    glXGetConfig(w.xDisplay, vi, GLX_DEPTH_SIZE, &x);

    if (x > 0)
    {
        mask |= TK_DEPTH;
    }

    glXGetConfig(w.xDisplay, vi, GLX_STENCIL_SIZE, &x);

    if (x > 0)
    {
        mask |= TK_STENCIL;
    }

    if (glXIsDirect(w.xDisplay, w.cMain))
    {
        mask |= TK_DIRECT;
    }
    else
    {
        mask |= TK_INDIRECT;
    }

    return GLenum(mask);
}


void tkSetRGBMap(int size, float *rgb)
{
    XColor c;
    int rShift, gShift, bShift, max, i;

    switch (w.vInfoMain->c_class)
    {
      case DirectColor:
          max = (size > w.vInfoMain->colormap_size)
              ? w.vInfoMain->colormap_size
              : size;

          for (i = 0; i < max; i++)
          {
              rShift = ffs(uint(w.vInfoMain->red_mask)) - 1;
              gShift = ffs(uint(w.vInfoMain->green_mask)) - 1;
              bShift = ffs(uint(w.vInfoMain->blue_mask)) - 1;
              c.pixel = ((i << rShift) & w.vInfoMain->red_mask) |
                  ((i << gShift) & w.vInfoMain->green_mask) |
                  ((i << bShift) & w.vInfoMain->blue_mask);
              c.red = ushort(rgb[i] * 65535.0 + 0.5);
              c.green = ushort(rgb[size+i] * 65535.0 + 0.5);
              c.blue = ushort(rgb[size*2+i] * 65535.0 + 0.5);
              c.flags = DoRed | DoGreen | DoBlue;
              XStoreColor(w.xDisplay, w.cMapMain, &c);
          }
          break;
        case GrayScale:
        case PseudoColor:
            max = (size > w.vInfoMain->colormap_size)
                ? w.vInfoMain->colormap_size
                : size;

            for (i = 0; i < max; i++)
            {
                c.pixel = i;
                c.red = ushort(rgb[i] * 65535.0 + 0.5);
                c.green = ushort(rgb[size+i] * 65535.0 + 0.5);
                c.blue = ushort(rgb[size*2+i] * 65535.0 + 0.5);
                c.flags = DoRed | DoGreen | DoBlue;
                XStoreColor(w.xDisplay, w.cMapMain, &c);
            }
            break;
    }

    XSync(w.xDisplay, 0);
}



GLenum XGLopenWindow
(
    Display **xDisplayPtr,
    char *title,
    const int winX,
    const int winY,
    const int winWidth,
    const int winHeight
)
{
    w.type = GLenum(AUX_SINGLE | AUX_RGB);
    w.x = winX;
    w.y = winY;
    w.w = winWidth;
    w.h = winHeight;

    XSetWindowAttributes wa;
    XTextProperty tp;
    XSizeHints sh;
    XEvent e;
    int erb, evb;

    Display* xDisplay = XOpenDisplay(0);
    w.xDisplay = xDisplay;
    *xDisplayPtr = xDisplay;

    if (!xDisplay)
    {
        fprintf(stderr, "Can't connect to xDisplay!\n");
        return GL_FALSE;
    }

    if (!glXQueryExtension(xDisplay, &erb, &evb))
    {
        fprintf(stderr, "No glx extension!\n");
        return GL_FALSE;
    }
    xScreen = DefaultScreen(xDisplay);
    wRoot = RootWindow(xDisplay, xScreen);
    XSetErrorHandler(ErrorHandler);

    w.type &= ~TK_OVERLAY;

    w.vInfoMain = FindMainVisual(w.type);

    if (!w.vInfoMain)
    {
        fprintf(stderr, "Window type not found!\n");
        return GL_FALSE;
    }

    w.cMain = glXCreateContext
    (
        xDisplay,
        w.vInfoMain,
        None,
        (TK_IS_DIRECT(w.type))?GL_TRUE:GL_FALSE
    );

    if (!w.cMain)
    {
        fprintf(stderr, "Can't create a context!\n");
        return GL_FALSE;
    }

    w.type = GetMainWindowType(w.vInfoMain);

    if (TK_IS_INDEX(w.type))
    {
        if (w.vInfoMain->c_class != StaticColor &&
        w.vInfoMain->c_class != StaticGray)
        {
            w.cMapMain = XCreateColormap(xDisplay, wRoot, w.vInfoMain->visual,
            AllocAll);
        }
        else
        {
            w.cMapMain = XCreateColormap(xDisplay, wRoot, w.vInfoMain->visual,
            AllocNone);
        }
    }
    else
    {
        w.cMapMain = XCreateColormap(xDisplay, wRoot, w.vInfoMain->visual,
        AllocNone);
    }

    tkSetRGBMap(256, colorMaps);
    wa.colormap = w.cMapMain;
    wa.background_pixmap = None;
    wa.border_pixel = 0;
    wa.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask |
            ButtonPressMask | ButtonReleaseMask | PointerMotionMask;

    w.wMain = XCreateWindow
    (
        xDisplay,
        wRoot,
        w.x, w.y, w.w, w.h, 0,
        w.vInfoMain->depth,
        InputOutput,
        w.vInfoMain->visual,
        CWBackPixmap|CWBorderPixel|CWEventMask|CWColormap,
        &wa
    );

    XStringListToTextProperty(&title, 1, &tp);
    sh.flags = USPosition | USSize;
    XSetWMProperties(xDisplay, w.wMain, &tp, &tp, 0, 0, &sh, 0, 0);
    deleteWindowAtom = XInternAtom(xDisplay, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(xDisplay, w.wMain, &deleteWindowAtom, 1);
    XMapWindow(xDisplay, w.wMain);
    drawAllowFlag = GL_FALSE;
    XIfEvent(xDisplay, &e, WaitForMainWindow, 0);

    if (!glXMakeCurrent(xDisplay, w.wMain, w.cMain))
    {
        fprintf(stderr, "Can't make window current drawable!\n");
        return GL_FALSE;
    }
    XFlush(xDisplay);

    return GL_TRUE;
}


void XGLcloseWindow()
{
    glFlush();
    glFinish();
    XDestroyWindow(w.xDisplay, w.wMain);
    glXDestroyContext(w.xDisplay, w.cMain);
    XFreeColormap(w.xDisplay, w.cMapMain);
    XFree(w.vInfoMain);
    XCloseDisplay(w.xDisplay);
    w.xDisplay = 0;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
