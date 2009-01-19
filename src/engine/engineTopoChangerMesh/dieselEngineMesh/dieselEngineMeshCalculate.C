// The FOAM Project // File: dieselEngineMesh.C
/* 
-------------------------------------------------------------------------------
 =========         | Class Implementation
 \\      /         |
  \\    /          | Name:   dieselEngineMesh
   \\  /           | Family: engine
    \\/            |
    F ield         | FOAM version: 2.3
    O peration     |
    A and          | Copyright (C) 1991-2004 Nabla Ltd.
    M anipulation  |          All Rights Reserved.
-------------------------------------------------------------------------------
DESCRIPTION

AUTHOR 

-------------------------------------------------------------------------------
*/

#include "dieselEngineMesh.H"
#include "slidingInterface.H"
#include "layerAdditionRemoval.H"
#include "surfaceFields.H"
#include "regionSplit.H"
#include "componentMixedTetPolyPatchVectorField.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


