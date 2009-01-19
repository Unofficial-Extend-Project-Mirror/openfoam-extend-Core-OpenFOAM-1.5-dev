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

public class ColorPanel extends javax.swing.JPanel
{
    //--------------------------------------------------------------------------
    /** Creates new form ColorPanel */
    public ColorPanel(String colorName)
    {
        super();

        initComponents(colorName);
    }

    //--------------------------------------------------------------------------

    public Color3f getColor3f() throws NumberFormatException
    {
        float rVal = Float.parseFloat(rText_.getText());
        float gVal = Float.parseFloat(gText_.getText());
        float bVal = Float.parseFloat(bText_.getText());

        return new Color3f(rVal, gVal, bVal);
    }

    //--------------------------------------------------------------------------

    public void setColor3f(Color3f colour)
    {
        rText_.setText(Float.toString(colour.x));
        gText_.setText(Float.toString(colour.y));
        bText_.setText(Float.toString(colour.z));
    }

    //--------------------------------------------------------------------------

    private void initComponents(String colorName)
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


        rText_ = new JTextField(Double.toString(1.0));
        rText_.setToolTipText("Red");
        PatchTool.setConstraint(0, 0, 1, 1, 1.0, 1.0, c);
        this.add(rText_, c);

        gText_ = new JTextField(Double.toString(1.0));
        gText_.setToolTipText("Green");
        PatchTool.setConstraint(0, 1, 1, 1, 1.0, 1.0, c);
        this.add(gText_, c);

        bText_ = new JTextField(Double.toString(1.0));
        bText_.setToolTipText("Blue");
        PatchTool.setConstraint(0, 2, 1, 1, 1.0, 1.0, c);
        this.add(bText_, c);
    }

    //--------------------------------------------------------------------------
    //---- UI components
    //--------------------------------------------------------------------------

    private JTextField rText_;
    private JTextField gText_;
    private JTextField bText_;

    //--------------------------------------------------------------------------
}



