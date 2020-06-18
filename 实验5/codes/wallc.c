#include "lib.h"
char str[CHSPSZ];
int main()
{
    _cls();
    init();
    scanf(0, 0, 0x0007, "%s", str);
    printf(12, 20, 0x0007, "%s", str);
    __asm__(
        "int $0x22\n");
    _get();
}