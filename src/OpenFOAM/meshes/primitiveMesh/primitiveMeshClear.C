/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

\*---------------------------------------------------------------------------*/

#include "primitiveMesh.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void primitiveMesh::printAllocated() const
{
    Pout<< "primitiveMesh allocated :" << endl;

    // Topology
    if (cellShapesPtr_)
    {
        Pout<< "    Cell shapes" << endl;
    }

    if (edgesPtr_)
    {
        Pout<< "    Edges" << endl;
    }

    if (ccPtr_)
    {
        Pout<< "    Cell-cells" << endl;
    }

    if (ecPtr_)
    {
        Pout<< "    Edge-cells" << endl;
    }

    if (pcPtr_)
    {
        Pout<< "    Point-cells" << endl;
    }

    if (cfPtr_)
    {
        Pout<< "    Cell-faces" << endl;
    }

    if (efPtr_)
    {
        Pout<< "    Edge-faces" << endl;
    }

    if (pfPtr_)
    {
        Pout<< "    Point-faces" << endl;
    }

    if (cePtr_)
    {
        Pout<< "    Cell-edges" << endl;
    }

    if (fePtr_)
    {
        Pout<< "    Face-edges" << endl;
    }

    if (pePtr_)
    {
        Pout<< "    Point-edges" << endl;
    }

    if (ppPtr_)
    {
        Pout<< "    Point-point" << endl;
    }
    
    if (cpPtr_)
    {
        Pout<< "    Cell-point" << endl;
    }

    // Geometry
    if (cellCentresPtr_)
    {
        Pout<< "    Cell-centres" << endl;
    }

    if (faceCentresPtr_)
    {
        Pout<< "    Face-centres" << endl;
    }

    if (cellVolumesPtr_)
    {
        Pout<< "    Cell-volumes" << endl;
    }

    if (faceAreasPtr_)
    {
        Pout<< "    Face-areas" << endl;
    }

}


void primitiveMesh::clearGeom()
{
    if (debug)
    {
        Pout<< "primitiveMesh::clearGeom() : "
            << "clearing geometric data"
            << endl;
    }

    deleteDemandDrivenData(cellCentresPtr_);
    deleteDemandDrivenData(faceCentresPtr_);
    deleteDemandDrivenData(cellVolumesPtr_);
    deleteDemandDrivenData(faceAreasPtr_);
}


void primitiveMesh::clearAddressing()
{
    if (debug)
    {
        Pout<< "primitiveMesh::clearAddressing() : "
            << "clearing topology"
            << endl;
    }

    deleteDemandDrivenData(cellShapesPtr_);

    clearOutEdges();

    deleteDemandDrivenData(ccPtr_);
    deleteDemandDrivenData(ecPtr_);
    deleteDemandDrivenData(pcPtr_);

    deleteDemandDrivenData(cfPtr_);
    deleteDemandDrivenData(efPtr_);
    deleteDemandDrivenData(pfPtr_);

    deleteDemandDrivenData(cePtr_);
    deleteDemandDrivenData(fePtr_);
    deleteDemandDrivenData(pePtr_);
    deleteDemandDrivenData(ppPtr_);
    deleteDemandDrivenData(cpPtr_);
}


void primitiveMesh::clearOut()
{
    clearGeom();
    clearAddressing();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
