    fvVectorMatrix UEqn
    (
        fvm::ddt(rho, U)
      + fvm::div(phi, U)
      + turbulence->divRhoR(U)
    );

    if (momentumPredictor)
    {
        solve(UEqn == -fvc::grad(p));
    }
