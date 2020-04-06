#include "fat12Header.h"
#include "file.h"
#include "ramFDD.h"
#include <conio.h>
#define ESC 0x1b
#define LEFT 37
#define UP 38
#define RIGHT 39
#define DOWN 40
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
int check_name(char *name)
{
    int i;
    for (i = 0; name[i]; i++)
    {
        if (name[i] == '.')
        {
            i++;
            continue;
        }
        if (!((name[i] >= '0' && name[i] <= '9') || (name[i] >= 'A' && name[i] <= 'Z')))
        {
            return 0;
        }
    }
    return 1;
}
void execute_command(char command[], char parameter[], unsigned char *cur_ptr,
                     unsigned char mbr[], unsigned char fat1[], unsigned char fat2[], struct RootEntry space[], unsigned char data[], struct ActiveFile active_list[], struct OpenFile open_list[])
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
        unsigned char cur = *cur_ptr;
        if (parameter[0] != '>')
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
            while (*parameter != ' ')
            {
                parameter++;
            }
            parameter++;
        }
        if (parameter[0] == '>' && parameter[1] == ' ')
        {
            char *file_name = split(parameter + 2);
            unsigned char temp;
            if (file_name != NULL)
            {
                temp = locate(parameter + 2, cur, space);
                if (temp == Null || space[temp].DIR_Attr != TYPE_DIR)
                {
                    puts("Invalid path");
                    return;
                }
            }
            else
            {
                file_name = parameter + 2;
                temp = cur;
            }
            if (!check_name(file_name))
            {
                puts("Invalid file name");
                return;
            }
            if (locate(file_name, temp, space) != Null)
            {
                puts("File already exists");
                return;
            }
            unsigned char file;
            if ((file = memory_alloc(space)) == Null)
            {
                puts("Space used up");
            }
            else
            {
                assign(file, file_name, TYPE_NORMAL_FILE, temp, fat1, fat2, space);
            }
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
            if (temp == Null || space[temp].DIR_Attr != TYPE_DIR)
            {
                puts("Invalid path");
                return;
            }
        }
        else
        {
            file_name = parameter;
            temp = *cur_ptr;
        }
        if (!check_name(file_name))
        {
            puts("Invalid directory name");
            return;
        }
        if (locate(file_name, temp, space) != Null)
        {
            puts("Directory already exists");
            return;
        }
        unsigned char file;
        if ((file = memory_alloc(space)) == Null)
        {
            puts("Space used up");
        }
        else
        {
            assign(file, file_name, TYPE_DIR, temp, fat1, fat2, space);
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
    else if (is_command(command, "DEL"))
    {
        unsigned char temp = locate(parameter, *cur_ptr, space);
        if (temp == Null)
        {
            puts("Invalid path");
        }
        else
        {
            if (space[temp].DIR_Attr == TYPE_DIR)
            {
                while (1)
                {
                    puts("All files in directory will be deleted!");
                    puts("Are you sure (Y/N)?");
                    char ch = getchar();
                    while (getchar() != '\n')
                        ;
                    if (ch == 'Y' || ch == 'y')
                    {
                        unsigned char p;
                        for (p = space[temp].last_child; p != Null; p = space[p].last_sibling)
                        {
                            if (space[p].DIR_Attr != TYPE_DIR)
                            {
                                break;
                            }
                        }
                        if (p == Null)
                        {
                            puts("File not found");
                            break;
                        }
                        memory_delete(p, space);
                        item_delete(*(unsigned short *)space[p].DIR_FstClus, fat1, fat2);
                        for (; p != Null; p = space[p].last_sibling)
                        {
                            if (space[p].DIR_Attr != TYPE_DIR)
                            {
                                memory_delete(p, space);
                                item_delete(*(unsigned short *)space[p].DIR_FstClus, fat1, fat2);
                            }
                        }
                        break;
                    }
                    else if (ch == 'N' || ch == 'n')
                    {
                        break;
                    }
                }
            }
            else
            {
                item_delete(*(unsigned short *)space[temp].DIR_FstClus, fat1, fat2);
                memory_delete(temp, space);
            }
        }
    }
    else if (is_command(command, "REN"))
    {
        unsigned char temp = locate(parameter, *cur_ptr, space);
        if (temp == Null)
        {
            puts("File not found");
            return;
        }
        while (*parameter != ' ')
        {
            parameter++;
        }
        parameter++;
        if (!check_name(parameter))
        {
            puts("Invalid file name");
            return;
        }
        store_name(space[temp].DIR_Name, parameter);
    }
    else if (is_command(command, "EDIT"))
    {
        unsigned char temp = locate(parameter, *cur_ptr, space);
        if (temp == Null)
        {
            puts("File not found");
            return;
        }
        struct OpenFile *fp;
        if ((fp = open(temp, 'w', active_list, open_list, fat1, space, data)) == NULL)
        {
            puts("Too much files opend");
            return;
        }
        system("cls");
        read(fp, -1, NULL);
        char ch;
        while ((ch = getch()) != ESC)
        {
            if (ch == '\b')
            {
                erase(fp);
            }
            else if (ch == LEFT)
            {
                seek(fp, -1, fp->posi);
            }
            else if (ch == RIGHT)
            {
                seek(fp, 1, fp->posi);
            }
            else if (ch == UP)
            {
                continue;
            }
            else if (ch == DOWN)
            {
                continue;
            }
            else
            {
                insert(fp, ch);
            }
            system("cls");
            read(fp, -1, NULL);
        }
        if (!close(fp, fat1, fat2, data))
        {
            puts("Fail saving, space used up");
        }
    }
    else
    {
        puts("Bad command or file name");
    }
}