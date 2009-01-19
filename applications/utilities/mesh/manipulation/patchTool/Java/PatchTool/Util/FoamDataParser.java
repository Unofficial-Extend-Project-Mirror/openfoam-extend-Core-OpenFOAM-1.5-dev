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

public class FoamDataParser
{
    StreamTokenizer st_;

    public FoamDataParser(Reader read)
    {
        st_ = new StreamTokenizer(read);

        st_.eolIsSignificant(false);
        st_.parseNumbers();
        st_.slashSlashComments(true);
        st_.slashStarComments(true);
    }


    public void printCurrent()
    {
        switch(st_.ttype)
        {
            case StreamTokenizer.TT_NUMBER:
                System.out.println("    Number:" + st_.nval);
                break;
            case StreamTokenizer.TT_WORD:
                System.out.println("    Word:" + st_.sval);
                break;
            case StreamTokenizer.TT_EOL:
                System.out.println("    EOL");
                break;
            case StreamTokenizer.TT_EOF:
                System.out.println("    EOF");
                break;
            default:
                System.out.println("    Generic:" + st_.ttype);
                break;
        }
    }

    //--------------------------------------------------------------------------

    public String readWord() throws NumberFormatException, IOException 
    {
        int token = st_.nextToken();

        if (st_.ttype == StreamTokenizer.TT_WORD)
        {
            return st_.sval;
        }
        throw new NumberFormatException
        (
            "illegal word"
        );
    }


    public int readInt() throws NumberFormatException, IOException 
    {
        // Make it stop at '.'
        st_.ordinaryChar('.'); 
 
        int token = st_.nextToken();

        // Reset to parse numbers
        st_.parseNumbers();

        if (st_.ttype == StreamTokenizer.TT_NUMBER)
        {
            return (int)st_.nval;
        }
        throw new NumberFormatException
        (
            "illegal integer"
        );
    }


    public double readDouble() throws NumberFormatException, IOException
    {
        int token = st_.nextToken();

        //printCurrent();

        if (token != StreamTokenizer.TT_NUMBER)
        {
            throw new NumberFormatException
            (
                "Illegal mantissa"
            );
        }
        double mantissa = st_.nval;

        int exponent = 0;

        token = st_.nextToken();

        //System.out.println("Token:");
        //printCurrent();

        if ((token == 'e') || (token == 'E'))
        {
            exponent = readInt();
        }
        else if (token == StreamTokenizer.TT_WORD)
        {
            String fullNumber = Double.toString(mantissa) + st_.sval;

            return Double.parseDouble(fullNumber);
        }
        else if (token == '+')
        {
            String fullNumber = Double.toString(mantissa) + 'E' + readInt();

            return Double.parseDouble(fullNumber);
        }
        else
        {
            st_.pushBack();
        }

        if (exponent == 0)
        {
            return mantissa;
        }
        else
        {
            return mantissa*Math.pow(10, exponent);
        }
    }


    public Point3f readPoint3f() throws NumberFormatException, IOException
    {
        readOpen();

        float x = (float)readDouble();
        float y = (float)readDouble();
        float z = (float)readDouble();

        readClose();

        return new Point3f(x, y, z);        
    }


    public int[] readEdge() throws NumberFormatException, IOException
    {
        readOpen();

        int[] edge = new int[2];
        edge[0] = readInt();
        edge[1] = readInt();

        readClose();

        return edge;        
    }


    public void readOpen() throws NumberFormatException, IOException
    {
        int token = st_.nextToken();

        if (token != '(')
        {
            throw new NumberFormatException
            (
                "did not find '('"
            );
        }
    }


    public void readClose() throws NumberFormatException, IOException
    {
        int token = st_.nextToken();

        if (token != ')')
        {
            throw new NumberFormatException
            (
                "did not find ')'"
            );
        }
    }


    public void readOpenCurly() throws NumberFormatException, IOException
    {
        int token = st_.nextToken();

        if (token != '{')
        {
            throw new NumberFormatException
            (
                "did not find '{'"
            );
        }
    }


    public void readCloseCurly() throws NumberFormatException, IOException
    {
        int token = st_.nextToken();

        if (token != '}')
        {
            throw new NumberFormatException
            (
                "did not find '}'"
            );
        }
    }


    public void readSemiColon() throws NumberFormatException, IOException
    {
        int token = st_.nextToken();

        if (token != ';')
        {
            throw new NumberFormatException
            (
                "did not find ';'"
            );
        }
    }


