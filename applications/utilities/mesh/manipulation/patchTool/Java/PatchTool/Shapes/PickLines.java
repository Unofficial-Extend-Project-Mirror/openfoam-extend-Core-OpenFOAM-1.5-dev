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


package PatchTool.Shapes;

import java.util.*;

import javax.media.j3d.*;
import javax.vecmath.*;
import com.sun.j3d.utils.picking.PickResult;
import com.sun.j3d.utils.picking.PickTool;
import com.sun.j3d.utils.picking.PickIntersection;

import PatchTool.PatchTool;
import PatchTool.Events.ElementEvent;
import PatchTool.Events.ElementSelectListener;
import PatchTool.Util.PickDragBehavior;
import PatchTool.Mesh.EdgeSet;
import PatchTool.Shapes.EditPanels.LinePropsDlg;
import PatchTool.Shapes.EditPanels.LinePropsPanel;

public class PickLines
    extends Shape3D
    implements PickableShape, ElementSelectListener
{
    static final int UNPICKEDCOLOR = 0;
    static final int PICKEDCOLOR = 1;

    PatchTool pp_;

    // Points
    Point3f[] points_;

    // Lines as 2 point labels per edge
    int[] vertexLabels_;

    // Current fore and background color. Referenced.
    Color3f[] colors_;

    // Current indices. Referenced.
    int[] colorIndices_;

    // Edge selection we're displaying and updating
    EdgeSet lineSet_;

    // Mesh edge label for every edge
    int[] edgeLabels_;

    public PickLines
    (
        PatchTool pp,
        Point3f[] points,
        int[] vertexLabels,
        Color3f color,
        EdgeSet lineSet,
        int[] edgeLabels
    )
    {
        // Store input data
        pp_ = pp;
        points_ = points;
        vertexLabels_ = vertexLabels;
        colors_ = new Color3f[2];
        colors_[UNPICKEDCOLOR] = color;
        colors_[PICKEDCOLOR] = new Color3f(1.0f, 0.0f, 0.0f);
        lineSet_ = lineSet;
        edgeLabels_ = edgeLabels;

        IndexedLineArray lineArray = new IndexedLineArray
        (
            points.length,
            GeometryArray.COORDINATES | GeometryArray.COLOR_3,
            vertexLabels_.length
        );

        lineArray.setCoordinates(0, points);
        lineArray.setCoordinateIndices(0, vertexLabels_);
        lineArray.setColors(0, colors_);

        //- Start off with all lines unpicked
        colorIndices_ = new int[vertexLabels_.length];
        for(int i = 0; i < vertexLabels_.length; i++)
        {
            colorIndices_[i] = UNPICKEDCOLOR;
        }
        lineArray.setColorIndices(0, colorIndices_);
    
        this.setGeometry(lineArray);

        this.setAppearance(PatchTool.createLineAppearance(true));

        // Set all capabilities

        lineArray.setCapability(IndexedLineArray.ALLOW_COORDINATE_READ);
        lineArray.setCapability(IndexedLineArray.ALLOW_FORMAT_READ);
        lineArray.setCapability(IndexedLineArray.ALLOW_COUNT_READ);
        lineArray.setCapability(IndexedLineArray.ALLOW_COORDINATE_INDEX_READ);
        lineArray.setCapability(IndexedLineArray.ALLOW_COLOR_WRITE);
        lineArray.setCapability(IndexedLineArray.ALLOW_COLOR_INDEX_WRITE);

        this.setCapability(Shape3D.ALLOW_GEOMETRY_WRITE);
        this.setCapability(Shape3D.ALLOW_APPEARANCE_READ);
        this.setCapability(Shape3D.ALLOW_GEOMETRY_READ);
        this.setCapability(Shape3D.ALLOW_BOUNDS_READ);

//// Hack to see if this disables edge selection.
lineArray.clearCapability(Geometry.ALLOW_INTERSECT);
lineArray.clearCapability(Node.ENABLE_PICK_REPORTING);
this.clearCapability(Geometry.ALLOW_INTERSECT);
this.clearCapability(Node.ENABLE_PICK_REPORTING);


        lineSet_.addElementSelectListener(this);
    }

    //--------------------------------------------------------------------------
    //---- PickableShape Interface
    //--------------------------------------------------------------------------

    // General handling of picks
    private void handlePicks(PickResult pickResult, boolean select)
    {
        IndexedLineArray lines = (IndexedLineArray)this.getGeometry();

        int nearest = PickDragBehavior.getNearestIndex(pickResult);
        PickIntersection pi = pickResult.getIntersection(nearest);

        // Get local edge/line index
        int[] indices = pi.getPrimitiveVertexIndices();
        int lineIndex = indices[0] / 2;  // 2 labels per line segment

        // Get mesh vertex nearest to pick point
        int nearestIndex = pi.getClosestVertexIndex();
        int nearestVertI = lines.getCoordinateIndex(indices[0] + nearestIndex);

        // Get pick point
        Point3d pt = pi.getPointCoordinates();

        //System.out.println
        //(
        //    "Picked on"
        //  + "\n    Coord:" + pt
        //  + "\n    Nearest Vertex:" + nearestVertI
        //  + "\n    Edge:" + lineIndex
        //  + "\n    Points:"
        //);
        //for(int j = 0; j < indices.length; j++)
        //{
        //    int vertI = vertexLabels_[indices[j]];
        //
        //    System.out.print(indices[j] + " " + points_[vertI] + "  ");
        //}
        //System.out.println();


        //
        // Handle according to global pick mode
        //

        if ((pp_.getPickMode() & PatchTool.SELECT_SINGLE) != 0)
        {
            if (select)
            {
                lineSet_.select(lineIndex);
            }
            else
            {
                lineSet_.deselect(lineIndex);
            }
        }
        else if ((pp_.getPickMode() & PatchTool.SELECT_ALL) != 0)
        {
            boolean[] selected = new boolean[lineSet_.getSelected().length];


            if (select)
            {
                Arrays.fill(selected, true);
            }
            else
            {
                Arrays.fill(selected, false);
            }

            lineSet_.setSelected(selected);
        }

        //
        // Warn PatchTool.and any listeners (using mesh indices, 
        // not local indices)
        //

        int meshEdgeI = edgeLabels_[lineIndex];

        if (select)
        {
            pp_.firePicked(this, pickResult, -1, meshEdgeI, nearestVertI, pt);
        }
        else
        {
            pp_.fireUnpicked(this, pickResult, -1, meshEdgeI, nearestVertI, pt);
        }
    }

    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------

    // Callback for if picked.
    public void picked(PickResult pickResult)
    {
        // Select based on pickResult
        handlePicks(pickResult, true);
    }

    //--------------------------------------------------------------------------

    // Callback for if unpicked.
    public void unpicked(PickResult pickResult)
    {
        // Deselect based on pickResult
        handlePicks(pickResult, false);
    }

    //--------------------------------------------------------------------------

    // Callback for if picked all.
    public void pickedAll(PickResult pickResult)
    {
        // Change to select-all mode and select all edges.
        int oldPickMode = pp_.getPickMode();

        pp_.setPickMode(PatchTool.SELECT_ALL);

        handlePicks(pickResult, true);

        pp_.setPickMode(oldPickMode);
    }


    //--------------------------------------------------------------------------

    // Callback for if unpicked all.
    public void unpickedAll(PickResult pickResult)
    {
        // Change to select-all mode and deselect all edges.
        int oldPickMode = pp_.getPickMode();

        pp_.setPickMode(PatchTool.SELECT_ALL);

        handlePicks(pickResult, false);

        pp_.setPickMode(oldPickMode);
    }

    //--------------------------------------------------------------------------

    // Completely remove
    public void remove()
    {
        lineSet_.removeElementSelectListener(this);

        if (this.numGeometries() != 0)
        {
            this.removeGeometry(0);
        }
    }

    //--------------------------------------------------------------------------

    // Edit
    public void edit()
    {
        LinePropsDlg editDlg = new LinePropsDlg(null, colors_[UNPICKEDCOLOR]);

        if (editDlg.getState() == LinePropsDlg.OK)
        {
            try
            {
                LinePropsPanel editPnl = editDlg.getLinePropsPanel();

                Color3f color = editPnl.getColor3f();

                // Check change of color
                if (!color.equals(colors_[UNPICKEDCOLOR]))
                {
                    System.out.println("Changing color to " + color);

                    IndexedLineArray lines =
                        (IndexedLineArray)this.getGeometry();

                    colors_[UNPICKEDCOLOR] = color;

                    lines.setColors(0, colors_);
                }
            }
            catch (NumberFormatException ex)
            {
                System.out.println("** Format exception:" + ex);
            }
        }
    }


    //--------------------------------------------------------------------------
    //---- ElementSelectListener Implementation
    //--------------------------------------------------------------------------

    public void elementSelected(ElementEvent evt)
    {
        IndexedLineArray lines = (IndexedLineArray)this.getGeometry();

        int lineIndex = evt.getElem();

        // Update display
        lines.setColorIndex(2*lineIndex, PICKEDCOLOR);
        lines.setColorIndex(2*lineIndex+1, PICKEDCOLOR);

        // Update local copy
        colorIndices_[2*lineIndex] = PICKEDCOLOR;
        colorIndices_[2*lineIndex+1] = PICKEDCOLOR;
    }

    //--------------------------------------------------------------------------

    public void elementDeselected(ElementEvent evt)
    {
        IndexedLineArray lines = (IndexedLineArray)this.getGeometry();

        int lineIndex = evt.getElem();

        // Update display
        lines.setColorIndex(2*lineIndex, UNPICKEDCOLOR);
        lines.setColorIndex(2*lineIndex+1, UNPICKEDCOLOR);

        // Update local copy
        colorIndices_[2*lineIndex] = UNPICKEDCOLOR;
        colorIndices_[2*lineIndex+1] = UNPICKEDCOLOR;
    }

    //--------------------------------------------------------------------------

    public void selectedSetChanged(ElementEvent evt)
    {
        // Get array of selected edges
        boolean[] selected = lineSet_.getSelected();

        for (int edgeI = 0; edgeI < selected.length; edgeI++)
        {
            if (selected[edgeI])
            {
                colorIndices_[2*edgeI] = PICKEDCOLOR;
                colorIndices_[2*edgeI+1] = PICKEDCOLOR;
            }
            else
            {
                colorIndices_[2*edgeI] = UNPICKEDCOLOR;
                colorIndices_[2*edgeI+1] = UNPICKEDCOLOR;
            }
        }

        IndexedLineArray lines = (IndexedLineArray)this.getGeometry();

        lines.setColorIndices(0, colorIndices_);
    }

    //--------------------------------------------------------------------------

} // end of class PickLines
