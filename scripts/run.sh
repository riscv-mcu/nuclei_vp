#! /bin/bash
BUILDDIR=${1:-vp.build}

export SC_SIGNAL_WRITE_CHECK=DISABLE
./$BUILDDIR/install/bin/riscv64_toplevel -c conf/riscv64.lua
