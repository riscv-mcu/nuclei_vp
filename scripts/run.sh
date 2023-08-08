#! /bin/bash
BUILDDIR=${1:-vp.build}
CONF=${2:-conf/riscv64.lua}

export SC_SIGNAL_WRITE_CHECK=DISABLE
# set extra library load path
export LD_LIBRARY_PATH=$(pwd)/$BUILDDIR/install/lib:$LD_LIBRARY_PATH

if [ ! -f $CONF ] ; then
    echo "Configuration file $CONF not exist, please check!"
    exit 1
fi
echo "Using configuration file $CONF"
./$BUILDDIR/install/bin/riscv_toplevel -c $CONF
