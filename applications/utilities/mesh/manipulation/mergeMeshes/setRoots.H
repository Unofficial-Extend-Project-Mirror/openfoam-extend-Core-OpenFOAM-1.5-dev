    argList::validArgs.clear();

    argList::validArgs.append("master root");
    argList::validArgs.append("master case");

    argList::validArgs.append("root to add");
    argList::validArgs.append("case to add");

    argList args(argc, argv);

    if (!args.check())
    {
         FatalError.exit();
    }

    fileName rootDirMaster(args.args()[1]);
    fileName caseDirMaster(args.args()[2]);

    fileName rootDirToAdd(args.args()[3]);
    fileName caseDirToAdd(args.args()[4]);

    Info<< "Master:      " << rootDirMaster << " " << caseDirMaster << nl
        << "mesh to add: " << rootDirToAdd << " " << caseDirToAdd << endl;
