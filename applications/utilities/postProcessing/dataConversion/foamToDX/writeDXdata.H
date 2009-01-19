{
    dataOut
        << "### GRID----------------------------------------------------"
        << endl
        << "object \"points\" class array type float rank 1 shape 3 items "
        << mesh.nPoints() << " data file dxPoints " << nl
        << "object \"shapes\" class array type int rank 1 shape 8 items "
        << mesh.nCells() << " data file dxCells " << nl
        << "attribute \"element type\" string \"cubes\"" << nl
        << "attribute \"ref\" string \"positions\" " << nl
        << "### DATA----------------------------------------------------"
        << endl;

    SLList<word> validFields;

    for (label i=0; i<nTypes; i++)
    {
        wordList fieldNames = objects.names(fieldTypes[i]);

        for (label j=0; j<fieldNames.size(); j++)
        {
            word fieldName = fieldNames[j];

            IOobject fieldObject
            (
                fieldName,
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            );

            if (fieldObject.headerOk())
            {
                if
                (
                    fieldObject.headerClassName()
                 == volScalarField::typeName
                )
                {
                    validFields.append(fieldName);

                    volScalarField DXScalarField(fieldObject, mesh);

                    pointScalarField tmp
                    (
                        pInterp.interpolate(DXScalarField)
                    );

                    dataOut
                        << "object " << "\"" << fieldName << "data\""
                        << " class array type float rank 0 items "
                        << tmp.size() << " data follows"
                        << endl;

                    forAll(tmp, i)
                    {
                        dataOut<< tmp[i] << endl;
                    }

                    dataOut
                        << "attribute \"dep\" string \"positions\" "
                        << endl;
                }
                else if
                (
                    fieldObject.headerClassName()
                 == volVectorField::typeName
                )
                {
                    validFields.append(fieldName);

                    volVectorField DXVectorField(fieldObject, mesh);

                    pointVectorField tmp
                    (
                        pInterp.interpolate(DXVectorField)
                    );

                    dataOut
                        << "object " << "\"" << fieldName << "data\""
                        << " class array type float rank 1 shape 3 items "
                        << tmp.size() << " data follows"
                       << endl;

                    forAll(tmp, i)
                    {
                        dataOut
                            << tmp[i].x() << tab
                            << tmp[i].y() << tab
                            << tmp[i].z() << endl;
                    }

                    dataOut
                        << "attribute \"dep\" string \"positions\" "
                        << endl;
                }
            }
        }
    }


    dataOut
        << "### FIELDS---------------------------------------------------"
        << endl;

    for
    (
        SLList<word>::iterator fieldIter = validFields.begin();
        fieldIter != validFields.end();
        ++fieldIter
    )
    {
        dataOut
            << "object " << "\"" << fieldIter() << "\"" << " class field" << nl
            << "component \"positions\" value \"points\"" << nl
            << "component \"connections\" value \"shapes\"" << nl
            << "component " << "\"data\" " << "value \""
            << fieldIter() << "data\"" << nl
            << "attribute " << "\"name\" string " << "\""
            << fieldIter() << "\"" << nl
            << endl;
    }

    dataOut
        << "### GROUP----------------------------------------------------"
        << endl
        << "object \"FIELDS\" class group"
        << endl;

    for
    (
        SLList<word>::iterator fieldIter = validFields.begin();
        fieldIter != validFields.end();
        ++fieldIter
    )
    {
        dataOut
            << "member \"" <<  fieldIter()
            << "\" value \"" << fieldIter() << "\"" << nl;
    }

    dataOut << "\nend" << endl;
}
