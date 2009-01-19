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

#include "parcel.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

parcel::parcel
(
    const Cloud<parcel>& cloud,
    Istream& is,
    bool readFields
)
:
    particle<parcel>(cloud, is),
    
    fuelNames_
    (
        (cloud.pMesh().lookupObject<dictionary>("thermophysicalProperties"))
       .lookup("liquidFuelComponents")
    ),
    X_(fuelNames_.size(), 0.0),
    
    tMom_(GREAT)
{

    label nX = fuelNames_.size();

    if (readFields)
    {
        if (is.format() == IOstream::ASCII)
        {
            d_ = readScalar(is);
            T_ = readScalar(is);
            m_ = readScalar(is);
            y_ = readScalar(is);
            yDot_ = readScalar(is);
            ct_ = readScalar(is);
            ms_ = readScalar(is);
            tTurb_ = readScalar(is);
            liquidCore_ = readScalar(is);
            injector_ = readScalar(is);
            is >> U_;
            is >> Uturb_;
            is >> n_;
            for(label j=0; j<nX; j++)
            {
                X_[j] = readScalar(is);
            }
        }
        else
        {
            is.read
            (
                reinterpret_cast<char*>(&d_),
                sizeof(d_) + sizeof(T_) + sizeof(m_) + sizeof(y_)
              + sizeof(yDot_) + sizeof(ct_) + sizeof(ms_) + sizeof(tTurb_) 
              + sizeof(liquidCore_) + sizeof(injector_) 
              + sizeof(U_) + sizeof(Uturb_) + sizeof(n_)
            );

            is.read
            (
                reinterpret_cast<char*>(X_.begin()),
                X_.size()*sizeof(scalar)
            );
        }
    }

    // Check state of Istream
    is.check("parcel::parcel(Istream&)");
}


template<>
void Cloud<parcel>::readFields()
{
    IOField<scalar> d(fieldIOobject("d"));
    IOField<scalar> T(fieldIOobject("T"));
    IOField<scalar> m(fieldIOobject("m"));
    IOField<scalar> y(fieldIOobject("y"));
    IOField<scalar> yDot(fieldIOobject("yDot"));
    IOField<scalar> ct(fieldIOobject("ct"));
    IOField<scalar> ms(fieldIOobject("ms"));
    IOField<scalar> tTurb(fieldIOobject("tTurb"));
    IOField<scalar> liquidCore(fieldIOobject("liquidCore"));
    IOField<scalar> injector(fieldIOobject("injector"));

    IOField<vector> U(fieldIOobject("U"));
    IOField<vector> Uturb(fieldIOobject("Uturb"));
    IOField<vector> n(fieldIOobject("n"));

    label i = 0;
    for
    (
        Cloud<parcel>::iterator iter = begin();
        iter != end();
        ++iter, ++i
    )
    {
        parcel& p = iter();

        p.d_ = d[i];
        p.T_ = T[i];
        p.m_ = m[i];
        p.y_ = y[i];
        p.yDot_ = yDot[i];
        p.ct_ = ct[i];
        p.ms_ = ms[i];
        p.tTurb_ = tTurb[i];
        p.liquidCore_ = liquidCore[i];
        p.injector_ = injector[i];

        p.U_ = U[i];
        p.Uturb_ = Uturb[i];
        p.n_ = n[i];
    }


    // read the liquid molar fractions
    if (size() > 0)
    {
        Cloud<parcel>::const_iterator iter = begin();
        const parcel& p0 = iter();

        label nX = p0.X().size();
        List<word> names(p0.fuelNames());

        for(label j=0; j<nX; j++)
        {

            IOField<scalar> X(fieldIOobject(names[j]));

            label i = 0;
            for
            (
                Cloud<parcel>::iterator iter = begin();
                iter != end();
                ++iter, ++i
            )
            {
                parcel& p = iter();
                p.X_[j] = X[i];
            }
        }
    }
}


