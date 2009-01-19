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

package PatchTool.Util;

import java.awt.GraphicsConfiguration;
import javax.media.j3d.*;
import javax.vecmath.*;
import java.awt.event.*;
import javax.swing.event.*;

import PatchTool.Events.RenderListener;

public class MyCanvas3D extends Canvas3D
{
    protected EventListenerList listenerList_;

    int count_;

    //--------------------------------------------------------------------------

    public MyCanvas3D(GraphicsConfiguration graphicsConfiguration)
    {
	super(graphicsConfiguration);

        listenerList_ = new EventListenerList();

        count_ = 0;
    }

    //--------------------------------------------------------------------------

    public void postSwap()
    {
	super.postSwap();

        firePostSwap();
    }

    //--------------------------------------------------------------------------

    public int getCount()
    {
	return count_;
    }

    //--------------------------------------------------------------------------
    //---- ElementSelectListener Methods
    //--------------------------------------------------------------------------

    public void addRenderListener(RenderListener l)
    {
        listenerList_.add(RenderListener.class, l);
    }

    //--------------------------------------------------------------------------

    public void removeRenderListener(RenderListener l)
    {
        listenerList_.remove(RenderListener.class, l);
    }

    //--------------------------------------------------------------------------

    protected void firePostSwap()
    {
        // Create event object.
        java.util.EventObject evt = new java.util.EventObject(this);

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == RenderListener.class)
            {
                ((RenderListener)listeners[i+1]).postSwap(evt);
            }
        }
    }
}
