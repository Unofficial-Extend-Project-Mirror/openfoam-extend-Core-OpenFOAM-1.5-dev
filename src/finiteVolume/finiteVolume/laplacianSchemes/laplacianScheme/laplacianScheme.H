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
    laplacianScheme

Description
    Abstract base class for laplacian schemes.

SourceFiles
    laplacianScheme.C

\*---------------------------------------------------------------------------*/

#ifndef laplacianScheme_H
#define laplacianScheme_H

#include "tmp.H"
#include "volFieldsFwd.H"
#include "surfaceFieldsFwd.H"
#include "linear.H"
#include "correctedSnGrad.H"
#include "typeInfo.H"
#include "runTimeSelectionTables.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

template<class Type>
class fvMatrix;

class fvMesh;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

/*---------------------------------------------------------------------------*\
                           Class laplacianScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class laplacianScheme
:
    public refCount
{

protected:

    // Protected data

        const fvMesh& mesh_;
        tmp<surfaceInterpolationScheme<scalar> > tinterpGammaScheme_;
        tmp<surfaceInterpolationScheme<tensor> > tinterpTensorGammaScheme_;
        tmp<snGradScheme<Type> > tsnGradScheme_;


    // Private Member Functions

        //- Disallow copy construct
        laplacianScheme(const laplacianScheme&);

        //- Disallow default bitwise assignment
        void operator=(const laplacianScheme&);


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            laplacianScheme,
            Istream,
            (const fvMesh& mesh, Istream& schemeData),
            (mesh, schemeData)
        );


    // Constructors

        //- Construct from mesh
        laplacianScheme(const fvMesh& mesh)
        :
            mesh_(mesh),
            tinterpGammaScheme_(new linear<scalar>(mesh)),
            tinterpTensorGammaScheme_(new linear<tensor>(mesh)),
            tsnGradScheme_(new correctedSnGrad<Type>(mesh))
        {}

        //- Construct from mesh and Istream
        laplacianScheme(const fvMesh& mesh, Istream& is)
        :
            mesh_(mesh),
            tinterpGammaScheme_(NULL),
            tinterpTensorGammaScheme_(new linear<tensor>(mesh)),
            tsnGradScheme_(NULL)
        {
            tinterpGammaScheme_ = tmp<surfaceInterpolationScheme<scalar> >
            (
                surfaceInterpolationScheme<scalar>::New(mesh, is)
            );

            tsnGradScheme_ = tmp<snGradScheme<Type> >
            (
                snGradScheme<Type>::New(mesh, is)
            );
        }

        //- Construct from mesh, interpolation and snGradScheme schemes
        laplacianScheme
        (
            const fvMesh& mesh,
            const tmp<surfaceInterpolationScheme<scalar> >& igs,
            const tmp<snGradScheme<Type> >& sngs
        )
        :
            mesh_(mesh),
            tinterpGammaScheme_(igs),
            tinterpTensorGammaScheme_(new linear<tensor>(mesh)),
            tsnGradScheme_(sngs)
        {}


    // Selectors

        //- Return a pointer to a new laplacianScheme created on freestore
        static tmp<laplacianScheme<Type> > New
        (
            const fvMesh& mesh,
            Istream& schemeData
        );


    // Destructor

        virtual ~laplacianScheme();


    // Member Functions

        //- Return mesh reference
        const fvMesh& mesh() const
        {
            return mesh_;
        }

        virtual tmp<fvMatrix<Type> > fvmLaplacian
        (
            const surfaceScalarField&,
            GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<fvMatrix<Type> > fvmLaplacian
        (
            const volScalarField&,
            GeometricField<Type, fvPatchField, volMesh>&
        );

        virtual tmp<fvMatrix<Type> > fvmLaplacian
        (
            const surfaceTensorField&,
            GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<fvMatrix<Type> > fvmLaplacian
        (
            const volTensorField&,
            GeometricField<Type, fvPatchField, volMesh>&
        );

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
        (
            const surfaceScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
        (
            const volScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
        (
            const surfaceTensorField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
        (
            const volTensorField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makeFvLaplacianTypeScheme(SS, Type)                                    \
                                                                               \
defineNamedTemplateTypeNameAndDebug(SS<Type>, 0);                              \
                                                                               \
laplacianScheme<Type>::addIstreamConstructorToTable<SS<Type> >                 \
    add##SS##Type##IstreamConstructorToTable_;


#define makeFvLaplacianScheme(SS)                                              \
                                                                               \
makeFvLaplacianTypeScheme(SS, scalar)                                          \
makeFvLaplacianTypeScheme(SS, vector)                                          \
makeFvLaplacianTypeScheme(SS, sphericalTensor)                                 \
makeFvLaplacianTypeScheme(SS, symmTensor)                                      \
makeFvLaplacianTypeScheme(SS, tensor)                                          \


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "laplacianScheme.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
