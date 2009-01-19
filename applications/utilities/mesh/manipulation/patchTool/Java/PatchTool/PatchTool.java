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

package PatchTool;

import java.util.*;
import java.io.*;
import java.awt.Color;
import java.applet.Applet;
import java.awt.BorderLayout;
import java.awt.event.*;
import java.awt.Point;
import java.awt.GraphicsConfiguration;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.WindowConstants;
//import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;

import com.sun.j3d.utils.applet.MainFrame;
import com.sun.j3d.utils.behaviors.vp.*;
import com.sun.j3d.utils.behaviors.keyboard.*;
import com.sun.j3d.utils.picking.PickTool;
import com.sun.j3d.utils.universe.*;
import com.sun.j3d.utils.behaviors.mouse.*;
import com.sun.j3d.utils.picking.PickResult;

import javax.media.j3d.*;
import javax.vecmath.*;

import org.omg.CosNaming.*;

import FoamXServer.PatchToolServer.IPatchToolServer;

import PatchTool.InteractiveNodes.*;
import PatchTool.Shapes.PickableShape;
import PatchTool.Shapes.PatchFaces;
import PatchTool.Shapes.PickLines;
import PatchTool.Mesh.PrimitivePatch;
import PatchTool.Mesh.BoundaryMesh;
import PatchTool.Mesh.SurfacePatch;
import PatchTool.Util.KeyBehavior;
import PatchTool.Util.Colorizer;
import PatchTool.Util.PickDragBehavior;
import PatchTool.Util.MyCanvas3D;
import PatchTool.Util.OptionsManager;
import PatchTool.Util.CorbaUtils;
import PatchTool.Events.TimeChangeListener;
import PatchTool.Events.CaseStatusListener;
import PatchTool.Events.PatchStatusListener;
import PatchTool.Events.PickListener;
import PatchTool.Events.TimeChangeEvent;
import PatchTool.Events.CaseEvent;
import PatchTool.Events.PatchEvent;
import PatchTool.Events.MeshPickEvent;

import PatchTool.Panels.PreProcDlg;

public class PatchTool extends Applet
{
    private static SimpleUniverse u_ = null;

    public static PatchTool pp_;

    public static Color3f black = new Color3f(Color.black);
    public static Color3f white = new Color3f(Color.white);

    // Selection modes for picking
    public static int SELECT_SINGLE = 1;
    public static int SELECT_ALL = 2;
    public static int FOLLOW = 4;

    EventListenerList listenerList_;

    String[] args_;

    OptionsManager options_;

    BoundaryMesh boundaryMesh_;

    IPatchToolServer server_;

    InteractiveNodeList shapes_;
    BranchGroup scene_;

    MyCanvas3D myCanvas_;

    javax.swing.Timer visDelayTimer_;

    PickDragBehavior mouseBehavior_;

    int pickMode_;

    PreProcDlg preDlg_;
    
    Colorizer colorizer_;

    Color bgColor_;
    Color3f bgColor3f_;

    Color fgColor_;
    Color3f fgColor3f_;

    // Appearance used for all faces
    Appearance polyAppearance_;

    // Currently visible nodes relating to mesh
    PatchFacesNode[] displayPatchFaces_;
    PickLinesNode displayEdges_;
    PickLinesNode[] displayFeatures_;
    PickLinesNode displayExtraEdges_;

    // Top level transformGroup.
    TransformGroup transformGroup_;
    
