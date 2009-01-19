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

package PatchTool.Util;

import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.awt.Container;
import java.awt.GridLayout;
import java.awt.Dimension;
import java.awt.event.*;
import java.awt.event.ActionEvent;
import java.awt.Graphics;
import java.awt.GraphicsConfiguration;
import java.io.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;

import com.sun.j3d.utils.universe.*;
import javax.media.j3d.*;
import javax.vecmath.*;


import PatchTool.PatchTool;
import PatchTool.Events.ElementEvent;
import PatchTool.Events.ElementSelectListener;
import PatchTool.Mesh.FaceSet;

public class MyFileSystemView implements ElementSelectListener
{
    //--------------------------------------------------------------------------

    protected PatchTool pp_;

    // Whether to select(true) or deselect(false) loop faces
    protected boolean selectLoop_;

    //--------------------------------------------------------------------------
    /** Creates new form MyFileSystemView */
    public MyFileSystemView(PatchTool pp, boolean selectLoop)
    {
        pp_ = pp;

        selectLoop_ = selectLoop;

        pp_.getMesh().getFaceSet().addElementSelectListener(this);
    }

    //--------------------------------------------------------------------------
    //---- ElementSelectListener Implementation
    //--------------------------------------------------------------------------

    public void elementSelected(ElementEvent evt)
    {
        FaceSet fs = pp_.getMesh().getFaceSet();

        fs.removeElementSelectListener(this);

        System.out.println("MyFileSystemView::elementSelected : clicked on "
            + " face " + evt.getElem());

        boolean[] reachable = new boolean[pp_.getMesh().getNFaces()];

        boolean[] currentSelection = fs.getSelected();

        if (selectLoop_)
        {
            // Get new selection based on floodfilling from picked face.
            // Two strategies:
            // - start from empty set. See which faces reachable. Merge into 
            //   existing.
            // - start from current set. See which faces reachable.
            // Second one sees currently selected faces as obstacles, first one
            // doesn't.

            //- Take over currently selected faces (Note:will block walking)
            for (int faceI = 0; faceI < currentSelection.length; faceI++)
            {
                //reachable[faceI] = currentSelection[faceI];
                reachable[faceI] = false;
            }

            // Get all faces reachable from selected face.
            pp_.getMesh().markLoopFaces(evt.getElem(), reachable);

            // Set selection
            fs.setSelected(reachable);
        }
        else
        {
            // Deselect all faces reachable from floodfilling.
            Arrays.fill(reachable, false);

            // Get all faces reachable from selected face.
            pp_.getMesh().markLoopFaces(evt.getElem(), reachable);

            // Deselect all ones reachable.
            for (int faceI = 0; faceI < currentSelection.length; faceI++)
            {
                if (reachable[faceI])
                {
                    currentSelection[faceI] = false;
                }
            }    

            fs.setSelected(currentSelection);
        }
    }

    public void elementDeselected(ElementEvent evt)
    {
        pp_.getMesh().getFaceSet().removeElementSelectListener(this);
        System.out.println("MyFileSystemView::elementDeselected : "
            + "removed listener");
    }

    public void selectedSetChanged(ElementEvent evt)
    {
        pp_.getMesh().getFaceSet().removeElementSelectListener(this);
        System.out.println("MyFileSystemView::selectedSetChanged : "
            + "removed listener");
    }

    //--------------------------------------------------------------------------
}



