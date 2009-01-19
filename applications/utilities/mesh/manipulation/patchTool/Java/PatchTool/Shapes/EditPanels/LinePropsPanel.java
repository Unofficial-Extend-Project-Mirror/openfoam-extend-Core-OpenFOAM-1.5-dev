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

package PatchTool.Shapes.EditPanels;

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
import PatchTool.Panels.ColorPanel;

public class LinePropsPanel extends javax.swing.JPanel
{
    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------
    /** Creates new form LinePropsPanel */
    public LinePropsPanel()
    {
        super();

        initComponents();
    }

    //--------------------------------------------------------------------------

    public Color3f getColor3f() throws NumberFormatException
    {
        return colorPanel_.getColor3f();
    }

    //--------------------------------------------------------------------------

    public void setColor3f(Color3f colour)
    {
        colorPanel_.setColor3f(colour);
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


        // Color
        colorLabel_ = new javax.swing.JLabel("Color");
        PatchTool.setConstraint(0, 0, 1, 1, 1.0, 1.0, c);
        this.add(colorLabel_, c);

        colorPanel_ = new ColorPanel("white");
        PatchTool.setConstraint(1, 0, 1, 1, 1.0, 1.0, c);
        this.add(colorPanel_, c);
    }


    //--------------------------------------------------------------------------
    //---- UI components
    //--------------------------------------------------------------------------

    private javax.swing.JLabel colorLabel_;
    private ColorPanel colorPanel_;

    //--------------------------------------------------------------------------
}

