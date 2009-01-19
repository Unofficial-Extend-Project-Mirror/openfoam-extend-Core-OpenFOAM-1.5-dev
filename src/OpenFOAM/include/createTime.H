    Info<< "Create time\n" << endl;

    Foam::Time runTime
    (
        Foam::Time::controlDictName,
        args.rootPath(),
        args.caseName()
    );
