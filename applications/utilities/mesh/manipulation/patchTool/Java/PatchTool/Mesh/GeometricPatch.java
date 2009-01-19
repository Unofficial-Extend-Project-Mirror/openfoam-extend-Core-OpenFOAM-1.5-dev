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

package PatchTool.Mesh;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;

import javax.media.j3d.*;
import javax.vecmath.Point3f;

import PatchTool.Util.FoamDataParser;

public class GeometricPatch
{
    String name_;
    String geometricType_;

    public GeometricPatch
    (
        String name,
        String geometricType
    )
    {
        name_ = name;
        geometricType_ = geometricType;
    }

    public GeometricPatch(Reader read)
    {
        try
        {
            FoamDataParser foamParser = new FoamDataParser(read);

            name_ = foamParser.readWord();

            geometricType_ = foamParser.readWord();
        }
        catch(NumberFormatException ex)
        {
            System.out.println("NumberFormatException " + ex);
        }
        catch(IOException ex)
        {
            System.out.println("IOException " + ex);
        }
    }


    public void write(PrintWriter wr)
    {
        wr.print(name_ + "\n");
        wr.print(geometricType_ + "\n");
    }


    public String getName()
    {
        return name_;
    }

    public String getGeometricType()
    {
        return geometricType_;
    }

//    public static void main(String[] args)
//    {
//        try
//        {
//            FileInputStream inStream = new FileInputStream
//            (
//                new File("triBoundary")
//            );
//            Reader read = new BufferedReader(new InputStreamReader(inStream));
//
//            GeometricPatch patch = new GeometricPatch(read);
//
//            System.out.println
//            (
//                patch.getName() + " " + patch.getSize()
//              + " " + patch.getStart()
//            );
//        }
//        catch(NumberFormatException ex)
//        {
//            System.out.println("NumberFormatException " + ex);
//        }
//        catch(IOException ex)
//        {
//            System.out.println("IOException " + ex);
//        }
//    }
}
