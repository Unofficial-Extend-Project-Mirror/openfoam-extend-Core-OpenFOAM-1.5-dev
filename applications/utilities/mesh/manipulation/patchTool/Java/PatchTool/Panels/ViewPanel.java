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

import com.sun.j3d.utils.applet.MainFrame;
import com.sun.j3d.utils.behaviors.vp.*;
import com.sun.j3d.utils.behaviors.keyboard.*;
import com.sun.j3d.utils.picking.PickTool;
import com.sun.j3d.utils.universe.*;


import com.sun.j3d.utils.universe.*;
import javax.media.j3d.*;
import javax.vecmath.*;


import PatchTool.PatchTool;
import PatchTool.Util.PickDragBehavior;
import PatchTool.Events.PickListener;
import PatchTool.Events.MeshPickEvent;

public class ViewPanel extends javax.swing.JPanel implements PickListener
{
    //--------------------------------------------------------------------------

    protected PatchTool pp_;


    // Picking mode
    static final int[] pickModeValues_ =
    {
        PatchTool.SELECT_SINGLE,
        PatchTool.SELECT_ALL,
        PatchTool.FOLLOW,
        0
    };

    static final String[] pickModeNames_ =
    {
        "Add Single",
        "Add All",
        "Follow",
        "None"
    };


    // Polygon display modes
    static final int[] drawModeValues_ =
    {
        PolygonAttributes.POLYGON_FILL,
        PolygonAttributes.POLYGON_LINE,
        PolygonAttributes.POLYGON_POINT
    };

    static final String[] drawModeNames_ =
    {
        "polygon",
        "wireframe",
        "points"
    };


    // Polygon culling policy
    static final int[] polyCullValues_ =
    {
        PolygonAttributes.CULL_NONE,
        PolygonAttributes.CULL_BACK,
        PolygonAttributes.CULL_FRONT
    };

    static final String[] polyCullNames_ =
    {
        "none",
        "back",
        "front"
    };


