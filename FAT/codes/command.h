#include "Fat12Header.h"
#include "FatItem.h"
#include "RootEntry.h"
#include "ramFDD.h"
void read_command(char command[], char parameter[])
{
    int i;
    char ch;
    while ((ch = getchar()) == ' ')
        ;
    for (i = 0; ch != ' '; i++)
    {
        if (ch == '\n')
        {
            command[i] = '\0';
            parameter[0] = '\0';
            return;
        }
        if (ch >= 'a' && ch <= 'z')
        {
            command[i] = ch - 'a' + 'A';
        }
        else
        {
            command[i] = ch;
        }
        ch = getchar();
    }
    command[i] = '\0';
    while ((ch = getchar()) == ' ')
        ;
    for (i = 0; ch != '\n'; i++)
    {
        if (ch == ' ')
        {
            parameter[i++] = ' ';
            while ((ch = getchar()) == ' ')
                ;
        }
        if (ch >= 'a' && ch <= 'z')
        {
            parameter[i] = ch - 'a' + 'A';
        }
        else
        {
            parameter[i] = ch;
        }
        ch = getchar();
    }
    parameter[i] = '\0';
}
int is_command(const char command1[], const char command2[])
{
    int i;
    for (i = 0; command1[i]; i++)
    {
        if (command1[i] != command2[i])
        {
            return 0;
        }
    }
    return command2[i] == 0;
}
char *split(char path[])
{
    int i;
    for (i = 0; path[i]; i++)
        ;
    for (; path[i] != '\\'; i--)
    {
        if (!i)
        {
            return NULL;
        }
    }
    path[i] = '\0';
    return path + i + 1;
}
void execute_command(char command[], char parameter[], unsigned char *cur_ptr,
                     unsigned char mbr[], struct FatItem fat1[], struct FatItem fat2[], struct RootEntry space[], unsigned char data[])
{
    if (is_command(command, "CHECK"))
    {
        if (check(mbr))
        {
            puts("Legal");
        }
        else
        {
            puts("Illegal");
        }
    }
    else if (is_command(command, "DIR"))
    {
        unsigned char temp = locate(parameter, *cur_ptr, space);
        if (temp == Null)
        {
            puts("File not found");
        }
        else
        {
            list_dir(temp, space);
        }
    }
    else if (is_command(command, "CD"))
    {
        unsigned char temp = locate(parameter, *cur_ptr, space);
        if (temp == Null || space[temp].DIR_Attr != TYPE_DIR)
        {
            puts("Invalid directory");
        }
        else
        {
            *cur_ptr = temp;
        }
    }
    else if (is_command(command, "TREE"))
    {
        unsigned char temp = locate(parameter, *cur_ptr, space);
        if (temp == Null)
        {
            puts("File not found");
        }
        else
        {
            dfs(temp, space, 0);
        }
    }
    else if (is_command(command, "MD"))
    {
        char *file_name = split(parameter);
        unsigned char temp;
        if (file_name != NULL)
        {
            temp = locate(parameter, *cur_ptr, space);
            if (temp == Null)
            {
                puts("Invalid path");
            }
            else
            {
                if (space[temp].DIR_Attr != TYPE_DIR)
                {
                    puts("Invalid path");
                }
                unsigned char file;
                if ((file = memory_alloc(space)) == Null)
                {
                    puts("Space used up");
                }
                else
                {

                    assign(file, file_name, TYPE_DIR, temp, space);
                }
            }
        }
        else
        {
            unsigned char file = memory_alloc(space);
            assign(file, parameter, TYPE_DIR, *cur_ptr, space);
        }
    }
    else if (is_command(command, "RD"))
    {
        unsigned char temp = locate(parameter, *cur_ptr, space);
        if (temp == Null)
        {
            puts("Invalid path");
        }
        else
        {
            if (space[temp].DIR_Attr != TYPE_DIR)
            {
                puts("Not directory");
            }
            else if (!memory_delete(temp, space))
            {
                puts("Directory not empty");
            }
        }
    }
}