.text
.code 16

.align 2
.global asm_BitUnPack
.thumb_func
asm_BitUnPack:
    swi 0x10
    bx lr
