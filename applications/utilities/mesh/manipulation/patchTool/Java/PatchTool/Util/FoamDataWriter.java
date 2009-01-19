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

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import javax.media.j3d.*;
import javax.vecmath.*;

public class FoamDataWriter
{
    public static void writeHeader
    (
        PrintWriter wr,
        String rootDir,
        String caseName,
        String instance,
        String local,
        String className,
        String object
    )
    {
        wr.print("FoamFile\n");
        wr.print("{\n");
        wr.print("    version 2.0;\n");
        wr.print("    format ascii;\n");
        wr.print("\n");
        wr.print("    root \"" + rootDir + "\";\n");
        wr.print("    case \"" + caseName + "\";\n");
        wr.print("    instance \"" + instance + "\";\n");
        wr.print("    local \"" + local + "\";\n");
        wr.print("\n");
        wr.print("    class " + className + ";\n");
        wr.print("    object " + object + ";\n");
        wr.print("\n");
    }

    public static void writeIntList(PrintWriter wr, int[] lst, boolean writeLen)
    {
        if (writeLen)
        {
            wr.print(lst.length);
        }
        if (lst.length < 10)
        {
            wr.print("(");

            for (int i = 0; i < lst.length; i++)
            {
                if (i != 0)
                {
                    wr.print(' ');
                }
                wr.print(lst[i]);
            }

            wr.print(")");
        }
        else
        {
            wr.print("\n(\n");

            for (int i = 0; i < lst.length; i++)
            {
                wr.print(lst[i]);
                wr.print('\n');
            }

            wr.print(")\n");
        }
    }

    public static void writeDoubleList(PrintWriter wr, double[] lst)
    {
        wr.println(lst.length);
        wr.print("(\n");

        for (int i = 0; i < lst.length; i++)
        {
            wr.print(lst[i]);
            wr.print('\n');
        }
        wr.print(")\n");
    }


    public static void writePoint3f(PrintWriter wr, Point3f pt)
    {
        wr.print("(");
        wr.print(pt.x);
        wr.print(" ");
        wr.print(pt.y);
        wr.print(" ");
        wr.print(pt.z);
        wr.print(")");
    }

    public static void writePoint3fList(PrintWriter wr, Point3f[] lst)
    {
        wr.println(lst.length);
        wr.print("(\n");

        for (int i = 0; i < lst.length; i++)
        {
            writePoint3f(wr, lst[i]);
            wr.print('\n');
        }
        wr.print(")\n");
    }

    public static void writeIntListList
    (
        PrintWriter wr,
        int[][] lst,
        boolean writeLen
    )
    {
        wr.println(lst.length);
        wr.print("(\n");

        for (int i = 0; i < lst.length; i++)
        {
            writeIntList(wr, lst[i], writeLen);
            wr.println();
        }
        wr.print(")\n");
    }

    //--------------------------------------------------------------------------

}
