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

import java.awt.GraphicsConfiguration;
import java.awt.image.BufferedImage;
import javax.media.j3d.*;
import javax.vecmath.*;


public class OffScreenCanvas3D extends Canvas3D {
    public OffScreenCanvas3D(GraphicsConfiguration graphicsConfiguration,
		      boolean offScreen) {

	super(graphicsConfiguration, offScreen);
    }

    public BufferedImage doRender(int width, int height) {

	BufferedImage bImage =
	    new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);

	ImageComponent2D buffer =
	    new ImageComponent2D(ImageComponent.FORMAT_RGBA, bImage);

	setOffScreenBuffer(buffer);
	renderOffScreenBuffer();
	waitForOffScreenRendering();
	bImage = getOffScreenBuffer().getImage();

	return bImage;
    }

    public void postSwap() {
	// No-op since we always wait for off-screen rendering to complete
    }
}
