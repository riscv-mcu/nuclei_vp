CPU = {
    library = "libqbox-riscv32-softmmu.so",
    extra_arguments = "-machine nuclei_evalsoc,download=flashxip -cpu nuclei-n900fd -nographic -smp 1 -m 1024m  -kernel images/rv32_bare.elf -nographic -nodefaults -serial stdio"
}

ram = {
    size = 2097152,
    target_port = {
        base_addr = 0x80000000,
        high_addr = 0xffffffff
    }
}


