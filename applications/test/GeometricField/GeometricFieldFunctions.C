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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Global functions  * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
void component
(
    GeometricField
    <
        typename GeometricField<Type, PatchField, GeoMesh>::cmptType,
        PatchField,
        GeoMesh
    >& gcf,
    const GeometricField<Type, PatchField, GeoMesh>& gf,
    const direction d
)
{
    component(gcf.field(), gf.field(), d);
    component(gcf.boundaryField(), gf.boundaryField(), d);
}


template<class Type, template<class> class PatchField, class GeoMesh>
void T
(
     GeometricField<Type, PatchField, GeoMesh>& gf,
     const GeometricField<Type, PatchField, GeoMesh>& gf1
)
{
    T(gf.field(), gf1.field());
    T(gf.boundaryField(), gf1.boundaryField());
}


template<class Type, template<class> class PatchField, class GeoMesh, int r>
void pow
(
    GeometricField<typename powProduct<Type, r>::type, PatchField, GeoMesh>& gf,
    const GeometricField<Type, PatchField, GeoMesh>& gf1
)
{
    pow(gf.field(), gf1.field(), r);
    pow(gf.boundaryField(), gf1.boundaryField(), r);
}

template<class Type, template<class> class PatchField, class GeoMesh, int r>
tmp<GeometricField<typename powProduct<Type, r>::type, PatchField, GeoMesh> >
pow
(
    const GeometricField<Type, PatchField, GeoMesh>& gf,
    typename powProduct<Type, r>::type
)
{
    typedef typename powProduct<Type, r>::type powProductType;

    tmp<GeometricField<powProductType, PatchField, GeoMesh> > tPow
    (
        new GeometricField<powProductType, PatchField, GeoMesh>
        (
            IOobject
            (
                "pow(" + gf.name() + ',' + name(r) + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            pow(gf.dimensions(), r)
        )
    );

    pow<Type, r, PatchField, GeoMesh>(tPow(), gf);

    return tPow;
}


template<class Type, template<class> class PatchField, class GeoMesh, int r>
tmp<GeometricField<typename powProduct<Type, r>::type, PatchField, GeoMesh> >
pow
(
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf,
    typename powProduct<Type, r>::type
)
{
    typedef typename powProduct<Type, r>::type powProductType;

    const GeometricField<Type, PatchField, GeoMesh>& gf = tgf();

    tmp<GeometricField<powProductType, PatchField, GeoMesh> > tPow
    (
        new GeometricField<powProductType, PatchField, GeoMesh>
        (
            IOobject
            (
                "pow(" + gf.name() + ',' + name(r) + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            pow(gf.dimensions(), r)
        )
    );

    pow<Type, r, PatchField, GeoMesh>(tPow(), gf);

    tgf.clear();

    return tPow;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void sqr
(
    GeometricField
    <typename outerProduct<Type, Type>::type, PatchField, GeoMesh>& gf,
    const GeometricField<Type, PatchField, GeoMesh>& gf1
)
{
    sqr(gf.field(), gf1.field());
    sqr(gf.boundaryField(), gf1.boundaryField());
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp
<
    GeometricField
    <
        typename outerProduct<Type, Type>::type,
        PatchField,
        GeoMesh
    >
>
sqr(const GeometricField<Type, PatchField, GeoMesh>& gf)
{
    typedef typename outerProduct<Type, Type>::type outerProductType;

    tmp<GeometricField<outerProductType, PatchField, GeoMesh> > tSqr
    (
        new GeometricField<outerProductType, PatchField, GeoMesh>
        (
            IOobject
            (
                "sqr(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            sqr(gf.dimensions())
        )
    );

    sqr(tSqr(), gf);

    return tSqr;
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp
<
    GeometricField
    <
        typename outerProduct<Type, Type>::type,
        PatchField,
        GeoMesh
    >
>
sqr(const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf)
{
    typedef typename outerProduct<Type, Type>::type outerProductType;

    const GeometricField<Type, PatchField, GeoMesh>& gf = tgf();

    tmp<GeometricField<outerProductType, PatchField, GeoMesh> > tSqr
    (
        new GeometricField<outerProductType, PatchField, GeoMesh>
        (
            IOobject
            (
                "sqr(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            sqr(gf.dimensions())
        )
    );

    sqr(tSqr(), gf);

    tgf.clear();

    return tSqr;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void magSqr
(
    GeometricField<scalar, PatchField, GeoMesh>& gsf,
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
{
    magSqr(gsf.field(), gf.field());
    magSqr(gsf.boundaryField(), gf.boundaryField());
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<scalar, PatchField, GeoMesh> > magSqr
(
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
{
    tmp<GeometricField<scalar, PatchField, GeoMesh> > tMagSqr
    (
        new GeometricField<scalar, PatchField, GeoMesh>
        (
            IOobject
            (
                "magSqr(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            sqr(gf.dimensions())
        )
    );

    magSqr(tMagSqr(), gf);

    return tMagSqr;
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<scalar, PatchField, GeoMesh> > magSqr
(
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf
)
{
    const GeometricField<Type, PatchField, GeoMesh>& gf = tgf();

    tmp<GeometricField<scalar, PatchField, GeoMesh> > tMagSqr
    (
        new GeometricField<scalar, PatchField, GeoMesh>
        (
            IOobject
            (
                "magSqr(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            sqr(gf.dimensions())
        )
    );

    magSqr(tMagSqr(), gf);

    tgf.clear();

    return tMagSqr;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mag
(
    GeometricField<scalar, PatchField, GeoMesh>& gsf,
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
{
    mag(gsf.field(), gf.field());
    mag(gsf.boundaryField(), gf.boundaryField());
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<scalar, PatchField, GeoMesh> > mag
(
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
{
    tmp<GeometricField<scalar, PatchField, GeoMesh> > tMag
    (
        new GeometricField<scalar, PatchField, GeoMesh>
        (
            IOobject
            (
                "mag(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            gf.dimensions()
        )
    );

    mag(tMag(), gf);

    return tMag;
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<scalar, PatchField, GeoMesh> > mag
(
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf
)
{
    const GeometricField<Type, PatchField, GeoMesh>& gf = tgf();

    tmp<GeometricField<scalar, PatchField, GeoMesh> > tMag
    (
        new GeometricField<scalar, PatchField, GeoMesh>
        (
            IOobject
            (
                "mag(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            gf.dimensions()
        )
    );

    mag(tMag(), gf);

    tgf.clear();

    return tMag;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void cmptAv
(
    GeometricField
    <
        typename GeometricField<Type, PatchField, GeoMesh>::cmptType,
        PatchField,
        GeoMesh
    >& gcf,
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
{
    cmptAv(gcf.field(), gf.field());
    cmptAv(gcf.boundaryField(), gf.boundaryField());
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp
<
    GeometricField
    <
        typename GeometricField<Type, PatchField, GeoMesh>::cmptType,
        PatchField,
        GeoMesh
    >
>
cmptAv(const GeometricField<Type, PatchField, GeoMesh>& gf)
{
    typedef typename GeometricField<Type, PatchField, GeoMesh>::cmptType
        cmptType;

    tmp<GeometricField<cmptType, PatchField, GeoMesh> > CmptAv
    (
        new GeometricField<scalar, PatchField, GeoMesh>
        (
            IOobject
            (
                "cmptAv(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            gf.dimensions()
        )
    );

    cmptAv(CmptAv(), gf);

    return CmptAv;
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp
<
    GeometricField
    <
        typename GeometricField<Type, PatchField, GeoMesh>::cmptType,
        PatchField,
        GeoMesh
    >
>
cmptAv(const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf)
{
    typedef typename GeometricField<Type, PatchField, GeoMesh>::cmptType 
        cmptType;

    const GeometricField<Type, PatchField, GeoMesh>& gf = tgf();

    tmp<GeometricField<cmptType, PatchField, GeoMesh> > CmptAv
    (
        new GeometricField<scalar, PatchField, GeoMesh>
        (
            IOobject
            (
                "cmptAv(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            gf.dimensions()
        )
    );

    cmptAv(CmptAv(), gf);

    tgf.clear();

    return CmptAv;
}


#define BINARY_FUNCTION(func)                                                 \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
void func                                                                     \
(                                                                             \
    GeometricField<Type, PatchField, GeoMesh>& gf,                            \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const GeometricField<Type, PatchField, GeoMesh>& gf2                      \
)                                                                             \
{                                                                             \
    Foam::func(gf.field(), gf1.field(), gf2.field()); \
    Foam::func(gf.boundaryField(), gf1.boundaryField(), gf2.boundaryField()); \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > func                          \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const GeometricField<Type, PatchField, GeoMesh>& gf2                      \
)                                                                             \
{                                                                             \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        new GeometricField<Type, PatchField, GeoMesh>                         \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                #func "(" + gf1.name() + ',' + gf2.name() + ')',              \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            func(gf1.dimensions(), gf2.dimensions())                          \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::func(tRes(), gf1, gf2);                                             \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > func                          \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf2               \
)                                                                             \
{                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf2 = tgf2();            \
                                                                              \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        GeometricField<Type, PatchField, GeoMesh>::New                        \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                #func "(" + gf1.name() + ',' + gf2.name() + ')',              \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            tgf2(),                                                           \
            func(gf1.dimensions(), gf2.dimensions())                          \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::func(tRes(), gf1, tRes());                                          \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > func                          \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1,              \
    const GeometricField<Type, PatchField, GeoMesh>& gf2                      \
)                                                                             \
{                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1 = tgf1();            \
                                                                              \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        GeometricField<Type, PatchField, GeoMesh>::New                        \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                #func "(" + gf1.name() + ',' + gf2.name() + ')',              \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            tgf1(),                                                           \
            func(gf1.dimensions(), gf2.dimensions())                          \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::func(tRes(), tRes(), gf2);                                          \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > func                          \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1,              \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf2               \
)                                                                             \
{                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1 = tgf1();            \
    const GeometricField<Type, PatchField, GeoMesh>& gf2 = tgf2();            \
                                                                              \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        GeometricField<Type, PatchField, GeoMesh>::New                        \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                #func "(" + gf1.name() + ',' + gf2.name() + ')',              \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            tgf2(),                                                           \
            func(gf1.dimensions(), gf2.dimensions())                          \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::func(tRes(), tgf1(), tRes());                                       \
                                                                              \
    tgf1.clear();                                                             \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
void func                                                                     \
(                                                                             \
    GeometricField<Type, PatchField, GeoMesh>& gf,                            \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const dimensioned<Type>& dt                                               \
)                                                                             \
{                                                                             \
    Foam::func(gf.field(), gf1.field(), dt.value());          \
    Foam::func(gf.boundaryField(), gf1.boundaryField(), dt.value());          \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > func                          \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const dimensioned<Type>& dt                                               \
)                                                                             \
{                                                                             \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        new GeometricField<Type, PatchField, GeoMesh>                         \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                #func "(" + gf1.name() + ',' + dt.name() + ')',               \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            func(gf1.dimensions(), dt.dimensions())                           \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::func(tRes(), gf1, dt);                                              \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > func                          \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const Type& t                                                             \
)                                                                             \
{                                                                             \
    return func(gf1, dimensioned<Type>(t));                                   \
}                                                                             \
                                                                              \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > func                          \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1,              \
    const dimensioned<Type>& dt                                               \
)                                                                             \
{                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1 = tgf1();            \
                                                                              \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        GeometricField<Type, PatchField, GeoMesh>::New                        \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                #func "(" + gf1.name() + ',' + dt.name() + ')',               \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            tgf1,                                                             \
            func(gf1.dimensions(), dt.dimensions())                           \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::func(tRes(), tRes(), dt);                                           \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > func                          \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1,              \
    const Type& t                                                             \
)                                                                             \
{                                                                             \
    return func(tgf1, dimensioned<Type>(t));                                  \
}

BINARY_FUNCTION(max)
BINARY_FUNCTION(min)
BINARY_FUNCTION(scale)

#undef BINARY_FUNCTION


#define UNARY_FUNCTION_WITH_BOUNDARY(returnType, func, gFunc)                 \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
dimensioned<returnType> func                                                  \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf                       \
)                                                                             \
{                                                                             \
    return dimensioned<Type>                                                  \
    (                                                                         \
        #func "(" + gf.name() + ')',                                          \
        gf.dimensions(),                                                      \
        Foam::func(gFunc(gf.field()), gFunc(gf.boundaryField()))      \
    );                                                                        \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
dimensioned<returnType> func                                                  \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1               \
)                                                                             \
{                                                                             \
    dimensioned<returnType> res = func(tgf1());                               \
    tgf1.clear();                                                             \
    return res;                                                               \
}

UNARY_FUNCTION_WITH_BOUNDARY(Type, max, gMax)
UNARY_FUNCTION_WITH_BOUNDARY(Type, min, gMin)

#undef UNARY_FUNCTION_WITH_BOUNDARY


#define UNARY_FUNCTION(returnType, func, gFunc)                               \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
dimensioned<returnType> func                                                  \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf                       \
)                                                                             \
{                                                                             \
    return dimensioned<Type>                                                  \
    (                                                                         \
        #func "(" + gf.name() + ')',                                          \
        gf.dimensions(),                                                      \
        gFunc(gf.field())                                             \
    );                                                                        \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
dimensioned<returnType> func                                                  \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1               \
)                                                                             \
{                                                                             \
    dimensioned<returnType> res = func(tgf1());                               \
    tgf1.clear();                                                             \
    return res;                                                               \
}

UNARY_FUNCTION(Type, sum, gSum)
UNARY_FUNCTION(scalar, sumMag, gSumMag)
UNARY_FUNCTION(Type, average, gAverage)

#undef UNARY_FUNCTION


// * * * * * * * * * * * * * * * Global operators  * * * * * * * * * * * * * //

#define UNARY_OPERATOR(op, opFunc)                                            \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
void opFunc                                                                   \
(                                                                             \
    GeometricField<Type, PatchField, GeoMesh>& gf,                            \
    const GeometricField<Type, PatchField, GeoMesh>& gf1                      \
)                                                                             \
{                                                                             \
    Foam::opFunc(gf.field(), gf1.field());                    \
    Foam::opFunc(gf.boundaryField(), gf1.boundaryField());                    \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1                      \
)                                                                             \
{                                                                             \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        new GeometricField<Type, PatchField, GeoMesh>                         \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                #op + gf1.name(),                                             \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            op gf1.dimensions()                                               \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), gf1);                                                \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1               \
)                                                                             \
{                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1 = tgf1();            \
                                                                              \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        GeometricField<Type, PatchField, GeoMesh>::New                        \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                #op + gf1.name(),                                             \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            tgf1,                                                             \
            op gf1.dimensions()                                               \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), tRes());                                             \
                                                                              \
    return tRes;                                                              \
}

UNARY_OPERATOR(-, negate)

#undef UNARY_OPERATOR


#define BINARY_OPERATOR_FF(Type1, Type2, op, opName, opFunc)                  \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
void opFunc                                                                   \
(                                                                             \
    GeometricField<Type, PatchField, GeoMesh>& gf,                            \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1,                    \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2                     \
)                                                                             \
{                                                                             \
    Foam::opFunc(gf.field(), gf1.field(), gf2.field());\
    Foam::opFunc(gf.boundaryField(), gf1.boundaryField(), gf2.boundaryField());\
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1,                    \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2                     \
)                                                                             \
{                                                                             \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        new GeometricField<Type, PatchField, GeoMesh>                         \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + opName + gf2.name() + ')',                 \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            gf1.dimensions() op gf2.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), gf1, gf2);                                           \
                                                                              \
    return tRes;                                                              \
}

#define BINARY_OPERATOR_FTR(Type1, Type2, op, opName, opFunc)                 \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1,                    \
    const tmp<GeometricField<Type2, PatchField, GeoMesh> >& tgf2              \
)                                                                             \
{                                                                             \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2 = tgf2();           \
                                                                              \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        GeometricField<Type, PatchField, GeoMesh>::New                        \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + opName + gf2.name() + ')',                 \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            tgf2,                                                             \
            gf1.dimensions() op gf2.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), gf1, tRes());                                        \
                                                                              \
    return tRes;                                                              \
}

#define BINARY_OPERATOR_FT(Type1, Type2, op, opName, opFunc)                  \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1,                    \
    const tmp<GeometricField<Type2, PatchField, GeoMesh> >& tgf2              \
)                                                                             \
{                                                                             \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes = gf1 op tgf2();     \
    tgf2.clear();                                                             \
    return tRes;                                                              \
}

#define BINARY_OPERATOR_TRF(Type1, Type2, op, opName, opFunc)                 \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const tmp<GeometricField<Type1, PatchField, GeoMesh> >& tgf1,             \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2                     \
)                                                                             \
{                                                                             \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1 = tgf1();           \
                                                                              \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        GeometricField<Type, PatchField, GeoMesh>::New                        \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + opName + gf2.name() + ')',                 \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            tgf1,                                                             \
            gf1.dimensions() op gf2.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), tRes(), gf2);                                        \
                                                                              \
    return tRes;                                                              \
}

#define BINARY_OPERATOR_TF(Type1, Type2, op, opName, opFunc)                  \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const tmp<GeometricField<Type1, PatchField, GeoMesh> >& tgf1,             \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2                     \
)                                                                             \
{                                                                             \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes = tgf1() op gf2;     \
    tgf1.clear();                                                             \
    return tRes;                                                              \
}

#define BINARY_OPERATOR_TRT(Type1, Type2, op, opName, opFunc)                 \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const tmp<GeometricField<Type1, PatchField, GeoMesh> >& tgf1,             \
    const tmp<GeometricField<Type2, PatchField, GeoMesh> >& tgf2              \
)                                                                             \
{                                                                             \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1 = tgf1();           \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2 = tgf2();           \
                                                                              \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        GeometricField<Type, PatchField, GeoMesh>::New                        \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + opName + gf2.name() + ')',                 \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            tgf1,                                                             \
            gf1.dimensions() op gf2.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), tRes(), tgf2());                                     \
                                                                              \
    tgf2.clear();                                                             \
                                                                              \
    return tRes;                                                              \
}

#define BINARY_OPERATOR_TTR(Type1, Type2, op, opName, opFunc)                 \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const tmp<GeometricField<Type1, PatchField, GeoMesh> >& tgf1,             \
    const tmp<GeometricField<Type2, PatchField, GeoMesh> >& tgf2              \
)                                                                             \
{                                                                             \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1 = tgf1();           \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2 = tgf2();           \
                                                                              \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        GeometricField<Type, PatchField, GeoMesh>::New                        \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + opName + gf2.name() + ')',                 \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            tgf2,                                                             \
            gf1.dimensions() op gf2.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), tgf1(), tRes());                                     \
                                                                              \
    tgf1.clear();                                                             \
                                                                              \
    return tRes;                                                              \
}

#define BINARY_OPERATOR_R(Type1, Type2, op, opName, opFunc)                   \
    BINARY_OPERATOR_FF(Type1, Type2, op, opName, opFunc)                      \
    BINARY_OPERATOR_FTR(Type1, Type2, op, opName, opFunc)                     \
    BINARY_OPERATOR_TRF(Type1, Type2, op, opName, opFunc)                     \
    BINARY_OPERATOR_TRT(Type1, Type2, op, opName, opFunc)

BINARY_OPERATOR_R(Type, Type, +, '+', add)
BINARY_OPERATOR_R(Type, Type, -, '-', subtract)

#if defined(__GNUC__) && !defined(__INTEL_COMPILER)
BINARY_OPERATOR_FF(scalar, Type, *, '*', multiply)
BINARY_OPERATOR_FTR(scalar, Type, *, '*', multiply)
BINARY_OPERATOR_TF(scalar, Type, *, '*', multiply)
BINARY_OPERATOR_TTR(scalar, Type, *, '*', multiply)
#endif

BINARY_OPERATOR_FF(Type, scalar, /, '|', divide)
BINARY_OPERATOR_FT(Type, scalar, /, '|', divide)
BINARY_OPERATOR_TRF(Type, scalar, /, '|', divide)
BINARY_OPERATOR_TRT(Type, scalar, /, '|', divide)

#undef BINARY_OPERATOR_R
#undef BINARY_OPERATOR_FF
#undef BINARY_OPERATOR_FTR
#undef BINARY_OPERATOR_TF
#undef BINARY_OPERATOR_TTR
#undef BINARY_OPERATOR_FT
#undef BINARY_OPERATOR_TRF
#undef BINARY_OPERATOR_TRT


#define BINARY_TYPE_OPERATOR_TF(TYPE, op, opName, opFunc)                     \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
void opFunc                                                                   \
(                                                                             \
    GeometricField<Type, PatchField, GeoMesh>& gf,                            \
    const dimensioned<TYPE>& dt,                                              \
    const GeometricField<Type, PatchField, GeoMesh>& gf1                      \
)                                                                             \
{                                                                             \
    Foam::opFunc(gf.field(), dt.value(), gf1.field());        \
    Foam::opFunc(gf.boundaryField(), dt.value(), gf1.boundaryField());        \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const dimensioned<TYPE>& dt,                                              \
    const GeometricField<Type, PatchField, GeoMesh>& gf1                      \
)                                                                             \
{                                                                             \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        new GeometricField<Type, PatchField, GeoMesh>                         \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + dt.name() + opName + gf1.name() + ')',                  \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            dt.dimensions() op gf1.dimensions()                               \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), dt, gf1);                                            \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const TYPE& t,                                                            \
    const GeometricField<Type, PatchField, GeoMesh>& gf1                      \
)                                                                             \
{                                                                             \
    return dimensioned<TYPE>(t) op gf1;                                       \
}                                                                             \
                                                                              \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const dimensioned<TYPE>& dt,                                              \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1               \
)                                                                             \
{                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1 = tgf1();            \
                                                                              \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        GeometricField<Type, PatchField, GeoMesh>::New                        \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + dt.name() + opName + gf1.name() + ')',                  \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            tgf1,                                                             \
            dt.dimensions() op gf1.dimensions()                               \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), dt, tRes());                                         \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const TYPE& t,                                                            \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1               \
)                                                                             \
{                                                                             \
    return dimensioned<TYPE>(t) op tgf1;                                      \
}


#define BINARY_TYPE_OPERATOR_FT(TYPE, op, opName, opFunc)                     \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
void opFunc                                                                   \
(                                                                             \
    GeometricField<Type, PatchField, GeoMesh>& gf,                            \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const dimensioned<TYPE>& dt                                               \
)                                                                             \
{                                                                             \
    Foam::opFunc(gf.field(), gf1.field(), dt.value());        \
    Foam::opFunc(gf.boundaryField(), gf1.boundaryField(), dt.value());        \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const dimensioned<TYPE>& dt                                               \
)                                                                             \
{                                                                             \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        new GeometricField<Type, PatchField, GeoMesh>                         \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + opName + dt.name() + ')',                  \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            gf1.dimensions() op dt.dimensions()                               \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), gf1, dt);                                            \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const TYPE& t                                                             \
)                                                                             \
{                                                                             \
    return gf1 op dimensioned<TYPE>(t);                                       \
}                                                                             \
                                                                              \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1,              \
    const dimensioned<TYPE>& dt                                               \
)                                                                             \
{                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1 = tgf1();            \
                                                                              \
    tmp<GeometricField<Type, PatchField, GeoMesh> > tRes                      \
    (                                                                         \
        GeometricField<Type, PatchField, GeoMesh>::New                        \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + opName + dt.name() + ')',                  \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            tgf1,                                                             \
            gf1.dimensions() op dt.dimensions()                               \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), tRes(), dt);                                         \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
