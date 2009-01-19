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

package PatchTool.Panels;

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
import PatchTool.Events.PatchEvent;
import PatchTool.Events.PatchStatusListener;
import PatchTool.Events.CaseEvent;
import PatchTool.Events.ElementEvent;
import PatchTool.Events.ElementSelectListener;
import PatchTool.Events.RenderListener;
import PatchTool.Events.PickListener;
import PatchTool.Events.MeshPickEvent;

import PatchTool.Mesh.BoundaryMesh;
import PatchTool.Mesh.SurfacePatch;
import PatchTool.Mesh.FaceSet;
import PatchTool.Mesh.SelectedSet;
import PatchTool.Mesh.BoundaryMeshUtils;

import PatchTool.Shapes.PickLines;

public class DisplayPanel
    extends javax.swing.JPanel
    implements ElementSelectListener, RenderListener, PickListener
{
    //--------------------------------------------------------------------------

    //protected EventListenerList listenerList_;

    protected PatchTool pp_;

    protected boolean[] newSelection_;

    protected boolean[] oldSelection_;

    String prevMsg_;

    int counter_;

    //--------------------------------------------------------------------------
    /** Creates new form DisplayPanel */
    public DisplayPanel(PatchTool pp)
    {
        super();

        // Create listener list.
        //listenerList_ = new EventListenerList();

        pp_ = pp;

        initComponents();

        // Get warned of picked face/edge/vertex so we can do our follow-edge
        // mode
        pp_.addPickSelectListener(this);
    }

    //--------------------------------------------------------------------------

    public float getFeatureAngle() throws NumberFormatException
    {
        return Float.parseFloat(createFeatText_.getText());
    }

    //--------------------------------------------------------------------------

    private void initComponents()
    {
        // This panel
        this.setLayout(new GridBagLayout());

        GridBagConstraints c = new GridBagConstraints();

        // Add insets around all to avoid clutter
        c.insets = new Insets(2, 2, 2, 2);

        // Anchor all same way
        c.anchor = GridBagConstraints.WEST;

        // Fill horizontal
        c.fill = GridBagConstraints.HORIZONTAL;


        // Create faces display
        JLabel faceLabel = new JLabel("Faces");
        PatchTool.setConstraint(0, 0, 1, 1, 1.0, 1.0, c);
        this.add(faceLabel, c);

        JButton createFaceButton = new JButton("Create");
        createFaceButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    createFaceButton_ActionPerformed(evt);
                }
            }
        );
        createFaceButton.setToolTipText
        (
            "Create display shapes for all faces"
        );
        PatchTool.setConstraint(1, 0, 1, 1, 1.0, 1.0, c);
        this.add(createFaceButton, c);

        JButton removeFaceButton = new JButton("Remove");
        removeFaceButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    removeFaceButton_ActionPerformed(evt);
                }
            }
        );
        removeFaceButton.setToolTipText
        (
            "Delete face display"
        );
        PatchTool.setConstraint(2, 0, 1, 1, 1.0, 1.0, c);
        this.add(removeFaceButton, c);


        // Create edges display
        JLabel edgeLabel = new JLabel("Edges");
        PatchTool.setConstraint(0, 1, 1, 1, 1.0, 1.0, c);
        this.add(edgeLabel, c);

        JButton createEdgeButton = new JButton("Create");
        createEdgeButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    createEdgeButton_ActionPerformed(evt);
                }
            }
        );
        createEdgeButton.setToolTipText
        (
            "Create display shapes for all edges"
        );
        PatchTool.setConstraint(1, 1, 1, 1, 1.0, 1.0, c);
        this.add(createEdgeButton, c);

        JButton removeEdgeButton = new JButton("Remove");
        removeEdgeButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    removeEdgeButton_ActionPerformed(evt);
                }
            }
        );
        removeEdgeButton.setToolTipText
        (
            "Delete edge display"
        );
        PatchTool.setConstraint(2, 1, 1, 1, 1.0, 1.0, c);
        this.add(removeEdgeButton, c);


        //
        // Feature lines
        //
        JLabel featLabel = new JLabel("Features");
        PatchTool.setConstraint(0, 2, 1, 1, 1.0, 1.0, c);
        this.add(featLabel, c);

        createFeatText_ = new JTextField("20");
        createFeatText_.setToolTipText
        (
            "Feature angle (0..180)"
        );
        PatchTool.setConstraint(1, 2, 1, 1, 1.0, 1.0, c);
        this.add(createFeatText_, c);

        JButton createFeatButton = new JButton("Create");
        createFeatButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    createFeatButton_ActionPerformed(evt);
                }
            }
        );
        createFeatButton.setToolTipText
        (
            "Create display shapes for feature edges"
        );
        PatchTool.setConstraint(2, 2, 1, 1, 1.0, 1.0, c);
        this.add(createFeatButton, c);

        JButton removeFeatButton = new JButton("Remove");
        removeFeatButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    removeFeatButton_ActionPerformed(evt);
                }
            }
        );
        removeFeatButton.setToolTipText
        (
            "Delete feature edge display"
        );
        PatchTool.setConstraint(3, 2, 1, 1, 1.0, 1.0, c);
        this.add(removeFeatButton, c);


