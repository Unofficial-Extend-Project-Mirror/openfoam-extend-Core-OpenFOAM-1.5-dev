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

import java.awt.*;
import java.awt.Font;

import com.sun.j3d.utils.applet.MainFrame;
import com.sun.j3d.utils.geometry.Text2D;
import com.sun.j3d.utils.picking.PickResult;


import javax.media.j3d.*;
import javax.vecmath.*;

import PatchTool.Shapes.AxisLines;
import PatchTool.Util.Text;

public class AxisNode implements InteractiveNode
{
    String name_;
    Color3f color3f_;
    Switch switch_;
    BranchGroup branchGroup_;
    AxisLines axisLines_;
    boolean isShowing_;

    public AxisNode(String name, Color3f color3f)
    {
        name_ = name;
        color3f_ = color3f;

        isShowing_ = false;

        switch_ = new Switch();
        switch_.setCapability(Switch.ALLOW_SWITCH_WRITE);
        switch_.setCapability(Node.ALLOW_BOUNDS_READ);
        
        branchGroup_ = new BranchGroup();
        branchGroup_.setCapability(Node.ALLOW_BOUNDS_READ);

        // Axis lines
        axisLines_ = new AxisLines(this, color3f_);
        switch_.addChild(axisLines_);

        branchGroup_.addChild(switch_);

        // Axis text
        //addText(switch_); // 2D text

        Text axisText = new Text();
        switch_.addChild
        (
            axisText.createLabel("X", 1.1f, 0f, 0f, color3f_.get())
        );
        switch_.addChild
        (
            axisText.createLabel("Y", 0f, 1.1f, 0f, color3f_.get())
        );
        switch_.addChild
        (
            axisText.createLabel("Z", 0f, 0f, 1.1f, color3f_.get())
        );
    }

    public BranchGroup getRoot()
    {
        return branchGroup_;
    }

    // Utility function to create text objects.
    private void addText(Group group)
    {
        Color3f axisColor = new Color3f(1f, 1f, 0);
        Shape3D textObject = null;
        Transform3D textTranslation = null;
        TransformGroup textTranslationGroup = null;

        textObject = new Text2D
        (
            "x",
            axisColor,
            "Serif",
            70,
	    Font.BOLD
        );
        //textObject.setCapability(Shape3D.ALLOW_GEOMETRY_READ);

	textTranslation = new Transform3D();
	textTranslation.setTranslation(new Vector3f(0.5f, 0f, 0f));
	textTranslationGroup = new TransformGroup(textTranslation);
	textTranslationGroup.addChild(textObject);
	group.addChild(textTranslationGroup);

	textObject = new Text2D
        (
            "y",
            axisColor,
            "Serif",
            70,
            Font.BOLD
        );
        //textObject.setCapability(Shape3D.ALLOW_GEOMETRY_READ);

        textTranslation = new Transform3D();
        textTranslation.setTranslation(new Vector3f(0f, 0.5f, 0f));
        textTranslationGroup = new TransformGroup(textTranslation);
        textTranslationGroup.addChild(textObject);
        group.addChild(textTranslationGroup);

        textObject = new Text2D
        (
            "z",
            axisColor,
            "Serif",
            70,
            Font.BOLD
        );
        //textObject.setCapability(Shape3D.ALLOW_GEOMETRY_READ);

        textTranslation = new Transform3D();
        textTranslation.setTranslation(new Vector3f(0f, 0f, 0.5f));
        textTranslationGroup = new TransformGroup(textTranslation);
        textTranslationGroup.addChild(textObject);
        group.addChild(textTranslationGroup);
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
    {}

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

        axisLines_.remove();
        axisLines_ = null;
        switch_ = null;
    }
} // end of class AxisNode
