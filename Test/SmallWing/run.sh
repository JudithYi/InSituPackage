

export OSMESA=/home/yi/software

export LIBDIR=$OSMESA/lib:$LIBDIR
export LD_LIBRARY_PATH=$OSMESA/lib:$LD_LIBRARY_PATH
export LD_RUN_PATH=$OSMESA/lib:$LD_RUN_PATH

export OSMESA_INCLUDE_DIR=$OSMESA/include
export OSMESA_LIBRARY=$OSMESA/lib

export PARAVIEW=/home/yi/paraview_build
export PATH=$PARAVIEW/bin:$PATH
export LD_LIBRARY_PATH=$PARAVIEW/lib:$LD_LIBRARY_PATH

export PYTHONPATH=$PARAVIEW/lib/python3.6/site-packages:$PYTHONPATH
export PYTHONPATH=$PARAVIEW/lib/python3.6/site-packages/paraview/:$PYTHONPATH

./makenek small_wing

mkdir perf
mkdir data
mkdir fig

echo 'small_wing' >> SESSION.NAME
pwd >> SESSION.NAME


#mpirun -n 16 ./nek5000
