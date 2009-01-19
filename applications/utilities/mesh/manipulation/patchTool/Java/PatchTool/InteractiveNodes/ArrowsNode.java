/*
 *
 */

package PatchTool.InteractiveNodes;

import javax.media.j3d.*;
import javax.vecmath.*;
import com.sun.j3d.utils.picking.PickResult;
import com.sun.j3d.utils.picking.PickIntersection;

import PatchTool.Shapes.Arrows;
import PatchTool.Util.Colorizer;

public class ArrowsNode implements InteractiveNode
{
    String name_;
    Switch switch_;
    BranchGroup branchGroup_;
    Arrows arrows_;
    boolean isShowing_;

    public ArrowsNode
    (
        final String name,
        final float[] sampleCoords,
        final float[] vecValues,
        final float sizeScale,
        final float[] scalValues,
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

        arrows_ =
            new Arrows
            (
                sampleCoords,
                vecValues,
                sizeScale,
                scalValues,
                colorizer,
                min,
                max
            );

        switch_.addChild(arrows_);

        branchGroup_.addChild(switch_);
    }

    public BranchGroup getRoot()
    {
        return branchGroup_;
    }

    public Arrows getArrows()
    {
        return arrows_;
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
        arrows_ = null;
        switch_ = null;
    }

} // end of class ArrowsNode
