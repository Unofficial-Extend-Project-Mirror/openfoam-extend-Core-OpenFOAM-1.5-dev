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

import PatchTool.Mesh.SurfacePatch;
import PatchTool.Mesh.BoundaryMesh;
import PatchTool.Mesh.BoundaryMeshUtils;
import PatchTool.Mesh.FaceSet;


public class PatchPanel
    extends javax.swing.JPanel
    implements PatchStatusListener, CaseStatusListener
{
    //--------------------------------------------------------------------------

    protected PatchTool pp_;


    //--------------------------------------------------------------------------
    /** Creates new form PatchPanel */
    public PatchPanel(PatchTool pp)
    {
        super();

        pp_ = pp;

        initComponents();

        // Make sure we know about loaded geometry.
        pp_.addCaseStatusListener(this);

        // Make sure we know about added/deleted patches.
        pp_.addPatchStatusListener(this);

        newTypeCombo_.addItem("patch");
        newTypeCombo_.addItem("wall");
        newTypeCombo_.addItem("symmetryPlane");
        //newTypeCombo_.addItem("cyclic");
        newTypeCombo_.addItem("wedge");
        newTypeCombo_.addItem("empty");
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


        // Patch Selection
        JLabel currentLabel = new JLabel("Patch");
        PatchTool.setConstraint(0, 0, 1, 1, 1.0, 1.0, c);
        this.add(currentLabel, c);

        patchCombo_ = new JComboBox();
        patchCombo_.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    patchCombo_ActionPerformed(evt);
                }
            }
        );
        PatchTool.setConstraint(1, 0, 2, 1, 1.0, 1.0, c);
        this.add(patchCombo_, c);


        // Patch info
        JLabel typeLabel = new JLabel("Type");
        PatchTool.setConstraint(3, 0, 1, 1, 1.0, 1.0, c);
        this.add(typeLabel, c);

        typeText_ = new JTextField();
        typeText_.setEditable(false);
        PatchTool.setConstraint(4, 0, 1, 1, 1.0, 1.0, c);
        this.add(typeText_, c);

        JLabel sizeLabel = new JLabel("Size");
        PatchTool.setConstraint(3, 1, 1, 1, 1.0, 1.0, c);
        this.add(sizeLabel, c);

        sizeText_ = new JTextField();
        sizeText_.setEditable(false);
        PatchTool.setConstraint(4, 1, 1, 1, 1.0, 1.0, c);
        this.add(sizeText_, c);


        // Change patch
        JLabel newTypeLabel = new JLabel("Type");
        PatchTool.setConstraint(0, 2, 1, 1, 1.0, 1.0, c);
        this.add(newTypeLabel, c);

        newTypeCombo_ = new JComboBox();
        PatchTool.setConstraint(1, 2, 1, 1, 1.0, 1.0, c);
        this.add(newTypeCombo_, c);

        JButton changeButton = new JButton("Change");
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
            "Change type of current patch"
        );
        PatchTool.setConstraint(2, 2, 1, 1, 1.0, 1.0, c);
        this.add(changeButton, c);


        // Delete patch
        JButton deleteButton = new JButton("Remove");
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
            "Delete current patch"
        );
        PatchTool.setConstraint(1, 3, 1, 1, 1.0, 1.0, c);
        this.add(deleteButton, c);


        // Add patch
        JLabel nameLabel = new JLabel("Name");
        PatchTool.setConstraint(0, 4, 1, 1, 1.0, 1.0, c);
        this.add(nameLabel, c);

        nameText_ = new JTextField();
        nameText_.setToolTipText
        (
            "Name of new patch"
        );
        PatchTool.setConstraint(1, 4, 1, 1, 1.0, 1.0, c);
        this.add(nameText_, c);

        JButton addButton = new JButton("Add");
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
            "Add patch"
        );
        PatchTool.setConstraint(2, 4, 1, 1, 1.0, 1.0, c);
        this.add(addButton, c);


        //
        // Auto patch
        //
        JLabel autoPatchLabel = new JLabel("AutoPatch");
        PatchTool.setConstraint(0, 5, 1, 1, 1.0, 1.0, c);
        this.add(autoPatchLabel, c);

        JButton autoPatchButton = new JButton("Create");
        autoPatchButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    autoPatchButton_ActionPerformed(evt);
                }
            }
        );
        autoPatchButton.setToolTipText
        (
            "Create patches based on feature angle"
        );
        PatchTool.setConstraint(1, 5, 1, 1, 1.0, 1.0, c);
        this.add(autoPatchButton, c);

    }

    //--------------------------------------------------------------------------
    //---- PatchStatusListener Implementation
    //--------------------------------------------------------------------------

    public void patchSelected(PatchEvent evt)
    {
        // Set size and type in here. No need to use event mechanism.
        SurfacePatch sp = pp_.getMesh().getPatch(evt.getPatchName());

        typeText_.setText(sp.getGeometricType());

        sizeText_.setText(Integer.toString(sp.getSize()));
    }

    public void patchAdded(PatchEvent evt)
    {
        // Update list of patches to select from
        patchCombo_.addItem(evt.getPatchName());
    }

    public void patchDeleted(PatchEvent evt)
    {
        // Update list of patches to select from
        int currentPatchI = patchCombo_.getSelectedIndex();

        SurfacePatch[] patches = pp_.getMesh().getPatches();

        patchCombo_.removeAllItems();

        for (int i = 0; i < patches.length; i++)
        {
            patchCombo_.addItem(patches[i].getName());
        }

        if (currentPatchI == patches.length)
        {
            patchCombo_.setSelectedIndex(currentPatchI - 1);
        }
        else if (currentPatchI >= 0)
        {
            patchCombo_.setSelectedIndex(currentPatchI);
        }
    }

    public void patchTypeChanged(PatchEvent evt)
    {
        // Update displayed patch. Do as if selected.
        patchSelected(evt);
    }

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
        // We now have mesh. Insert patch names into patch selector

        SurfacePatch[] patches = pp_.getMesh().getPatches();

        patchCombo_.removeAllItems();

        for (int i = 0; i < patches.length; i++)
        {
            patchCombo_.addItem(patches[i].getName());
        }
    }

    //--------------------------------------------------------------------------

    private void patchCombo_ActionPerformed(ActionEvent evt)
    {
        if (patchCombo_.getSelectedItem() != null)
        {
            String patchName = (String)patchCombo_.getSelectedItem();

            pp_.setCurrentPatch(patchName);
        }
    }

    //--------------------------------------------------------------------------

    private void addButton_ActionPerformed(ActionEvent evt)
    {
        if (pp_.getMesh() == null)
        {
            JOptionPane.showMessageDialog
            (
                this,
                "No mesh loaded",
                "Add Patch",
                JOptionPane.ERROR_MESSAGE
            );
        }
        else if (pp_.getMesh().getPatchIndex(nameText_.getText()) != -1)
        {
            JOptionPane.showMessageDialog
            (
                this,
                "Already have patch " + nameText_.getText(),
                "Add Patch",
                JOptionPane.ERROR_MESSAGE
            );
        }
        else
        {
            // Add to underlying mesh
            pp_.addPatch(nameText_.getText());
            pp_.changePatchType(nameText_.getText(), "patch");
        }
    }

    //--------------------------------------------------------------------------

    private void changeButton_ActionPerformed(ActionEvent evt)
    {
        if
        (
            (patchCombo_.getSelectedItem() != null)
         && (newTypeCombo_.getSelectedItem() != null)
        )
        {
            String patchName = (String)patchCombo_.getSelectedItem();
            String patchType = (String)newTypeCombo_.getSelectedItem();

            System.out.println
            (
                "Change patch " + patchName + " to " + patchType
            );

            pp_.changePatchType(patchName, patchType);
        }
    }

    //--------------------------------------------------------------------------

    private void deleteButton_ActionPerformed(ActionEvent evt)
    {
        if (patchCombo_.getSelectedItem() != null)
        {
            String patchName = (String)patchCombo_.getSelectedItem();

            SurfacePatch sp = pp_.getMesh().getPatch(patchName);

            if (sp.getSize() != 0)
            {
                // Issue warning.
                JOptionPane.showMessageDialog
                (
                    this,
                    "Patch " + patchName + " not empty",
                    "Delete Patch",
                    JOptionPane.ERROR_MESSAGE
                );
            }
            else
            {
                if
                (
                    JOptionPane.showConfirmDialog
                    (
                        this,
                        "Delete patch " + patchName + " ?",
                        "Delete Patch",
                        JOptionPane.YES_NO_OPTION,
                        JOptionPane.QUESTION_MESSAGE
                    )
                    ==
                    JOptionPane.OK_OPTION
                )
                {
                    // No need to remove any displayed shapes since only
                    // empty patches can be removed and these are not in
                    // displaylist anyway.
                    pp_.deletePatch(patchName);
                }
            }
        }
    }

    //--------------------------------------------------------------------------

    private void autoPatchButton_ActionPerformed(ActionEvent evt)
    {
        BoundaryMesh mesh = pp_.getMesh();

        if (mesh.getFaceSet().getNSelected() == 0)
        {
            // No selected faces.
            JOptionPane.showMessageDialog
            (
                this,
                "Empty face set. Please select faces to autoPatch",
                "AutoPatch",
                JOptionPane.ERROR_MESSAGE
            );

            return;
        }


        if
        (
            JOptionPane.showConfirmDialog
            (
                this,
                "Create patches from feature lines?\n"
              + "(works on current face set)",
                "AutoPatch",
                JOptionPane.YES_NO_OPTION,
                JOptionPane.QUESTION_MESSAGE
            )
            !=
            JOptionPane.OK_OPTION
        )
        {
            return;
        }

        try
        {
            //
            // Remove (if any) display shapes
            //
            boolean hasPatchDisplay = (pp_.getDisplayPatchFaces() != null);

            if (hasPatchDisplay)
            {
                pp_.removeFaceDisplay();
            }

            boolean hasEdgeDisplay = (pp_.getDisplayEdges() != null);

            if (hasEdgeDisplay)
            {
                pp_.removeEdgeDisplay();
            }

            boolean hasExtraEdgeDisplay = (pp_.getDisplayExtraEdges() != null);

            if (hasExtraEdgeDisplay)
            {
                pp_.removeExtraEdgeDisplay();
            }

            boolean hasFeaturesDisplay = (pp_.getDisplayFeatures() != null);

            if (hasFeaturesDisplay)
            {
                pp_.removeFeatureEdgeDisplay();
            }


            //
            // Set feature edges.
            //

            //float degAngle = Float.parseFloat(createFeatText_.getText());
            PreProcPanel pnl = pp_.getPreDialog().getPreProcPanel();
            float degAngle = pnl.getDisplayPanel().getFeatureAngle();

            double radAngle = degAngle * Math.PI / 180.0;

            // Select all feature edges (in edgeSets)
            System.out.println("Creating featureLines from angle "
                + radAngle);

            mesh.setFeatureEdges(Math.cos(radAngle));

            BoundaryMeshUtils.selectAllFeatureEdges(mesh);


            FaceSet fs = mesh.getFaceSet();

            // Remember existing face set.
            // (Note no need to copy; only use setSelected with new array)
            boolean[] selected = fs.getSelected();


            // Newly created patch number (still doesn't guarantee that new 
            // patch name doesn't exist but is good enough for me ;-)
            int newPatchI = mesh.getPatches().length;

            // New patchIDs
            int[] patchIDs = new int[mesh.getNFaces()];

            // Fill PatchID with current patch ids except for faces in current
            // faceset which get -1.
            SurfacePatch[] patches = pp_.getMesh().getPatches();

            for (int patchI = 0; patchI < patches.length; patchI++)
            {
                SurfacePatch sp = patches[patchI];

                for 
                (
                    int faceI = sp.getStart();
                    faceI < sp.getStart() + sp.getSize();
                    faceI++
                )
                {
                    patchIDs[faceI] = patchI;
                }
            }

            // Currently selected faces get PatchID = -1
            for (int faceI = 0; faceI < selected.length; faceI++)
            {
                if (selected[faceI])
                {
                     patchIDs[faceI] = -1;
                }
            }


            do
            {
                // Find not yet visited face in current FaceSet
                int unsetFaceI = -1;

                for (int faceI = 0; faceI < selected.length; faceI++)
                {
                    if (selected[faceI] && (patchIDs[faceI] == -1))
                    {
                        unsetFaceI = faceI;
                        break;
                    }
                }

                if (unsetFaceI == -1)
                {
                    // Can't find face which hasn't been assigned a patchID.
                    break;
                }


                // Get all faces visited from unsetFaceI
                boolean[] localVisited = new boolean[patchIDs.length];

                mesh.markLoopFaces(unsetFaceI, localVisited);

                // All visited faces which are in the faceSet
                // get the new patchID
                for (int faceI = 0; faceI < patchIDs.length; faceI++)
                {
                    // Limit localVisited to current face set.
                    localVisited[faceI] &= selected[faceI];

                    if (localVisited[faceI])
                    {
                        // Update patchID so we know how much of faceSet has
                        // been repatched.
                        patchIDs[faceI] = newPatchI;
                    }
                }

                // Update selection.
                //?Is this nessecary
                mesh.getFaceSet().setSelected(localVisited);

                // Create a patch and set as current.
                String patchName = "dummyPatch" + newPatchI;

                pp_.addPatch(patchName);

                pp_.setCurrentPatch(patchName);

                newPatchI++;
            }
            while (true);


//for (int faceI = 0; faceI < selected.length; faceI++)
//{
//    System.out.println
//    (
//        "face:" + faceI
//      + " oldPatch:" + mesh.whichPatch(faceI)
//      + " newPatch:" + patchIDs[faceI]
//    );
//}


            //
            // Actually update mesh
            //
            int[] oldToNew = new int[patchIDs.length];

            // Dummy patch name.
            pp_.changeFaces("somePatch", patchIDs, oldToNew);

            // Update derived information.
            mesh.setFeatureEdges(Math.cos(radAngle));


            // Recreate display shapes
            // Note: extraEdges now invalidated.
            if (hasPatchDisplay)
            {
                pp_.addFaceDisplay();
            }
            if (hasEdgeDisplay)
            {
                pp_.addEdgeDisplay();
            }
            if (hasFeaturesDisplay)
            {
                pp_.addFeatureEdgeDisplay();
            }
        }
        catch(NumberFormatException ex)
        {
            System.out.println("numberFormat:" + ex);
        }
    }

    //--------------------------------------------------------------------------
    //---- UI components
    //--------------------------------------------------------------------------

    private JComboBox patchCombo_;
    private JTextField typeText_;
    private JTextField sizeText_;
    private JTextField nameText_;
    private JComboBox newTypeCombo_;

    //--------------------------------------------------------------------------
}



