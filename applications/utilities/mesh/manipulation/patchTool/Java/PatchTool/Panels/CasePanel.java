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
import PatchTool.Events.CaseEvent;
import PatchTool.Events.CaseStatusListener;
import PatchTool.Events.TimeChangeEvent;
import PatchTool.Events.TimeChangeListener;
import PatchTool.Mesh.BoundaryMesh;
import PatchTool.Mesh.SurfacePatch;
import PatchTool.Util.ExampleFileFilter;
import PatchTool.Util.FoamDataWriter;

public class CasePanel
    extends javax.swing.JPanel
    implements CaseStatusListener
{
    //--------------------------------------------------------------------------

    protected PatchTool pp_;


    //--------------------------------------------------------------------------
    /** Creates new form CasePanel */
    public CasePanel(PatchTool pp)
    {
        super();

        pp_ = pp;

        initComponents();

        // Make sure we know about loaded geometry.
        pp_.addCaseStatusListener(this);
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


        // Connect button
        JButton connectButton = new JButton("Connect");
        connectButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    connectButton_ActionPerformed(evt);
                }
            }
        );
        connectButton.setToolTipText
        (
            "Connect to a running nameserver and patch server"
        );
        PatchTool.setConstraint(1, 0, 1, 1, 1.0, 1.0, c);
        this.add(connectButton, c);
        

        // Root entry
        JLabel rootLabel = new JLabel("Root");
        PatchTool.setConstraint(0, 1, 1, 1, 1.0, 1.0, c);
        this.add(rootLabel, c);

        rootText_ = new JTextField();
        rootText_.setEditable(false);
        PatchTool.setConstraint(1, 1, 2, 1, 1.0, 1.0, c);
        this.add(rootText_, c);


        // Case entry
        JLabel caseLabel = new JLabel("Case");
        PatchTool.setConstraint(0, 2, 1, 1, 1.0, 1.0, c);
        this.add(caseLabel, c);

        caseText_ = new JTextField();
        caseText_.setEditable(false);
        PatchTool.setConstraint(1, 2, 2, 1, 1.0, 1.0, c);
        this.add(caseText_, c);


        // Time entry
        JLabel timeLabel = new JLabel("Time");
        PatchTool.setConstraint(0, 3, 1, 1, 1.0, 1.0, c);
        this.add(timeLabel, c);

        timeText_ = new JTextField();
        timeText_.setEditable(false);
        PatchTool.setConstraint(1, 3, 2, 1, 1.0, 1.0, c);
        this.add(timeText_, c);


        // Load mesh
        JButton loadMeshButton = new JButton("Load Mesh");
        loadMeshButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    loadMeshButton_ActionPerformed(evt);
                }
            }
        );
        loadMeshButton.setToolTipText
        (
            "Tell the patch server to load from a polyMesh"
        );
        PatchTool.setConstraint(1, 4, 1, 1, 1.0, 1.0, c);
        this.add(loadMeshButton, c);


        // Load file
        fileChooser_ = new JFileChooser("./");

        ExampleFileFilter filter = new ExampleFileFilter();
        filter.addExtension("ftr");
        filter.setDescription("Foam boundaryMesh");
        fileChooser_.setFileFilter(filter);
        fileChooser_.setSelectedFile(new File("boundaryMesh.ftr"));

        JButton loadFileButton = new JButton("Load File");
        loadFileButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    loadFileButton_ActionPerformed(evt);
                }
            }
        );
        loadFileButton.setToolTipText
        (
            "Tell the patch server to load from a surface file"
        );
        PatchTool.setConstraint(3, 4, 1, 1, 1.0, 1.0, c);
        this.add(loadFileButton, c);

        // Save mesh
        JButton saveMeshButton = new JButton("Save Mesh");
        saveMeshButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    saveMeshButton_ActionPerformed(evt);
                }
            }
        );
        saveMeshButton.setToolTipText
        (
            "Apply patch information to the polyMesh and save"
        );
        PatchTool.setConstraint(1, 5, 1, 1, 1.0, 1.0, c);
        this.add(saveMeshButton, c);


        // Save file
        JButton saveFileButton = new JButton("Save File");
        saveFileButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    saveFileButton_ActionPerformed(evt);
                }
            }
        );
        saveFileButton.setToolTipText
        (
            "Apply patch information and write surface file"
        );
        PatchTool.setConstraint(3, 5, 1, 1, 1.0, 1.0, c);
        this.add(saveFileButton, c);

        // Close button
        JButton disconnectButton = new JButton("Disconnect");
        disconnectButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    disconnectButton_ActionPerformed(evt);
                }
            }
        );
        disconnectButton.setToolTipText
        (
            "Disconnect from running nameserver and patch server"
        );
        PatchTool.setConstraint(1, 6, 1, 1, 1.0, 1.0, c);
        this.add(disconnectButton, c);
        
    }

    //--------------------------------------------------------------------------

    private void connectButton_ActionPerformed(ActionEvent evt)
    {
        if (pp_.getServer() != null)
        {
            JOptionPane.showMessageDialog
            (
                this,
                "Already connected to server.",
                "Connected",
                JOptionPane.ERROR_MESSAGE
            );
        }
        else
        {
            pp_.connect();
        }
    }

    //--------------------------------------------------------------------------

    private void disconnectButton_ActionPerformed(ActionEvent evt)
    {
        if (pp_.getServer() == null)
        {
            JOptionPane.showMessageDialog
            (
                this,
                "Not connected to server.",
                "Not connected",
                JOptionPane.ERROR_MESSAGE
            );

            return;
        }

        pp_.disconnect();
    }

    //--------------------------------------------------------------------------

    private void loadMeshButton_ActionPerformed(ActionEvent evt)
    {
        if (pp_.getServer() == null)
        {
            JOptionPane.showMessageDialog
            (
                this,
                "Not connected to server.",
                "Not connected",
                JOptionPane.ERROR_MESSAGE
            );

            return;
        }


        if
        (
            (pp_.getMesh() == null)
         || (
                JOptionPane.showConfirmDialog
                (
                    this,
                    "Mesh already loaded. Reload and lose all changes ?",
                    "Load Mesh",
                    JOptionPane.YES_NO_OPTION,
                    JOptionPane.QUESTION_MESSAGE
                )
                ==
                JOptionPane.OK_OPTION
            )
        )
        {
            System.out.println
            (
                "Reading boundaryMesh from "
              + pp_.getServer().caseName()
            );

            BoundaryMesh mesh = new BoundaryMesh(pp_.getServer());

            mesh.read();

            pp_.setMesh(mesh);
        }
    }

    //--------------------------------------------------------------------------

    private void loadFileButton_ActionPerformed(ActionEvent evt)
    {
        if (pp_.getServer() == null)
        {
            JOptionPane.showMessageDialog
            (
                this,
                "Not connected to server.",
                "Not connected",
                JOptionPane.ERROR_MESSAGE
            );

            return;
        }


        if
        (
            (pp_.getMesh() == null)
         || (
                JOptionPane.showConfirmDialog
                (
                    this,
                    "Mesh already loaded. Reload and lose all changes ?",
                    "Load Mesh",
                    JOptionPane.YES_NO_OPTION,
                    JOptionPane.QUESTION_MESSAGE
                )
                ==
                JOptionPane.OK_OPTION
            )
        )
        {
            fileChooser_.setSelectedFile(new File("boundaryMesh.ftr"));
            fileChooser_.setApproveButtonText("Load");
            if
            (
                fileChooser_.showOpenDialog(this)
             == JFileChooser.APPROVE_OPTION
            )
            {
                System.out.println
                (
                    "Loading boundaryMesh from file "
                  + fileChooser_.getSelectedFile()
                );

                BoundaryMesh mesh = new BoundaryMesh(pp_.getServer());

                mesh.readTriSurface(fileChooser_.getSelectedFile());

                pp_.setMesh(mesh);
            }
        }
    }

    //--------------------------------------------------------------------------

    private void saveMeshButton_ActionPerformed(ActionEvent evt)
    {
        if (pp_.getMesh() == null)
        {
            JOptionPane.showMessageDialog
            (
                this,
                "No mesh loaded.",
                "Save Mesh",
                JOptionPane.ERROR_MESSAGE
            );

            return;
        }

        if
        (
            JOptionPane.showConfirmDialog
            (
                this,
                "Apply patch info to mesh ?",
                "Change Mesh",
                JOptionPane.YES_NO_OPTION,
                JOptionPane.QUESTION_MESSAGE
            )
            ==
            JOptionPane.OK_OPTION
        )
        {
            File file = new File("boundaryMesh.ftr");

            System.out.println("Writing boundary mesh to file " + file);
            System.out.println
            (
                "Use this file if modifying mesh directly fails."   
            );

            pp_.getMesh().writeTriSurface(file);


            System.out.println("Modifying mesh.");

            // Save polyMesh.
            pp_.getMesh().write();
        }
    }

    //--------------------------------------------------------------------------

    private void saveFileButton_ActionPerformed(ActionEvent evt)
    {
        fileChooser_.setSelectedFile(new File("boundaryMesh.ftr"));
        fileChooser_.setApproveButtonText("Save");
        if
        (
            fileChooser_.showOpenDialog(this)
         == JFileChooser.APPROVE_OPTION
        )
        {
            File file = fileChooser_.getSelectedFile();

            System.out.println("Writing boundary mesh to file " + file);

            pp_.getMesh().writeTriSurface(file);
        }
    }

    //--------------------------------------------------------------------------
    //---- CaseStatusListener Implementation
    //--------------------------------------------------------------------------

    public void rootSelected(CaseEvent evt)
    {
        rootText_.setText(evt.getCaseRoot());
    }

    public void caseSelected(CaseEvent evt)
    {
        caseText_.setText(evt.getCaseName());
    }

    public void timeSelected(CaseEvent evt)
    {
        timeText_.setText(evt.getTimeName());
    }

    public void geomLoaded(CaseEvent evt)
    {}


    //--------------------------------------------------------------------------
    //---- UI components
    //--------------------------------------------------------------------------

    JTextField rootText_;
    JTextField caseText_;
    JTextField timeText_;
    JFileChooser fileChooser_;

    //--------------------------------------------------------------------------
}



