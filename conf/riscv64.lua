CPU = {
    library = "libqbox-riscv64-softmmu.so",
    extra_arguments = "-machine nuclei_evalsoc,download=flashxip -cpu nuclei-ux900fd -nographic -smp 8 -m 2048  -bios images/freeloader.elf -nographic -drive file=images/disk.img,if=sd,format=raw"
}

ram = {
    size = 2097152,
    target_port = {
        base_addr = 0x80000000,
        high_addr = 0xffffffff
    }
}


