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

package PatchTool.InteractiveNodes;

import com.sun.j3d.utils.picking.PickResult;

import javax.media.j3d.*;
import javax.vecmath.Color3f;

import PatchTool.PatchTool;
import PatchTool.Shapes.PatchFaces;
import PatchTool.Mesh.PrimitivePatch;


public class PatchFacesNode implements InteractiveNode
{
    String name_;
    Switch switch_;
    BranchGroup branchGroup_;
    PatchFaces patchFaces_;
    boolean isShowing_;


    public PatchFacesNode
    (
        PatchTool pp,
        String name,
        PrimitivePatch patch,
        Color3f color,
        double creaseAngle
    )
    {
        name_ = name;

        isShowing_ = false;

        switch_ = new Switch();
        switch_.setCapability(Switch.ALLOW_SWITCH_WRITE);
        switch_.setCapability(Node.ALLOW_BOUNDS_READ);

        branchGroup_ = new BranchGroup();
        branchGroup_.setCapability(Node.ALLOW_BOUNDS_READ);

        patchFaces_ = new PatchFaces
        (
            pp,
            this,
            patch,
            color,
            creaseAngle
        );

        switch_.addChild(patchFaces_);

        branchGroup_.addChild(switch_);
    }


    public PatchFaces getPatchFaces()
    {
        return patchFaces_;
    }


    //--------------------------------------------------------------------------
    //---- InteractiveNode Interface
    //--------------------------------------------------------------------------


    public String getName()
    {
        return name_;
    }

    //--------------------------------------------------------------------------

    public BranchGroup getRoot()
    {
        return branchGroup_;
    }

    //--------------------------------------------------------------------------

    // Include in display
    public void show()
    {
        switch_.setWhichChild(Switch.CHILD_ALL);
        isShowing_ = true;
    }

    //--------------------------------------------------------------------------

    // Exclude from display
    public void hide()
    {
        switch_.setWhichChild(Switch.CHILD_NONE);
        isShowing_ = false;
    }

    //--------------------------------------------------------------------------

    // Edit properties
    public void edit()
    {
        patchFaces_.edit();
    }

    //--------------------------------------------------------------------------

    // Check status
    public boolean isShowing()
    {
        return isShowing_;
    }

    //--------------------------------------------------------------------------

    // Completely remove
    public void remove()
    {
        hide();
        patchFaces_.remove();
        patchFaces_ = null;
        switch_ = null;
    }

    //--------------------------------------------------------------------------
}
