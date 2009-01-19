{
    const pointField& points = mesh.points();
    const cellShapeList& cells = mesh.cellShapes();

    fileName dxPosFile(dxDataPath/"dxPoints");
    OFstream pointOut(dxPosFile);

    forAll(points, pointi)
    {
        pointOut
            << points[pointi].x() << tab
            << points[pointi].y() << tab
            << points[pointi].z()
            << endl;
    }
    pointOut << endl;


    dxPosFile = dxDataPath/"dxCells";
    OFstream shapeOut(dxPosFile);

    forAll(cells, celli)
    {
        const labelList& shapeLabels = cells[celli];

        switch(shapeLabels.size())
        {
            case 8:  // hex
                shapeOut
                    << shapeLabels[0] << tab
                    << shapeLabels[1] << tab
                    << shapeLabels[3] << tab
                    << shapeLabels[2] << tab
                    << shapeLabels[4] << tab
                    << shapeLabels[5] << tab
                    << shapeLabels[7] << tab
                    << shapeLabels[6] << endl;
            break;

            case 7:  // wedge
                shapeOut
                    << shapeLabels[0] << tab
                    << shapeLabels[1] << tab
                    << shapeLabels[0] << tab
                    << shapeLabels[2] << tab
                    << shapeLabels[3] << tab
                    << shapeLabels[4] << tab
                    << shapeLabels[6] << tab
                    << shapeLabels[5] << endl;
            break;

            case 6:  // prism
                shapeOut
                    << shapeLabels[0] << tab
                    << shapeLabels[1] << tab
                    << shapeLabels[0] << tab
                    << shapeLabels[2] << tab
                    << shapeLabels[3] << tab
                    << shapeLabels[4] << tab
                    << shapeLabels[3] << tab
                    << shapeLabels[5] << endl;
            break;

            case 5:  // pyramid
                shapeOut
                    << shapeLabels[0] << tab
                    << shapeLabels[1] << tab
                    << shapeLabels[3] << tab
                    << shapeLabels[2] << tab
                    << shapeLabels[4] << tab
                    << shapeLabels[4] << tab
                    << shapeLabels[4] << tab
                    << shapeLabels[4] << endl;
            break;

            case 4:  // tet
                shapeOut
                    << shapeLabels[0] << tab
                    << shapeLabels[1] << tab
                    << shapeLabels[0] << tab
                    << shapeLabels[2] << tab
                    << shapeLabels[3] << tab
                    << shapeLabels[3] << tab
                    << shapeLabels[3] << tab
                    << shapeLabels[3] << endl;
            break;

            default:
                FatalErrorIn(args.executable())
                    << "Wrong number of vertices in cell\n"
                    << "    expected 4,5,6,7 or 8, found " << shapeLabels.size()
                    << abort(FatalError);
            break;
        }
    }
    shapeOut << endl;
}
