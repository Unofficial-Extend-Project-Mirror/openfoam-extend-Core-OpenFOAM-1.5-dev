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

#include "lduMatrix.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::lduMatrix::initMatrixInterfaces
(
    const FieldField<Field, scalar>& coupleCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const scalarField& psiif,
    scalarField& result,
    const direction cmpt
) const
{
    const lduSchedule& patchSchedule = this->patchSchedule();

    // Loop over all the "normal" interfaces relating to standard patches
    forAll (patchSchedule, i)
    {
        label interfaceI = patchSchedule[i].patch;

        if (interfaces.set(interfaceI))
        {
            // This is a dangerous and stupid bug: initInterfaceMatrixUpdate
            // is called only for the patches where bufferedTransfer is set
            // to true, which in practice means only processor patches
            // Fixed by HJ, 6/Oct/2007
            if (patchSchedule[i].init)
            {
                interfaces[interfaceI].initInterfaceMatrixUpdate
                (
                    psiif,
                    result,
                    *this,
                    coupleCoeffs[interfaceI],
                    cmpt,
                    patchSchedule[i].bufferedTransfer
                );
            }
        }
    }

    // Loop over the "global" patches are on the list of interfaces but
    // beyond the end of the schedule which only handles "normal" patches
    for
    (
        label interfaceI = patchSchedule.size()/2;
        interfaceI < interfaces.size();
        interfaceI++
    )
    {
        if (interfaces.set(interfaceI))
        {
            interfaces[interfaceI].initInterfaceMatrixUpdate
            (
                psiif,
                result,
                *this,
                coupleCoeffs[interfaceI],
                cmpt,
                true
            );
        }
    }
}


void Foam::lduMatrix::updateMatrixInterfaces
(
    const FieldField<Field, scalar>& coupleCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const scalarField& psiif,
    scalarField& result,
    const direction cmpt
) const
{
    const lduSchedule& patchSchedule = this->patchSchedule();

    // Loop over all the "normal" interfaces relating to standard patches
    forAll (patchSchedule, i)
    {
        label interfaceI = patchSchedule[i].patch;

        if (interfaces.set(interfaceI))
        {
            if (patchSchedule[i].init)
            {
                if (!patchSchedule[i].bufferedTransfer)
                {
                    interfaces[interfaceI].initInterfaceMatrixUpdate
                    (
                        psiif,
                        result,
                        *this,
                        coupleCoeffs[interfaceI],
                        cmpt,
                        patchSchedule[i].bufferedTransfer
                    );
                }
            }
            else
            {
                interfaces[interfaceI].updateInterfaceMatrix
                (
                    psiif,
                    result,
                    *this,
                    coupleCoeffs[interfaceI],
                    cmpt
                );
            }
        }
    }

    // Loop over the "global" patches are on the list of interfaces but
    // beyond the end of the schedule which only handles "normal" patches
    for
    (
        label interfaceI=patchSchedule.size()/2;
        interfaceI<interfaces.size();
        interfaceI++
    )
    {
        if (interfaces.set(interfaceI))
        {
            interfaces[interfaceI].updateInterfaceMatrix
            (
                psiif,
                result,
                *this,
                coupleCoeffs[interfaceI],
                cmpt
            );
        }
    }
}


// Buffered transfer.  Useful for debugging only
// HJ, 6/Nov/2007
// void Foam::lduMatrix::initMatrixInterfaces
// (
//     const FieldField<Field, scalar>& coupleCoeffs,
//     const lduInterfaceFieldPtrsList& interfaces,
//     const scalarField& psiif,
//     scalarField& result,
//     const direction cmpt
// ) const
// {
//     forAll (interfaces, interfaceI)
//     {
//         if (interfaces.set(interfaceI))
//         {
//             interfaces[interfaceI].initInterfaceMatrixUpdate
//             (
//                 psiif,
//                 result,
//                 *this,
//                 coupleCoeffs[interfaceI],
//                 cmpt,
//                 true
//             );
//         }
//     }
// }


// void Foam::lduMatrix::updateMatrixInterfaces
// (
//     const FieldField<Field, scalar>& coupleCoeffs,
//     const lduInterfaceFieldPtrsList& interfaces,
//     const scalarField& psiif,
//     scalarField& result,
//     const direction cmpt
// ) const
// {
//     forAll (interfaces, interfaceI)
//     {
//         if (interfaces.set(interfaceI))
//         {
//             interfaces[interfaceI].updateInterfaceMatrix
//             (
//                 psiif,
//                 result,
//                 *this,
//                 coupleCoeffs[interfaceI],
//                 cmpt
//             );
//         }
//     }
// }


// ************************************************************************* //
