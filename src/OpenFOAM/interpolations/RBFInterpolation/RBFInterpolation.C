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
   
	// Fill Nb x Nb matrix
	scalarMatrix A(controlPoints_.size() + 4);
	const label nControlPoints = controlPoints_.size();

	for (label i = 0; i < nControlPoints; i++)
    {
		scalarField weights = RBF_->weights(controlPoints_, controlPoints_[i]);

        for (label col = 0; col < nControlPoints; col++)
		{
            A[i][col] = weights[col];
		}	
    }	

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
    innerRadius_(dict.lookupOrDefault<scalar>("innerRadius", 1.0)),
    outerRadius_(dict.lookupOrDefault<scalar>("outerRadius", 2.5))
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