    public void skipHeader()
        throws NumberFormatException, IOException
    {
        int token = st_.nextToken();

        if
        (
            (token == StreamTokenizer.TT_WORD)
         && (st_.sval.equals("FoamFile"))
        )
        {
            System.out.println("Found header");

            token = st_.nextToken();

            if (token != '{')
            {
                throw new NumberFormatException
                (
                    "did not find '{' after header 'FoamFile'"
                );
            }

            while (token != '}')
            {
                token = st_.nextToken();
            }
        }
        else
        {
            st_.pushBack();
        }
    }


    //--------------------------------------------------------------------------



    //--------------------------------------------------------------------------
    // Compound data readers

    public int[] readIntList(String context)
    {
        int[] vals = null;

        int elemI = 0;

        try
        {
            int nElems = readInt();

            vals = new int[nElems];

            readOpen();

            while(elemI < nElems)
            {
                vals[elemI++] = readInt();
            }
            readClose();
        }
        catch(NumberFormatException ex)
        {
            System.out.println
            (
                "NumberFormatException " + ex + " while reading element "
              + elemI + " at line " + st_.lineno() + " of intList "
              + context
            );
        }
        catch(IOException ex)
        {
            System.out.println
            (
                "IOException " + ex + " while reading element "
              + elemI + " at line " + st_.lineno() + " of intList "
              + context
            );
        }

        return vals;
    }

    public double[] readDoubleList(String context)
    {
        double[] vals = null;

        int elemI = 0;

        try
        {
            int nElems = readInt();

            vals = new double[nElems];

            readOpen();

            while(elemI < nElems)
            {
                vals[elemI++] = readDouble();
            }
            readClose();
        }
        catch(NumberFormatException ex)
        {
            System.out.println
            (
                "NumberFormatException " + ex + " while reading element "
              + elemI + " at line " + st_.lineno() + " of doubleList "
              + context
            );
        }
        catch(IOException ex)
        {
            System.out.println
            (
                "IOException " + ex + " while reading element "
              + elemI + " at line " + st_.lineno() + " of doubleList "
              + context
            );
        }
        return vals;
    }
        
    public Point3f[] readPoint3fList(String context)
    {
        Point3f[] vals = null;

        int elemI = 0;

        try
        {
            int nElems = readInt();

            vals = new Point3f[nElems];

            readOpen();

            while(elemI < nElems)
            {
                vals[elemI++] = readPoint3f();
            }
            readClose();
        }
        catch(NumberFormatException ex)
        {
            System.out.println
            (
                "NumberFormatException " + ex + " while reading element "
              + elemI + " at line " + st_.lineno() + " of pointList "
              + context
            );
        }
        catch(IOException ex)
        {
            System.out.println
            (
                "IOException " + ex + " while reading element "
              + elemI + " at line " + st_.lineno() + " of pointList "
              + context
            );
        }
        return vals;
    }
        
    public int[][] readEdgeList(String context)
    {
        int[][] vals = null;

        int elemI = 0;

        try
        {
            int nElems = readInt();

            vals = new int[nElems][];

            readOpen();

            while(elemI < nElems)
            {
                vals[elemI++] = readEdge();
            }
            readClose();
        }
        catch(NumberFormatException ex)
        {
            System.out.println
            (
                "NumberFormatException " + ex + " while reading element "
              + elemI + " at line " + st_.lineno() + " of pointList "
              + context
            );
        }
        catch(IOException ex)
        {
            System.out.println
            (
                "IOException " + ex + " while reading element "
              + elemI + " at line " + st_.lineno() + " of pointList "
              + context
            );
        }
        return vals;
    }
        
    public int[][] readIntListList(String context)
    {
        int[][] vals = null;

        int elemI = 0;

        try
        {
            int nElems = readInt();

            vals = new int[nElems][];

            readOpen();

            while(elemI < nElems)
            {
                vals[elemI++] = readIntList(context);
            }
            readClose();
        }
        catch(NumberFormatException ex)
        {
            System.out.println
            (
                "NumberFormatException " + ex + " while reading element "
              + elemI + " at line " + st_.lineno() + " of intListList "
              + context
            );
        }
        catch(IOException ex)
        {
            System.out.println
            (
                "IOException " + ex + " while reading element "
              + elemI + " at line " + st_.lineno() + " of intListList "
              + context
            );
        }
        return vals;
    }
        
    //--------------------------------------------------------------------------
    // Utility

    //--------------------------------------------------------------------------

    /**
     *  Main method, only used when running as an application.
     */
    public static void main(String[] args)
    {
        try
        {
            FileInputStream inStream = new FileInputStream
            (
                new File("scores.foam")
            );
            Reader read = new BufferedReader(new InputStreamReader(inStream));

            FoamDataParser foamParser = new FoamDataParser(read);

            foamParser.skipHeader();

            Point3f[] vals = foamParser.readPoint3fList("triPoints");

            for(int i = 0; i < vals.length; i++)
            {
                System.out.println("(" + vals[i] + ")");
            }
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