    //--------------------------------------------------------------------------
    /** Creates new form ViewPanel */
    public ViewPanel(PatchTool pp)
    {
        super();

        pp_ = pp;

        initComponents();


        for (int i = 0; i < pickModeNames_.length; i++)
        {
            pickModeCombo_.addItem(pickModeNames_[i]);
        }


        for (int i = 0; i < drawModeNames_.length; i++)
        {
            drawModeCombo_.addItem(drawModeNames_[i]);
        }


        for (int i = 0; i < polyCullNames_.length; i++)
        {
            cullModeCombo_.addItem(polyCullNames_[i]);
        }
    
        // Get warned of picked face/edge/vertex
        pp_.addPickSelectListener(this);
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


//        // Eye position
//        JLabel eyeLabel = new JLabel("Eye");
//        PatchTool.setConstraint(0, 0, 1, 1, 1.0, 1.0, c);
//        this.add(eyeLabel, c);
//
//        eyeVector_ = new VectorPanel(new Vector3d(1,0,0));
//        PatchTool.setConstraint(1, 0, 1, 1, 1.0, 1.0, c);
//        this.add(eyeVector_, c);
//
//
//        // Look at
//        JLabel lookLabel = new JLabel("Look at");
//        PatchTool.setConstraint(2, 0, 1, 1, 1.0, 1.0, c);
//        this.add(lookLabel, c);
//
//        lookVector_ = new VectorPanel(new Vector3d(0,0,0));
//        PatchTool.setConstraint(3, 0, 1, 1, 1.0, 1.0, c);
//        this.add(lookVector_, c);
//
//
//        // Up
//        JLabel upLabel = new JLabel("Up");
//        PatchTool.setConstraint(4, 0, 1, 1, 1.0, 1.0, c);
//        this.add(upLabel, c);
//
//        upCombo_ = new JComboBox();
//        upCombo_.addActionListener
//        (
//            new ActionListener()
//            {
//                public void actionPerformed(ActionEvent evt)
//                {
//                    upCombo_ActionPerformed(evt);
//                }
//            }
//        );
//        PatchTool.setConstraint(5, 0, 1, 1, 1.0, 1.0, c);
//        this.add(upCombo_, c);

        // Set view
        JButton viewButton = new JButton("Reset view");
        viewButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    viewButton_ActionPerformed(evt);
                }
            }
        );
        PatchTool.setConstraint(1, 0, 1, 1, 1.0, 1.0, c);
        this.add(viewButton, c);

        // Front clip
        JLabel frontClipLabel = new JLabel("FrontClip");
        PatchTool.setConstraint(0, 1, 1, 1, 1.0, 1.0, c);
        this.add(frontClipLabel, c);

        frontClipText_ = new JTextField("0.1");
        frontClipText_.setToolTipText("Front clip distance");
        PatchTool.setConstraint(1, 1, 1, 1, 1.0, 1.0, c);
        this.add(frontClipText_, c);

        JButton frontClipButton = new JButton("Set");
        frontClipButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    frontClipButton_ActionPerformed(evt);
                }
            }
        );
        frontClipButton.setToolTipText("Set front clip distance");
        PatchTool.setConstraint(2, 1, 1, 1, 1.0, 1.0, c);
        this.add(frontClipButton, c);

        // Back clip
        JLabel backClipLabel_ = new JLabel("BackClip");
        PatchTool.setConstraint(0, 2, 1, 1, 1.0, 1.0, c);
        this.add(backClipLabel_, c);

        backClipText_ = new JTextField("1000");
        backClipText_.setToolTipText("Back clip distance");
        PatchTool.setConstraint(1, 2, 1, 1, 1.0, 1.0, c);
        this.add(backClipText_, c);

        JButton backClipButton = new JButton("Set");
        backClipButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    backClipButton_ActionPerformed(evt);
                }
            }
        );
        backClipButton.setToolTipText("Set back clip distance");
        PatchTool.setConstraint(2, 2, 1, 1, 1.0, 1.0, c);
        this.add(backClipButton, c);

        // Pick mode
        JLabel pickLabel = new JLabel("PickMode");
        PatchTool.setConstraint(0, 3, 1, 1, 1.0, 1.0, c);
        this.add(pickLabel, c);

        pickModeCombo_ = new JComboBox();
        pickModeCombo_.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    pickModeCombo_ActionPerformed(evt);
                }
            }
        );
        PatchTool.setConstraint(1, 3, 1, 1, 1.0, 1.0, c);
        this.add(pickModeCombo_, c);


        // Draw mode for polygons
        JLabel polyModeLabel = new JLabel("DrawMode");
        PatchTool.setConstraint(0, 4, 1, 1, 1.0, 1.0, c);
        this.add(polyModeLabel, c);

        drawModeCombo_ = new JComboBox();
        drawModeCombo_.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    drawModeCombo_ActionPerformed(evt);
                }
            }
        );
        PatchTool.setConstraint(1, 4, 1, 1, 1.0, 1.0, c);
        this.add(drawModeCombo_, c);

        cullModeCombo_ = new JComboBox();
        cullModeCombo_.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    cullModeCombo_ActionPerformed(evt);
                }
            }
        );
        cullModeCombo_.setToolTipText("Polygon culling method");
        PatchTool.setConstraint(2, 4, 1, 1, 1.0, 1.0, c);
        this.add(cullModeCombo_, c);

        transText_ = new JTextField("0");
        transText_.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    transText_ActionPerformed(evt);
                }
            }
        );
        transText_.setToolTipText("Transparency for all display shapes");
        PatchTool.setConstraint(3, 4, 1, 1, 1.0, 1.0, c);
        this.add(transText_, c);



        // Current face
        JLabel faceLabel = new JLabel("Face");
        PatchTool.setConstraint(0, 5, 1, 1, 1.0, 1.0, c);
        this.add(faceLabel, c);

        faceText_ = new JTextField("-1");
        faceText_.setEditable(false);
        PatchTool.setConstraint(0, 6, 1, 1, 1.0, 1.0, c);
        this.add(faceText_, c);

        // Current edge
        JLabel edgeLabel = new JLabel("Edge");
        PatchTool.setConstraint(1, 5, 1, 1, 1.0, 1.0, c);
        this.add(edgeLabel, c);

        edgeText_ = new JTextField("-1");
        edgeText_.setEditable(false);
        PatchTool.setConstraint(1, 6, 1, 1, 1.0, 1.0, c);
        this.add(edgeText_, c);

        // Current vertex
        JLabel vertexLabel = new JLabel("Vertex");
        PatchTool.setConstraint(2, 5, 1, 1, 1.0, 1.0, c);
        this.add(vertexLabel, c);

        vertexText_ = new JTextField("-1");
        vertexText_.setEditable(false);
        PatchTool.setConstraint(2, 6, 1, 1, 1.0, 1.0, c);
        this.add(vertexText_, c);

        // Current position
        JLabel positionLabel = new JLabel("Coord");
        PatchTool.setConstraint(0, 7, 1, 1, 1.0, 1.0, c);
        this.add(positionLabel, c);

        positionPanel_ = new VectorPanel(new Vector3d(0.0, 0.0, 0.0));
        PatchTool.setConstraint(1, 7, 1, 1, 1.0, 1.0, c);
        this.add(positionPanel_, c);

    }

    //--------------------------------------------------------------------------

