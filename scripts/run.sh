#! /bin/bash
BUILDDIR=${1:-vp.build}

export SC_SIGNAL_WRITE_CHECK=DISABLE
# set extra library load path
export LD_LIBRARY_PATH=$(pwd)/$BUILDDIR/install/lib:$LD_LIBRARY_PATH

./$BUILDDIR/install/bin/riscv64_toplevel -c conf/riscv64.lua
