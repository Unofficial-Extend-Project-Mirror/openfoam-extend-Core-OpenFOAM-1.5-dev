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
import PatchTool.Mesh.GeometricPatch;

public class LabelledTriMesh
{
    Point3f[] points_;

    int[][] faces_;

    int[] patchIDs_;

    GeometricPatch[] patches_;


    public LabelledTriMesh
    (
        Point3f[] points,
        int[][] faces,
        int[] patchIDs,
        GeometricPatch[] patches
    )
    {
        points_ = points;
        faces_ = faces;
        patchIDs_ = patchIDs;
        patches_ = patches;
    }


    public LabelledTriMesh(Reader read)
    {
        try
        {
            FoamDataParser foamParser = new FoamDataParser(read);

            // Read patch types and names
            int nPatches = foamParser.readInt();

            patches_ = new GeometricPatch[nPatches];

            foamParser.readOpen();

            for(int i = 0; i < nPatches; i++)
            {
                patches_[i] = new GeometricPatch(read);
                System.out.println("Read patch " + i);
            }

            foamParser.readClose();


            // Read points
            points_ = foamParser.readPoint3fList("points");
            System.out.println("Number of points:" + points_.length);

            //for(int i = 0; i < points_.length; i++)
            //{
            //    System.out.println("point:" + points_[i]);
            //}



            // Read labelledTris
            int nFaces = foamParser.readInt();

            System.out.println("Number of faces:" + nFaces);

            faces_ = new int[nFaces][3];
            patchIDs_ = new int[nFaces];

            foamParser.readOpen();

            for(int i = 0; i < nFaces; i++)
            {
                foamParser.readOpen();
                foamParser.readOpen();

                int[] face = new int[3];

                face[0] = foamParser.readInt();
                face[1] = foamParser.readInt();
                face[2] = foamParser.readInt();

                faces_[i] = face;

                //System.out.println
                //(
                //    "Read face " + i + " " + faces_[i][0]
                //  + " " + faces_[i][1] + " " + faces_[i][2]
                //);

                foamParser.readClose();

                int patchID = foamParser.readInt();

                if ((patchID < 0) || (patchID >= patches_.length))
                {
                    throw new NumberFormatException
                    (
                        "patchID outside range 0.." + patches_.length
                    );
                }
                patchIDs_[i] = patchID;

                foamParser.readClose();
            }

            foamParser.readClose();
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
        wr.print(patches_.length + "(\n");
        for(int i = 0; i < patches_.length; i++)
        {
            patches_[i].write(wr);
            wr.print("\n");
        }
        wr.print(")\n");

        wr.print(points_.length + "(\n");
        for(int i = 0; i < points_.length; i++)
        {
            Point3f pt = points_[i];

            wr.print("(");
            wr.print(pt.toString());
            wr.print(")\n");
            wr.print("\n");
        }
        wr.print(")\n");

        wr.print(faces_.length + "(\n");
        for(int i = 0; i < points_.length; i++)
        {
            wr.print("((");
            wr.print(faces_[i][0]);
            wr.print(" ");
            wr.print(faces_[i][1]);
            wr.print(" ");
            wr.print(faces_[i][2]);
            wr.print(") ");
            wr.print(patchIDs_[i]);
            wr.print(")\n");
        }
        wr.print(")\n");

        wr.flush();
    }


    public Point3f[] getPoints()
    {
        return points_;
    }

    public int[][] getFaces()
    {
        return faces_;
    }

    public int[] getPatchIDs()
    {
        return patchIDs_;
    }

    public GeometricPatch[] getPatches()
    {
        return patches_;
    }


    public static void main(String[] args)
    {
        try
        {
            FileInputStream inStream = new FileInputStream
            (
                new File("triSurface.ftr")
            );
            Reader read = new BufferedReader(new InputStreamReader(inStream));

            LabelledTriMesh surf = new LabelledTriMesh(read);

            PrintWriter out = new PrintWriter
            (
                new OutputStreamWriter(System.out)
            );
            surf.write(out);
        }
        catch(FileNotFoundException ex)
        {
            System.out.println("FileNotFoundException " + ex);
        }
        catch(IOException ex)
        {
            System.out.println("IOException " + ex);
        }
        catch(NumberFormatException ex)
        {
            System.out.println("NumberFormatException " + ex);
        }
    }
}
