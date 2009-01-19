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

import javax.media.j3d.*;
import javax.vecmath.*;
import com.sun.j3d.utils.picking.PickResult;
import com.sun.j3d.utils.picking.PickIntersection;

import PatchTool.PatchTool;
import PatchTool.Shapes.PickLines;
import PatchTool.Mesh.EdgeSet;

public class PickLinesNode implements InteractiveNode
{
    String name_;
    Switch switch_;
    BranchGroup branchGroup_;
    PickLines pickLines_;
    boolean isShowing_;

    public PickLinesNode
    (
        PatchTool pp,
        String name,
        Point3f[] points,
        int[] vertexLabels,
        Color3f color,
        EdgeSet lineSet,
        int[] edgeLabels
    )
    {
        name_ = name;

        isShowing_ = false;

        switch_ = new Switch();
        switch_.setCapability(Switch.ALLOW_SWITCH_WRITE);
        switch_.setCapability(Node.ALLOW_BOUNDS_READ);

        branchGroup_ = new BranchGroup();
        branchGroup_.setCapability(Node.ALLOW_BOUNDS_READ);

        pickLines_ = new PickLines
        (
            pp,
            points,
            vertexLabels,
            color,
            lineSet,
            edgeLabels
        );

        switch_.addChild(pickLines_);

// Hack to see if this disables edge selection.
switch_.clearCapability(Node.ENABLE_PICK_REPORTING);
// Hack to see if this disables edge selection.
branchGroup_.clearCapability(Node.ENABLE_PICK_REPORTING);


        branchGroup_.addChild(switch_);
    }

    public BranchGroup getRoot()
    {
        return branchGroup_;
    }

    public PickLines getPickLines()
    {
        return pickLines_;
    }


    //--------------------------------------------------------------------------
    //---- InteractiveNode Interface
    //--------------------------------------------------------------------------


    public String getName()
    {
        return name_;
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
        System.out.println("edit:" + this.getName());
        pickLines_.edit();
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
        pickLines_.remove();
        pickLines_ = null;
        switch_ = null;
    }

    //--------------------------------------------------------------------------

} // end of class PickLinesNode
