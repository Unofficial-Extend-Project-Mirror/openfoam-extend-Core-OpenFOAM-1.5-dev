#!/bin/bash -f
#------------------------------------------------------------------------------
# =========                 |
# \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
#  \\    /   O peration     |
#   \\  /    A nd           | Copyright held by original author
#    \\/     M anipulation  |
#------------------------------------------------------------------------------
# License
#     This file is part of OpenFOAM.
#
#     OpenFOAM is free software; you can redistribute it and/or modify it
#     under the terms of the GNU General Public License as published by the
#     Free Software Foundation; either version 2 of the License, or (at your
#     option) any later version.
#
#     OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
#     ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#     FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#     for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with OpenFOAM; if not, write to the Free Software Foundation,
#     Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
#
# Script
#     .bashrc
#
# Description
#     Startup File for OpenFoam.
#     Sourced from .OpenFoam-??/bashrc
#
#------------------------------------------------------------------------------

if [ "$PS1" -a "$foamDotFile" ]; then
    if [ ! "$FOAM_SILENT" ]; then
        echo "Executing: $foamDotFile"
    fi
fi

AddPath(){
    export PATH=$1:$PATH
    if [ ! -d $1 ]; then
        mkdir -p $1
    fi
}

AddLib(){
    if [ $WM_ARCH == "darwin" ]	    
    then
	export DYLD_LIBRARY_PATH=$1:$DYLD_LIBRARY_PATH
    fi
    export LD_LIBRARY_PATH=$1:$LD_LIBRARY_PATH
    if [ ! -d $1 ]; then
        mkdir -p $1
    fi
}

#- Add the system-specifc executables path to the path
export PATH=${WM_PROJECT_DIR}/bin:$WM_PROJECT_INST_DIR/$WM_ARCH/bin:$PATH

#- Location of the jobControl directory
export FOAM_JOB_DIR=$WM_PROJECT_INST_DIR/jobControl

export WM_DIR=$WM_PROJECT_DIR/wmake
export WM_LINK_LANGUAGE=c++
export WM_OPTIONS=$WM_ARCH${WM_COMPILER}$WM_PRECISION_OPTION$WM_COMPILE_OPTION

export PATH=$WM_DIR:$PATH

#export WM_DECOMP_INC=-DCELL_DECOMP
#export WM_DECOMP_LIBS="-lcellDecompFiniteElement -lcellDecompositionMotionSolver"
export WM_DECOMP_INC=-DFACE_DECOMP
export WM_DECOMP_LIBS="-lfaceDecompFiniteElement -lfaceDecompositionMotionSolver"

export FOAM_DOT_DIR=.$WM_PROJECT-$WM_PROJECT_VERSION

export FOAM_SRC=$WM_PROJECT_DIR/src
export FOAM_LIB=$WM_PROJECT_DIR/lib
export FOAM_LIBBIN=$FOAM_LIB/$WM_OPTIONS
AddLib $FOAM_LIBBIN

export FOAM_APP=$WM_PROJECT_DIR/applications
export FOAM_APPBIN=$WM_PROJECT_DIR/applications/bin/$WM_OPTIONS
AddPath $FOAM_APPBIN

export FOAM_TUTORIALS=$WM_PROJECT_DIR/tutorials
export FOAM_UTILITIES=$FOAM_APP/utilities
export FOAM_SOLVERS=$FOAM_APP/solvers

export FOAM_USER_LIBBIN=$WM_PROJECT_USER_DIR/lib/$WM_OPTIONS
AddLib $FOAM_USER_LIBBIN

export FOAM_USER_APPBIN=$WM_PROJECT_USER_DIR/applications/bin/$WM_OPTIONS
AddPath $FOAM_USER_APPBIN

export FOAM_RUN=$WM_PROJECT_USER_DIR/run


# Detect system type and set environment variables
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
machineTest=`uname -s`


WM_COMPILER_BIN=
WM_COMPILER_LIB=

