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
    this class samples the given volScalarField
    along a line specified with its start, end and
    the number of points

\*---------------------------------------------------------------------------*/

#include "samplingLine.H"
#include "Istream.H"
#include "fvMesh.H"
#include "volFields.H"
#include "fvc.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void samplingLine::calcAddressing()
{
    // calculate the addressing information
    Info<< "\n Constructing the sampling line" << endl;

    // calculate the deltaX vector
    vector deltaX(0.0, 0.0, 0.0);
    vector endStart = end_ - start_;

    if (nPoints_ < 2)
    {
        FatalErrorIn("samplingLine::calcAddressing()")
            << "Number of sampling points less than two"
            << abort(FatalError);
    }
    else
    {
        deltaX = 1.0/(nPoints_ - 1.0)*endStart;
    }


    const vectorField& centres = mesh_.cellCentres();
    const labelListList& cc = mesh_.cellCells();

    // keep the label of the last cell found
    label lastCell = -1;

    // Algorithm:
    // Use the points uniformly distributed along the line to determine all the
    // cells that intersect with the sampling line. When a new cell is found,
    // calculate the nearest point on the line and use it as a sampling point.
    // The points on the line that are outside of the cell they are closest to
    // are rejected.



    // Create SLLists for points
    SLList<point> SLpoints;
    SLList<scalar> SLparametricX;

    // set curCell label to zero (start)
    register label curCell = 0;

    scalar distStart(GREAT);
    forAll(centres, cellI)
    {
        if( mag(centres[cellI] - start_) < distStart )
        {
            distStart = mag(centres[cellI] - start_);
            curCell = cellI;
        }
    }

    for (label pointI = 0; pointI < nPoints_; pointI++)
    {
        // calculate the location of the point along the line
        vector curPoint = start_ + scalar(pointI)*deltaX;

        // search for the closest cell

        // set the distance
        scalar distance = mag(centres[curCell] - curPoint);

        bool closer;

        do
        {
            closer = false;

            // set the current list of neighbouring cells
            const labelList& neighbours = cc[curCell];

            forAll (neighbours, nI)
            {
                scalar curDist = mag(centres[neighbours[nI]] - curPoint);

                // search through all the neighbours.
                // If the cell is closer, reset current cell and distance
                if (curDist < distance)
                {
                    distance = curDist;
                    curCell = neighbours[nI];
                    closer = true;    // a closer neighbour has been found
                }
            }
        } while (closer);


        // The cell nearest to the point is curCell.
        // Check if the point is inside that cell
        if (mesh_.pointInCell(curPoint, curCell))
        {
            // point is inside the cell. If it is not the same as the last cell
            // found, add it to the list
            if (curCell != lastCell)
            {
                // valid cell
                lastCell = curCell;

                // calculate the nearest point on the sampling line
                vector posVector = centres[curCell] - start_;

                scalar parX = (posVector & endStart)/magSqr(endStart);

                vector newPoint = start_ + parX*endStart;

                // check if the new point is inside the cell
                if (mesh_.pointInCell(newPoint, curCell))
                {
                    // add it as the sampling point
                    SLpoints.append(newPoint);
                    SLparametricX.append(parX);

                    addressingData_.append
                    (
                        pointAddressing(curCell, newPoint - centres[curCell])
                    );
                }
            }
        }
    }


    if (addressingData_.size() < 1)
    {
        FatalErrorIn("samplingLine::calcAddressing()")
            << "no sampling points found"
            << abort(FatalError);
    }


    points_.setSize(SLpoints.size());
    parametricX_.setSize(SLparametricX.size());

    SLList<point>::iterator pointIter(SLpoints.begin());
    SLList<scalar>::iterator parIter(SLparametricX.begin());

    label pI = 0;
    for
    (
        ;
        pointIter != SLpoints.end()
     && parIter != SLparametricX.end();
        ++pointIter, ++parIter)
    {
        points_[pI] = pointIter();
        parametricX_[pI++] = parIter();
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

samplingLine::samplingLine
(
    const fvMesh& mesh,
    const point& start,
    const point& end,
    const label noPoints
)
:
    mesh_(mesh),
    start_(start),
    end_(end),
    nPoints_(noPoints),
    points_(0, vector::zero),
    parametricX_(0, 0.0),
    addressingData_()
{
    calcAddressing();
}


samplingLine::samplingLine
(
    const fvMesh& mesh,
    Istream& is
)
:
    mesh_(mesh),
    start_(is),
    end_(is),
    nPoints_(readLabel(is)),
    points_(0, vector::zero),
    parametricX_(0, 0.0),
    addressingData_()
{
    calcAddressing();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

scalarField samplingLine::simpleInterpolate
(
    const volScalarField& f
) const
{
    scalarField res(points_.size());

    label resI = 0;

    for
    (
        SLList<pointAddressing>::const_iterator addressIter =
            addressingData_.begin();
        addressIter != addressingData_.end();
        ++addressIter
    )
    {
        res[resI++] = f[addressIter().nearestCell()];
    }

    return res;
}


scalarField samplingLine::interpolate
(
    const volScalarField& f
) const
{
    return interpolate(f, fvc::grad(f));
}


scalarField samplingLine::interpolate
(
    const volScalarField& f,
    const volVectorField& gradf
) const
{
    scalarField res(points_.size());

    label resI = 0;

    for
    (
        SLList<pointAddressing>::const_iterator addressIter =
            addressingData_.begin();
        addressIter != addressingData_.end();
        ++addressIter
    )
    {
        res[resI++] =
            f[addressIter().nearestCell()]
          + (addressIter().diffVector() & gradf[addressIter().nearestCell()]);
    }

    return res;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
