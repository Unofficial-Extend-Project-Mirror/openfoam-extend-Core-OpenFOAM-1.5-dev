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

\*---------------------------------------------------------------------------*/

package PatchTool.Util;

import com.sun.j3d.utils.behaviors.mouse.*;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.media.j3d.*;
import javax.vecmath.*;


 
public class myMouseZoom extends MouseBehavior {

    double z_factor = .04;
    Vector3d translation = new Vector3d();
  
    private MouseBehaviorCallback callback = null;

    /**
     * Creates a zoom behavior given the transform group.
     * @param transformGroup The transformGroup to operate on.
     */
    public myMouseZoom(TransformGroup transformGroup) {
	super(transformGroup);
    }

    /**
     * Creates a default mouse zoom behavior.
     **/
    public myMouseZoom(){
	super(0);
    }

    /**
     * Creates a zoom behavior.
     * Note that this behavior still needs a transform
     * group to work on (use setTransformGroup(tg)) and
     * the transform group must add this behavior.
     * @param flags
     */
    public myMouseZoom(int flags) {
	super(flags);
    }

    /**
     * Creates a zoom behavior that uses AWT listeners and behavior
     * posts rather than WakeupOnAWTEvent.  The behavior is added to the
     * specified Component.  A null component can be passed to specify
     * the behavior should use listeners.  Components can then be added
     * to the behavior with the addListener(Component c) method.
     * @param c The Component to add the MouseListener
     * and MouseMotionListener to.
     * @since Java 3D 1.2.1
     */
    public myMouseZoom(Component c) {
	super(c, 0);
    }

    /**
     * Creates a zoom behavior that uses AWT listeners and behavior
     * posts rather than WakeupOnAWTEvent.  The behaviors is added to
     * the specified Component and works on the given TransformGroup.
     * @param c The Component to add the MouseListener and
     * MouseMotionListener to.  A null component can be passed to specify
     * the behavior should use listeners.  Components can then be added
     * to the behavior with the addListener(Component c) method.
     * @param transformGroup The TransformGroup to operate on.
     * @since Java 3D 1.2.1
     */
    public myMouseZoom(Component c, TransformGroup transformGroup) {
	super(c, transformGroup);
    }

    /**
     * Creates a zoom behavior that uses AWT listeners and behavior
     * posts rather than WakeupOnAWTEvent.  The behavior is added to the
     * specified Component.  A null component can be passed to specify
     * the behavior should use listeners.  Components can then be added
     * to the behavior with the addListener(Component c) method.
     * Note that this behavior still needs a transform
     * group to work on (use setTransformGroup(tg)) and the transform
     * group must add this behavior.
     * @param flags interesting flags (wakeup conditions).
     * @since Java 3D 1.2.1
     */
    public myMouseZoom(Component c, int flags) {
	super(c, flags);
    }

    public void initialize() {
	super.initialize();
	if ((flags & INVERT_INPUT) == INVERT_INPUT) {
	    z_factor *= -1;
	    invert = true;
	}
    }
    
    /**
     * Return the y-axis movement multipler.
     **/
    public double getFactor() {
	return z_factor;
    }
  
    /**
     * Set the y-axis movement multipler with factor.
     **/
    public void setFactor( double factor) {
	z_factor = factor;
    }
  

    public void processStimulus (Enumeration criteria) {
	WakeupCriterion wakeup;
	AWTEvent[] events;
 	MouseEvent evt;
// 	int id;
// 	int dx, dy;

        System.out.println("processStimulus");

	while (criteria.hasMoreElements()) {
	    wakeup = (WakeupCriterion) criteria.nextElement();
	    if (wakeup instanceof WakeupOnAWTEvent) {
		events = ((WakeupOnAWTEvent)wakeup).getAWTEvent();
		if (events.length > 0) {
		    evt = (MouseEvent) events[events.length-1];
		    doProcess(evt);
		}
	    }

	    else if (wakeup instanceof WakeupOnBehaviorPost) {
		while (true) {
		    synchronized (mouseq) {
			if (mouseq.isEmpty()) break;
			evt = (MouseEvent)mouseq.remove(0);
			// consolodate MOUSE_DRAG events
			while((evt.getID() == MouseEvent.MOUSE_DRAGGED) &&
			      !mouseq.isEmpty() &&
			      (((MouseEvent)mouseq.get(0)).getID() ==
			       MouseEvent.MOUSE_DRAGGED)) {
			    evt = (MouseEvent)mouseq.remove(0);
			}
		    }
		    doProcess(evt);
		}
	    }
	    
	}
	wakeupOn (mouseCriterion);
    }

    void doProcess(MouseEvent evt) {
	int id;
	int dx, dy;

	processMouseEvent(evt);
	
        System.out.println("doProcess");

	if (((buttonPress)&&((flags & MANUAL_WAKEUP) == 0)) ||
	    ((wakeUp)&&((flags & MANUAL_WAKEUP) != 0))){
	    id = evt.getID();
	    if ((id == MouseEvent.MOUSE_DRAGGED) &&
		evt.isAltDown() && !evt.isMetaDown()){
		
		x = evt.getX();
		y = evt.getY();
		
		dx = x - x_last;
		dy = y - y_last;
		
		if (!reset){
		    transformGroup.getTransform(currXform);
		    
		    translation.z  = dy*z_factor;
		    
		    transformX.set(translation);
		    
		    if (invert) {
			currXform.mul(currXform, transformX);
		    } else {
			currXform.mul(transformX, currXform);
		    }
		    
		    transformGroup.setTransform(currXform);
		    
		    transformChanged( currXform );
		    
		    if (callback!=null)
			callback.transformChanged( MouseBehaviorCallback.ZOOM,
						   currXform );
		    
		}
		else {
		    reset = false;
		}
		
		x_last = x;
		y_last = y;
	    }
	    else if (id == MouseEvent.MOUSE_PRESSED) {
		x_last = evt.getX();
		y_last = evt.getY();
	    }
	}
    }

    
  /**
    * Users can overload this method  which is called every time
    * the Behavior updates the transform
    *
    * Default implementation does nothing
    */
  public void transformChanged( Transform3D transform ) {
  }
 
  /**
    * The transformChanged method in the callback class will
    * be called every time the transform is updated
    */
  public void setupCallback( MouseBehaviorCallback callback ) {
      this.callback = callback;
  }
}

