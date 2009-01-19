if (runTime.outputTime())
{
    // Local free surface curvature and flux
    if(Pstream::master())
    {
        OFstream file
            (
                runTime.timePath()/"fsData.dat"
            );
        
        const vectorField& C = 
            mesh.Cf().boundaryField()[interface.aPatchID()];

        file<< "X" << tab 
            << "Y" << tab 
            << "Z" << endl;

        forAll(C, faceI)
        {
            file<< C[faceI].x() << tab 
                << C[faceI].y() << tab
                << C[faceI].z() << endl;
        }
    }
}
