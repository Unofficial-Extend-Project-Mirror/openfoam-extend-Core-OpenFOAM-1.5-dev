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

Description

\*---------------------------------------------------------------------------*/

package PatchTool.Mesh;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.awt.event.ActionEvent;
import javax.swing.event.*;

import javax.media.j3d.*;

import PatchTool.Util.FoamDataParser;
import PatchTool.Mesh.BoundaryMesh;
import PatchTool.Events.ElementEvent;
import PatchTool.Events.ElementSelectListener;


public class FaceSet extends SelectedSet
{

    //--------------------------------------------------------------------------

    /** Construct from components */
    public FaceSet(int len)
    {
        super(len);
    }

    //--------------------------------------------------------------------------

    /** Construct from stream */
    public FaceSet(Reader readStream)
    {
        super(readStream);
    }

    //--------------------------------------------------------------------------

    public void show(BoundaryMesh bMesh, PrintWriter wr)
    {
        wr.println("Face\tPatch\tName");
        wr.println("----\t-----\t----");

        for (int faceI = 0; faceI < selected_.length; faceI++)
        {
            if (selected_[faceI])
            {
                int patchI = bMesh.whichPatch(faceI);

                SurfacePatch sp = bMesh.getPatches()[patchI];

                wr.println(faceI + "\t" + patchI + "\t" + sp.getName());
            }
        }
        wr.flush();
    }

    //--------------------------------------------------------------------------

    /** Adds all faces from patch */
    public void selectPatch(BoundaryMesh bMesh, String patchName)
    {
        SurfacePatch sp = bMesh.getPatch(patchName);

        for 
        (
            int faceI = sp.getStart();
            faceI < sp.getStart() + sp.getSize();
            faceI++
        )
        {
            silentSelect(faceI);
        }
        fireSetChanged(getSelected());
    }

    //--------------------------------------------------------------------------

    /** Adds all faces from patch */
    public void deselectPatch(BoundaryMesh bMesh, String patchName)
    {
        SurfacePatch sp = bMesh.getPatch(patchName);

        for 
        (
            int faceI = sp.getStart();
            faceI < sp.getStart() + sp.getSize();
            faceI++
        )
        {
            silentDeselect(faceI);
        }
        fireSetChanged(getSelected());
    }
}
