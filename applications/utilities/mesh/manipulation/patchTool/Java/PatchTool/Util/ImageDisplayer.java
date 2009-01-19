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

import javax.swing.*;
import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.awt.event.*;

public class ImageDisplayer extends JFrame implements ActionListener {
    BufferedImage bImage;

    private class ImagePanel extends JPanel {
	public void paint(Graphics g) {
	    g.setColor(Color.black);
	    g.fillRect(0, 0, getSize().width, getSize().height);
	    g.drawImage(bImage, 0, 0, this);
	}

	private ImagePanel() {
	    setPreferredSize(new Dimension(bImage.getWidth(),
					   bImage.getHeight()));
	}
    }

    private JMenuItem printItem;
    private JMenuItem closeItem;

    public void actionPerformed (ActionEvent event) {
	Object target = event.getSource();

	if (target == printItem) {
	    new ImagePrinter(bImage).print();
	}
	else if (target == closeItem) {
	    this.removeAll();
	    this.setVisible(false);
	    bImage = null;
	}
    }

    private JMenuBar createMenuBar() {
	JMenuBar menuBar = new JMenuBar();
	JMenu fileMenu = new JMenu("File");
	printItem = new JMenuItem("Print...");
	printItem.addActionListener(this);
	closeItem = new JMenuItem("Close");
	closeItem.addActionListener(this);
	fileMenu.add(printItem);
	fileMenu.add(new JSeparator());
	fileMenu.add(closeItem);
	menuBar.add(fileMenu);
	return menuBar;
    }

    public ImageDisplayer(BufferedImage bImage) {
	this.bImage = bImage;
	this.setTitle("Off-screen Canvas3D Snapshot");

	// Create and initialize menu bar
	this.setJMenuBar(createMenuBar());

	// Create scroll pane, and embedded image panel
	ImagePanel imagePanel = new ImagePanel();
	JScrollPane scrollPane = new JScrollPane(imagePanel);

        int width = Math.min(700, bImage.getWidth());
        int height = Math.min(700, bImage.getHeight());

//	scrollPane.getViewport().setPreferredSize(new Dimension(700, 700));
	scrollPane.getViewport().setPreferredSize(new Dimension(width, height));

	// Add scroll pane to the frame and make it visible
	this.getContentPane().add(scrollPane);
	this.pack();
	this.setVisible(true);
    }
}
