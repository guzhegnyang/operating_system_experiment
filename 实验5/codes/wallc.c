#include "lib.h"
char str[CHSPSZ];
int main()
{
    __asm__(
        "int $0x22\n");
    __asm__(
        "    mov  $0x3, %ax\n"
        "    int  $0xa\n");
    _cls();
    _get();
}