tmp<GeometricField<Type, PatchField, GeoMesh> > operator op                   \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1,              \
    const TYPE& t                                                             \
)                                                                             \
{                                                                             \
    return tgf1 op dimensioned<TYPE>(t);                                      \
}

#define BINARY_TYPE_OPERATOR(TYPE, op, opName, opFunc)                        \
    BINARY_TYPE_OPERATOR_TF(TYPE, op, opName, opFunc)                         \
    BINARY_TYPE_OPERATOR_FT(TYPE, op, opName, opFunc)

BINARY_TYPE_OPERATOR(Type, +, '+', add)
BINARY_TYPE_OPERATOR(Type, -, '-', subtract)

BINARY_TYPE_OPERATOR(scalar, *, '*', multiply)
BINARY_TYPE_OPERATOR_FT(scalar, /, '|', divide)

#undef BINARY_TYPE_OPERATOR
#undef BINARY_TYPE_OPERATOR_TF
#undef BINARY_TYPE_OPERATOR_FT


#define PRODUCT_OPERATOR(product, op, opFunc)                                 \
                                                                              \
template                                                                      \
<class Type1, class Type2, template<class> class PatchField, class GeoMesh>   \
void opFunc                                                                   \
(                                                                             \
    GeometricField                                                            \
    <typename product<Type1, Type2>::type, PatchField, GeoMesh>& gf,          \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1,                    \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2                     \
)                                                                             \
{                                                                             \
    Foam::opFunc(gf.field(), gf1.field(), gf2.field());\
    Foam::opFunc(gf.boundaryField(), gf1.boundaryField(), gf2.boundaryField());\
}                                                                             \
                                                                              \