template<>
void Cloud<parcel>::writeFields() const
{
    label np = size();

    IOField<scalar> d(fieldIOobject("d"), np);
    IOField<scalar> T(fieldIOobject("T"), np);
    IOField<scalar> m(fieldIOobject("m"), np);
    IOField<scalar> y(fieldIOobject("y"), np);
    IOField<scalar> yDot(fieldIOobject("yDot"), np);
    IOField<scalar> ct(fieldIOobject("ct"), np);
    IOField<scalar> ms(fieldIOobject("ms"), np);
    IOField<scalar> tTurb(fieldIOobject("tTurb"), np);
    IOField<scalar> liquidCore(fieldIOobject("liquidCore"), np);
    IOField<scalar> injector(fieldIOobject("injector"), np);

    IOField<vector> U(fieldIOobject("U"), np);
    IOField<vector> Uturb(fieldIOobject("Uturb"), np);
    IOField<vector> n(fieldIOobject("n"), np);

    label i = 0;
    for
    (
        Cloud<parcel>::const_iterator iter = begin();
        iter != end();
        ++iter, ++i
    )
    {
        const parcel& p = iter();

        d[i] = p.d_;
        T[i] = p.T_;
        m[i] = p.m_;
        y[i] = p.y_;
        yDot[i] = p.yDot_;
        ct[i] = p.ct_;
        ms[i] = p.ms_;
        tTurb[i] = p.tTurb_;
        liquidCore[i] = p.liquidCore_;
        injector[i] = p.injector_;

        U[i] = p.U_;
        Uturb[i] = p.Uturb_;
        n[i] = p.n_;
    }

    d.write();
    T.write();
    m.write();
    y.write();
    yDot.write();
    ct.write();
    ms.write();
    tTurb.write();
    liquidCore.write();
    injector.write();

    U.write();
    Uturb.write();
    n.write();

    // write the liquid molar fractions
    if (np > 0)
    {
        Cloud<parcel>::const_iterator iter = begin();
        const parcel& p0 = iter();

        label nX = p0.X().size();

        List<word> names(p0.fuelNames());

        for(label j=0; j<nX; j++)
        {
            IOField<scalar> X(fieldIOobject(names[j]), np);

            label i = 0;
            for
            (
                Cloud<parcel>::const_iterator iter = begin();
                iter != end();
                ++iter, ++i
            )
            {
                const parcel& p = iter();
                X[i] = p.X()[j];
            }

            X.write();
        }
    }
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Ostream& operator<<(Ostream& os, const parcel& p)
{

    if (os.format() == IOstream::ASCII)
    {
        os  << static_cast<const particle<parcel>&>(p)
            << token::SPACE << p.d_
            << token::SPACE << p.T_
            << token::SPACE << p.m_
            << token::SPACE << p.y_
            << token::SPACE << p.yDot_
            << token::SPACE << p.ct_
            << token::SPACE << p.ms_
            << token::SPACE << p.tTurb_
            << token::SPACE << p.liquidCore_
            << token::SPACE << p.injector_
            << token::SPACE << p.U_
            << token::SPACE << p.Uturb_
            << token::SPACE << p.n_
            << token::SPACE << p.X_;
    }
    else
    {
        os  << static_cast<const particle<parcel>&>(p);
        os.write
        (
            reinterpret_cast<const char*>(&p.d_),
            sizeof(p.d_) + sizeof(p.T_) + sizeof(p.m_) + sizeof(p.y_)
          + sizeof(p.yDot_) + sizeof(p.ct_) + sizeof(p.ms_) + sizeof(p.tTurb_) 
          + sizeof(p.liquidCore_) + sizeof(p.injector_)
          + sizeof(p.U_) + sizeof(p.Uturb_) + sizeof(p.n_)
        );

        os.write
        (
            reinterpret_cast<const char*>(p.X_.begin()),
            p.X_.size()*sizeof(scalar)
        );
    }

    // Check state of Ostream
    os.check("Ostream& operator<<(Ostream&, const parcel&)");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
