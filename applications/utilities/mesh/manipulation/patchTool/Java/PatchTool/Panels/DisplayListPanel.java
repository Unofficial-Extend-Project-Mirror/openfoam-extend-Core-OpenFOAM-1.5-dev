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


import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.WindowConstants;
import javax.swing.table.AbstractTableModel;

import javax.swing.event.TableModelListener; 
import javax.swing.event.TableModelEvent; 


import PatchTool.PatchTool;
import PatchTool.InteractiveNodes.InteractiveNodeList;
import PatchTool.InteractiveNodes.InteractiveNode;


public class DisplayListPanel
    extends javax.swing.JPanel
    implements TableModelListener
{
    private boolean DEBUG = true;

    protected PatchTool pp_;

    //--------------------------------------------------------------------------
    

    public DisplayListPanel(PatchTool pp)
    {
        super();

        pp_ = pp;

        table_ = new JTable(pp_.getShapes());

        initComponents();

        pp_.getShapes().addTableModelListener(this);
    }

    //--------------------------------------------------------------------------

    private void initComponents()
    {
        table_.setPreferredScrollableViewportSize(new Dimension(200, 150));

        JPanel mainPanel = new JPanel();
        this.setLayout(new GridBagLayout());

        GridBagConstraints c = new GridBagConstraints();

        // Add insets around all to avoid clutter
        c.insets = new Insets(2, 2, 2, 2);

        // Anchor all same way
        c.anchor = GridBagConstraints.WEST;

        // Fill horizontal
        c.fill = GridBagConstraints.HORIZONTAL;


        // Panel with buttons
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new GridBagLayout());

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
            "Show selected entries"
        );
        PatchTool.setConstraint(0, 0, 1, 1, 1.0, 1.0, c);
        buttonPanel.add(showButton, c);

        JButton hideButton = new JButton("Hide");
        hideButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    hideButton_ActionPerformed(evt);
                }
            }
        );
        hideButton.setToolTipText
        (
            "Hide selected entries"
        );
        PatchTool.setConstraint(1, 0, 1, 1, 1.0, 1.0, c);
        buttonPanel.add(hideButton, c);

        JButton editButton = new JButton("Edit");
        editButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    editButton_ActionPerformed(evt);
                }
            }
        );
        editButton.setToolTipText
        (
            "Edit properties of selected entries"
        );
        PatchTool.setConstraint(2, 0, 1, 1, 1.0, 1.0, c);
        buttonPanel.add(editButton, c);

        JButton removeButton = new JButton("Remove");
        removeButton.addActionListener
        (
            new ActionListener()
            {
                public void actionPerformed(ActionEvent evt)
                {
                    removeButton_ActionPerformed(evt);
                }
            }
        );
        removeButton.setToolTipText
        (
            "Remove selected entries"
        );
        PatchTool.setConstraint(3, 0, 1, 1, 1.0, 1.0, c);
        buttonPanel.add(removeButton, c);

        // Create the scroll pane and add the table to it.
        JScrollPane scrollPane = new JScrollPane(table_);
        scrollPane.setPreferredSize(new java.awt.Dimension(200, 200));
        scrollPane.setMinimumSize(new java.awt.Dimension(200, 200));
        PatchTool.setConstraint(0, 0, 1, 1, 1.0, 1.0, c);
        this.add(scrollPane, c);

        PatchTool.setConstraint(0, 1, 1, 1, 1.0, 1.0, c);
        this.add(buttonPanel, c);
    }

    //--------------------------------------------------------------------------

    /** show button handler */
    private void showButton_ActionPerformed(ActionEvent evt)
    {
        InteractiveNodeList model = (InteractiveNodeList)table_.getModel();
        int startRow = table_.getSelectionModel().getMinSelectionIndex();
        int endRow = table_.getSelectionModel().getMaxSelectionIndex();
        if (startRow < 0)
        {
            return;
        }
        for (int row = startRow; row <= endRow; row++)
        {
            table_.setValueAt(Boolean.TRUE, row, 1);
        }
        //table_.getSelectionModel().clearSelection();
        table_.repaint();
    }

    //--------------------------------------------------------------------------

    /** hide button handler */
    private void hideButton_ActionPerformed(ActionEvent evt)
    {
        InteractiveNodeList model = (InteractiveNodeList)table_.getModel();
        int startRow = table_.getSelectionModel().getMinSelectionIndex();
        int endRow = table_.getSelectionModel().getMaxSelectionIndex();
        if (startRow < 0)
        {
            return;
        }
        for (int row = startRow; row <= endRow; row++)
        {
            table_.setValueAt(Boolean.FALSE, row, 1);
        }
        //table_.getSelectionModel().clearSelection();
        table_.repaint();
    }

    //--------------------------------------------------------------------------

    /** edit button handler */
    private void editButton_ActionPerformed(ActionEvent evt)
    {
        InteractiveNodeList model = (InteractiveNodeList)table_.getModel();
        int startRow = table_.getSelectionModel().getMinSelectionIndex();
        int endRow = table_.getSelectionModel().getMaxSelectionIndex();
        if (startRow < 0)
        {
            return;
        }
        for (int row = startRow; row <= endRow; row++)
        {
            InteractiveNode shape = model.getShapeAt(row);

            if (shape != null)
            {
                shape.edit();
            }
        }
        table_.getSelectionModel().clearSelection();
        table_.repaint();
    }

    //--------------------------------------------------------------------------

    /** remove button handler */
    private void removeButton_ActionPerformed(ActionEvent evt)
    {
        InteractiveNodeList model = (InteractiveNodeList)table_.getModel();
        int startRow = table_.getSelectionModel().getMinSelectionIndex();
        int endRow = table_.getSelectionModel().getMaxSelectionIndex();
        if (startRow < 0)
        {
            return;
        }
        for (int row = startRow; row <= endRow; row++)
        {
            //model.removeElementAt(row);
            model.removeElementAt(startRow);
        }

        table_.getSelectionModel().clearSelection();
        initTableSize(table_, model);
        table_.repaint();
    }

    //--------------------------------------------------------------------------

    /** Size table itself */
    static private void initTableSize(JTable table, InteractiveNodeList model)
    {
        int height = table.getRowHeight() * model.getRowCount();

        table.setPreferredSize
        (
            new java.awt.Dimension(table.getWidth(), height)
        );
        table.revalidate();
    } 


    //--------------------------------------------------------------------------
    //---- TableModelListener Implementation
    //--------------------------------------------------------------------------

    public void tableChanged(TableModelEvent e) {
        //int row = e.getFirstRow();
        //int column = e.getColumn();

        initTableSize(table_, pp_.getShapes());
        table_.repaint();
    }

    //--------------------------------------------------------------------------
    //---- UI components
    //--------------------------------------------------------------------------

    private JTable table_;

    //--------------------------------------------------------------------------
}
