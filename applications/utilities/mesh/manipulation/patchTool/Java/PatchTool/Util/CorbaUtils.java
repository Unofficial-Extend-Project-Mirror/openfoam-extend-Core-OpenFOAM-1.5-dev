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

package PatchTool.Util;

import java.awt.*;
import java.awt.event.*;
import java.awt.Window;

import java.beans.*;
import java.io.*;
import java.net.URL;
import java.util.*;

import javax.swing.*;
import javax.swing.event.*;

import org.omg.CosNaming.*;

import PatchTool.PatchTool;

import FoamXServer.FoamXIOError;
import FoamXServer.FoamXError;
import FoamXServer.FoamXSYSError;

import FoamXServer.PatchToolServer.IPatchToolServerHelper;
import FoamXServer.PatchToolServer.IPatchToolServer;

public class CorbaUtils
{
    public static final int DEBUGLEVEL_ERROR   = 0;
    public static final int DEBUGLEVEL_NORMAL  = 1;
    public static final int DEBUGLEVEL_VERBOSE = 2;
    public static final int DEBUGLEVEL_DEBUG   = 3;

    //--------------------------------------------------------------------------

    /** Do all. Get the preprocessor server */
    public static IPatchToolServer getPatchToolServer
    (
        String[] args,
        OptionsManager options
    )
    {
        IPatchToolServer patchToolServer_ = null;

        try
        {
            // Use Sun jdk Corba. Instead of command line arguments.
            //options.put("org.omg.CORBA.ORBClass", "com.sun.CORBA.iiop.ORB");
            //options.put("org.omg.CORBA.ORBInitialHost", "penfold");
            //options.put("org.omg.CORBA.ORBInitialPort", "1234");

            // Create th'orb.
            org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init
            (
                args,
                options
            );

            // Get the root naming context from the naming service.
            NamingContextExt     rootContext = null;
            org.omg.CORBA.Object nsObj       = null;

            try
            {
                // Try resolve_initial_references first.
                nsObj = orb.resolve_initial_references("NameService");

                // Print nice status message.
                CorbaUtils.printMessage
                (
                    CorbaUtils.DEBUGLEVEL_VERBOSE,
                    "Connecting to name service "
                  + orb.toString()
                );
            }
            catch (Exception ex)
            {
                // The resolve_initial_references call didn't work so try
                // and read the name service IOR written to the ns2.ref file
                // in the FoamX user directory.
                String userPath = options.foamXUserConfigPath();

                if (userPath != null)
                {
                    String nsFileName = userPath + "/ns2.ref";

                    // Print nice status message.
                    CorbaUtils.printMessage
                    (
                        CorbaUtils.DEBUGLEVEL_VERBOSE,
                        "Reading name service reference from '"
                      + nsFileName + "'."
                    );

                    // Check that the nameservice reference file exists.
                    File nsFile = new File(nsFileName);
                    if (nsFile.exists())
                    {
                        BufferedReader br = new BufferedReader
                        (
                            new FileReader(nsFileName)
                        );
                        String ior = br.readLine();
                        nsObj = orb.string_to_object(ior);

                        // Print nice status message.
                        CorbaUtils.printMessage
                        (
                            CorbaUtils.DEBUGLEVEL_DEBUG,
                            "Connecting to name service."
                        );
                    }
                    else
                    {
                        // Print error message.
                        CorbaUtils.printMessage
                        (
                            CorbaUtils.DEBUGLEVEL_ERROR,
                            "Name service reference file not found in '"
                          + nsFileName + "'."
                        );
                    }
                }
            }

            try
            {
                rootContext = NamingContextExtHelper.narrow(nsObj);

                if (rootContext == null || rootContext._non_existent())
                {
                    // Print error message.
                    CorbaUtils.printMessage
                    (
                        CorbaUtils.DEBUGLEVEL_ERROR,
                        "Cannot find rootContext in name service."
                    );

                    rootContext = null;
                }
                else
                {
                    // Print success message.
                    CorbaUtils.printMessage
                    (
                        CorbaUtils.DEBUGLEVEL_NORMAL,
                        "Successfully connected to name service."
                    );
                }
            }
            catch (Exception ex)
            {
                // Print error message.
                CorbaUtils.printMessage
                (
                    CorbaUtils.DEBUGLEVEL_ERROR,
                    "Default name service could not be contacted."
                );
            }


            // Get a reference to the host browser.
            try
            {
                NameComponent[] name = new NameComponent[1];
                name[0] = new NameComponent("PatchToolServer", "");

                org.omg.CORBA.Object obj = rootContext.resolve(name);

                patchToolServer_ = IPatchToolServerHelper.narrow(obj);
                if 
                (
                    patchToolServer_ == null
                 || patchToolServer_._non_existent()
                )
                {
                    patchToolServer_ = null;

                    // Print error message.
                    CorbaUtils.printMessage
                    (
                        CorbaUtils.DEBUGLEVEL_ERROR,
                        "Cannot find PatchToolServer in name service."
                    );
                }
            }
            catch (Exception ex)
            {
                // Print error message.
                CorbaUtils.printMessage
                (
                    CorbaUtils.DEBUGLEVEL_ERROR,
                    "PatchToolServer object could not be contacted."
                );
            }
        }
        catch (Exception ex)
        {
            // Print error message.
            CorbaUtils.printMessage
            (
                CorbaUtils.DEBUGLEVEL_ERROR,
                "Could not initialize Orb."
            );
        }

        return patchToolServer_;
    }

    //--------------------------------------------------------------------------

    /** Handle a message. For now just print */
    public static void printMessage(int level, String message)
    {
        System.out.println(message);
    }
}


//------------------------------------------------------------------------------
