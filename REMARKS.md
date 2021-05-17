### 3.0 gcc 9.0 or older version
gcc 10.0 will bring errors due to stricter rules for type convertion.

### 3.3 ParaView Linux Ubuntu 18.04:
Also works with ParaView 5.9.1:

https://github.com/Kitware/ParaView/tree/v5.9.1

0) dependencies:

a)

```bash
    sudo apt-get install git cmake build-essential libgl1-mesa-dev libxt-dev qt5-default libqt5x11extras5-dev libqt5help5 qttools5-dev qtxmlpatterns5-dev-tools libqt5svg5-dev python3-dev python3-numpy libopenmpi-dev libtbb-dev ninja-build
```

b) Qt 5.12 or newer is required if PARAVIEW_USE_QT=ON. Qt5.12.10 can be downloaded from:
    https://download.qt.io/official_releases/qt/5.12/5.12.10/qt-opensource-linux-x64-5.12.10.run

c) CMake 3.12 or newer is recommended

d) [Adios2](https://github.com/ornladios/ADIOS2) is recommended:

```bash
    git clone https://github.com/ornladios/ADIOS2.git
    mkdir adios2-build && cd adios2-build
    ccmake ../ADIOS2
```
 
Recommended CMake Options:

```bash
    ADIOS2_USE_MPI=ON
    ADIOS2_USE_Fortran=ON
    CMAKE_INSTALL_PREFIX=/path/to/install
```

Then compile and install with:

```bash
    make
    make install
```

1) build ParaView 5.9.1 from source code:


```bash
    git clone https://github.com/Kitware/ParaView.git
    mkdir paraview_build
    cd ParaView
    git checkout v5.9.1
    git submodule update --init --recursive
    cd ../paraview_build

    export OSMESA=/path/to/osmesa

    export LIBDIR=$OSMESA/lib:$LIBDIR
    export LD_LIBRARY_PATH=$OSMESA/lib:$LD_LIBRARY_PATH
    export LD_RUN_PATH=$OSMESA/lib:$LD_RUN_PATH

    export OSMESA_INCLUDE_DIR=$OSMESA/include
    export OSMESA_LIBRARY=$OSMESA/lib
    ccmake ../Paraview
```

2) Recommended CMake Options:

```bash
    CMAKE_BUILD_TYPE=Release
    CMAKE_INSTALL_PREFIX=/path/to/install
    PARAVIEW_ENABLE_ADIOS2=ON
    ADIOS2_DIR=/path/to/adios2
    PARAVIEW_BUILD_SHARED_LIBS=ON
    PARAVIEW_USE_FORTRAN=ON
    PARAVIEW_USE_MPI=ON
    PARAVIEW_USE_PYTHON=ON
    VTK_USE_X=OFF
    VTK_OPENGL_HAS_OSMESA=ON
```

3)Then compile and install with:

```bash
    make
    make install
```