//        //
//        // Check loop
//        //
//        JButton closedButton = new JButton("Check closed");
//        closedButton.addActionListener
//        (
//            new ActionListener()
//            {
//                public void actionPerformed(ActionEvent evt)
//                {
//                    closedButton_ActionPerformed(evt);
//                }
//            }
//        );
//        closedButton.setToolTipText
//        (
//            "Check selected feature edges for closedness"
//        );
//        PatchTool.setConstraint(3, 2, 1, 1, 1.0, 1.0, c);
//        this.add(closedButton, c);


        //
        // Extra edges
        //
        JLabel extraEdgeLabel = new JLabel("EdgeI");
        PatchTool.setConstraint(0, 3, 1, 1, 1.0, 1.0, c);
        this.add(extraEdgeLabel, c);

        extraEdgeText_ = new JTextField("0");
        extraEdgeText_.setToolTipText
        (
            "Label of edge"
        );
        PatchTool.setConstraint(1, 3, 1, 1, 1.0, 1.0, c);
        this.add(extraEdgeText_, c);

        JButton addExtraEdgeButton = new JButton("Create");
        addExtraEdgeButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    addExtraEdgeButton_ActionPerformed(evt);
                }
            }
        );
        addExtraEdgeButton.setToolTipText
        (
            "Create display shapes for all edges near selected edge"
        );
        PatchTool.setConstraint(2, 3, 1, 1, 1.0, 1.0, c);
        this.add(addExtraEdgeButton, c);

        JButton removeExtraEdgeButton = new JButton("Remove");
        removeExtraEdgeButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    removeExtraEdgeButton_ActionPerformed(evt);
                }
            }
        );
        removeExtraEdgeButton.setToolTipText
        (
            "Remove extra edge display"
        );
        PatchTool.setConstraint(3, 3, 1, 1, 1.0, 1.0, c);
        this.add(removeExtraEdgeButton, c);
    }

    //--------------------------------------------------------------------------

    private void createFaceButton_ActionPerformed(ActionEvent evt)
    {
        // Update display for patches
        pp_.addFaceDisplay();
    }


    //--------------------------------------------------------------------------

    private void removeFaceButton_ActionPerformed(ActionEvent evt)
    {
        // Update display for patches
        pp_.removeFaceDisplay();
    }

    //--------------------------------------------------------------------------

    private void createEdgeButton_ActionPerformed(ActionEvent evt)
    {
        // Update display for edges
        pp_.addEdgeDisplay();
    }

    //--------------------------------------------------------------------------

    private void removeEdgeButton_ActionPerformed(ActionEvent evt)
    {
        // Update display for edges
        pp_.removeEdgeDisplay();
    }

    //--------------------------------------------------------------------------

    private void createFeatButton_ActionPerformed(ActionEvent evt)
    {
        try
        {
            float degAngle = Float.parseFloat(createFeatText_.getText());

            double radAngle = degAngle * Math.PI / 180.0;

            System.out.println
            (
                "Creating featureLines from angle " + radAngle
            );

            pp_.getMesh().setFeatureEdges(Math.cos(radAngle));

            pp_.addFeatureEdgeDisplay();
        }
        catch(NumberFormatException ex)
        {
            System.out.println("numberFormat:" + ex);
        }
        catch(Exception ex)
        {
            ex.printStackTrace();
        }
    }

    //--------------------------------------------------------------------------

    private void removeFeatButton_ActionPerformed(ActionEvent evt)
    {
        pp_.removeFeatureEdgeDisplay();
    }

    //--------------------------------------------------------------------------

    private void closedButton_ActionPerformed(ActionEvent evt)
    {
        prevMsg_ = pp_.setMsg("Please select face on inside of loop");

        System.out.println("Select face on inside of loop");

        pp_.getMesh().getFaceSet().addElementSelectListener(this);
    }

    //--------------------------------------------------------------------------

    private void addExtraEdgeButton_ActionPerformed(ActionEvent evt)
    {
        try
        {
            int edgeI = Integer.parseInt(extraEdgeText_.getText());

            pp_.getMesh().setExtraEdges(edgeI);

            pp_.addExtraEdgeDisplay();
        }
        catch(NumberFormatException ex)
        {
            System.out.println("numberFormat:" + ex);
        }
    }

    //--------------------------------------------------------------------------

    private void removeExtraEdgeButton_ActionPerformed(ActionEvent evt)
    {
        pp_.removeExtraEdgeDisplay();
    }

    //--------------------------------------------------------------------------
    //---- ElementSelectListener Implementation
    //--------------------------------------------------------------------------

    public void elementSelected(ElementEvent evt)
    {
        pp_.getMesh().getFaceSet().removeElementSelectListener(this);
        pp_.setMsg(prevMsg_);

        System.out.println("DisplayPanel::elementSelected : meshfaceI:"
             + evt.getElem());


        //
        // Get new selection based on floodfilling from picked face.
        //

        newSelection_ = new boolean[pp_.getMesh().getNFaces()];

        Arrays.fill(newSelection_, false);

        // Get all faces reachable from evt.getElem (=meshFaceI)
        pp_.getMesh().markLoopFaces(evt.getElem(), newSelection_);

        //for (int faceI = 0; faceI < newSelection_.length; faceI++)
        //{
        //    if (newSelection_[faceI])
        //    {
        //        System.out.println("newSelection face:" + faceI);
        //    }
        //}
        int nSelected = SelectedSet.getNSelected(newSelection_);

        System.out.println("DisplayPanel::elementSelected : new selection "
            + " contains " + nSelected + " faces");


        //
        // Save current selection
        //

        boolean[] currentSelection =
            pp_.getMesh().getFaceSet().getSelected();

        oldSelection_ = new boolean[currentSelection.length];

        for (int faceI = 0; faceI < currentSelection.length; faceI++)
        {
            oldSelection_[faceI] = currentSelection[faceI];
        }

        //for (int faceI = 0; faceI < oldSelection_.length; faceI++)
        //{
        //    if (oldSelection_[faceI])
        //    {
        //        System.out.println("Saved selection face:" + faceI);
        //    }
        //}


        // Reset display counter and start flickering by attaching onto
        // postRenderer.

        counter_ = 0;

        pp_.getCanvas().addRenderListener(this);
    }

    public void elementDeselected(ElementEvent evt)
    {
        System.out.println("DisplayPanel::elementDeselected : meshfaceI:"
             + evt.getElem());
    }

    public void selectedSetChanged(ElementEvent evt)
    {
        System.out.println("DisplayPanel::selectedSetChanged");
    }

    //--------------------------------------------------------------------------
    //---- PickListener Methods
    //--------------------------------------------------------------------------

    // Single element picked
    public void picked(MeshPickEvent evt)
    {
        if ((evt.getEdge() != -1) && (pp_.getPickMode() == PatchTool.FOLLOW))
        {
            extraEdgeText_.setText(Integer.toString(evt.getEdge()));

            addExtraEdgeButton_ActionPerformed(new ActionEvent(this, 0, ""));
        }
    }

    // Single element unpicked
    public void unpicked(MeshPickEvent evt)
    {
        picked(evt);
    }

    //--------------------------------------------------------------------------
    //---- RenderListener Implementation
    //--------------------------------------------------------------------------

    public void postSwap(java.util.EventObject evt)
    {

        if ((counter_ % 2) == 0)
        {
            // Display new selection
            System.out.println("postSwap : insert new selection");
            pp_.getMesh().getFaceSet().setSelected(newSelection_);
        }
        else
        {
            // Display saved selection
            System.out.println("postSwap : insert old/saved selection");
            pp_.getMesh().getFaceSet().setSelected(oldSelection_);
    
        }


        if (counter_ == 10)
        {
            System.out.println("postSwap : removing listener");
            pp_.getCanvas().removeRenderListener(this);
        }
        else
        {
            counter_++;

            pp_.getCanvas().removeRenderListener(this);
            try
            {
                Thread.currentThread().sleep(500); 
            }
            catch (InterruptedException ie)
            {
                System.out.println("Interrupted exception");
            }
            pp_.getCanvas().addRenderListener(this);
        }
    }


    //--------------------------------------------------------------------------
    //---- UI components
    //--------------------------------------------------------------------------

    private JTextField createFeatText_;
    private JTextField extraEdgeText_;

    //--------------------------------------------------------------------------
}