    public BranchGroup createSceneGraph(SimpleUniverse u)
    {

	// Create the root of the branch graph
	BranchGroup objRoot = new BranchGroup();

        BoundingSphere bounds =
            new BoundingSphere(new Point3d(0.0,0.0,0.0), 1000000.0);

        transformGroup_ = new TransformGroup();
        transformGroup_.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
        transformGroup_.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
        transformGroup_.setCapability(TransformGroup.ALLOW_CHILDREN_EXTEND);
        transformGroup_.setCapability(TransformGroup.ALLOW_CHILDREN_WRITE);
        transformGroup_.setCapability(TransformGroup.ALLOW_CHILDREN_READ);
        objRoot.addChild(transformGroup_);

        // Create the mouse pick and drag behavior node
        mouseBehavior_ = new PickDragBehavior
        (
            u.getCanvas(),
            objRoot,
            transformGroup_
        );
        mouseBehavior_.setSchedulingBounds(bounds);
        objRoot.addChild(mouseBehavior_);

        // Add keyboard navigation to the ViewingPlatform
        ViewingPlatform viewingPlatform = u.getViewingPlatform();

        TransformGroup keyTG = viewingPlatform.getViewPlatformTransform();

        KeyBehavior key = new KeyBehavior(this, transformGroup_);
        key.setSchedulingBounds(bounds);
        key.setEnable(true);
        objRoot.addChild(key);

        // Set up the background
        Background bgNode = new Background(bgColor3f_);
        bgNode.setApplicationBounds(bounds);
        objRoot.addChild(bgNode);

        // Axis
        AxisNode axis = new AxisNode("axis", new Color3f(0.6f, 0.6f, 0.0f));
        transformGroup_.addChild(axis.getRoot());
        shapes_.addElement(axis);
        shapes_.show("axis");


        //// Read files
        //SampleSurfaceByRefNode ss0 = new SampleSurfaceByRefNode
        //(
        //    0,
        //    Math.PI / 4.5,
        //    args_[0],       // points file
        //    args_[1],       // triangles file
        //    args_[2],       // values file
        //    0.0f,           // Dummy
        //    1.0f            // Dummy
        //);
        //SampleSurfaceByRef ss = ss0.getSampleSurfaceByRef();
        //ss.setAppearance(createMaterialAppearance());
        //
        //objRoot.addChild(ss0.getRoot());
        //shapes_.addElement(ss0);
        //shapes_.show("sampleSurface_0");



        // Set up the ambient light
        AmbientLightNode ambientLightNode = new AmbientLightNode
        (
            "ambient_0",
            new Color3f(1.0f, 1.0f, 1.0f),
            bounds
        );
        objRoot.addChild(ambientLightNode.getRoot());
        ambientLightNode.show();
        shapes_.addElement(ambientLightNode);

        // Set up the directional lights
        DirectionalLightNode light0 = new DirectionalLightNode
        (
            "dirLight_0",
            new Color3f(1.0f, 1.0f, 1.0f),
            new Vector3f(1.0f, 1.0f, 1.0f),
            bounds
        );
        objRoot.addChild(light0.getRoot());
        light0.show();
        shapes_.addElement(light0);
        
        DirectionalLightNode light1 = new DirectionalLightNode
        (
            "dirLight_1",
            new Color3f(1.0f, 1.0f, 1.0f),
            new Vector3f(-1.0f, -1.0f, -1.0f),
            bounds
        );
        objRoot.addChild(light1.getRoot());
        light1.show();
        shapes_.addElement(light1);

        objRoot.setCapability(BranchGroup.ALLOW_BOUNDS_READ);

        // Have Java 3D perform optimizations on this scene graph.
        objRoot.compile();

	return objRoot;
    }


    public PatchTool(String[] args)
    {
        args_ = args;
    }


    public void init()
    {
        //setDefaultCloseOperation
        //(
        //    javax.swing.WindowConstants.DO_NOTHING_ON_CLOSE
        //);

        // Create listener list.
        listenerList_ = new EventListenerList();

        // Initialise the option manager object.
        options_ = new OptionsManager();

        // Display list
        shapes_ = new InteractiveNodeList();

        // Current primitivePatches
        displayPatchFaces_ = null;

        // Current edges
        displayEdges_ = null;

        // Current feature edge sets
        displayFeatures_ = null;

        // Current extra edges to display
        displayExtraEdges_ = null;

        // Current mesh
        boundaryMesh_ = null;

        pickMode_ = SELECT_SINGLE;

        // Preprocessing dialog
        preDlg_ = new PreProcDlg(null, this);
        preDlg_.setLocation(new Point(0,430));
        preDlg_.setVisible(true);

        // Value to colour
        colorizer_ = new Colorizer(this);

        // Colours in general
        fgColor_ = Color.white;
        fgColor3f_ = new Color3f(fgColor_);
        bgColor_ = Color.black;
        bgColor3f_ = new Color3f(bgColor_);

        // Default polygon appearance
        polyAppearance_ = createMaterialAppearance(true);

        setLayout(new BorderLayout());
        GraphicsConfiguration config =
        SimpleUniverse.getPreferredConfiguration();

        myCanvas_ = new MyCanvas3D(config);
        add("Center", myCanvas_);

        u_ = new SimpleUniverse(myCanvas_);

        // Create a simple scene and attach it to the virtual universe
        scene_ = createSceneGraph(u_);

        // Viewing behaviour
        ViewingPlatform viewingPlatform = u_.getViewingPlatform();
        View view = u_.getViewer().getView();
        //view.setProjectionPolicy(View.PARALLEL_PROJECTION);
        view.setProjectionPolicy(View.PERSPECTIVE_PROJECTION);

        // This will move the ViewPlatform back a bit so the
        // objects in the scene can be viewed.
        viewingPlatform.setNominalViewingTransform();

//        Transform3D allTransform =
//            viewAll(view, (BoundingSphere)scene_.getBounds());
//        transformGroup_.setTransform(allTransform);

    	u_.addBranchGraph(scene_);
    }

