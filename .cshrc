#!/bin/csh -f
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
#     cshrc
#
# Description
#     Startup File for OpenFoam.
#     Sourced from .OpenFoam-??/cshrc
#
#------------------------------------------------------------------------------

if ($?prompt && $?foamDotFile) then
    if (! $?FOAM_SILENT) then
        echo "Executing: $foamDotFile"
    endif
endif

alias AddPath 'set path=(\!* $path) ; if ( ! -d \!* ) mkdir -p \!*'
alias AddLib 'setenv LD_LIBRARY_PATH \!*\:${LD_LIBRARY_PATH} ; if ( ! -d \!* ) mkdir -p \!*'

#- Add the system-specific executables path to the path
set path=($WM_PROJECT_DIR/bin $WM_PROJECT_INST_DIR/$WM_ARCH/bin $path)

#- Location of the jobControl directory
setenv FOAM_JOB_DIR $WM_PROJECT_INST_DIR/jobControl

setenv WM_DIR $WM_PROJECT_DIR/wmake
setenv WM_LINK_LANGUAGE c++
setenv WM_OPTIONS $WM_ARCH$WM_COMPILER$WM_PRECISION_OPTION$WM_COMPILE_OPTION

set path=($WM_DIR $path)

#setenv WM_DECOMP_INC -DCELL_DECOMP
#setenv WM_DECOMP_LIBS "-lcellDecompFiniteElement -lcellDecompositionMotionSolver"
setenv WM_DECOMP_INC -DFACE_DECOMP
setenv WM_DECOMP_LIBS "-lfaceDecompFiniteElement -lfaceDecompositionMotionSolver"

setenv FOAM_DOT_DIR .$WM_PROJECT-$WM_PROJECT_VERSION

setenv FOAM_SRC $WM_PROJECT_DIR/src
setenv FOAM_LIB $WM_PROJECT_DIR/lib
setenv FOAM_LIBBIN $FOAM_LIB/$WM_OPTIONS
AddLib $FOAM_LIBBIN

setenv FOAM_APP $WM_PROJECT_DIR/applications
setenv FOAM_APPBIN $WM_PROJECT_DIR/applications/bin/$WM_OPTIONS
AddPath $FOAM_APPBIN

setenv FOAM_TUTORIALS $WM_PROJECT_DIR/tutorials
setenv FOAM_UTILITIES $FOAM_APP/utilities
setenv FOAM_SOLVERS $FOAM_APP/solvers

setenv FOAM_USER_LIBBIN $WM_PROJECT_USER_DIR/lib/$WM_OPTIONS
AddLib $FOAM_USER_LIBBIN

setenv FOAM_USER_APPBIN $WM_PROJECT_USER_DIR/applications/bin/$WM_OPTIONS
AddPath $FOAM_USER_APPBIN

setenv FOAM_RUN $WM_PROJECT_USER_DIR/run


# Detect system type and set environment variables
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
set machineTest=`uname -s`


set WM_COMPILER_BIN=
set WM_COMPILER_LIB=

if ($WM_COMPILER == "Gcc" || $machineTest == "Linux" && $?WM_COMPILER) then
    setenv WM_COMPILER_DIR $WM_PROJECT_INST_DIR/$WM_ARCH/gcc-4.2.4$WM_COMPILER_ARCH
    set WM_COMPILER_BIN="$WM_COMPILER_DIR/bin $WM_COMPILER_DIR/../gdb-6.4/bin"
    set WM_COMPILER_LIB=$WM_COMPILER_DIR/lib${WM_COMPILER_LIB_ARCH}:$WM_COMPILER_DIR/lib:
endif

if ($?WM_COMPILER_BIN) then
    if ($?LD_LIBRARY_PATH) then
        setenv LD_LIBRARY_PATH ${WM_COMPILER_LIB}:${LD_LIBRARY_PATH}
    else
        setenv LD_LIBRARY_PATH ${WM_COMPILER_LIB}
    endif

    set path=($WM_COMPILER_BIN $path)
endif


# Java
# ~~~~

# Linux workstation
# ~~~~~~~~~~~~~~~~~
if ($machineTest == Linux) then

#    setenv JAVA_HOME $WM_PROJECT_INST_DIR/$WM_ARCH/j2sdk1.4.2_05
    setenv JAVA_HOME /usr/lib/jvm/java-1.5.0-sun

    set processor = `uname -m`

    if ($processor == x86_64) then
	setenv JAVA_HOME /usr/lib64/jvm/java-1.5.0-sun
    else if ($processor == ia64) then
        setenv JAVA_HOME /usr/java/j2sdk1.4.2
    endif

# SUN workstation 
# ~~~~~~~~~~~~~~~
else if ($machineTest == SunOS) then

    setenv JAVA_HOME $WM_PROJECT_INST_DIR/$WM_ARCH/j2re1.4.2_05

# Silicon Graphics workstation
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
else if ($machineTest == IRIX || $machineTest == IRIX64) then

    setenv JAVA_HOME $WM_PROJECT_INST_DIR/$WM_ARCH/j2re1.4.1

# IBM RS6000 workstation
# ~~~~~~~~~~~~~~~~~~~~~~
else if ($machineTest == AIX) then

    setenv JAVA_HOME /usr/java14/jre