if [ "$WM_COMPILER" = "Gcc" -o "$machineTest" = "Linux" -a "$WM_COMPILER" = "" ]; then
    export WM_COMPILER_DIR=$WM_PROJECT_INST_DIR/$WM_ARCH/gcc-4.2.4$WM_COMPILER_ARCH
    WM_COMPILER_BIN="$WM_COMPILER_DIR/bin:$WM_COMPILER_DIR/../gdb-6.4/bin"
    WM_COMPILER_LIB=$WM_COMPILER_DIR/lib${WM_COMPILER_LIB_ARCH}:$WM_COMPILER_DIR/lib:
fi

if [ "$WM_COMPILER_BIN" != "" ]; then
    export LD_LIBRARY_PATH=$WM_COMPILER_LIB:$LD_LIBRARY_PATH
    export PATH=$WM_COMPILER_BIN:$PATH
fi


# Java
# ~~~~

# Linux workstation
# ~~~~~~~~~~~~~~~~~
if [ "$machineTest" = "Linux" ]; then

#    export JAVA_HOME=$WM_PROJECT_INST_DIR/$WM_ARCH/j2sdk1.4.2_05
    export JAVA_HOME=/usr/lib/jvm/java-1.5.0-sun

    processor=`uname -m`

    if [ "$processor" = "ia64" ]; then
        export JAVA_HOME=/usr/java/j2sdk1.4.2
    fi

# SUN workstation 
# ~~~~~~~~~~~~~~~
elif [ "$machineTest" = "SunOS" ]; then

    export JAVA_HOME=$WM_PROJECT_INST_DIR/$WM_ARCH/j2re1.4.2_05

# Silicon Graphics workstation
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
elif [ "$machineTest" = "IRIX" -o "$machineTest" = "IRIX64" ]; then

    export JAVA_HOME=$WM_PROJECT_INST_DIR/$WM_ARCH/j2re1.4.1

# IBM RS6000 workstation
# ~~~~~~~~~~~~~~~~~~~~~~
elif [ "$machineTest" = "AIX" ]; then

    export JAVA_HOME=/usr/java14/jre

# DEC Alpha workstation
# ~~~~~~~~~~~~~~~~~~~~~
elif [ "$machineTest" = "OSF1" ]; then

    export JAVA_HOME=/opt/java1.4

# HP workstation
# ~~~~~~~~~~~~~~
elif [ "$machineTest" = "HP-UX" ]; then

    export JAVA_HOME=/opt/java1.4

# Mac OS X 10.4 Darwin
# ~~~~~~~~~~~~~~~~~~~~
elif [ "$machineTest" = "Darwin" ]; then

    export JAVA_HOME=/System/Library/Frameworks/JavaVM.framework/Versions/1.4.2/Home
    export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH
    export MACOSX_DEPLOYMENT_TARGET=10.4

# Mac OS X 10.4 Darwin
# ~~~~~~~~~~~~~~~~~~~~
elif [ "$machineTest" = "Darwin" ]; then

    export JAVA_HOME=/System/Library/Frameworks/JavaVM.framework/Versions/1.4.2/Home

# An unsupported operating system
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

else

    echo \
        Your $machineTest operating system is not supported\
        by this release of OpenFOAM. For further assistance,\
        please contact www.openfoam.org.
    echo

fi

export JAVA_ROOT=$JAVA_HOME
export JAVA_PATH=$JAVA_HOME
export PATH=$JAVA_PATH/bin:$PATH
export CLASSPATH=.


# MICO
# ~~~~
export MICO_VERSION=2.3.12
export MICO_PATH=$FOAM_SRC/mico-$MICO_VERSION
export MICO_ARCH_PATH=$MICO_PATH/platforms/$WM_OPTIONS
#if [ $WM_ARCH == "darwin" ]	    
#then
#    export MICO_ARCH_PATH=/opt/local
#fi
export PATH=$MICO_ARCH_PATH/bin:$PATH

# FoamX
# ~~~~~
export FOAMX_PATH=$FOAM_UTILITIES/preProcessing/FoamX
export FOAMX_CONFIG=$HOME/$FOAM_DOT_DIR/apps/FoamX
if [ ! -d $FOAMX_CONFIG ]; then
    export FOAMX_CONFIG=$WM_PROJECT_DIR/$FOAM_DOT_DIR/apps/FoamX
