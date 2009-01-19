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

public class VectorPanel extends javax.swing.JPanel
{
    //--------------------------------------------------------------------------
    /** Creates new form VectorPanel */
    public VectorPanel(Vector3d vec)
    {
        super();

        initComponents(vec);
    }

    //--------------------------------------------------------------------------

    public Point3d getPoint3d() throws NumberFormatException
    {
        float xVal = Float.parseFloat(xText_.getText());
        float yVal = Float.parseFloat(yText_.getText());
        float zVal = Float.parseFloat(zText_.getText());

        return new Point3d(xVal, yVal, zVal);
    }

    //--------------------------------------------------------------------------

    public Point3f getPoint3f() throws NumberFormatException
    {
        float xVal = Float.parseFloat(xText_.getText());
        float yVal = Float.parseFloat(yText_.getText());
        float zVal = Float.parseFloat(zText_.getText());

        return new Point3f(xVal, yVal, zVal);
    }

    //--------------------------------------------------------------------------

    public Vector3d getVector3d() throws NumberFormatException
    {
        float xVal = Float.parseFloat(xText_.getText());
        float yVal = Float.parseFloat(yText_.getText());
        float zVal = Float.parseFloat(zText_.getText());

        return new Vector3d(xVal, yVal, zVal);
    }

    //--------------------------------------------------------------------------

    public Vector3f getVector3f() throws NumberFormatException
    {
        float xVal = Float.parseFloat(xText_.getText());
        float yVal = Float.parseFloat(yText_.getText());
        float zVal = Float.parseFloat(zText_.getText());

        return new Vector3f(xVal, yVal, zVal);
    }

    //--------------------------------------------------------------------------

    public void setPoint(Point3f pt)
    {
        xText_.setText(Float.toString(pt.x));
        yText_.setText(Float.toString(pt.y));
        zText_.setText(Float.toString(pt.z));
    }

    //--------------------------------------------------------------------------

    public void setPoint(Point3d pt)
    {
        xText_.setText(Double.toString(pt.x));
        yText_.setText(Double.toString(pt.y));
        zText_.setText(Double.toString(pt.z));
    }

    //--------------------------------------------------------------------------

    private void initComponents(Vector3d vec)
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


        xText_ = new JTextField(Double.toString(vec.x));
        PatchTool.setConstraint(0, 0, 1, 1, 1.0, 1.0, c);
        this.add(xText_, c);

        yText_ = new JTextField(Double.toString(vec.y));
        PatchTool.setConstraint(0, 1, 1, 1, 1.0, 1.0, c);
        this.add(yText_, c);

        zText_ = new JTextField(Double.toString(vec.z));
        PatchTool.setConstraint(0, 2, 1, 1, 1.0, 1.0, c);
        this.add(zText_, c);
    }

    //--------------------------------------------------------------------------
    //---- UI components
    //--------------------------------------------------------------------------

    private JTextField xText_;
    private JTextField yText_;
    private JTextField zText_;

    //--------------------------------------------------------------------------
}



