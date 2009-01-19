/*
 *
 */

package PatchTool.InteractiveNodes;

import javax.media.j3d.*;
import javax.vecmath.*;
import com.sun.j3d.utils.picking.PickResult;
import com.sun.j3d.utils.picking.PickIntersection;

import PatchTool.Util.Colorizer;
import PatchTool.Shapes.ColorBar;

public class ColorBarNode implements InteractiveNode
{
    String name_;
    Switch switch_;
    BranchGroup branchGroup_;
    ColorBar colorBar_;
    boolean isShowing_;

    public ColorBarNode
    (
        final String name,
        final Colorizer ci,
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

        colorBar_ = new ColorBar(this, ci, min, max);

        switch_.addChild(colorBar_);

        branchGroup_.addChild(switch_);
    }

    public BranchGroup getRoot()
    {
        return branchGroup_;
    }

    public ColorBar getColorBar()
    {
        return colorBar_;
    }


    //--------------------------------------------------------------------------
    //---- InteractiveNode Interface
    //--------------------------------------------------------------------------


    public String getName()
    {
        return name_;
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
        colorBar_.remove();
        colorBar_ = null;
        switch_ = null;
    }

} // end of class ColorBarNode
