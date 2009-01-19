    // Solve the Momentum equation

    tmp<fvVectorMatrix> UEqn
    (
        fvm::div(phi, U)
      - fvm::Sp(fvc::div(phi), U)
      + turbulence->divRhoR(U)
    );

    UEqn().relax();

    solve(UEqn() == -fvc::grad(pd) - fvc::grad(rho)*gh);
