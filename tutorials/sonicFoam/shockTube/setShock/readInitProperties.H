IOdictionary initProperties
(
    IOobject
    (
        "initProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    )
);


scalar x0
(
    readScalar(initProperties.lookup("x0"))
);

scalar pL
(
    readScalar(initProperties.lookup("pL"))
);
scalar TL
(
    readScalar(initProperties.lookup("TL"))
);
vector UL
(
    initProperties.lookup("UL")
);

scalar pR
(
    readScalar(initProperties.lookup("pR"))
);
scalar TR
(
    readScalar(initProperties.lookup("TR"))
);
vector UR
(
    initProperties.lookup("UR")
);


