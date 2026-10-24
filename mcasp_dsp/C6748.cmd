
/* C6748 LCDK linker command file */

-stack 0x4000
-heap  0x4000

MEMORY
{
    L2RAM  (RWX) : origin = 0x11800000, length = 0x00040000
    L1PRAM (RWX) : origin = 0x11E00000, length = 0x00008000
    L1DRAM (RW)  : origin = 0x11F00000, length = 0x00008000
    DDR2   (RWX) : origin = 0xC0000000, length = 0x08000000
}

SECTIONS
{
    .text          > DDR2
    .cinit         > DDR2
    .pinit         > DDR2
    .binit         > DDR2
    .init_array    > DDR2
    .const         > DDR2
    .switch        > DDR2
    .rodata        > DDR2
    .neardata      > DDR2
    .fardata       > DDR2
    .far           > DDR2
    .bss           > DDR2
    .data          > DDR2
    .sysmem        > DDR2
    .cio           > DDR2
    .args          > DDR2
    .stack         > DDR2
    .ppinfo        > DDR2
    .ppdata        > DDR2
    .c6xabi.exidx  > DDR2
    .c6xabi.extab  > DDR2
    .vecs          > DDR2
}