    //--------------------------------------------------------------------------

    public void destroy() 
    {
    	u_.removeAllLocales();
    }

    //--------------------------------------------------------------------------

    public String[] getArgs()
    {
        return args_;
    }

    //--------------------------------------------------------------------------

    public OptionsManager getOptions()
    {
        return options_;
    }

    //--------------------------------------------------------------------------

    public IPatchToolServer getServer()
    {
        return server_;
    }

    //--------------------------------------------------------------------------

    public void connect()
    {
        server_ = CorbaUtils.getPatchToolServer(getArgs(), getOptions());

        if (server_ != null)
        {
            try
            {
                // We now have root and case.
                fireRootSelected(server_.caseRoot());
                fireCaseSelected(server_.caseRoot(), server_.caseName());
                fireTimeSelected(server_.getTime());
            }
            catch (FoamXServer.FoamXIOError ioErr)
            {
                System.out.println("connect() : Caught exception");
            }
            catch (FoamXServer.FoamXError fxErr)
            {
                System.out.println("connect() : Caught exception");
            }
        }
    }

    //--------------------------------------------------------------------------

    public void disconnect()
    {
        //- Nothing needs to be done here. Don't want to do a server_.close()
        //  since this kills off server

        //if (server_ != null)
        //{
        //    server_.close();
        //
            server_ = null;
        //}

    }


    //--------------------------------------------------------------------------

    // Forcibly quit.
    public void quit()
    {
        if (server_ != null)
        {
            server_.close();
        
            server_ = null;
        }

        // Exit the application.
        class ExitEvent implements Runnable
        {
            public void run()
            {
                System.exit(0);
            }
        }

        // Put an exit application event into the event dispatch
        // thread event queue.
        SwingUtilities.invokeLater(new ExitEvent());
    }

    //--------------------------------------------------------------------------

    public InteractiveNodeList getShapes()
    {
        return shapes_;
    }

    //--------------------------------------------------------------------------

    // Return root of scene
    public TransformGroup getScene()
    {
        //return scene_;
        return transformGroup_;
    }

    //--------------------------------------------------------------------------

    public SimpleUniverse getUniverse()
    {
        return u_;
    }

    //--------------------------------------------------------------------------

    public MyCanvas3D getCanvas()
    {
        return myCanvas_;
    }

    //--------------------------------------------------------------------------

    public Color getBackgroundColor()
    {
        return bgColor_;
    }

    //--------------------------------------------------------------------------

    public Color3f getBackgroundColor3f()
    {
        return bgColor3f_;
    }

    //--------------------------------------------------------------------------

    public Color getForegroundColor()
    {
        return fgColor_;
    }

    //--------------------------------------------------------------------------

    public Color3f getForegroundColor3f()
    {
        return fgColor3f_;
    }

    //--------------------------------------------------------------------------

    public Appearance getPolyAppearance()
    {
        return polyAppearance_;
    }

    //--------------------------------------------------------------------------

    public PickDragBehavior getMouseBehavior()
    {
        return mouseBehavior_;
    }

    //--------------------------------------------------------------------------

    public int getPickMode()
    {
        return pickMode_;
    }

    //--------------------------------------------------------------------------

    public void setPickMode(int mode)
    {
        pickMode_ = mode;
    }

    //--------------------------------------------------------------------------

