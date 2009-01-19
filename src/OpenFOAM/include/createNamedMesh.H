    word meshName;

    if (args.options().found("mesh"))
    {
        meshName = args.options()["mesh"];

        Info<< "Create mesh " << meshName << " for time = "
            << runTime.timeName() << nl << endl;
    }
    else
    {
        meshName = fvMesh::defaultRegion;

        Info<< "Create mesh for time = "
            << runTime.timeName() << nl << endl;
    }

    fvMesh mesh
    (
        IOobject
        (
            meshName,
            runTime.timeName(),
            runTime,
            IOobject::MUST_READ
        )
    );