fi


# Communications library
# ~~~~~~~~~~~~~~~~~~~~~~

if [ .$WM_MPLIB = .OPENMPI ]; then

    export OPENMPI_VERSION=1.2.6
    export OPENMPI_HOME=$FOAM_SRC/openmpi-$OPENMPI_VERSION
    export OPENMPI_ARCH_PATH=$OPENMPI_HOME/platforms/$WM_OPTIONS

    # Tell OpenMPI where to find it's install directory
    export OPAL_PREFIX=$OPENMPI_ARCH_PATH

    AddLib $OPENMPI_ARCH_PATH/lib
    AddPath $OPENMPI_ARCH_PATH/bin

    export FOAM_MPI_LIBBIN=$FOAM_LIBBIN/openmpi-$OPENMPI_VERSION

elif [ .$WM_MPLIB = .LAM ]; then

    export LAM_VERSION=7.1.4
    export LAMHOME=$FOAM_SRC/lam-$LAM_VERSION
    export LAM_ARCH_PATH=$LAMHOME/platforms/$WM_OPTIONS

    AddLib $LAM_ARCH_PATH/lib
    AddPath $LAM_ARCH_PATH/bin

    export FOAM_MPI_LIBBIN=$FOAM_LIBBIN/lam-$LAM_VERSION

elif [ .$WM_MPLIB = .MPICH ]; then

    export MPICH_VERSION=1.2.4
    export MPICH_PATH=$FOAM_SRC/mpich-$MPICH_VERSION
    export MPICH_ARCH_PATH=$MPICH_PATH/platforms/$WM_OPTIONS
    export MPICH_ROOT=$MPICH_ARCH_PATH

    AddLib $MPICH_ARCH_PATH/lib
    AddPath $MPICH_ARCH_PATH/bin

    export FOAM_MPI_LIBBIN=$FOAM_LIBBIN/mpich-$MPICH_VERSION

elif [ .$WM_MPLIB = .MPICH-GM ]; then

    export MPICH_PATH=/opt/mpi
    export MPICH_ARCH_PATH=$MPICH_PATH
    export MPICH_ROOT=$MPICH_ARCH_PATH
    export GM_LIB_PATH=/opt/gm/lib64

    AddLib $MPICH_ARCH_PATH/lib
    AddLib $GM_LIB_PATH
    AddPath $MPICH_ARCH_PATH/bin

    export FOAM_MPI_LIBBIN=$FOAM_LIBBIN/mpich-gm

elif [ .$WM_MPLIB = .GAMMA ]; then

    export GAMMA_ARCH_PATH=/usr

    #AddLib $GAMMA_ARCH_PATH/lib
    #AddPath $GAMMA_ARCH_PATH/bin

    export FOAM_MPI_LIBBIN=$FOAM_LIBBIN/gamma

elif [ .$WM_MPLIB = .MPIGAMMA ]; then

    export GAMMA_ARCH_PATH=/usr
    export MPIGAMMA_ARCH_PATH=/usr/local/mpigamma

    #AddLib $MPIGAMMA_ARCH_PATH/lib
    #AddPath $MPIGAMMA_ARCH_PATH/bin

    export FOAM_MPI_LIBBIN=$FOAM_LIBBIN/mpigamma

elif [ .$WM_MPLIB = .MPI ]; then

    export FOAM_MPI_LIBBIN=$FOAM_LIBBIN/mpi

else
    export FOAM_MPI_LIBBIN=$FOAM_LIBBIN/dummy
fi

AddLib $FOAM_MPI_LIBBIN


# Set the MPI buffer size (used by all platforms except SGI MPI)
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
export MPI_BUFFER_SIZE=20000000


# CGAL library if available
# ~~~~~~~~~~~~~~~~~~~~~~~~~
if [ "$CGAL_LIB_DIR" ]; then
    AddLib $CGAL_LIB_DIR
fi


# -----------------------------------------------------------------------------
