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
    pointHit

Description
    This class describes the interaction of a face and a point. It
    carries the info of a successful hit and (if successful), returns
    the interaction point.

\*---------------------------------------------------------------------------*/

#ifndef pointHit_H
#define pointHit_H

#include "bool.H"
#include "point.H"
#include "token.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class pointHit Declaration
\*---------------------------------------------------------------------------*/

class pointHit
{
    // Private data

        //- Hit success
        bool hit_;

        //- Point of hit; for miss holds best estimate outside the object
        point hitPoint_;

        //- Distance to hit point
        scalar distance_;

        //- Eligible miss
        bool eligibleMiss_;


public:

    // Constructors

        //- Construct from components
        pointHit
        (
            const bool hit,
            const point& p,
            const scalar dist,
            const bool eligibleMiss
        )
        :
            hit_(hit),
            hitPoint_(p),
            distance_(dist),
            eligibleMiss_(eligibleMiss)
        {}

        //- Construct from point. Hit and distance set later
        pointHit(const point& p)
        :
            hit_(false),
            hitPoint_(p),
            distance_(GREAT),
            eligibleMiss_(false)
        {}


    // Member Functions

        //- Is there a hit
        bool hit() const
        {
            return hit_;
        }

        //- Return hit point
        const point& hitPoint() const
        {
            if (!hit_)
            {
                FatalErrorIn("const point& pointHit::hitPoint() const")
                    << "requested a hit point for a miss"
                    << abort(FatalError);
            }

            return hitPoint_;
        }

        //- Return distance to hit
        scalar distance() const
        {
            return distance_;
        }

        //- Return miss point
        const point& missPoint() const
        {
            if (hit_)
            {
                FatalErrorIn("const point& pointHit::missPoint() const")
                    << "requested a miss point for a hit"
                    << abort(FatalError);
            }

            return hitPoint_;
        }

        //- Return point with no checking
        const point& rawPoint() const
        {
            return hitPoint_;
        }

        //- Is this an eligible miss
        bool eligibleMiss() const
        {
            return eligibleMiss_;
        }

        void setHit()
        {
            hit_ = true;
            eligibleMiss_ = false;
        }

        void setMiss(const bool eligible)
        {
            hit_ = false;
            eligibleMiss_ = eligible;
        }

        void setPoint(const point& p)
        {
            hitPoint_ = p;
        }

        void setDistance(const scalar d)
        {
            distance_ = d;
        }


    // Ostream operator

        friend Ostream& operator<<(Ostream& os, const pointHit& b)
        {
            os  << b.hit() << token::SPACE
                << b.rawPoint() << token::SPACE
                << b.distance() << token::SPACE
                << b.eligibleMiss();

            return os;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
