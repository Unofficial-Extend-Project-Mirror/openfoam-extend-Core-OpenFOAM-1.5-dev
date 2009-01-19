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

Class
    SimpleViewer

Description
    bare bones Java text viewer.

\*---------------------------------------------------------------------------*/

package PatchTool.Util;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.Hashtable;

import javax.swing.*;
import javax.swing.text.*;
import javax.swing.event.*;
import javax.swing.undo.*;

public class SimpleViewer extends JFrame
{
    /** Construct from String */
    public SimpleViewer(String msg)
    {
        //some initial setup
        super("Simple Viewer");

        initComponents(msg);
    }

    /** Construct from File */
    public SimpleViewer(File inFile)
    {
        super(inFile.getName());

        String msg = "";

        try
        {
            // Read data from file
            int size = (int)inFile.length();

            FileInputStream inStream = new FileInputStream(inFile);

            int nRead = 0;
            byte[] data = new byte[size];
            while(nRead < size)
            {
                nRead += inStream.read(data, nRead, size-nRead);
            }
            inStream.close();

            msg = new String(data);
        }
        catch (IOException ex)
        {
            System.out.println("Problem reading file " + inFile);
        }

        initComponents(msg);

    }

    //--------------------------------------------------------------------------
    /** Initialize gui
      */
    private void initComponents(String msg)
    {
       //Create a text area.
        textArea_ = new JTextArea(msg);
        textArea_.setLineWrap(true);
        textArea_.setWrapStyleWord(true);
        textArea_.setCaretPosition(0);
        textArea_.setMargin(new Insets(10, 10, 10, 10));
        textArea_.setEditable(false);

        JScrollPane areaScrollPane = new JScrollPane(textArea_);
        areaScrollPane.setVerticalScrollBarPolicy(
                        JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        areaScrollPane.setPreferredSize(new Dimension(480, 300));

//        JScrollBar sb = areaScrollPane.getVerticalScrollBar();
//
//System.out.println("Value set:" + sb.getValue());
//
//        sb.setValue(10); 
//  
//        JViewport vp = areaScrollPane.getViewport();
//
//        vp.setViewPosition(new Point(0,0));
//
        setContentPane(areaScrollPane);
    }


    //--------------------------------------------------------------------------
    //---- UI components
    //--------------------------------------------------------------------------

    private JTextArea textArea_;


    //--------------------------------------------------------------------------

    //The standard main method.
    public static void main(String[] args)
    {
        File file = new File(args[0]);

        final SimpleViewer frame =
            new SimpleViewer(file);

        frame.pack();
        frame.setVisible(true);
    }
}
