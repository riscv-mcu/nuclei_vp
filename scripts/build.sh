#! /bin/bash

BUILDDIR=${1:-vp.build}
V=${V:-0}
JOBS=${JOBS:-8}

mkdir -p $BUILDDIR

if [ "x$V" == "x1" ] ; then
    export VERBOSE=1
fi

BUILDDIR=$(realpath $BUILDDIR)

SCRIPTDIR=$(dirname $(realpath $0))
VPSRCDIR=$(realpath $SCRIPTDIR/..)

if [ -d $VPSRCDIR/libs/systemc ] ; then
    syscbuild=$BUILDDIR/libs/systemc.build
    systemcver=$(head -1 $VPSRCDIR/libs/systemc/RELEASENOTES | sed 's/.*SystemC //')
    if [ ! -f $syscbuild/STAMP ] ; then
        echo "Build and install systemc $systemcver library"
        mkdir -p $syscbuild/install
        pushd $syscbuild
        if [ -f $VPSRCDIR/libs/systemc/cmake/INSTALL_USING_CMAKE ] ; then
            echo "SystemC $systemcver configured using cmake"
            cmake -DCMAKE_INSTALL_PREFIX=$syscbuild/install $VPSRCDIR/libs/systemc/
        else
            echo "SystemC $systemcver configured using auto tools"
            $VPSRCDIR/libs/systemc/configure --prefix=$syscbuild/install
        fi
        make -j
        make -j install
        if [ "x$?" == "x0" ] ; then
            echo $systemcver > $syscbuild/STAMP
        else
            echo "Failed to build systemc library"
            exit 1
        fi
        popd
    else
        echo "SystemC and TLM library is built already, no need to rebuild it"
    fi
fi

mkdir -p $BUILDDIR
pushd $BUILDDIR
if [ ! -f $BUILDDIR/Makefile ] ; then
    echo "Configure Nuclei Virtual Platform..."
    cmake -DCMAKE_INSTALL_PREFIX=install -DSYSTEMC_PREFIX=$syscbuild/install -DCMAKE_BUILD_TYPE=Debug $VPSRCDIR
else
    echo "Project is already configured!"
fi
make -j$JOBS | tee make.log
make install
popd
