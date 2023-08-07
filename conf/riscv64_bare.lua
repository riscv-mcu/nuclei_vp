CPU = {
    library = "libqbox-riscv64-softmmu.so",
    extra_arguments = "-machine nuclei_evalsoc,download=flashxip -cpu nuclei-nx900fd -nographic -smp 1 -m 1024m  -kernel images/baremetal.elf -nographic -nodefaults -serial stdio"
}

ram = {
    size = 2097152,
    target_port = {
        base_addr = 0x80000000,
        high_addr = 0xffffffff
    }
}


