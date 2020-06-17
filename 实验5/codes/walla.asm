BITS 16
global _start
extern main
_start:
    call dword main
    retf
cls:
    mov ax, 0x3
    int 0xa
    o32 ret