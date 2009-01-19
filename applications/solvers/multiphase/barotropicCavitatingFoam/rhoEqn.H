{
    tmp<fv::convectionScheme<scalar> > divRho
    (
        fv::convectionScheme<scalar>::New
        (
            mesh,
            phi,
            mesh.divScheme("div(phi,rho)")
        )
    );

    solve
    (
        fvm::ddt(rho)
      + divRho->fvmDiv(phi, rho)
    );

    rhoPhi = divRho->flux(phi, rho);

    Info<< "rho: " << max(rho).value() << tab << min(rho).value() << tab
        << "p:" << max(p).value() << tab << min(p).value() << endl;

    volScalarField rhog = max(p*psig, 0.1*rhogSat);
    volScalarField rhol = rholSat + (p - pSat)*psil;

    gamma = min(max((rhol - rho)/(rhol - rhog), 0.0), 1.0);

    psiByRho =
        (gamma*rhog + (1.0 - gamma)*rhol)
       *(gamma*psig/rhog + (1.0 - gamma)*psil/rhol)/rho;
}
