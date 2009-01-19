    fvVectorMatrix UEqn
    (
        fvm::ddt(U)
      + fvm::div(phi, U)
      + turbulence->divR(U)
    );

    if (momentumPredictor)
    {
        solve(UEqn == -fvc::grad(p));
    }
