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
import PatchTool.Mesh.BoundaryMesh;
import PatchTool.Events.ElementEvent;
import PatchTool.Events.ElementSelectListener;


public class EdgeSet extends SelectedSet
{

    //--------------------------------------------------------------------------

    /** Construct from components */
    public EdgeSet(int len)
    {
        super(len);
    }

    //--------------------------------------------------------------------------

    /** Construct from stream */
    public EdgeSet(Reader readStream)
    {
        super(readStream);
    }

    //--------------------------------------------------------------------------

}
