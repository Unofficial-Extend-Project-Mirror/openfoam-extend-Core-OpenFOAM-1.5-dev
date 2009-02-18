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

Author
    Frank Bos, TU Delft.  All rights reserved.
    Dubravko Matijasevic, FSB Zagreb.

\*---------------------------------------------------------------------------*/

#include "RBFInterpolation.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

const Foam::scalarMatrix& Foam::RBFInterpolation::B() const
{
    if (!BPtr_)
    {
        calcB();
    }

    return *BPtr_;
}


void Foam::RBFInterpolation::calcB() const
{
    // Determine inverse of boundary connectivity matrix
    label polySize(4); 
    if(!polyNomials_)
    {
        polySize = 0;
    }

    // Fill Nb x Nb matrix  
    simpleMatrix<scalar> A(controlPoints_.size()+polySize);

    //global size of controlPoints

    // ==================================
    // FM BOS [22 Jan 2009]
    // PARALLEL PIECE OF CODE
//    globalPoints parallelPoints(mesh_);
  
//     IOobject addrHeader
//     (
//         "pointProcAddressing",
//         mesh_.facesInstance()/mesh_.meshSubDir,
//         mesh_,
//         IOobject::MUST_READ
//     );
//         if (addrHeader.headerOk())
//         {
//             // There is a pointProcAddressing file so use it to get labels
//             // on the original mesh
//             Pout<< "globalMeshData::sharedPointGlobalLabels : "
//                 << "Reading pointProcAddressing" << endl;
// 
//             labelIOList pointProcAddressing(addrHeader);
// //             forAll(sharedPointLabels_, i)
// //             {
// //                 // Get my mesh point
// //                 label pointI = sharedPointLabels_[i];
// // 
// //                 // Map to mesh point of original mesh
// //                 sharedPointGlobalLabels[i] = pointProcAddressing[pointI];
// //             }
//         }
// 
// 
//     Info << "control points: " << controlPoints_ << endl;
// 
//     if (Pstream::master())
//     {
//         Info << "Master node" << endl;
//         Info << Pstream::parRun() << endl;
//         for
//         (
//             int slave=Pstream::firstSlave();
//             slave<=Pstream::lastSlave();
//             slave++
//         )
//         {
//             Info << "CPU number: " << slave << endl;
//             forAll(controlPoints_, i)
//             {
//                 Info<<controlPoints_[i]<<endl;
//                 OPstream toMaster(Pstream::blocking, Pstream::masterNo());
//                 toMaster<<controlPoints_;
//             }
//         }
//     }
//     else
//     {
//         Info << "Slave node" << endl;
//     }
// 
//     Info << "Pstream::firstSlave(): " << Pstream::firstSlave() << endl;
//     Info << "Pstream::lastSlave(): " << Pstream::lastSlave() << endl;
    // FM BOS [22 Jan 2009]
    // END PARALLEL PIECE OF CODE
    // ==================================

    const label nControlPoints = controlPoints_.size();
    for (label i = 0; i < nControlPoints; i++)
    {
        scalarField weights = RBF_->weights(controlPoints_, controlPoints_[i]);

        for (label col = 0; col < nControlPoints; col++)
        {
            A[i][col] = weights[col];
        }    
    }    

    if(polyNomials_)
    {
        for 
        (
            label row = nControlPoints;
            row < nControlPoints + 1;
            row++
        )
        {
            for (label col = 0; col < nControlPoints; col++)
            {
                A[col][row] = 1.0;
                A[row][col] = 1.0;
            }
        }
    
        // Fill in X components of polynomial part of matrix
        for 
        (
            label row = nControlPoints + 1;
            row < nControlPoints + 2;
            row++
        )
        {
            for (label col = 0; col < nControlPoints; col++)
            {
                A[col][row] = controlPoints_[col].x();
                A[row][col] = controlPoints_[col].x();
            }
        }
    
        // Fill in Y components of polynomial part of matrix
        for 
        (
            label row = nControlPoints + 2;
            row < nControlPoints + 3;
            row++
        )
        {
            for (label col = 0; col < nControlPoints; col++)
            {
                A[col][row] = controlPoints_[col].y();
                A[row][col] = controlPoints_[col].y();
            }
        }
        // Fill in Z components of polynomial part of matrix
        for 
        (
            label row = nControlPoints + 3;
            row < nControlPoints + 4;
            row++
        )
        {
            for (label col = 0; col < nControlPoints; col++)
            {
                A[col][row] = controlPoints_[col].z();
                A[row][col] = controlPoints_[col].z();
            }
        }
    
        // Fill 4x4 zero part of matrix    
        for 
        (
            label row = nControlPoints;
            row < nControlPoints + 4; 
            row++
        )
        {
            for 
            (
                label col = nControlPoints;
                col < nControlPoints + 4; 
                col++
            )
            {
                A[row][col] = 0.0;
            }
        }
    }

    // HJ and FB (05 Jan 2009)
    // Collect ALL control points from ALL CPUs
    // Create an identical inverse for all CPUs

    BPtr_ = new scalarMatrix(A.LUinvert());
}


void Foam::RBFInterpolation::clearOut()
{
    deleteDemandDrivenData(BPtr_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RBFInterpolation::RBFInterpolation
(
    const polyMesh& mesh,
    const dictionary& dict,
    const vectorField& controlPoints,
    const vectorField& allPoints
)
:
    mesh_(mesh),
    dict_(dict),
    controlPoints_(controlPoints),
    allPoints_(allPoints),
    RBF_(RBFFunction::New(word(dict.lookup("RBF")), dict)),
    BPtr_(NULL),
    innerRadius_(readScalar(dict.lookup("innerRadius"))),
    outerRadius_(readScalar(dict.lookup("outerRadius"))),
    polyNomials_(readBool(dict.lookup("polyNomials")))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::RBFInterpolation::~RBFInterpolation()
{
    clearOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::RBFInterpolation::movePoints()
{
    clearOut();
}


// ************************************************************************* //