# DEC Alpha workstation
# ~~~~~~~~~~~~~~~~~~~~~
else if ($machineTest == OSF1) then

    setenv JAVA_HOME /opt/java1.4

# HP workstation
# ~~~~~~~~~~~~~~
else if ($machineTest == HP-UX) then

    setenv JAVA_HOME /opt/java1.4

# Mac OS X 10.4 Darwin
# ~~~~~~~~~~~~~~~~~~~~
else if ($machineTest == Darwin) then

    setenv JAVA_HOME /System/Library/Frameworks/JavaVM.framework/Versions/1.4.2/Home
    setenv DYLD_LIBRARY_PATH ${LD_LIBRARY_PATH}
    setenv MACOSX_DEPLOYMENT_TARGET 10.4

# An unsupported operating system
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

else

    echo \
        Your $machineTest operating system is not supported\
        by this release of OpenFOAM. For further assistance,\
        please contact www.openfoam.org.
    echo

endif

setenv JAVA_ROOT $JAVA_HOME
setenv JAVA_PATH $JAVA_HOME
set path=($JAVA_PATH/bin $path)
setenv CLASSPATH .


# MICO
# ~~~~
setenv MICO_VERSION 2.3.12
setenv MICO_PATH $FOAM_SRC/mico-$MICO_VERSION
setenv MICO_ARCH_PATH $MICO_PATH/platforms/$WM_OPTIONS
set path=($MICO_ARCH_PATH/bin $path)


# FoamX
# ~~~~~
setenv FOAMX_PATH $FOAM_UTILITIES/preProcessing/FoamX
setenv FOAMX_CONFIG $HOME/$FOAM_DOT_DIR/apps/FoamX
if ( ! -d $FOAMX_CONFIG ) then
    setenv FOAMX_CONFIG $WM_PROJECT_DIR/$FOAM_DOT_DIR/apps/FoamX
endif


# Communications library
# ~~~~~~~~~~~~~~~~~~~~~~

if ($WM_MPLIB == OPENMPI) then

    setenv OPENMPI_VERSION 1.2.6
    setenv OPENMPI_HOME $FOAM_SRC/openmpi-$OPENMPI_VERSION
    setenv OPENMPI_ARCH_PATH $OPENMPI_HOME/platforms/$WM_OPTIONS

    # Tell OpenMPI where to find it's install directory
    setenv OPAL_PREFIX $OPENMPI_ARCH_PATH

    AddLib $OPENMPI_ARCH_PATH/lib
    AddPath $OPENMPI_ARCH_PATH/bin

    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/openmpi-$OPENMPI_VERSION

else if ($WM_MPLIB == LAM) then

    setenv LAM_VERSION 7.1.4
    setenv LAMHOME $FOAM_SRC/lam-$LAM_VERSION
    setenv LAM_ARCH_PATH $LAMHOME/platforms/$WM_OPTIONS

    AddLib $LAM_ARCH_PATH/lib
    AddPath $LAM_ARCH_PATH/bin

    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/lam-$LAM_VERSION

else if ($WM_MPLIB == MPICH) then

    setenv MPICH_VERSION 1.2.4
    setenv MPICH_PATH $FOAM_SRC/mpich-$MPICH_VERSION
    setenv MPICH_ARCH_PATH $MPICH_PATH/platforms/$WM_OPTIONS
    setenv MPICH_ROOT $MPICH_ARCH_PATH

    AddLib $MPICH_ARCH_PATH/lib
    AddPath $MPICH_ARCH_PATH/bin

    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/mpich-$MPICH_VERSION

else if ($WM_MPLIB == MPICH-GM) then

    setenv MPICH_PATH /opt/mpi
    setenv MPICH_ARCH_PATH $MPICH_PATH
    setenv MPICH_ROOT $MPICH_ARCH_PATH
    setenv GM_LIB_PATH /opt/gm/lib64

    AddLib $MPICH_ARCH_PATH/lib
    AddLib $GM_LIB_PATH
    AddPath $MPICH_ARCH_PATH/bin

    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/mpich-gm

else if ($WM_MPLIB == GAMMA) then

    setenv GAMMA_ARCH_PATH /usr

    #AddLib $GAMMA_ARCH_PATH/lib
    #AddPath $GAMMA_ARCH_PATH/bin

    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/gamma

else if ($WM_MPLIB == MPIGAMMA) then

    setenv GAMMA_ARCH_PATH /usr
    setenv MPIGAMMA_ARCH_PATH /usr/local/mpigamma

    #AddLib $MPIGAMMA_ARCH_PATH/lib
    #AddPath $MPIGAMMA_ARCH_PATH/bin

    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/mpigamma

else if ($WM_MPLIB == MPI) then

    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/mpi

else
    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/dummy
endif

AddLib $FOAM_MPI_LIBBIN


# Set the MPI buffer size (used by all platforms except SGI MPI)
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
setenv MPI_BUFFER_SIZE 20000000


# CGAL library if available
# ~~~~~~~~~~~~~~~~~~~~~~~~~
if ( $?CGAL_LIB_DIR ) then
    AddLib $CGAL_LIB_DIR
endif


# -----------------------------------------------------------------------------
