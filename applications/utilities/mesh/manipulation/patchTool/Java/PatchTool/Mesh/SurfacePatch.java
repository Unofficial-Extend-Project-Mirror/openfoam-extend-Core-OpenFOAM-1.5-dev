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

public class SurfacePatch
{
    String name_;
    String geometricType_;
    int size_;
    int start_;

    public SurfacePatch
    (
        String name,
        String geometricType,
        int size,
        int start
    )
    {
        name_ = name;
        geometricType_ = geometricType;
        size_ = size;
        start_ = start;
    }

    public SurfacePatch(Reader read)
    {
        try
        {
            FoamDataParser foamParser = new FoamDataParser(read);

            name_ = foamParser.readWord();

            foamParser.readOpenCurly();

            // Read geometricType
            String key = foamParser.readWord();

            if (!key.equals("geometricType") && !key.equals("type"))
            {
                throw new NumberFormatException
                (
                    "did not find geometricType entry"
                );
            }
            geometricType_ = foamParser.readWord();

            foamParser.readSemiColon();

            // Read nFaces
            key = foamParser.readWord();

            if (!key.equals("nFaces"))
            {
                throw new NumberFormatException
                (
                    "did not find nFaces entry"
                );
            }
            size_ = foamParser.readInt();

            foamParser.readSemiColon();

            // Read startFace
            key = foamParser.readWord();

            if (!key.equals("startFace"))
            {
                throw new NumberFormatException
                (
                    "did not find startFace entry"
                );
            }
            start_ = foamParser.readInt();

            foamParser.readSemiColon();

            foamParser.readCloseCurly();
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
//        try
//        {
            wr.println(name_);
            wr.println("{");
            wr.println("    geometricType " + geometricType_ + ";");
            wr.println("    nFaces " + size_ + ";");
            wr.println("    startFace " + start_ + ";");
            wr.println("}");
            wr.flush();
//        }
//        catch(IOException ex)
//        {
//            System.out.println("IOException " + ex);
//        }
    }


    public String getName()
    {
        return name_;
    }

    public String getGeometricType()
    {
        return geometricType_;
    }

    public void setGeometricType(String type)
    {
        geometricType_ = type;
    }

    public int getSize()
    {
        return size_;
    }

    public int getStart()
    {
        return start_;
    }

    public static void main(String[] args)
    {
        try
        {
            FileInputStream inStream = new FileInputStream
            (
                new File("triBoundary")
            );
            Reader read = new BufferedReader(new InputStreamReader(inStream));

            SurfacePatch patch = new SurfacePatch(read);

            System.out.println
            (
                patch.getName() + " " + patch.getSize()
              + " " + patch.getStart()
            );
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
}
