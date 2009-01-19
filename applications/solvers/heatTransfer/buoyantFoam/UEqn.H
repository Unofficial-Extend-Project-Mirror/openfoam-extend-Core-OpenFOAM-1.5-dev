    // Solve the Momentum equation

    fvVectorMatrix UEqn
    (
        fvm::ddt(rho, U)
      + fvm::div(phi, U)
      + turbulence->divRhoR(U)
    );

    solve(UEqn == -fvc::grad(pd) - fvc::grad(rho)*gh);
