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

public class CaseModel extends javax.swing.JPanel
{
    //--------------------------------------------------------------------------

    protected PatchTool pp_;

    protected EventListenerList listenerList_;

    protected BoundaryMesh mesh_;

    //--------------------------------------------------------------------------
    /** Creates new form CaseModel */
    public CaseModel(PatchTool pp)
    {
        super();

        // Create listener list.
        listenerList_ = new EventListenerList();

        pp_ = pp;

        initComponents();
    }

    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------
    //---- TimeChangeListener Methods
    //--------------------------------------------------------------------------

    public void addTimeChangeListener(TimeChangeListener l)
    {
        listenerList_.add(TimeChangeListener.class, l);
    }

    //--------------------------------------------------------------------------

    public void removeTimeChangeListener(TimeChangeListener l)
    {
        listenerList_.remove(TimeChangeListener.class, l);
    }

    //--------------------------------------------------------------------------

    protected void fireTimeChanged(int timeIndex, double timeValue)
    {
        System.out.println("CaseModel:fireTimeChanged for " + timeValue);

        // Create event object.
        TimeChangeEvent evt = new TimeChangeEvent(this, timeIndex, timeValue);

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == TimeChangeListener.class)
            {
                ((TimeChangeListener)listeners[i+1]).timeChanged(evt);
            }
        }
    }


    //--------------------------------------------------------------------------
    //---- TimeChangeListener Methods
    //--------------------------------------------------------------------------

    public void addTimeChangeListener(TimeChangeListener l)
    {
        listenerList_.add(TimeChangeListener.class, l);
    }

    //--------------------------------------------------------------------------

    public void removeTimeChangeListener(TimeChangeListener l)
    {
        listenerList_.remove(TimeChangeListener.class, l);
    }

    //--------------------------------------------------------------------------

    protected void fireTimeChanged(int timeIndex, double timeValue)
    {
        System.out.println("CasePanel:fireTimeChanged for " + timeValue);

        // Create event object.
        TimeChangeEvent evt = new TimeChangeEvent(this, timeIndex, timeValue);

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == TimeChangeListener.class)
            {
                ((TimeChangeListener)listeners[i+1]).timeChanged(evt);
            }
        }
    }

    //--------------------------------------------------------------------------
    //---- CaseStatusListener Methods
    //--------------------------------------------------------------------------

    public void addCaseStatusListener(CaseStatusListener l)
    {
        listenerList_.add(CaseStatusListener.class, l);
    }

    //--------------------------------------------------------------------------

    public void removeCaseStatusListener(CaseStatusListener l)
    {
        listenerList_.remove(CaseStatusListener.class, l);
    }

    //--------------------------------------------------------------------------

    protected void fireRootSelected()
    {
        // Create event object.
        CaseEvent evt = new CaseEvent
        (
            this,
            rootText_.getText(),
            caseText_.getText(),
            timeText_.getText(),
            fileChooser_.getSelectedFile()
        );

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == CaseStatusListener.class)
            {
                ((CaseStatusListener)listeners[i+1]).rootSelected(evt);
            }
        }
    }

    //--------------------------------------------------------------------------

    protected void fireCaseSelected()
    {
        // Create event object.
        CaseEvent evt = new CaseEvent
        (
            this,
            rootText_.getText(),
            caseText_.getText(),
            timeText_.getText(),
            fileChooser_.getSelectedFile()
        );

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == CaseStatusListener.class)
            {
                ((CaseStatusListener)listeners[i+1]).caseSelected(evt);
            }
        }
    }

    //--------------------------------------------------------------------------

    protected void fireCaseLoaded()
    {
        // Create event object.
        CaseEvent evt = new CaseEvent
        (
            this,
            rootText_.getText(),
            caseText_.getText(),
            timeText_.getText(),
            fileChooser_.getSelectedFile()
        );

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == CaseStatusListener.class)
            {
                ((CaseStatusListener)listeners[i+1]).caseLoaded(evt);
            }
        }
    }

    //--------------------------------------------------------------------------

    protected void fireGeomLoaded()
    {
        // Create event object.
        CaseEvent evt = new CaseEvent
        (
            this,
            rootText_.getText(),
            caseText_.getText(),
            timeText_.getText(),
            fileChooser_.getSelectedFile()
        );

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == CaseStatusListener.class)
            {
                ((CaseStatusListener)listeners[i+1]).geomLoaded(evt);
            }
        }
    }

    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------
    //---- UI components
    //--------------------------------------------------------------------------

    javax.swing.JLabel rootLabel_;
    javax.swing.JTextField rootText_;
    javax.swing.JLabel caseLabel_;
    javax.swing.JTextField caseText_;
    javax.swing.JLabel timeLabel_;
    javax.swing.JTextField timeText_;
    JFileChooser fileChooser_;
    javax.swing.JButton connectButton_;
    javax.swing.JButton loadMeshButton_;
    javax.swing.JButton loadFileButton_;
    javax.swing.JButton saveMeshButton_;
    javax.swing.JButton saveFileButton_;

    //--------------------------------------------------------------------------
}



