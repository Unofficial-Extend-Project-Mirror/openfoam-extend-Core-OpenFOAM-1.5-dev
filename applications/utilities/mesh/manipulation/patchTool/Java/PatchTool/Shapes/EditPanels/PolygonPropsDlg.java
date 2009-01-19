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
import com.sun.j3d.utils.universe.*;
import javax.media.j3d.*;
import javax.vecmath.*;



//import PatchTool.Shapes.PolygonPropsPanel;


public class PolygonPropsDlg extends javax.swing.JDialog
{
    //--------------------------------------------------------------------------

    public static final int OK = 0;
    public static final int CANCEL = 1;

    protected int state_;


    //--------------------------------------------------------------------------
    /** Creates new form PolygonPropsDlg */
    public PolygonPropsDlg(java.awt.Frame parent, Color3f colour)
    {
        super(parent, "Polygon Properties", true);   // modal.

        state_ = -1;

        initComponents();

        polygonPropsPanel_.setColor3f(colour);

        setVisible(true);
    }

    //--------------------------------------------------------------------------

    public int getState()
    {
        return state_;
    }

    //--------------------------------------------------------------------------

    public PolygonPropsPanel getPolygonPropsPanel()
    {
        return polygonPropsPanel_;
    }

    //--------------------------------------------------------------------------

    private void initComponents()
    {
        // This panel
        getContentPane().setLayout(new GridBagLayout());

        GridBagConstraints c = new GridBagConstraints();

        // Add insets around all to avoid clutter
        c.insets = new Insets(2, 2, 2, 2);

        // Anchor all same way
        c.anchor = GridBagConstraints.WEST;

        // Fill horizontal
        c.fill = GridBagConstraints.HORIZONTAL;


        // Polygon edit panel
        polygonPropsPanel_ = new PolygonPropsPanel();
        setConstraint(0, 0, 1, 1, 1.0, 1.0, c);
        getContentPane().add(polygonPropsPanel_, c);

        // Ok button
        okButton_ = new javax.swing.JButton("Ok");
        okButton_.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    state_ = OK;
                    setVisible(false);
                    dispose();
                }
            }
        );
        setConstraint(0, 1, 1, 1, 1.0, 1.0, c);
        getContentPane().add(okButton_, c);

        // Cancel button
        cancelButton_ = new javax.swing.JButton("Cancel");
        cancelButton_.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    state_ = CANCEL;
                    setVisible(false);
                    dispose();
                }
            }
        );
        setConstraint(1, 1, 1, 1, 1.0, 1.0, c);
        getContentPane().add(cancelButton_, c);

        pack();
    }

    //--------------------------------------------------------------------------

    private static void setConstraint
    (
        int x,
        int y,
        int w,
        int h,
        double wx,
        double wy,
        GridBagConstraints c
    )
    {
        c.gridx = x;
        c.gridy = y;
        c.gridwidth = w;
        c.gridheight = h;
        c.weightx = wx;
        c.weighty = wy;
    }

    //--------------------------------------------------------------------------
    //---- UI components
    //--------------------------------------------------------------------------

    private PolygonPropsPanel polygonPropsPanel_;
    private javax.swing.JButton okButton_;
    private javax.swing.JButton cancelButton_;

    //--------------------------------------------------------------------------
}



