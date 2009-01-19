    fvVectorMatrix UEqn
    (
        fvm::ddt(U)
      + fvm::div(phi, U)
      + turbulence->divR(U)
    );

    if (ocorr != nOuterCorr-1)
    {
        UEqn.relax();
    }

    if (momentumPredictor)
    {
        solve(UEqn == -fvc::grad(p));
    }
