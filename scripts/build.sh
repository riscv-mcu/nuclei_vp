#! /bin/bash

BUILDDIR=${1:-vp.build}

mkdir -p $BUILDDIR

BUILDDIR=$(realpath $BUILDDIR)

SCRIPTDIR=$(dirname $(realpath $0))
VPSRCDIR=$(realpath $SCRIPTDIR/..)

if [ -d $VPSRCDIR/libs/systemc ] ; then
    syscbuild=$BUILDDIR/libs/systemc.build
    if [ ! -f $syscbuild/STAMP ] ; then
        echo "Build and install systemc library"
        mkdir -p $syscbuild/install
        pushd $syscbuild
        $VPSRCDIR/libs/systemc/configure --prefix=$syscbuild/install
        make -j
        make -j install
        touch $syscbuild/STAMP
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
make | tee make.log
make install
popd
