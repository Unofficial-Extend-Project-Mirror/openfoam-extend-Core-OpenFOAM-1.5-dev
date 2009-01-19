{
    fileName dxDataFile(dxDataPath/Times[i].name());

    Info<< "Checking timestep " << Times[i].name() << endl;

    nFields = 0;
    for (int n=0; n<nFieldsTmp; n++)
    {
        word fieldName = fieldNameList[n];

        IOobject ioFieldObject
        (
            fieldName,
            runTime.timeName(),
            mesh,
            IOobject::NO_READ
        );

        if (ioFieldObject.headerOk())
        {
            fieldNameList[nFields++] = fieldName;
        }
    }
    fieldNameList.setSize(nFields);
}
