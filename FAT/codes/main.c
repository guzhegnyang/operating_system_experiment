#include "command.h"
unsigned char ramFDD144[1474560];
unsigned char cur = ROOT;
char command[10];
char parameter[100];
char ch;
int i;
int main()
{
    Init_ramFDD144(ramFDD144, "dossys.img");
    struct FatItem *fat1 = (struct FatItem *)(ramFDD144 + 512);
    struct FatItem *fat2 = (struct FatItem *)(ramFDD144 + 5120);
    struct RootEntry *space = (struct RootEntry *)(ramFDD144 + 9728);
    unsigned char *data = (unsigned char *)(ramFDD144 + 16896);
    demo(space);
    while (1)
    {
        print_path(cur, space);
        putchar('>');
        read_command(command, parameter);
        if (is_command(command, "EXIT")) {
        	break;
        }
        //puts(command);
        //puts(parameter);
        execute_command(command, parameter, &cur, ramFDD144, fat1, fat2, space, data);
    }
}