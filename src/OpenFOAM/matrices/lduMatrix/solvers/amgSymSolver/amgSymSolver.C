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

Description
    Agglomerated algebraic multigrid solver tuned for the FV elliptic matrices.

\*---------------------------------------------------------------------------*/

#include "amgSymSolver.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(amgSymSolver, 0);
lduMatrix::solver::addsymMatrixConstructorToTable<amgSymSolver>
    addamgSymSolverMatrixConstructorToTable_;

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// Max number of levels
label amgSymSolver::maxLevels_ = 50;

// Max number of cycles
label amgSymSolver::maxCycles_ = 500;

// Number of post-smoothing sweeps
label amgSymSolver::nPostSweeps_ = 2;

// Number of smoothing sweeps on finest mesh
label amgSymSolver::nBottomSweeps_ = 2;


IStringStream amgSymSolver::solverDataStream
(
    const word& solverName,
    Istream& solverData
) const
{
    scalar tolerance(readScalar(solverData));
    scalar relTol(readScalar(solverData));
    scalar nCellsInTopLevel(readLabel(solverData));

    return IStringStream
    (
        solverName +
        "  minIter 0; maxIter 1000; "
        "  tolerance " + name(tolerance) + "; relTol " + name(relTol) + "; "
        "  nCellsInTopLevel " + name(nCellsInTopLevel)
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

amgSymSolver::amgSymSolver
(
    const word& fieldName,
    const lduMatrix& matrix,
    const FieldField<Field, scalar>& coupleBouCoeffs,
    const FieldField<Field, scalar>& coupleIntCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    Istream& solverData
)
:
    lduMatrix::solver
    (
        fieldName,
        matrix,
        coupleBouCoeffs,
        coupleIntCoeffs,
        interfaces,
        solverDataStream(word::null, solverData)()
    ),
    nCellsInTopLevel_(readLabel(dict().lookup("nCellsInTopLevel"))),

    restrictAddressing_(maxLevels_),
    meshLevels_(maxLevels_),
    matrixLevels_(maxLevels_),
    interfaceLevels_(maxLevels_),
    interfaceCoeffs_(maxLevels_),
    cpu_()
{
    // Create coarse levels and calculate agglomeration

    label nCreatedLevels = 0;

#   ifdef FULLDEBUG
    if (debug >= 2)
    {
        Info << "number of cells per level: ";
    }
#   endif

    while (nCreatedLevels < maxLevels_ - 1)
    {
        if (!calcAgglomeration(nCreatedLevels)) break;

        makeCoarseMatrix(nCreatedLevels);
        nCreatedLevels++;
    }

#   ifdef FULLDEBUG
    if (debug >= 3)
    {
        Info << endl;

        Info<< "coarse levels construction " << cpu_.cpuTimeIncrement() << endl;
    }
#   endif

    restrictAddressing_.setSize(nCreatedLevels);
    meshLevels_.setSize(nCreatedLevels);
    matrixLevels_.setSize(nCreatedLevels);
    interfaceLevels_.setSize(nCreatedLevels);
    interfaceCoeffs_.setSize(nCreatedLevels);

#   ifdef FULLDEBUG
    if (debug >= 2)
    {
        Info<< "Levels ";
        forAll(meshLevels_, i)
        {
            Info<< meshLevels_[i].size() << " ";
        }
        Info<< endl;
    }
#   endif

#   ifdef FULLDEBUG
    if (debug >= 3)
    {
        Info<< "coarse levels addressing " << cpu_.cpuTimeIncrement() << endl;
    }
#   endif
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

amgSymSolver::~amgSymSolver()
{
    // Clear the interface storage by hand.  It is a list of ptrs not a PtrList
    // for consistency of the interface
    forAll (interfaceLevels_, levelI)
    {
        lduInterfaceFieldPtrsList& curLevel = interfaceLevels_[levelI];

        forAll (curLevel, i)
        {
            if (curLevel.set(i))
            {
                delete curLevel(i);
            }
        }
    }
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

const lduMatrix& amgSymSolver::matrixLevel(const label i) const
{
    if (i == 0)
    {
        return matrix_;
    }
    else
    {
        return matrixLevels_[i - 1];
    }
}


const lduInterfaceFieldPtrsList& amgSymSolver::interfaceLevel
(
    const label i
) const
{
    if (i == 0)
    {
        return interfaces_;
    }
    else
    {
        return interfaceLevels_[i - 1];
    }
}


const FieldField<Field, scalar>& amgSymSolver::interfaceCoeffsLevel
(
    const label i
) const
{
    if (i == 0)
    {
        return coupleBouCoeffs_;
    }
    else
    {
        return interfaceCoeffs_[i - 1];
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void amgSymSolver::read(Istream& solverData)
{
    lduSolver::read(solverData);
    solverData >> nCellsInTopLevel_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
