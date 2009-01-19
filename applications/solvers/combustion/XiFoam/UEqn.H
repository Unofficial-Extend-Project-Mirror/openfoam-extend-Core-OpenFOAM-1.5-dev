    fvVectorMatrix UEqn
    (
        fvm::ddt(rho, U)
      + fvm::div(phi, U)
      + turbulence->divRhoR(U)
     ==
        rho*g
    );

    if (momentumPredictor)
    {
        solve(UEqn == -fvc::grad(p));
    }
