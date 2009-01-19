    // Construct the Momentum equation

    tmp<fvVectorMatrix> UEqn
    (
        fvm::div(phi, U)
      - fvm::Sp(fvc::div(phi), U)
      + turbulence->divRhoR(U)
    );

    UEqn().relax();

    // Include the porous media resistance and solve the momentum equation
    // either implicit in the tensorial resistance or transport using by
    // including the spherical part of the resistance in the momentum diagonal

    tmp<volScalarField> trAU;
    tmp<volTensorField> trTU;

    if (pressureImplicitPorousity)
    {
        tmp<volTensorField> tTU = tensor(I)*UEqn().A();
        pZones.addResistance(UEqn(), tTU());
        trTU = inv(tTU);
        trTU().rename("rAU");

        U = trTU() & (UEqn().H() - fvc::grad(p));
        U.correctBoundaryConditions();
    }
    else
    {
        pZones.addResistance(UEqn());

        solve(UEqn() == -fvc::grad(p));

        trAU = 1.0/UEqn().A();
        trAU().rename("rAU");
    }
