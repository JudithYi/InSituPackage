# In-situ analysis in Catalyst and Nek5000 with Mesa

In this repository, we describe a possible implementation of an in-situ adaptor for Nek5000 and ParaView-Catalyst, as well the building process of all its components and an  *almost realistic* test case. 

We are not using this repository for active development. We suggest considering it as a snapshot of a particular state of our code that we consider relevant.

The present document is organized in 4 sections: 1) Funding and Contributors 2) Description of the in-situ adaptor and the interface in *Nek5000*, 3) Building instructions and 4) Description of the test case.

## 1. Funding and Contributors

The development of this code is part of the effort to provide new data-analysis tools for numerical simulations undergone by the *In-Situ Big Data Analysis for Flow and Climate Simulations* consortium, funded by the Swedish Foundation of Strategic Research.

It was possible thanks to the following contributors:

1. Mohammad Rezai and Niclas Jansson, who developed the adaptor between the numerical code *Nek5000* and the software for data analysis and visualization *Paraview*, and the building procedure.
2. Anke Friederici, Wiebke Köpp and Prof. Tino Weinkauf, who worked on the python pipeline and the timers for performace analysis.
3. Marco Atzori, who provided the test case and composed this guide.
4. Prof. Erwin Laure, Prof. Philipp Schlatter and Prof. Tino Weinkauf, who supervised the work.

## 1. In-situ adaptor

The in-situ adaptor includes both the code that converts the data structures from Nek5000 to VTK format, and the interface between Paraview/Catalyst and Nek5000.

The most relevant parts of the code that we developped is contained in the following files:

### *Nek5000/core/3rd_party/nek_in_situ.f*

This file is part of Nek5000. It is modified to include the three subroutines *catalyst_init*, *catalyst_process*, and *catalyst_end* in the corresponding default subroutines for in-situ operations in Nek5000. Note that this is also the file that includes the subroutines for in-situ operation with the visualization software visit. 

### *Nek5000/core/3rd_party/catalyst.f*

This file is not part of Nek5000. It contains the definition of the subroutines *catalyst_init*, *catalyst_process* and *catalyst_end*, which employ default Catalyst functions (such as *requestdatadescription*) as well the ones that we developed for mapping Nek fields in VTK format (such as *creategrid* and *add_scalar_field*). Note that this file also contains timers, which, at present, write a separate record for each MPI rank.

### *Nek5000/core/3rd_party/nek_catalyst.cxx*

This file is not part of Nek5000. It contains the actual adaptor, *i.e.* the functions that map Nek5000 fields in VTK format.

### *Nek5000/core/mkuserfile*

This file is part of Nek5000. It is modified to add the subroutine *catalyst_usrpipe* to the case_name.f, which will be compiled. At present, it only allows for using a single pipeline, with name "pipe.py", located in the working directory.

## 2. Building instructions
These instructions were used to compile Nek5000+ParaView/Catalyst on Ubuntu 18.04 (09/12/2019), with the aim of reproducing a similar build for the HPC system *Beskow* at PDC, Stockholm (Cray XC40).

### Prerequisite

Install packages:

*sudo apt install build-essential cmake-curses-gui llvm mpich libboost-all-dev*

and Python 3.7:
 *sudo apt-get install python3.6*

The source codes of mesa-18.3.3 and ParaView-v5.6.3 are located in *~/InSituPackage*. Binaries file will be placed in *~/InSituPackage/local*.

### Mesa (version 18.3.3):

1) Run in *~/InSituPackage/mesa-18.3.3* the following script:

```bash
export PYTHON=/usr/bin/python3

./configure                                          \
      --enable-opengl --disable-gles1 --disable-gles2   \
      --disable-va --disable-xvmc --disable-vdpau       \
      --enable-shared-glapi                             \
      --with-gallium-drivers=swrast                     \
      --disable-dri --with-dri-drivers=                 \
      --disable-egl --with-egl-platforms= --disable-gbm \
      --disable-glx                                     \
      --disable-osmesa --enable-gallium-osmesa          \
      --enable-shared \
      --enable-texture-float \
      --prefix=/home/marco/InSituPackage/local/
```

**Note:** *prefix* is the path were the bin will be located.

***TODO: add comments about shared/static option***

2) In *~/InSituPackage/mesa-18.3.3*

*make -j*

*make install*

3) export mesa variable, using the script:

```bash
export OSMESA=$HOME/InSituPackage/local

export LIBDIR=$OSMESA/lib:$LIBDIR
export LD_LIBRARY_PATH=$OSMESA/lib:$LD_LIBRARY_PATH
export LD_RUN_PATH=$OSMESA/lib:$LD_RUN_PATH

export OSMESA_INCLUDE_DIR=$OSMESA/include
export OSMESA_LIBRARY=$OSMESA/lib
```

### ParaView (version 5.6.3):

1) Run the following script in *~/InSituPackage/build*:

```bash
cmake \
      -DCMAKE_BUILD_TYPE=Release                                  \
      -DPARAVIEW_INSTALL_DEVELOPMENT_FILES=ON                     \
      -DPARAVIEW_ENABLE_PYTHON=ON                                 \
      -DVTK_PYTHON_VERSION=3                                      \
      -DPARAVIEW_BUILD_QT_GUI=OFF                                 \
      -DPARAVIEW_USE_MPI=ON                                       \
      -DPARAVIEW_ENABLE_CATALYST=ON                               \
      -DVTK_USE_X=OFF                                             \
      -DVTK_OPENGL_HAS_OSMESA=ON                                  \
      -DVTK_USE_OFFSCREEN=OFF                                     \
      -DOSMESA_INCLUDE_DIR=$OSMESA_INCLUDE_DIR                     \
      -DOSMESA_LIBRARY=$OSMESA_LIBRARY/libOSMesa.so                \
      -DPYTHON_INCLUDE_DIR=/usr/include/python3.6m \
      -DPYTHON_LIBRARY=/usr/lib/python3.6/config-3.6m-x86_64-linux-gnu/libpython3.6m.so   \
      -DCMAKE_INSTALL_PREFIX=$HOME/InSituPackage/local      \
      $HOME/InSituPackage/ParaView-v5.6.3
```

**Note:** the last two lines specify the install and the source codes locations, respectively. The forth and third-last lines need to be set according with the system (where Python is installed).

**Note:** in this example, both Mesa and Paraview share the same the path for binaries (*/home/marco/InSituPackage/local*).

2) In *~/InSituPackage/build*
*make -j*

*make install*

### Nek5000 (v17):

1) Before compiling a case with the InSitu implementation, export Python-path enrivoment variables with the script:

```bash

export PARAVIEW=$HOME/InSituPackage/local
export PATH=$PARAVIEW/bin:$PATH
export LD_LIBRARY_PATH=$PARAVIEW/lib:$LD_LIBRARY_PATH

export PYTHONPATH=$PARAVIEW/lib/python3.6/site-packages:$PYTHONPATH
export PYTHONPATH=$PARAVIEW/lib/python3.6/site-packages/paraview/:$PYTHONPATH

```

2) In *makenek* (within the case directory, see below) add:

2.1) a CXX compiler:

```bash
# Fortran/C compiler
FC="mpif77"
CC="mpicc"
CXX="mpic++"

```

2.2) Paraview in the optional compiler flags for the C compiler only:

```bash
FFLAGS="-I./inc_src -g"
CFLAGS="-I./inc_src -I$PARAVIEW/include/paraview-5.6"
```

2.3) Catalyst enviroment variables:

```bash
CATALYST=1
CATALYST_LIBS=`paraview-config --libs vtkPVPythonCatalyst`
CATALYST_INCS=`paraview-config --include vtkPVPythonCatalyst`
```

3) The pipeline has a fixed name, *pipe.py*, and needs to be in the run folder of the simulation.

## 3. Test Case

### Case Description

This repository contains an exemplary Nek5000 simulation with ParaView Catalyst that simulates the flow around a NACA4412 airfoil at $$Re_c=75,000$$. 
The python pipeline computes an isosurface of the $$\lambda_2$$ criterion.

To run the SmallWing case, make the modifications for Nek5000 in the Test/SmallWing directory and run the case with

### Run

```bash
source run.sh
```

### Pipeline

```bash
Time: [120, 320], Resolution: [720, 480],	Data: Velocity Magnitude, Mode: Compute slice without rendering.
Time: [340, 540], Resolution: [1280, 720],	Data: Velocity Magnitude, Mode: Compute slice without rendering.
Time: [560, 760], Resolution: [1920, 1080],	Data: Velocity Magnitude, Mode: Compute slice without rendering.
Time: [780, 980], Resolution: [720, 480],	Data: Velocity Magnitude, Mode: Render slice without saving.
Time: [1000, 1200], Resolution: [1280, 720],	Data: Velocity Magnitude, Mode: Render slice without saving.
Time: [1220, 1420], Resolution: [1920, 1080],	Data: Velocity Magnitude, Mode: Render slice without saving.
Time: [1440, 1640], Resolution: [720, 480],	Data: Velocity Magnitude, Mode: Save slice.
Time: [1660, 1860], Resolution: [1280, 720],	Data: Velocity Magnitude, Mode: Save slice.
Time: [1880, 2080], Resolution: [1920, 1080],	Data: Velocity Magnitude, Mode: Save slice.
Time: [2100, 2300], Resolution: [720, 480],	Data: Velocity Magnitude, Mode: Compute iso surface without rendering.
Time: [2320, 2520], Resolution: [1280, 720],	Data: Velocity Magnitude, Mode: Compute iso surface without rendering.
Time: [2540, 2740], Resolution: [1920, 1080],	Data: Velocity Magnitude, Mode: Compute iso surface without rendering.
Time: [2760, 2960], Resolution: [720, 480],	Data: Velocity Magnitude, Mode: Render iso surface without saving.
Time: [2980, 3180], Resolution: [1280, 720],	Data: Velocity Magnitude, Mode: Render iso surface without saving.
Time: [3200, 3400], Resolution: [1920, 1080],	Data: Velocity Magnitude, Mode: Render iso surface without saving.
Time: [3420, 3620], Resolution: [720, 480],	Data: Velocity Magnitude, Mode: Save iso surface.
Time: [3640, 3840], Resolution: [1280, 720],	Data: Velocity Magnitude, Mode: Save iso surface.
Time: [3860, 4060], Resolution: [1920, 1080],	Data: Velocity Magnitude, Mode: Save iso surface.
Time: [4080, 4280],	Data: All, Mode: Save.
```