    /** Autostart a timer with small cycle */
    public javax.swing.Timer getTimer()
    {
        if (visDelayTimer_ == null)
        {
            visDelayTimer_ =
                new javax.swing.Timer
                (
                    500,
                    new ActionListener()
                    {
                        public void actionPerformed(ActionEvent evt)
                        {
                            System.out.println("Timer fired");
                        }
                    }
                );
            visDelayTimer_.start();
        }
        return visDelayTimer_;
    }

    //--------------------------------------------------------------------------

    public void addShape(InteractiveNode shape, boolean visible)
    {
        getScene().addChild(shape.getRoot());
        if (visible)
        {
            shape.show();
        }
        shapes_.addElement(shape);
    }

    //--------------------------------------------------------------------------

    public void deleteShape(InteractiveNode shape)
    {
        shapes_.removeElement(shape);
    }

    //--------------------------------------------------------------------------

    public void deleteShape(String name)
    {
        InteractiveNode node = shapes_.getByName(name);

        shapes_.removeElement(node);
    }

    //--------------------------------------------------------------------------

    public void deleteAllShapes(Class classType)
    {
        Enumeration enumEls = shapes_.getShapes().elements();

        while (enumEls.hasMoreElements())
        {
            InteractiveNode shape = (InteractiveNode)enumEls.nextElement();

            if (classType.isInstance((Object)shape))
            {
                deleteShape(shape);
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

    protected void fireRootSelected(String rootDir)
    {
        // Create event object.
        CaseEvent evt = new CaseEvent
        (
            this,
            rootDir,
            null,
            null,
            null
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

    protected void fireCaseSelected(String rootDir, String caseName)
    {
        // Create event object.
        CaseEvent evt = new CaseEvent
        (
            this,
            rootDir,
            caseName,
            null,
            null
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

    protected void fireTimeSelected(String timeName)
    {
        // Create event object.
        CaseEvent evt = new CaseEvent
        (
            this,
            null,
            null,
            timeName,
            null
        );

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == CaseStatusListener.class)
            {
                ((CaseStatusListener)listeners[i+1]).timeSelected(evt);
            }
        }
    }

    //--------------------------------------------------------------------------

    protected void fireGeomLoaded(File file)
    {
        // Create event object.
        CaseEvent evt = new CaseEvent
        (
            this,
            null,
            null,
            null,
            file
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
    //---- PatchStatusListener Methods
    //--------------------------------------------------------------------------

    public void addPatchStatusListener(PatchStatusListener l)
    {
        listenerList_.add(PatchStatusListener.class, l);
    }

    //--------------------------------------------------------------------------

    public void removePatchStatusListener(PatchStatusListener l)
    {
        listenerList_.remove(PatchStatusListener.class, l);
    }

    //--------------------------------------------------------------------------

    protected void firePatchSelected(String patchName)
    {
        // Create event object.
        PatchEvent evt = new PatchEvent(this, patchName, "");

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == PatchStatusListener.class)
            {
                ((PatchStatusListener)listeners[i+1]).patchSelected(evt);
            }
        }
    }

    //--------------------------------------------------------------------------

    protected void firePatchAdded(String patchName)
    {
        // Create event object.
        PatchEvent evt = new PatchEvent(this, patchName, "");

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == PatchStatusListener.class)
            {
                ((PatchStatusListener)listeners[i+1]).patchAdded(evt);
            }
        }
    }

    //--------------------------------------------------------------------------

    protected void firePatchDeleted(String patchName)
    {
        // Create event object.
        PatchEvent evt = new PatchEvent(this, patchName, "");

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == PatchStatusListener.class)
            {
                ((PatchStatusListener)listeners[i+1]).patchDeleted(evt);
            }
        }
    }

    //--------------------------------------------------------------------------

    protected void firePatchTypeChanged(String patchName, String patchType)
    {
        // Create event object.
        PatchEvent evt = new PatchEvent(this, patchName, patchType);

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == PatchStatusListener.class)
            {
                ((PatchStatusListener)listeners[i+1]).patchTypeChanged(evt);
            }
        }
    }

    //--------------------------------------------------------------------------

    protected void firePatchFacesChanged(String patchName)
    {
        // Create event object.
        PatchEvent evt = new PatchEvent(this, patchName, "");

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == PatchStatusListener.class)
            {
                ((PatchStatusListener)listeners[i+1]).patchFacesChanged(evt);
            }
        }
    }

    //--------------------------------------------------------------------------
    //---- PickListener Methods
    //--------------------------------------------------------------------------

    public void addPickSelectListener(PickListener l)
    {
        listenerList_.add(PickListener.class, l);
    }

    //--------------------------------------------------------------------------

    public void removePickListener(PickListener l)
    {
        listenerList_.remove(PickListener.class, l);
    }

    //--------------------------------------------------------------------------

    public void firePicked
    (
        PickableShape shape,
        PickResult pickResult,
        int meshFaceI,
        int meshEdgeI,
        int meshVertI,
        Point3d point
    )
    {
        // Create event object.
        MeshPickEvent evt =
            new MeshPickEvent
            (
                shape,      // Use pickableShape as source
                shape,
                pickResult,
                meshFaceI,
                meshEdgeI,
                meshVertI,
                point
            );

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == PickListener.class)
            {
                ((PickListener)listeners[i+1]).picked(evt);
            }
        }
    }

    //--------------------------------------------------------------------------

    public void fireUnpicked
    (
        PickableShape shape,
        PickResult pickResult,
        int meshFaceI,
        int meshEdgeI,
        int meshVertI,
        Point3d point
    )
    {
        // Create event object.
        MeshPickEvent evt =
            new MeshPickEvent
            (
                shape,  // Use pickableShape as source
                shape,
                pickResult,
                meshFaceI,
                meshEdgeI,
                meshVertI,
                point
            );

        // Process the listeners last to first, notifying those that
        // are interested in this event.
        Object[] listeners = listenerList_.getListenerList();
        for (int i=listeners.length-2; i>= 0; i-=2)
        {
            if (listeners[i] == PickListener.class)
            {
                ((PickListener)listeners[i+1]).unpicked(evt);
            }
        }
    }

    //--------------------------------------------------------------------------

    /** Set mesh */
    public void setMesh(BoundaryMesh mesh)
    {
        // Delete anything related to old mesh
        removeFaceDisplay();
        removeEdgeDisplay();
        removeFeatureEdgeDisplay();

        boundaryMesh_ = mesh;

        // At some point: get rootdir, casename in here. Dummy for now.
        fireGeomLoaded(null);
    }

    //--------------------------------------------------------------------------

    public BoundaryMesh getMesh()
    {
        return boundaryMesh_;
    }

    //--------------------------------------------------------------------------

    public PickLinesNode[] getDisplayFeatures()
    {
        return displayFeatures_;
    }

    //--------------------------------------------------------------------------

    public PatchFacesNode[] getDisplayPatchFaces()
    {
        return displayPatchFaces_;
    }

    //--------------------------------------------------------------------------

    public PickLinesNode getDisplayEdges()
    {
        return displayEdges_;
    }

    //--------------------------------------------------------------------------

    public PickLinesNode getDisplayExtraEdges()
    {
        return displayExtraEdges_;
    }

    //--------------------------------------------------------------------------
    /** Select patch */
    public void setCurrentPatch(String patchName)
    {
        // Current patch not stored (yet?) since transferred in event
        firePatchSelected(patchName);
    }

    //--------------------------------------------------------------------------
    /** Delete patch */
    public void deletePatch(String patchName)
    {
        getMesh().deletePatch(patchName);

        firePatchDeleted(patchName);
    }

    //--------------------------------------------------------------------------
    /** Add patch */
    public void addPatch(String patchName)
    {
        getMesh().addPatch(patchName);

        firePatchAdded(patchName);
    }

    //--------------------------------------------------------------------------
    /** Change patch type */
    public void changePatchType(String patchName, String patchType)
    {
        getMesh().changePatchType(patchName, patchType);

        firePatchTypeChanged(patchName, patchType);
    }

    //--------------------------------------------------------------------------
    /** Change face patch due to changing of patch. Sets faceMap oldToNew */
    public void changeFaces(String patchName, int[] patchID, int[] oldToNew)
    {
        getMesh().changeFaces(patchID, oldToNew);

        firePatchFacesChanged(patchName);
    }

    //--------------------------------------------------------------------------


    /** Remove displayable triangulated patches */
    public void removeFaceDisplay()
    {
        if (displayPatchFaces_ != null)
        {
            for (int patchI = 0; patchI < displayPatchFaces_.length; patchI++)
            {
                // Delete patchFacesNode. Knows itself how to unlink with
                // listener.
                PatchFacesNode pn = displayPatchFaces_[patchI];

                if (pn != null)
                {
                    deleteShape(pn);

                    displayPatchFaces_[patchI] = null;
                }
            }
            displayPatchFaces_ = null;

            System.gc();
        }
    }

    //--------------------------------------------------------------------------

    /** Add displayable triangulated patches */
    public void addFaceDisplay()
    {
        // Remove old patch faces
        removeFaceDisplay();

        // And recreate new ones

        SurfacePatch[] patches = boundaryMesh_.getPatches();

        int nValidPatches = 0;

        for (int patchI = 0; patchI < patches.length; patchI++)
        {
            // Check for empty patches; cannot be displayed (Java3D limitation)
            if (patches[patchI].getSize() > 0)
            {
                nValidPatches++;
            }
        }

        displayPatchFaces_ = new PatchFacesNode[nValidPatches];

        int newPatchI = 0;

        for (int patchI = 0; patchI < patches.length; patchI++)
        {
            SurfacePatch sp = patches[patchI];

            // Check for empty patches; cannot be displayed (Java3D limitation)
            if (sp.getSize() > 0)
            {
                PrimitivePatch patch = new PrimitivePatch
                (
                    boundaryMesh_,
                    sp.getName()
                );

                Color3f unpickedColour = new Color3f();

                // Create value between 0,1
                float patchVal = 1.0f*newPatchI / (nValidPatches - 1);

                colorizer_.color(patchVal, unpickedColour);

                displayPatchFaces_[newPatchI] = new PatchFacesNode
                (
                    this,
                    sp.getName(),
                    patch,
                    unpickedColour,
                    Math.PI / 4.5
                );

                addShape(displayPatchFaces_[newPatchI], true);

                newPatchI++;
            }
        }
    }

    //--------------------------------------------------------------------------

    /** Remove displayable edges */
    public void removeEdgeDisplay()
    {
        if (displayEdges_ != null)
        {
            // Delete picklines. Knows itself how to unlink with listener.
            deleteShape(displayEdges_);

            displayEdges_ = null;

            System.gc();
        }
    }

    //--------------------------------------------------------------------------

    /** Add displayable edges */
    public void addEdgeDisplay()
    {
        // Remove old edge lines
        removeEdgeDisplay();

        // And recreate new ones

        // Vertices in mesh coordinates (ok since all edges)
        int[] verts = boundaryMesh_.getEdges();

        Color3f lineCol = new Color3f(0.1f, 1.0f, 0.1f);

        displayEdges_ = new PickLinesNode
        (
            this,
            "edges",
            boundaryMesh_.getPoints(),
            verts,
            lineCol,
            boundaryMesh_.getEdgeSet(),
            null
        );

        addShape(displayEdges_, true);
    }

    //--------------------------------------------------------------------------

    /** Remove displayable feature edges */
    public void removeFeatureEdgeDisplay()
    {
        // Remove old feature edges display

        if (displayFeatures_ != null)
        {
            for
            (
                int segmentI = 0;
                segmentI < displayFeatures_.length;
                segmentI++
            )
            {
                PickLinesNode pn = displayFeatures_[segmentI];

                if (pn != null)
                {
                    deleteShape(pn);

                    displayFeatures_[segmentI] = null;
                }
            }
            displayFeatures_ = null;

            System.gc();
        }
    }

    //--------------------------------------------------------------------------

    /** Add displayable feature edges */
    public void addFeatureEdgeDisplay()
    {
        Color3f lineCol = new Color3f(0.1f, 0.1f, 1.0f);

        // Remove old feature edges display
        removeFeatureEdgeDisplay();

        // And recreate new feature edges display

        int[][] segments = boundaryMesh_.getFeatureSegments();

        // Feature edges as 2 consecutive indices into featurePoints
        int[] fEdges = boundaryMesh_.getFeatureEdges();

        // Map from feature edge to mesh edge
        int[] featToEdge = boundaryMesh_.getFeatureToEdge();

        displayFeatures_ = new PickLinesNode[segments.length];

        for (int segmentI = 0; segmentI < segments.length; segmentI++)
        {
            int[] segment = segments[segmentI];

            // Construct lines (two consecutive verts per edge)
            int[] verts = new int[2 * segment.length];

            // From local to mesh edge mapping
            int[] toMeshEdge = new int[segment.length];

            int vertI = 0;

            for (int edgeLabel = 0; edgeLabel < segment.length; edgeLabel++)
            {
                int fEdgeI = segment[edgeLabel];

                toMeshEdge[edgeLabel] = featToEdge[fEdgeI];

                verts[vertI++] = fEdges[2*fEdgeI];
                verts[vertI++] = fEdges[2*fEdgeI+1];
            }

            // Construct pickable lines

            displayFeatures_[segmentI] = new PickLinesNode
            (
                this,
                "fEdges_" + segmentI,
                boundaryMesh_.getFeaturePoints(),
                verts,
                lineCol,
                boundaryMesh_.getFeatureEdgeSets()[segmentI],
                toMeshEdge
            );

            addShape(displayFeatures_[segmentI], true);
        }
    }

    //--------------------------------------------------------------------------

    /** Remove displayable extra edges */
    public void removeExtraEdgeDisplay()
    {
        // Remove old extra edges display

        if (displayExtraEdges_ != null)
        {
            deleteShape(displayExtraEdges_);

            displayExtraEdges_ = null;

            System.gc();
        }
    }

    //--------------------------------------------------------------------------

    /** Add displayable feature edges */
    public void addExtraEdgeDisplay()
    {
        // Indices of edges
        int[] extraEdges = boundaryMesh_.getExtraEdges();

        if (extraEdges == null)
        {
            return;
        }

        Color3f lineCol = new Color3f(1.0f, 1.0f, 0.2f);

        // Vertices per edge
        int[] edges = boundaryMesh_.getEdges();

        // Remove old feature edges display
        removeExtraEdgeDisplay();

        //
        // And recreate new extra edges display
        //

        // Construct lines (two consecutive verts per edge)
        int[] verts = new int[2 * extraEdges.length];

        int vertI = 0;

        for (int edgeLabel = 0; edgeLabel < extraEdges.length; edgeLabel++)
        {
            int edgeI = extraEdges[edgeLabel];

            verts[vertI++] = edges[2*edgeI];

            verts[vertI++] = edges[2*edgeI+1];
        }


        // Construct pickable lines

        displayExtraEdges_ = new PickLinesNode
        (
            this,
            "extraEdges",
            boundaryMesh_.getPoints(),
            verts,
            lineCol,
            boundaryMesh_.getExtraEdgeSet(),
            extraEdges
        );

        addShape(displayExtraEdges_, true);
    }

    //--------------------------------------------------------------------------

    public PreProcDlg getPreDialog()
    {
        return preDlg_;
    }

    //--------------------------------------------------------------------------

    public String setMsg(String msg)
    {
        String oldMsg = preDlg_.getPreProcPanel().getMsg();

        preDlg_.getPreProcPanel().setMsg(msg);

        return oldMsg;
    }

    //--------------------------------------------------------------------------

    public static Appearance createMaterialAppearance(boolean allowAll)
    {
        Appearance materialAppear = new Appearance();

        // Shiny default material
       	materialAppear.setMaterial(new Material());

        PolygonAttributes polyAttrib = new PolygonAttributes();
        //polyAttrib.setCullFace(PolygonAttributes.CULL_BACK);
        polyAttrib.setCullFace(PolygonAttributes.CULL_NONE);

        materialAppear.setPolygonAttributes(polyAttrib);

	TransparencyAttributes transp = new TransparencyAttributes();
	transp.setTransparencyMode(TransparencyAttributes.NONE);
	transp.setTransparency(0.0f);
 	materialAppear.setTransparencyAttributes(transp);

        if (allowAll)
        {
            materialAppear.setCapability
            (
                Appearance.ALLOW_TRANSPARENCY_ATTRIBUTES_READ 
            );
            materialAppear.setCapability
            (
                Appearance.ALLOW_TRANSPARENCY_ATTRIBUTES_WRITE
            );
            materialAppear.setCapability
            (
                Appearance.ALLOW_POLYGON_ATTRIBUTES_READ
            );
            materialAppear.setCapability
            (
                Appearance.ALLOW_POLYGON_ATTRIBUTES_WRITE
            );

            // Access to rendering mode
            polyAttrib.setCapability
            (
                PolygonAttributes.ALLOW_MODE_READ
            );
            polyAttrib.setCapability
            (
                PolygonAttributes.ALLOW_MODE_WRITE
            );

            polyAttrib.setCapability
            (
                PolygonAttributes.ALLOW_CULL_FACE_READ
            );
            polyAttrib.setCapability
            (
                PolygonAttributes.ALLOW_CULL_FACE_WRITE
            );

            polyAttrib.setCapability
            (
                PolygonAttributes.ALLOW_NORMAL_FLIP_READ
            );
            polyAttrib.setCapability
            (
                PolygonAttributes.ALLOW_NORMAL_FLIP_WRITE
            );

            // Access to transparency
            transp.setCapability
            (
               TransparencyAttributes.ALLOW_VALUE_READ
            );
            transp.setCapability
            (
               TransparencyAttributes.ALLOW_VALUE_WRITE
            );
            transp.setCapability
            (
                TransparencyAttributes.ALLOW_MODE_READ 
            );
            transp.setCapability
            (
                TransparencyAttributes.ALLOW_MODE_WRITE
            );
        }

        return materialAppear;
    }

    //--------------------------------------------------------------------------

    public static Appearance createLineAppearance(boolean allowAll)
    {
        Appearance materialAppear = new Appearance();

	TransparencyAttributes transp = new TransparencyAttributes();
	transp.setTransparencyMode(TransparencyAttributes.FASTEST);
	transp.setTransparency(0.0f);
	materialAppear.setTransparencyAttributes(transp);


        if (allowAll)
        {
            materialAppear.setCapability
            (
                Appearance.ALLOW_TRANSPARENCY_ATTRIBUTES_READ 
            );
            materialAppear.setCapability
            (
                Appearance.ALLOW_TRANSPARENCY_ATTRIBUTES_WRITE
            );

            // Access to transparency
            transp.setCapability
            (
               TransparencyAttributes.ALLOW_VALUE_READ
            );
            transp.setCapability
            (
               TransparencyAttributes.ALLOW_VALUE_WRITE
            );
        }

        return materialAppear;
    }

    //--------------------------------------------------------------------------

    public static void setConstraint
    (
        int x,
        int y,
        int w,
        int h,
        double wx,
        double wy,
        GridBagConstraints c
    )
    {
        c.gridx = x;
        c.gridy = y;
        c.gridwidth = w;
        c.gridheight = h;
        c.weightx = wx;
        c.weighty = wy;
    }

    //--------------------------------------------------------------------------

    public static Transform3D viewAll(View view, BoundingSphere bounds)
    {
        Transform3D viewTrans = new Transform3D();

//        Point3d center = new Point3d();
//        bounds.getCenter(center);
//        double radius = bounds.getRadius();
//
//        double eyeDist = 1.2 * radius / Math.tan(view.getFieldOfView() / 2.0);
//
//        Vector3d up = new Vector3d(0, 0, 1);
//        Point3d eyePos = new Point3d(center);
//        eyePos.z += eyeDist;
//
//
//        System.out.println("eyePos : " + eyePos);
//        System.out.println("center : " + center);
//        System.out.println("up     : " + up);

//        viewTrans.lookAt(eyePos, center, up);
//        viewTrans.invert();

//        if ( view.getBackClipDistance() < eyeDist )
//        {
//            view.setBackClipDistance(eyeDist);
//            view.setFrontClipDistance(eyeDist / 3000);
//        }

        double rad=bounds.getRadius();
        Point3d ctr=new Point3d();
        bounds.getCenter(ctr);
        double viewDistance=2.0*rad/Math.tan(view.getFieldOfView()/2.0);
        Transform3D dist = new Transform3D();
        dist.set(new Vector3d(0, 0, viewDistance));
        Transform3D rotation = new Transform3D();

        double rx = 0.2;
        double ry = 0.0;
        double rz = 0.0;

        rotation.setEuler(new Vector3d(rx, ry, rz));
        rotation.mul(dist);

        viewTrans.set(new Vector3d(ctr.x, ctr.y, ctr.z));
        viewTrans.mul(rotation);

        return viewTrans;
    }

    //--------------------------------------------------------------------------


    //
    // The following allows PatchTool to be run as an application
    // as well as an applet
    //
    public static void main(String[] args)
    {
        pp_ = new PatchTool(args);

    	new MainFrame(pp_, 400, 400);
    }
}
