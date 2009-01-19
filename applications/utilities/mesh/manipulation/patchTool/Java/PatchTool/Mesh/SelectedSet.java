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

package PatchTool.Mesh;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.awt.event.ActionEvent;
import javax.swing.event.*;

import javax.media.j3d.*;

import PatchTool.Util.FoamDataParser;
import PatchTool.Events.ElementEvent;
import PatchTool.Events.ElementSelectListener;


public class SelectedSet
{
    protected EventListenerList listenerList_;

    boolean[] selected_;
    int nSelected_;

    //--------------------------------------------------------------------------

    /** Construct from components */
    public SelectedSet(int len)
    {
        // Create listener list.
        listenerList_ = new EventListenerList();

        selected_ = new boolean[len];

        clear();
    }

    //--------------------------------------------------------------------------

    /** Construct from stream */
    public SelectedSet(Reader readStream)
    {
        // Create listener list.
        listenerList_ = new EventListenerList();

        read(readStream);
    }

    //--------------------------------------------------------------------------

    public int getNSelected()
    {
        return nSelected_;
    }

    //--------------------------------------------------------------------------

    /** Utility function to determine count of selected items */
    static public int getNSelected(boolean[] selected)
    {
        int n = 0;

        for (int i = 0; i < selected.length; i++)
        {
            if (selected[i])
            {
                n++;
            }
        }
        return n;
    }

    //--------------------------------------------------------------------------

    public boolean[] getSelected()
    {
        return selected_;
    }

    //--------------------------------------------------------------------------

    public void setSelected(boolean[] selected)
    {
        selected_ = selected;

        nSelected_ = 0;

        for (int elemI = 0; elemI < selected_.length; elemI++)
        {
            if (selected_[elemI])
            {
                nSelected_++;
            }
        }
        fireSetChanged(selected_);
    }

    //--------------------------------------------------------------------------

    public void read(Reader readStream)
    {
        try
        {
            FoamDataParser foamParser = new FoamDataParser(readStream);

            int[] elementLabels = foamParser.readIntList("SelectedSet");

            // Clear old status (Bug: generates event)
            clear();

            // Select listed edges
            for(int i = 0; i < elementLabels.length; i++)
            {
                silentSelect(elementLabels[i]);
            }
            fireSetChanged(selected_);
        }
        catch(NumberFormatException ex)
        {
            System.out.println("NumberFormatException " + ex);
        }
    }

    //--------------------------------------------------------------------------

    public void write(PrintWriter wr)
    {
        wr.print("// SelectedSet file.");
        wr.println(" Contains labels of currently selected elements\n");
        wr.print(nSelected_ + "(\n");

        for (int elemI = 0; elemI < selected_.length; elemI++)
        {
            if (selected_[elemI])
            {
                wr.println(elemI);
            }
        }
        wr.print(")\n");
        wr.flush();
    }

    //--------------------------------------------------------------------------

    public void clear()
    {
        for (int elemI = 0; elemI < selected_.length; elemI++)
        {
            selected_[elemI] = false;
        }
        nSelected_ = 0;

        fireSetChanged(selected_);
    }

    //--------------------------------------------------------------------------

    /** Invert selection */
    public void invert()
    {
        for (int elemI = 0; elemI < selected_.length; elemI++)
        {
            if (selected_[elemI])
            {
                selected_[elemI] = false;

                nSelected_--;
            }
            else
            {
                selected_[elemI] = true;

                nSelected_++;
            }
        }
        fireSetChanged(selected_);
    }

    //--------------------------------------------------------------------------

    public void reorder(int[] oldToNew)
    {
        boolean[] oldSelected = selected_;

        selected_ = new boolean[oldSelected.length];

        for (int faceI = 0; faceI < selected_.length; faceI++)
        {
            selected_[oldToNew[faceI]] = oldSelected[faceI];
        }
        fireSetChanged(selected_);
    }

    //--------------------------------------------------------------------------

    public void select(int elemI)
    {
        if (!selected_[elemI])
        {
            selected_[elemI] = true;

            nSelected_++;

            fireSelect(elemI);
        }
    }

    //--------------------------------------------------------------------------

    public void deselect(int elemI)
    {
        if (selected_[elemI])
        {
            selected_[elemI] = false;

            nSelected_--;

            fireDeselect(elemI);
        }
    }

    //--------------------------------------------------------------------------

    public void silentSelect(int elemI)
    {
        if (!selected_[elemI])
        {
            selected_[elemI] = true;

            nSelected_++;
        }
    }

    //--------------------------------------------------------------------------

    public void silentDeselect(int elemI)
    {
        if (selected_[elemI])
        {
            selected_[elemI] = false;

            nSelected_--;
        }
    }


    //--------------------------------------------------------------------------
    //---- ElementSelectListener Methods
    //--------------------------------------------------------------------------

    public void addElementSelectListener(ElementSelectListener l)
    {
        listenerList_.add(ElementSelectListener.class, l);
    }

    //--------------------------------------------------------------------------

    public void removeElementSelectListener(ElementSelectListener l)
    {
        listenerList_.remove(ElementSelectListener.class, l);
    }

    //--------------------------------------------------------------------------

    protected void fireSelect(int elemI)
    {
        // Create event object.
        ElementEvent evt = new ElementEvent(this, elemI, null);

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == ElementSelectListener.class)
            {
                ((ElementSelectListener)listeners[i+1]).elementSelected(evt);
            }
        }
    }

    //--------------------------------------------------------------------------

    protected void fireDeselect(int elemI)
    {
        // Create event object.
        ElementEvent evt = new ElementEvent(this, elemI, null);

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == ElementSelectListener.class)
            {
                ((ElementSelectListener)listeners[i+1]).elementDeselected(evt);
            }
        }
    }

    //--------------------------------------------------------------------------

    protected void fireSetChanged(boolean[] selected)
    {
        // Create event object.
        ElementEvent evt = new ElementEvent(this, -1, selected);

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == ElementSelectListener.class)
            {
                ((ElementSelectListener)listeners[i+1]).selectedSetChanged(evt);
            }
        }
    }
}
