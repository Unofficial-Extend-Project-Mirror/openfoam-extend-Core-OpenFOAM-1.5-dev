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
import java.awt.Container;
import java.awt.GridLayout;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.event.*;
import java.awt.event.ActionEvent;
import java.awt.Graphics;
import java.awt.GraphicsConfiguration;
import java.io.*;
import java.util.*;
import javax.swing.*;
import javax.swing.WindowConstants;
import javax.swing.event.*;

import com.sun.j3d.utils.universe.*;
import javax.media.j3d.*;
import javax.vecmath.*;


import PatchTool.PatchTool;
import PatchTool.Panels.CasePanel;
import PatchTool.Panels.PatchPanel;
import PatchTool.Panels.FaceSetPanel;
import PatchTool.Events.CaseEvent;
import PatchTool.Events.CaseStatusListener;


public class PreProcPanel
    extends javax.swing.JPanel
    implements CaseStatusListener
{
    //--------------------------------------------------------------------------

    private PatchTool pp_;

    //--------------------------------------------------------------------------
    /** Creates new form PreProcPanel */
    public PreProcPanel(PatchTool pp)
    {
        super();

        pp_ = pp;

        initComponents();

        // Make sure we know about geometry loaded (to enable relevant tabs)
        pp.addCaseStatusListener(this);
    }

    //--------------------------------------------------------------------------

    public CasePanel getCasePanel()
    {
        return casePanel_;
    }

    //--------------------------------------------------------------------------

    public ViewPanel getViewPanel()
    {
        return viewPanel_;
    }

    //--------------------------------------------------------------------------

    public PatchPanel getPatchPanel()
    {
        return patchPanel_;
    }

    //--------------------------------------------------------------------------

    public FaceSetPanel getFaceSetPanel()
    {
        return faceSetPanel_;
    }

    //--------------------------------------------------------------------------

    public DisplayPanel getDisplayPanel()
    {
        return displayPanel_;
    }

    //--------------------------------------------------------------------------

    public DisplayListPanel getDisplayListPanel()
    {
        return displayListPanel_;
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

        // Info line
        infoLabel_ = new JLabel("shift+Mouse1:select    shift+Mouse3:deselect");
        PatchTool.setConstraint(0, 0, 1, 1, 1.0, 1.0, c);
        this.add(infoLabel_, c);

        //
        // Construct tabbedPane
        //
        viewPanel_ = new ViewPanel(pp_);
        casePanel_ = new CasePanel(pp_);
        displayPanel_ = new DisplayPanel(pp_);
        displayListPanel_ = new DisplayListPanel(pp_);
        patchPanel_ = new PatchPanel(pp_);
        faceSetPanel_ = new FaceSetPanel(pp_);

        tabbedPanel_ = new JTabbedPane();
        tabbedPanel_.addTab("Case", casePanel_);
        tabbedPanel_.addTab("Patch", patchPanel_);
        tabbedPanel_.addTab("Faces", faceSetPanel_);
        tabbedPanel_.addTab("Display", displayPanel_);
        tabbedPanel_.addTab("DisplayList", displayListPanel_);
        tabbedPanel_.addTab("View", viewPanel_);


        // Start off with case panel
        tabbedPanel_.setSelectedComponent(casePanel_); 

        // Add to this
        PatchTool.setConstraint(0, 1, 1, 1, 1.0, 1.0, c);
        this.add(tabbedPanel_, c);
    }

    //--------------------------------------------------------------------------

    public String getMsg()
    {
        return infoLabel_.getText();
    }

    //--------------------------------------------------------------------------

    public void setMsg(String msg)
    {
        infoLabel_.setText(msg);
    }

    //--------------------------------------------------------------------------
    //---- CaseStatusListener Implementation
    //--------------------------------------------------------------------------

    public void rootSelected(CaseEvent evt)
    {}

    public void caseSelected(CaseEvent evt)
    {}

    public void timeSelected(CaseEvent evt)
    {}

    public void geomLoaded(CaseEvent evt)
    {
        int index = tabbedPanel_.indexOfComponent(displayPanel_);
        tabbedPanel_.setEnabledAt(index, true);

        index = tabbedPanel_.indexOfComponent(patchPanel_);
        tabbedPanel_.setEnabledAt(index, true);

        index = tabbedPanel_.indexOfComponent(faceSetPanel_);
        tabbedPanel_.setEnabledAt(index, true);

        // Bring display panel to front
        tabbedPanel_.setSelectedComponent(displayPanel_); 
    }

    //--------------------------------------------------------------------------
    //---- UI components
    //--------------------------------------------------------------------------

    private JLabel infoLabel_;
    private JTabbedPane tabbedPanel_;
    private ViewPanel viewPanel_;
    private CasePanel casePanel_;
    private PatchPanel patchPanel_;
    private FaceSetPanel faceSetPanel_;
    private DisplayPanel displayPanel_;
    private DisplayListPanel displayListPanel_;

    //--------------------------------------------------------------------------
}



