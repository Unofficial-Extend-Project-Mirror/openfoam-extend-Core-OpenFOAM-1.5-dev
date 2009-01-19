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
import PatchTool.Events.CaseStatusListener;
import PatchTool.Events.ElementEvent;
import PatchTool.Events.ElementSelectListener;
import PatchTool.InteractiveNodes.PatchFacesNode;
import PatchTool.Mesh.SurfacePatch;
import PatchTool.Mesh.FaceSet;
import PatchTool.Mesh.BoundaryMeshUtils;
import PatchTool.Mesh.BoundaryMesh;
import PatchTool.Util.ExampleFileFilter;
import PatchTool.Util.SelectLoop;

public class FaceSetPanel
    extends javax.swing.JPanel
    implements PatchStatusListener, CaseStatusListener, ElementSelectListener
{
    //--------------------------------------------------------------------------

    protected PatchTool pp_;

    // Current file name counter
    protected int saveCounter_;

    // Whether to select(true) or deselect(false) loop faces
    boolean selectLoop_;

    //--------------------------------------------------------------------------
    /** Creates new form FaceSetPanel */
    public FaceSetPanel(PatchTool pp)
    {
        super();

        pp_ = pp;

        saveCounter_ = 0;

        selectLoop_ = true;

        initComponents();

        // Make sure we know about geometry loaded
        pp_.addCaseStatusListener(this);

        // Make sure we know about current patch
        pp_.addPatchStatusListener(this);

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


        // Global

        // Filename (not displayed on panel)
        fileChooser_ = new JFileChooser("./");
        ExampleFileFilter filter = new ExampleFileFilter();
        filter.addExtension("foam");
        filter.setDescription("Foam set");
        fileChooser_.setFileFilter(filter);

        fileChooser_.setSelectedFile
        (
            new File("faceList" + saveCounter_ + ".foam")
        );

        JButton loadButton = new JButton("Load");
        loadButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    loadButton_ActionPerformed(evt);
                }
            }
        );
        loadButton.setToolTipText
        (
            "Load face set from file"
        );
        PatchTool.setConstraint(0, 0, 1, 1, 1.0, 1.0, c);
        this.add(loadButton, c);

        JButton saveButton = new JButton("Save");
        saveButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    saveButton_ActionPerformed(evt);
                }
            }
        );
        saveButton.setToolTipText
        (
            "Save current face set to file"
        );
        PatchTool.setConstraint(1, 0, 1, 1, 1.0, 1.0, c);
        this.add(saveButton, c);

        JButton showButton = new JButton("Show");
        showButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    showButton_ActionPerformed(evt);
                }
            }
        );
        showButton.setToolTipText
        (
            "Print current face set"
        );
        PatchTool.setConstraint(2, 0, 1, 1, 1.0, 1.0, c);
        this.add(showButton, c);

        JButton clearButton = new JButton("Clear");
        clearButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    clearButton_ActionPerformed(evt);
                }
            }
        );
        clearButton.setToolTipText
        (
            "Clear current face set"
        );
        PatchTool.setConstraint(3, 0, 1, 1, 1.0, 1.0, c);
        this.add(clearButton, c);


        // Size display
        JLabel sizeLabel = new JLabel("Size");
        PatchTool.setConstraint(0, 1, 1, 1, 1.0, 1.0, c);
        this.add(sizeLabel, c);

        sizeText_ = new JTextField();
        sizeText_.setEditable(false);
        PatchTool.setConstraint(1, 1, 1, 1, 1.0, 1.0, c);
        this.add(sizeText_, c);


        // Add from patch
        JButton addButton = new JButton("Add from");
        addButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    addButton_ActionPerformed(evt);
                }
            }
        );
        addButton.setToolTipText
        (
            "Add all faces of selected patch to face set"
        );
        PatchTool.setConstraint(0, 2, 1, 1, 1.0, 1.0, c);
        this.add(addButton, c);

        currentPatchText_ = new JTextField();
        currentPatchText_.setEditable(false);
        PatchTool.setConstraint(1, 2, 1, 1, 1.0, 1.0, c);
        this.add(currentPatchText_, c);

        // Delete from patch
        JButton deleteButton = new JButton("Delete from");
        deleteButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    deleteButton_ActionPerformed(evt);
                }
            }
        );
        deleteButton.setToolTipText
        (
            "Delete all faces of selected patch from face set"
        );
        PatchTool.setConstraint(2, 2, 1, 1, 1.0, 1.0, c);
        this.add(deleteButton, c);


        // Loop
        JButton addLoopButton = new JButton("Add from loop");
        addLoopButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    addLoopButton_ActionPerformed(evt);
                }
            }
        );
        addLoopButton.setToolTipText
        (
            "Add all faces on inside of edge loop to face set"
        );
        PatchTool.setConstraint(0, 3, 1, 1, 1.0, 1.0, c);
        this.add(addLoopButton, c);

        JButton deleteLoopButton = new JButton("Delete from loop");
        deleteLoopButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    deleteLoopButton_ActionPerformed(evt);
                }
            }
        );
        deleteLoopButton.setToolTipText
        (
            "Delete all faces on inside of edge loop from face set"
        );
        PatchTool.setConstraint(1, 3, 1, 1, 1.0, 1.0, c);
        this.add(deleteLoopButton, c);


        // Invert
        JButton invertButton = new JButton("Invert");
        invertButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    invertButton_ActionPerformed(evt);
                }
            }
        );
        invertButton.setToolTipText
        (
            "Invert face set"
        );
        PatchTool.setConstraint(0, 4, 1, 1, 1.0, 1.0, c);
        this.add(invertButton, c);


        // Change
        JButton changeButton = new JButton("Change to");
        changeButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    changeButton_ActionPerformed(evt);
                }
            }
        );
        changeButton.setToolTipText
        (
            "Change patch type of faces in face set to selected patch"
        );
        PatchTool.setConstraint(0, 5, 1, 1, 1.0, 1.0, c);
        this.add(changeButton, c);

        changeText_ = new JTextField();
        changeText_.setEditable(false);
        PatchTool.setConstraint(1, 5, 1, 1, 1.0, 1.0, c);
        this.add(changeText_, c);
    }

    //--------------------------------------------------------------------------
    //---- PatchStatusListener Implementation
    //--------------------------------------------------------------------------

    // Use to update current patch name
    public void patchSelected(PatchEvent evt)
    {
        currentPatchText_.setText(evt.getPatchName());
        changeText_.setText(evt.getPatchName());
    }

    public void patchAdded(PatchEvent evt)
    {}

    public void patchDeleted(PatchEvent evt)
    {}

    public void patchTypeChanged(PatchEvent evt)
    {}

    public void patchFacesChanged(PatchEvent evt)
    {}

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
        // Only now do we have mesh and faceSet.
        FaceSet fs = pp_.getMesh().getFaceSet();

        // Need to get warned of change of number of selected faces
        // (since we maintain a running display of number of selected faces)
        fs.addElementSelectListener(this);
    }

    //--------------------------------------------------------------------------

    private void loadButton_ActionPerformed(ActionEvent evt)
    {
        FaceSet fs = pp_.getMesh().getFaceSet();

        fileChooser_.setApproveButtonText("Load");
        if
        (
            fileChooser_.showOpenDialog(this)
         == JFileChooser.APPROVE_OPTION
        )
        {
            System.out.println("Load from " + fileChooser_.getSelectedFile());

            try
            {
                FileInputStream inStream = new FileInputStream
                (
                    fileChooser_.getSelectedFile()
                );

                Reader rdr = new BufferedReader
                (
                    new InputStreamReader(inStream)
                );

                fs.read(rdr);
            }
            catch(FileNotFoundException ex)
            {
                System.out.println("FileNotFoundException " + ex);
            }

            saveCounter_--;
        }
    }

    //--------------------------------------------------------------------------

    private void saveButton_ActionPerformed(ActionEvent evt)
    {
        saveCounter_++;

        fileChooser_.setApproveButtonText("Save");
        fileChooser_.setSelectedFile
        (
            new File("faceList" + saveCounter_ + ".foam")
        );

        if
        (
            fileChooser_.showOpenDialog(this)
         == JFileChooser.APPROVE_OPTION
        )
        {
            System.out.println("Save to " + fileChooser_.getSelectedFile());

            try
            {
                PrintWriter out = new PrintWriter
                (
                    new OutputStreamWriter
                    (
                        new FileOutputStream(fileChooser_.getSelectedFile())
                    )
                );

                pp_.getMesh().getFaceSet().write(out);
            }
            catch(FileNotFoundException ex)
            {
                System.out.println("FileNotFoundException " + ex);
            }
        }
    }

    //--------------------------------------------------------------------------

    private void showButton_ActionPerformed(ActionEvent evt)
    {
        FaceSet fs = pp_.getMesh().getFaceSet();

        PrintWriter out = new PrintWriter
        (
            new OutputStreamWriter(System.out)
        );

        fs.show(pp_.getMesh(), out);
    }

    //--------------------------------------------------------------------------

    private void clearButton_ActionPerformed(ActionEvent evt)
    {
        if
        (
            JOptionPane.showConfirmDialog
            (
                this,
                "Clear set?",
                "Clear Set",
                JOptionPane.YES_NO_OPTION,
                JOptionPane.QUESTION_MESSAGE
            )
            ==
            JOptionPane.OK_OPTION
        )
        {
            FaceSet fs = pp_.getMesh().getFaceSet();

            fs.clear();
        }
    }

    //--------------------------------------------------------------------------

    private void addButton_ActionPerformed(ActionEvent evt)
    {
        FaceSet fs = pp_.getMesh().getFaceSet();

        fs.selectPatch(pp_.getMesh(), currentPatchText_.getText());
    }

    //--------------------------------------------------------------------------

    private void deleteButton_ActionPerformed(ActionEvent evt)
    {
        FaceSet fs = pp_.getMesh().getFaceSet();

        fs.deselectPatch(pp_.getMesh(), currentPatchText_.getText());
    }

    //--------------------------------------------------------------------------

    private void addLoopButton_ActionPerformed(ActionEvent evt)
    {
        // Use flood fill to select faces.
        SelectLoop fillFaceSet = new SelectLoop(pp_, true);
    }

    //--------------------------------------------------------------------------

    private void deleteLoopButton_ActionPerformed(ActionEvent evt)
    {
        // Use flood fill to deselect faces.
        SelectLoop fillFaceSet = new SelectLoop(pp_, false);
    }

    //--------------------------------------------------------------------------

    private void invertButton_ActionPerformed(ActionEvent evt)
    {
        FaceSet fs = pp_.getMesh().getFaceSet();

        fs.invert();
    }

    //--------------------------------------------------------------------------

    private void changeButton_ActionPerformed(ActionEvent evt)
    {
        String patchName = currentPatchText_.getText();

        BoundaryMesh mesh = pp_.getMesh();

        // Index of patch to change faces into
        int currentPatchI = mesh.getPatchIndex(patchName);

        System.out.println
        (
            "Changing selected faces to " + patchName + " index "
          + currentPatchI
        );

        // Fill patchID with patchID for every face
        int[] patchID = new int[mesh.getNFaces()];

        int nChanged = BoundaryMeshUtils.getPatchIDs
        (
            mesh,
            mesh.getFaceSet().getSelected(),
            currentPatchI,
            patchID
        );

        if (nChanged > 0)
        {
            System.out.println("Changing " + nChanged + " faces");
            
            // Remove (if any) display shapes (patchFaceNodes).
            boolean hasPatchDisplay = (pp_.getDisplayPatchFaces() != null);

            if (hasPatchDisplay)
            {
                pp_.removeFaceDisplay();
            }

            // Update mesh
            int[] oldToNew = new int[patchID.length];

            pp_.changeFaces(patchName, patchID, oldToNew);

            // Recreate display shapes (patchFaceNodes).
            if (hasPatchDisplay)
            {
                pp_.addFaceDisplay();
            }
        }
    }

    //--------------------------------------------------------------------------
    //---- ElementSelectListener Implementation
    //--------------------------------------------------------------------------

    public void elementSelected(ElementEvent evt)
    {
        FaceSet fs = pp_.getMesh().getFaceSet();

        // Update number of faces display
        sizeText_.setText(Integer.toString(fs.getNSelected()));
    }

    public void elementDeselected(ElementEvent evt)
    {
        FaceSet fs = pp_.getMesh().getFaceSet();

        // Update number of faces display
        sizeText_.setText(Integer.toString(fs.getNSelected()));
    }

    public void selectedSetChanged(ElementEvent evt)
    {
        FaceSet fs = pp_.getMesh().getFaceSet();

        // Update number of faces display
        sizeText_.setText(Integer.toString(fs.getNSelected()));
    }


    //--------------------------------------------------------------------------
    //---- UI components
    //--------------------------------------------------------------------------

    private JFileChooser fileChooser_;
    private JTextField sizeText_;
    private JTextField currentPatchText_;
    private JTextField changeText_;

    //--------------------------------------------------------------------------
}



