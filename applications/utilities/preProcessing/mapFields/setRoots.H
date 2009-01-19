    argList::validArgs.clear();

    argList::validArgs.append("source root");
    argList::validArgs.append("source case");

    argList::validArgs.append("target root");
    argList::validArgs.append("target case");

    argList::validOptions.erase(argList::validOptions.find("parallel"));
    argList::validOptions.insert("parallelSource", "");
    argList::validOptions.insert("parallelTarget", "");
    argList::validOptions.insert("consistent", "");

    argList args(argc, argv);

    if (!args.check())
    {
         FatalError.exit();
    }

    fileName rootDirSource(args.args()[1]);
    fileName caseDirSource(args.args()[2]);

    fileName rootDirTarget(args.args()[3]);
    fileName caseDirTarget(args.args()[4]);

    Info<< "Source: " << rootDirSource << " " << caseDirSource << nl
        << "Target: " << rootDirTarget << " " << caseDirTarget << endl;

    bool parallelSource = false;
    if (args.options().found("parallelSource"))
    {
        parallelSource = true;
    }

    bool parallelTarget = false;
    if (args.options().found("parallelTarget"))
    {
        parallelTarget = true;
    }

    bool consistent = false;
    if (args.options().found("consistent"))
    {
        consistent = true;
    }
