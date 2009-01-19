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
import javax.swing.WindowConstants;
import javax.swing.event.*;

import com.sun.j3d.utils.universe.*;
import javax.media.j3d.*;
import javax.vecmath.*;


import PatchTool.PatchTool;
import PatchTool.Panels.PreProcPanel;
import PatchTool.Util.SimpleViewer;

public class PreProcDlg extends javax.swing.JDialog
{
    //--------------------------------------------------------------------------

    protected PatchTool pp_;

    //--------------------------------------------------------------------------
    /** Creates new form PreProcDlg */
    public PreProcDlg(java.awt.Frame parent, PatchTool pp)
    {
        super(parent, "Preprocess", false);   // non Modal.

        pp_ = pp;

        initComponents(pp);
    }

    //--------------------------------------------------------------------------

    public PreProcPanel getPreProcPanel()
    {
        return preProcPanel_;
    }

    //--------------------------------------------------------------------------

    private void initComponents(PatchTool pp)
    {
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        addWindowListener
        (
            new java.awt.event.WindowAdapter()
            {
                public void windowClosing(java.awt.event.WindowEvent evt)
                {
                    quit_ActionPerformed();
                }
            }
        );

        JMenuBar menuBar = new JMenuBar();

        // Add File menu
	JMenu fileMenu = buildFileMenu();
	menuBar.add(fileMenu);

        // Add Help menu
	JMenu helpMenu = buildHelpMenu();
	menuBar.add(helpMenu);

        // Add menubar to pane
	setJMenuBar(menuBar);	

        preProcPanel_ = new PreProcPanel(pp);

        getContentPane().add(preProcPanel_);
        pack();
    }

    //--------------------------------------------------------------------------

    private JMenu buildFileMenu() {
	JMenu fileMenu = new JMenu("File");
	JMenuItem quitItem = new JMenuItem("Quit");
        quitItem.setMnemonic(KeyEvent.VK_Q);

	quitItem.addActionListener
        (
            new ActionListener()
            {
	        public void actionPerformed(ActionEvent e)
                {
		   quit_ActionPerformed();
                }
            }
        );

	fileMenu.add(quitItem);

	return fileMenu;
    }

    //--------------------------------------------------------------------------

    private void quit_ActionPerformed()
    {
        if
        (
            JOptionPane.showConfirmDialog
            (
                this,
                "Are you sure you want to quit?",
                "Confirm Quit Operation",
                JOptionPane.YES_NO_OPTION,
                JOptionPane.QUESTION_MESSAGE
            ) == JOptionPane.OK_OPTION
        )
        {
            pp_.quit();
        }
    }

    //--------------------------------------------------------------------------

    private JMenu buildHelpMenu()
    {
	JMenu helpMenu = new JMenu("Help");

        // 'About'
	JMenuItem aboutItem = new JMenuItem("About");

	aboutItem.addActionListener
        (
            new ActionListener()
            {
	        public void actionPerformed(ActionEvent e)
                {
		   about_ActionPerformed(e);
                }
            }
        );

	helpMenu.add(aboutItem);


        // 'Help'
	JMenuItem helpItem = new JMenuItem("Help");

	helpItem.addActionListener
        (
            new ActionListener()
            {
	        public void actionPerformed(ActionEvent e)
                {
		   help_ActionPerformed(e);
                }
            }
        );

	helpMenu.add(helpItem);

	return helpMenu;
    }

    //--------------------------------------------------------------------------

    private void about_ActionPerformed(ActionEvent e)
    {
        String aboutMsg =
            "PatchTool v1.0\n"
          + "\n"
          + "Tool to add patches to Foam meshes (or surfaces)\n";

        JOptionPane.showMessageDialog(this, aboutMsg);
    }


    //--------------------------------------------------------------------------

    private void help_ActionPerformed(ActionEvent e)
    {
        String sysPath = System.getProperty("FoamX.PatchToolPath", ",");

        String fName = sysPath + "/doc/help.txt";

        System.out.println("Reading help file from " + fName);

        SimpleViewer frame = new SimpleViewer(new File(fName));

        frame.pack();
        frame.setVisible(true);
    }

    //--------------------------------------------------------------------------
    //---- UI components
    //--------------------------------------------------------------------------

    private PreProcPanel preProcPanel_;

    //--------------------------------------------------------------------------
}



