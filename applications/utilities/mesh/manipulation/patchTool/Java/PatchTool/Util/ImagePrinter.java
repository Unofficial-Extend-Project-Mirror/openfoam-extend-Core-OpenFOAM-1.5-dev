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

import java.awt.print.*;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;
import java.awt.geom.AffineTransform;

public class ImagePrinter implements Printable, ImageObserver {
    BufferedImage bImage;

    public int print(Graphics g, PageFormat pf, int pi)
	throws PrinterException {


	if (pi >= 1) {
	    return Printable.NO_SUCH_PAGE;
	}

	Graphics2D g2d = (Graphics2D)g;
	//g2d.translate(pf.getImageableX(), pf.getImageableY());
	AffineTransform t2d = new AffineTransform();
	t2d.translate(pf.getImageableX(), pf.getImageableY());
	double xscale  = pf.getImageableWidth() / (double)bImage.getWidth();
	double yscale  = pf.getImageableHeight() / (double)bImage.getHeight();
	double scale = Math.min(xscale, yscale);
	t2d.scale(scale, scale);
	try {
	    g2d.drawImage(bImage,t2d, this);
	}
	catch (Exception ex) {
	    ex.printStackTrace();
	    return Printable.NO_SUCH_PAGE;
	}
	return Printable.PAGE_EXISTS;
    }

    void print() {
	PrinterJob printJob = PrinterJob.getPrinterJob();
	PageFormat pageFormat = printJob.defaultPage();
	pageFormat.setOrientation(PageFormat.LANDSCAPE);
	pageFormat = printJob.validatePage(pageFormat);
	printJob.setPrintable(this, pageFormat);
	if (printJob.printDialog()) {
	    try {
		printJob.print();
	    }
	    catch (PrinterException ex) {
		ex.printStackTrace();
	    }
	}
    }

    public boolean imageUpdate(Image img,
			       int infoflags,
			       int x,
			       int y,
			       int width,
			       int height) {
	return false;
    }

    ImagePrinter(BufferedImage bImage) {
	this.bImage = bImage;
    }
}
