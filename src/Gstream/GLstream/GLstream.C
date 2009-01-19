// GLstream.C : provides code to implement a simple plotting xwindow
// by declaring it to be an object.

#include <cstring>
#include "GLstream.H"
#include "XGLwindow.H"
#include "labelList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

short GLstream::convertLinestyleToGL(const scalarList& dStyle)
{
    // convert the dashstyle to a 16 pixel length element
    labelList tile(dStyle.size());

    label sum = 0;
    for (label i=0; i<tile.size(); i++)
    {
        tile[i] = int(16*dStyle[i]);
        sum += tile[i];
    }

    if (sum != 16)
    {
        FatalErrorIn("GLstream::convertLinestyleToGL(const scalarList& dStyle)")
            << " problem converting linestyles to GL"
            << abort(FatalError);
    }

    // now convert this to a 16 bit short

    short placeval = 1;
    short pattern = 0;
    short numeral = 1;
    int section = 0;

    for (label i=0; i<16;)
    {
        for (label j=0; j<tile[section]; j++)
        {
            pattern += placeval*numeral;
            i++;
            placeval *= 2;
        }

        if (numeral == 1) numeral = 0;
        else numeral = 1;

        section++;
    }

    return pattern;
}


void GLstream::makeRasterFont()
{
    XFontStruct *fontInfo;
    Font id;
    unsigned int first, last;

    fontInfo = XLoadQueryFont(mydisplay, font().c_str());

    if (fontInfo == NULL)
    {
        FatalErrorIn("GLstream::makeRasterFont()")
            << "no font found"
            << abort(FatalError);
    }

    id = fontInfo->fid;
    first = fontInfo->min_char_or_byte2;
    last = fontInfo->max_char_or_byte2;

    base = glGenLists(GLuint(last+1));

    if (base == 0)
    {
        FatalErrorIn("GLstream::makeRasterFont()")
            << "GLstream::makeRasterFont() : "
            << "out of display lists"
            << abort(FatalError);
    }
    glXUseXFont(id, first, last-first+1, base+first);
}


void GLstream::printString(char *s)
{
    glPushAttrib (GL_LIST_BIT);
    glListBase(base);
    glCallLists(strlen(s), GL_UNSIGNED_BYTE, reinterpret_cast<GLubyte*>(s));
    glPopAttrib ();
}


GLstream::GLstream
(
    const word& fname,
    const colour& forecol,
    const colour& backcol,
    double sX,
    double sY,
    double cX,
    double cY,
    int widthX,
    int widthY
)
:
    Gstream(fname, sX, sY, cX, cY, widthX, widthY),
    dashSty(1)
{
    // open the window
    if
    (
       !XGLopenWindow
        (
            &mydisplay,
            const_cast<char*>(winName().c_str()),
            0, 0,
            width_.x(), width_.y()
        )
    )
    {
        FatalErrorIn
        (
            "GLstream::GLstream(const word& fname, const colour& forecol, "
            "const colour& backcol, double sX, double sY, double cX, "
            "double cY, int widthX, int widthY)"
        )   << "cannot create window"
            << abort(FatalError);
    }

    // set background colour
    glClearColor (backcol.r(), backcol.g(), backcol.b(), 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // set foreground colour
    glColor3f(forecol.r(),forecol.g(),forecol.b());

    // set 2D projection
    glOrtho
    (
        -center_.x()*width_.x()/scale_.x(),
        (width_.x() - center_.x()*width_.x())/scale_.x(),
        -center_.y()*width_.y()/scale_.y(),
        (width_.y() - center_.y()*width_.y())/scale_.y(),
        -1, -1
    );

    glViewport(0, 0, width_.x(), width_.y());

    makeRasterFont();
    glShadeModel(GL_FLAT);

    setLineStyle(lineStyle());
}


// destruction operator : clear up afterwards

GLstream::~GLstream()
{
    XGLcloseWindow();
}


int GLstream::waitForEvent(GEvent& event)
{
    int notDone(1);
    int leave(1);
    XEvent flag;

    while (notDone)
    {
        XNextEvent(mydisplay, &flag);

        switch (flag.type)
        {
        case Expose:
            if (flag.xexpose.count==0)
            {
                notDone=0;
                leave = 1;
                glClear(GL_COLOR_BUFFER_BIT);
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

            scale_ = point2D
            (
                double(flag.xconfigure.width)*scale_.x()/double(width_.x()),
                double(flag.xconfigure.height)*scale_.y()/double(width_.y())
            );

            width_ = coord2D(flag.xconfigure.width, flag.xconfigure.height);

            glViewport(0,0,width_.x(),width_.y());

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

void GLstream::draw(const line2D& l)
{
    glBegin(GL_LINES);
    glVertex2d(double(x(l.start())), double(y(l.start())));
    glVertex2d(double(x(l.end())), double(y(l.end())));

    glEnd();
    glFlush();
}


void GLstream::draw(const rectangle2D& r)
{
    glRectd
    (
        double(x(r.first())), double(y(r.first())),
        double(x(r.second())), double(y(r.second()))
    );

    glFlush();
}


void GLstream::draw(const polygon2D& p)
{
    glBegin(GL_POLYGON);

    const List <point2D> & vertex = p.verts();

    forAll(vertex, i)
    {
        glVertex2d(double(x(vertex[i])), double(y(vertex[i])));
    }

    glEnd();
    glFlush();
}


void GLstream::draw(const structuredScalars2D& sS)
{
    scalar ycoord(sS.origin().y());
    glShadeModel(GL_SMOOTH);

    for (label i=0; i<sS.ynum()-1; i+=sS.yjump())
    {
        scalar xcoord(sS.origin().x());
        const scalarList& fstvals = sS.DataRow(i);
        const scalarList& sndvals = sS.DataRow(i+1);

        glBegin(GL_QUAD_STRIP);
        for (label j=0; j<sS.xnum(); j+=sS.xjump())
        {
            setColour(sS.newColour(fstvals[j]));
            glVertex2f(xcoord, ycoord);

            setColour(sS.newColour(sndvals[j]));
            glVertex2f(xcoord,ycoord+sS.yjump()*sS.yskip());

            xcoord += sS.xskip()*sS.xjump();
        }
        glEnd();
        ycoord += sS.yskip()*sS.yjump();
    }
    glFlush();
}


void GLstream::draw(const string2D& s2D)
{
    if (s2D.horizontal())
    {
        glRasterPos2d(s2D.x(),s2D.y());
        glPushAttrib(GL_LIST_BIT);
        glListBase(base);
        glCallLists
        (
            s2D.text().size(),
            GL_UNSIGNED_BYTE,
            reinterpret_cast<const GLubyte*>(s2D.text().c_str())
        );
        glPopAttrib();

    }
}


// reset foreground and background colours
void GLstream::setColour(const colour& fore)
{
    glColor3f(fore.r(),fore.g(),fore.b());
}


void GLstream::setLineStyle(const lineStyle& ls)
{
    // determine thickness
    float thick(ls.thickness());

    glLineWidth(thick);


    // dashed style :
    // number of elements in pattern

    int n(ls.pattern().size());

    if (n==1)
    {
        // solid line
        glDisable(GL_LINE_STIPPLE);
    }
    else
    {
        // dashed line
        int factor(int((ls.patternLength()+8.0)/16.0));
        if (factor<1) factor = 1;

        glLineStipple(factor,convertLinestyleToGL(ls.pattern()));
        glEnable(GL_LINE_STIPPLE);
    }
}


void GLstream::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