template                                                                      \
<class Type1, class Type2, template<class> class PatchField, class GeoMesh>   \
tmp                                                                           \
<                                                                             \
    GeometricField<typename product<Type1, Type2>::type, PatchField, GeoMesh> \
>                                                                             \
operator op                                                                   \
(                                                                             \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1,                    \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2                     \
)                                                                             \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes               \
    (                                                                         \
        new GeometricField<productType, PatchField, GeoMesh>                  \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + #op + gf2.name() + ')',                    \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            gf1.dimensions() op gf2.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), gf1, gf2);                                           \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<class Type1, class Type2, template<class> class PatchField, class GeoMesh>   \
tmp                                                                           \
<                                                                             \
    GeometricField<typename product<Type1, Type2>::type, PatchField, GeoMesh> \
>                                                                             \
operator op                                                                   \
(                                                                             \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1,                    \
    const tmp<GeometricField<Type2, PatchField, GeoMesh> >& tgf2              \
)                                                                             \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
                                                                              \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2 = tgf2();           \
                                                                              \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes               \
    (                                                                         \
        new GeometricField<productType, PatchField, GeoMesh>                  \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + #op + gf2.name() + ')',                    \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            gf1.dimensions() op gf2.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), gf1, gf2);                                           \
                                                                              \
    tgf2.clear();                                                             \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<class Type1, class Type2, template<class> class PatchField, class GeoMesh>   \
tmp                                                                           \
<                                                                             \
    GeometricField<typename product<Type1, Type2>::type, PatchField, GeoMesh> \
>                                                                             \
operator op                                                                   \
(                                                                             \
    const tmp<GeometricField<Type1, PatchField, GeoMesh> >& tgf1,             \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2                     \
)                                                                             \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
                                                                              \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1 = tgf1();           \
                                                                              \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes               \
    (                                                                         \
        new GeometricField<productType, PatchField, GeoMesh>                  \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + #op + gf2.name() + ')',                    \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            gf1.dimensions() op gf2.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), gf1, gf2);                                           \
                                                                              \
    tgf1.clear();                                                             \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<class Type1, class Type2, template<class> class PatchField, class GeoMesh>   \
tmp                                                                           \
<                                                                             \
    GeometricField<typename product<Type1, Type2>::type, PatchField, GeoMesh> \
>                                                                             \
operator op                                                                   \
(                                                                             \
    const tmp<GeometricField<Type1, PatchField, GeoMesh> >& tgf1,             \
    const tmp<GeometricField<Type2, PatchField, GeoMesh> >& tgf2              \
)                                                                             \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
                                                                              \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1 = tgf1();           \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2 = tgf2();           \
                                                                              \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes               \
    (                                                                         \
        new GeometricField<productType, PatchField, GeoMesh>                  \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + #op + gf2.name() + ')',                    \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            gf1.dimensions() op gf2.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), gf1, gf2);                                           \
                                                                              \
    tgf1.clear();                                                             \
    tgf2.clear();                                                             \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<class Form, class Type, template<class> class PatchField, class GeoMesh>     \
void opFunc                                                                   \
(                                                                             \
    GeometricField                                                            \
    <typename product<Type, Form>::type, PatchField, GeoMesh>& gf,            \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const dimensioned<Form>& dvs                                              \
)                                                                             \
{                                                                             \
    Foam::opFunc(gf.field(), gf1.field(), dvs.value());       \
    Foam::opFunc(gf.boundaryField(), gf1.boundaryField(), dvs.value());       \
}                                                                             \
                                                                              \
template                                                                      \
<class Form, class Type, template<class> class PatchField, class GeoMesh>     \
tmp<GeometricField<typename product<Type, Form>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const dimensioned<Form>& dvs                                              \
)                                                                             \
{                                                                             \
    typedef typename product<Type, Form>::type productType;                   \
                                                                              \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes               \
    (                                                                         \
        new GeometricField<productType, PatchField, GeoMesh>                  \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + #op + dvs.name() + ')',                    \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            gf1.dimensions() op dvs.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), gf1, dvs);                                           \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<                                                                             \
    class Form,                                                               \
    class Cmpt,                                                               \
    int nCmpt,                                                                \
    class Type, template<class> class PatchField,                             \
    class GeoMesh                                                             \
>                                                                             \
tmp<GeometricField<typename product<Form, Type>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const VectorSpace<Form,Cmpt,nCmpt>& vs                                    \
)                                                                             \
{                                                                             \
    return gf1 op dimensioned<Form>(static_cast<const Form&>(vs));            \
}                                                                             \
                                                                              \
                                                                              \
template                                                                      \
<class Form, class Type, template<class> class PatchField, class GeoMesh>     \
tmp<GeometricField<typename product<Type, Form>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1,              \
    const dimensioned<Form>& dvs                                              \
)                                                                             \
{                                                                             \
    typedef typename product<Type, Form>::type productType;                   \
                                                                              \
    const GeometricField<Type, PatchField, GeoMesh>& gf1 = tgf1();            \
                                                                              \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes               \
    (                                                                         \
        new GeometricField<productType, PatchField, GeoMesh>                  \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + #op + dvs.name() + ')',                    \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            gf1.dimensions() op dvs.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), gf1, dvs);                                           \
                                                                              \
    tgf1.clear();                                                             \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<                                                                             \
    class Form,                                                               \
    class Cmpt,                                                               \
    int nCmpt,                                                                \
    class Type, template<class> class PatchField,                             \
    class GeoMesh                                                             \
>                                                                             \
tmp<GeometricField<typename product<Form, Type>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1,              \
    const VectorSpace<Form,Cmpt,nCmpt>& vs                                    \
)                                                                             \
{                                                                             \
    return tgf1 op dimensioned<Form>(static_cast<const Form&>(vs));           \
}                                                                             \
                                                                              \
                                                                              \
template                                                                      \
<class Form, class Type, template<class> class PatchField, class GeoMesh>     \
void opFunc                                                                   \
(                                                                             \
    GeometricField                                                            \
    <typename product<Form, Type>::type, PatchField, GeoMesh>& gf,            \
    const dimensioned<Form>& dvs,                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1                      \
)                                                                             \
{                                                                             \
    Foam::opFunc(gf.field(), dvs.value(), gf1.field());       \
    Foam::opFunc(gf.boundaryField(), dvs.value(), gf1.boundaryField());       \
}                                                                             \
                                                                              \
template                                                                      \
<class Form, class Type, template<class> class PatchField, class GeoMesh>     \
tmp<GeometricField<typename product<Form, Type>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const dimensioned<Form>& dvs,                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1                      \
)                                                                             \
{                                                                             \
    typedef typename product<Form, Type>::type productType;                   \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes               \
    (                                                                         \
        new GeometricField<productType, PatchField, GeoMesh>                  \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + dvs.name() + #op + gf1.name() + ')',                    \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            dvs.dimensions() op gf1.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), dvs, gf1);                                           \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<                                                                             \
    class Form,                                                               \
    class Cmpt,                                                               \
    int nCmpt,                                                                \
    class Type, template<class> class PatchField,                             \
    class GeoMesh                                                             \
>                                                                             \
tmp<GeometricField<typename product<Form, Type>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const VectorSpace<Form,Cmpt,nCmpt>& vs,                                   \
    const GeometricField<Type, PatchField, GeoMesh>& gf1                      \
)                                                                             \
{                                                                             \
    return dimensioned<Form>(static_cast<const Form&>(vs)) op gf1;            \
}                                                                             \
                                                                              \
template                                                                      \
<class Form, class Type, template<class> class PatchField, class GeoMesh>     \
tmp<GeometricField<typename product<Form, Type>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const dimensioned<Form>& dvs,                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1               \
)                                                                             \
{                                                                             \
    typedef typename product<Form, Type>::type productType;                   \
                                                                              \
    const GeometricField<Type, PatchField, GeoMesh>& gf1 = tgf1();            \
                                                                              \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes               \
    (                                                                         \
        new GeometricField<productType, PatchField, GeoMesh>                  \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + dvs.name() + #op + gf1.name() + ')',                    \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            dvs.dimensions() op gf1.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    Foam::opFunc(tRes(), dvs, gf1);                                           \
                                                                              \
    tgf1.clear();                                                             \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<                                                                             \
    class Form,                                                               \
    class Cmpt,                                                               \
    int nCmpt,                                                                \
    class Type, template<class> class PatchField,                             \
    class GeoMesh                                                             \
>                                                                             \
tmp<GeometricField<typename product<Form, Type>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const VectorSpace<Form,Cmpt,nCmpt>& vs,                                   \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1               \
)                                                                             \
{                                                                             \
    return dimensioned<Form>(static_cast<const Form&>(vs)) op tgf1;           \
}                                                                             \
                                                                              \

PRODUCT_OPERATOR(outerProduct, *, outer)
PRODUCT_OPERATOR(crossProduct, ^, cross)
PRODUCT_OPERATOR(innerProduct, &, dot)
PRODUCT_OPERATOR(scalarProduct, &&, dotdot)

#undef PRODUCT_OPERATOR


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
