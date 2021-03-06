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

template<class Type>
inline const Field< Field<Type> >& Foam::correlationFunction<Type>::
tZeroBuffers() const
{
    return tZeroBuffers_;
}


template<class Type>
inline scalar Foam::correlationFunction<Type>::duration() const
{
    return duration_;
}


template<class Type>
inline scalar Foam::correlationFunction<Type>::sampleInterval() const
{
    return sampleInterval_;
}


template<class Type>
inline scalar Foam::correlationFunction<Type>::averagingInterval() const
{
    return averagingInterval_;
}


template<class Type>
inline label Foam::correlationFunction<Type>::sampleSteps() const
{
    return sampleSteps_;
}


template<class Type>
inline label Foam::correlationFunction<Type>::measurandFieldSize() const
{
    return tZeroBuffers_[0].size();
}


// ************************************************************************* //