//    private void upCombo_ActionPerformed(ActionEvent evt)
//    {
//        String fieldType = (String)upCombo_.getSelectedItem();
//
//        up_ = (Vector3d)namedVectors_.get(fieldType);
//    }

    //--------------------------------------------------------------------------

    private void viewButton_ActionPerformed(ActionEvent evt)
    {
//        try
//        {
//            // Set rotation/translation
//            Point3d eye = eyeVector_.getPoint3d();
//            Point3d center = lookVector_.getPoint3d();
//
//            // Set transform matrix
//            look_.lookAt(eye, center, up_);
//            look_.invert();
//
//            // Set matrix as current transform
//            ViewingPlatform viewingPlatform =
//                pp_.getUniverse().getViewingPlatform();
//            TransformGroup viewTrans =
//            viewingPlatform.getViewPlatformTransform();
//            viewTrans.setTransform(look_);
//
//            // Reset center of rotation
//            OrbitBehavior orbit =
//                (OrbitBehavior) viewingPlatform.getViewPlatformBehavior();
//
//            if (orbit != null)
//            {
//                orbit.setRotationCenter(center);
//
//                // Hack to cause update of orbitbehavior
//                orbit.setViewingPlatform(viewingPlatform);
//            }
//        }
//        catch(NumberFormatException nex)
//        {
//            throw new IllegalArgumentException
//            (
//                "view specification should only contain numbers"
//            );
//        }
        pp_.getMouseBehavior().reset();
    }

    //--------------------------------------------------------------------------

    private void frontClipButton_ActionPerformed(ActionEvent evt)
    {
        try
        {
            float val = Float.parseFloat(frontClipText_.getText());

            View view = pp_.getUniverse().getViewer().getView();

            view.setFrontClipDistance(val);
        }
        catch(NumberFormatException nex)
        {
            System.out.println
            (
                "view specification should only contain numbers"
            );
        }
    }

    //--------------------------------------------------------------------------

    private void backClipButton_ActionPerformed(ActionEvent evt)
    {
        try
        {
            float val = Float.parseFloat(backClipText_.getText());

            View view = pp_.getUniverse().getViewer().getView();

            view.setBackClipDistance(val);
        }
        catch(NumberFormatException nex)
        {
            System.out.println
            (
                "view specification should only contain numbers"
            );
        }
    }

    //--------------------------------------------------------------------------

    private void pickModeCombo_ActionPerformed(ActionEvent evt)
    {
        int index = pickModeCombo_.getSelectedIndex();

        pp_.setPickMode(pickModeValues_[index]);
    }

    //--------------------------------------------------------------------------

    private void drawModeCombo_ActionPerformed(ActionEvent evt)
    {
        int index = drawModeCombo_.getSelectedIndex();

        Appearance app = pp_.getPolyAppearance();

        if (app != null)
        {
            PolygonAttributes polyAttrib = app.getPolygonAttributes();

            // Check change of polygon rendering mode
            if (drawModeValues_[index] != polyAttrib.getPolygonMode())
            {
                polyAttrib.setPolygonMode(drawModeValues_[index]);
            }
        }
    }

    //--------------------------------------------------------------------------

    private void cullModeCombo_ActionPerformed(ActionEvent evt)
    {
        int index = cullModeCombo_.getSelectedIndex();

        Appearance app = pp_.getPolyAppearance();

        if (app != null)
        {
            PolygonAttributes polyAttrib = app.getPolygonAttributes();

            // Check change of polygon rendering mode
            if (polyCullValues_[index] != polyAttrib.getCullFace())
            {
                polyAttrib.setCullFace(polyCullValues_[index]);
            }
        }
    }

    //--------------------------------------------------------------------------

    private void transText_ActionPerformed(ActionEvent evt)
    {
        Appearance app = pp_.getPolyAppearance();

        if (app == null)
        {
            return;
        }

        try
        {
            TransparencyAttributes attr = app.getTransparencyAttributes();

            float trans = Float.valueOf(transText_.getText()).floatValue();

            if (trans != attr.getTransparency())
            {
                if (trans < 0.01f)
                {
                    attr.setTransparency(0.0f);
                    attr.setTransparencyMode(TransparencyAttributes.NONE);
                }
                else
                {
                    attr.setTransparency(trans);
                    attr.setTransparencyMode(TransparencyAttributes.NICEST);
                }
            }
        }
        catch (NumberFormatException nex)
        {
            System.out.println("Illegal float " + transText_.getText());
        }
    }

    //--------------------------------------------------------------------------
    //---- PickListener Methods
    //--------------------------------------------------------------------------

    // Single element picked
    public void picked(MeshPickEvent evt)
    {
        if (evt.getFace() != -1)
        {
            faceText_.setText(Integer.toString(evt.getFace()));
        }

        if (evt.getEdge() != -1)
        {
            edgeText_.setText(Integer.toString(evt.getEdge()));
        }

        if (evt.getVertex() != -1)
        {
            vertexText_.setText(Integer.toString(evt.getVertex()));
        }

        Point3d pt = evt.getPoint();

        positionPanel_.setPoint(pt);
    }

    // Single element unpicked
    public void unpicked(MeshPickEvent evt)
    {}


    //--------------------------------------------------------------------------
    //---- UI components
    //--------------------------------------------------------------------------

    private VectorPanel eyeVector_;
    private VectorPanel lookVector_;
    private JComboBox upCombo_;

    private JComboBox pickModeCombo_;

    private JTextField frontClipText_;
    private JTextField backClipText_;

    private JComboBox drawModeCombo_;
    private JComboBox cullModeCombo_;
    private JTextField transText_;

    private JTextField faceText_;
    private JTextField edgeText_;
    private JTextField vertexText_;
    private VectorPanel positionPanel_;

    //--------------------------------------------------------------------------
}



