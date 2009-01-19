    word meshName;

    if (args.options().found("mesh"))
    {
        meshName = args.options()["mesh"];

        Info<< "Create polyMesh " << meshName << " for time = "
            << runTime.timeName() << nl << endl;
    }
    else
    {
        meshName = polyMesh::defaultRegion;

        Info<< "Create polyMesh for time = "
            << runTime.timeName() << nl << endl;
    }

    polyMesh mesh
    (
        IOobject
        (
            meshName,
            runTime.timeName(),
            runTime,
            IOobject::MUST_READ
        )
    );
