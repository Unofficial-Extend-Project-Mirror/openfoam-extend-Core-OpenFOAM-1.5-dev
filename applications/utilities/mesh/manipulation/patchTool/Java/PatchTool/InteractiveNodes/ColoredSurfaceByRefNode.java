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

import PatchTool.Shapes.ColoredSurfaceByRef;
import PatchTool.Util.Colorizer;

public class ColoredSurfaceByRefNode implements InteractiveNode
{
    String name_;
    Switch switch_;
    BranchGroup branchGroup_;
    ColoredSurfaceByRef coloredSurfaceByRef_;
    boolean isShowing_;

    //- From unstripped file
    public ColoredSurfaceByRefNode
    (
        final String name,
        final double creaseAngle,
        final boolean stripify,
        final String pointsFileName,
        final String facesFileName,
        final String valuesFileName,
        final Colorizer colorizer,
        final float min,
        final float max
    )
    {
        this
        (
            name, false, false, stripify, creaseAngle,
            pointsFileName, null, facesFileName, valuesFileName,
            colorizer, min, max
        );
    }

    //- From file stripped or not stripped
    public ColoredSurfaceByRefNode
    (
        final String name,
        final boolean readTriStrip,
        final boolean readNormals,
        final boolean stripify,
        final double creaseAngle,
        final String pointsFileName,
        final String normalsFileName,
        final String facesFileName,
        final String valuesFileName,
        final Colorizer colorizer,
        final float min,
        final float max
    )
    {
        name_ = name;

        isShowing_ = false;

        switch_ = new Switch();
        switch_.setCapability(Switch.ALLOW_SWITCH_WRITE);
        switch_.setCapability(Node.ALLOW_BOUNDS_READ);

        branchGroup_ = new BranchGroup();
        branchGroup_.setCapability(Node.ALLOW_BOUNDS_READ);

        coloredSurfaceByRef_ =
            new ColoredSurfaceByRef
            (
                this,
                readTriStrip,
                readNormals,
                stripify,
                creaseAngle,
                pointsFileName,
                normalsFileName,
                facesFileName,
                valuesFileName,
                colorizer,
                min,
                max
            );

        switch_.addChild(coloredSurfaceByRef_);

        branchGroup_.addChild(switch_);
    }


    // From values
    public ColoredSurfaceByRefNode
    (
        final String name,
        final boolean readTriStrip,
        final boolean stripify,
        final double creaseAngle,
        final float[] points,
        final float[] normals,
        final int[] triFaces,
        final float[] values,
        final Colorizer colorizer,
        final float min,
        final float max
    )
    {
        name_ = name;

        isShowing_ = false;

        switch_ = new Switch();
        switch_.setCapability(Switch.ALLOW_SWITCH_WRITE);
        switch_.setCapability(Node.ALLOW_BOUNDS_READ);

        branchGroup_ = new BranchGroup();
        branchGroup_.setCapability(Node.ALLOW_BOUNDS_READ);

        coloredSurfaceByRef_ =
            new ColoredSurfaceByRef
            (
                this,
                readTriStrip,
                stripify,
                creaseAngle,
                points,
                normals,
                triFaces,
                null,
                values,
                colorizer,
                min,
                max
            );

        switch_.addChild(coloredSurfaceByRef_);

        branchGroup_.addChild(switch_);
    }

    public BranchGroup getRoot()
    {
        return branchGroup_;
    }

    public ColoredSurfaceByRef getColoredSurfaceByRef()
    {
        return coloredSurfaceByRef_;
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
        coloredSurfaceByRef_.remove();
        coloredSurfaceByRef_ = null;
        switch_ = null;
    }

} // end of class ColoredSurfaceByRefNode
