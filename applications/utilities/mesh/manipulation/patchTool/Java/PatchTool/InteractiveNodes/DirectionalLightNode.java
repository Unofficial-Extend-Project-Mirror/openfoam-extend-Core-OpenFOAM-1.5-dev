/*
 *      LightNode.java
 */
package PatchTool.InteractiveNodes;

import java.awt.*;
import java.awt.Font;

import com.sun.j3d.utils.applet.MainFrame;
import com.sun.j3d.utils.geometry.Text2D;
import com.sun.j3d.utils.picking.PickResult;


import javax.media.j3d.*;
import javax.vecmath.*;

import PatchTool.Util.Text;

public class DirectionalLightNode implements InteractiveNode
{
    String name_;
    Switch switch_;
    BranchGroup branchGroup_;
    DirectionalLight light_;
    boolean isShowing_;

    public DirectionalLightNode
    (
        final String name,
        final Color3f color,
        final Vector3f direction,
        final BoundingSphere bounds
    )
    {
        name_ = name;

        isShowing_ = false;

        switch_ = new Switch();
        switch_.setCapability(Switch.ALLOW_SWITCH_WRITE);
        switch_.setCapability(Node.ALLOW_BOUNDS_READ);
        
        branchGroup_ = new BranchGroup();
        branchGroup_.setCapability(Node.ALLOW_BOUNDS_READ);

        // Create light
        light_ = new DirectionalLight(color, direction);
        light_.setInfluencingBounds(bounds);

        switch_.addChild(light_);

        branchGroup_.addChild(switch_);
    }

    public BranchGroup getRoot()
    {
        return branchGroup_;
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
        light_ = null;
        switch_ = null;
    }
} // end of class LightNode
