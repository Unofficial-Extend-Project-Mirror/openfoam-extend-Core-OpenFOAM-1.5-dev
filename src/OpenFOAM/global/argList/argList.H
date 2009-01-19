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
    argList

Description
    Make, print and check argList

\*---------------------------------------------------------------------------*/

#ifndef argList_H
#define argList_H

#include "stringList.H"
#include "SubList.H"
#include "SLList.H"
#include "HashTable.H"
#include "word.H"
#include "fileName.H"
#include "parRun.H"
#include "sigFpe.H"
#include "sigInt.H"
#include "sigQuit.H"
#include "sigSegv.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class argList
{
    // Private data

        stringList args_;
        HashTable<string> options_;

        word executable_;
        fileName rootPath_;
        fileName globalCase_;
        fileName case_;
        bool parRun_;

        ParRunControl parRunControl_;

        // Signal handlers
        sigFpe sigFpe_;
        sigInt sigInt_;
        sigQuit sigQuit_;
        sigSegv sigSegv_;


    // Private member functions

        int findArg(const string& arg) const;


public:

    // Static data members

        static SLList<string> validArgs;
        static HashTable<string> validOptions;
        static HashTable<string> validParOptions;

        class initValidTables
        {
        public:

            initValidTables();
        };


    // Constructors

        //- Construct from argc and argv
        //  checking the arguments and options as requested
        argList
        (
            int& argc,
            char**& argv,
            bool checkArgs = true,
            bool checkOpts = true
        );


    // Destructor

        virtual ~argList();


    // Member functions

        // Access

            //- Return arguments
            const stringList& args() const
            {
                return args_;
            }

            //- Return parameters - ie, arguments without args[0] (executable)
            stringList::subList params() const
            {
                return stringList::subList(args_, args_.size() - 1, 1);
            }

            //- Return options
            const HashTable<string>& options() const
            {
                return options_;
            }

            //- Name of executable
            const word& executable() const
            {
                return executable_;
            }

            //- Return root path
            const fileName& rootPath() const
            {
                return rootPath_;
            }

            //- Return case name
            const fileName& globalCaseName() const
            {
                return globalCase_;
            }

            //- Return case name
            //  or parallel processor case name for parallel run
            const fileName& caseName() const
            {
                return case_;
            }

            //- Return path
            fileName path() const
            {
                return rootPath()/caseName();
            }

            //- Is this a parallel run
            bool parRun() const
            {
                return parRun_;
            }


        // Edit

            //- Remove the parallel options
            static void noParallel();


        // Print

            //- Print usage
            void printUsage() const;


        // Check

            //- Check argument list
            bool check(bool checkArgs = true, bool checkOpts = true) const;

            //- Check root path and case path
            bool checkRootCase() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
