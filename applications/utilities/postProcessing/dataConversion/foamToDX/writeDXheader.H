{
    // Translating Foam types to DX types
    // Lists for DX data description

    wordList DXstructureList;
    DXstructureList.setSize(nFields);
    wordList DXtypeList;
    DXtypeList.setSize(nFields);

    for (int n=0; n<nFields; n++)
    {
        DXtypeList = "float";

        if (isScalar[n])
        {
            DXstructureList[n] = "scalar";
        }
        else
        {
            DXstructureList[n] = "3-vector";
        }

        word writeFmt = "ascii";

        /* This will not work unless the streams are opened binary
        if (runTime.writeFormat() == IOstream::BINARY)
        {
            writeFmt = "ascii";
        }
        else
        {
            writeFmt = "binary";
        }
        */

        fileName generalFile(dxDataPath/"dx.general");
        fileName dataFile(dxDataPath/Times[i].name());
        OFstream dxGeneral(generalFile);

        dxGeneral<< "file = " << dataFile << endl;
        dxGeneral<< "points = " << mesh.nCells() << endl;
        dxGeneral<< "format = " << writeFmt << endl ;
        dxGeneral<< "interleaving = field" << endl ;
        dxGeneral<< "field = " << "locations";

        for (int n=0; n<nFields; n++)
        {
            dxGeneral<< ", " << fieldNameList[n];
        }
        dxGeneral << endl;

        dxGeneral<< "structure = 3-vector";
        for (int n=0; n<nFields; n++)
        {
            if (isScalar[n])
            {
                dxGeneral<< ", scalar";
            }
            else
            {
                dxGeneral<< ", 3-vector";
            }
        }
        dxGeneral<< endl;

        dxGeneral<< "type = float";
        for (int n=0; n<nFields; n++)
        {
            dxGeneral<< ", float";
        }
        dxGeneral<< endl;
}
