
    fvVectorMatrix UEqn
    (
        fvm::ddt(rho, U)
      + fvm::div(phi, U)
      + turbulence->divRhoR(U)
    );

    UEqn.relax();

    solve(UEqn == -fvc::grad(p));
