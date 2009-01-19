#ifndef parRun_H
#define parRun_H

#include "OPstream.H"
#include "IPstream.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class ParRunControl Declaration
\*---------------------------------------------------------------------------*/

class ParRunControl
{
    bool RunPar;

public:

    ParRunControl()
    :
        RunPar(false)
    {}

    ~ParRunControl()
    {
        if (RunPar)
        {
            Info<< "Finalising parallel run" << endl;
            Pstream::exit(0);
        }
    }

    void runPar(int& argc, char**& argv)
    {
        RunPar = true;

        if (!Pstream::init(argc, argv))
        {
            Info<< "Failed to start parallel run" << endl;
            Pstream::exit(1);
        }
    }

    bool parRun() const
    {
        return RunPar;
    }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
