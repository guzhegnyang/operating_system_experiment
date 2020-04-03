#include <stdio.h>
void Init_ramFDD144(unsigned char *ramFDD144, const char *path)
{
    FILE *fp;
    fp = fopen(path, "rb");
    fread(ramFDD144, 1, 1474560, fp);
    fclose(fp);
} /*
void Read_ramFDD_Block(unsigned char *ramFDD_Block, const unsigned char *ramFDD144, size_t offset)
{
    ramFDD144 += offset * 512;
    for (offset = 0; offset < 512; offset++)
    {
        ramFDD_Block[offset] = ramFDD144[offset];
    }
}
void Write_ramFDD_Block(const unsigned char *ramFDD_Block, unsigned char *ramFDD144, size_t offset)
{
    ramFDD144 += offset * 512;
    for (offset = 0; offset < 512; offset++)
    {
        ramFDD144[offset] = ramFDD_Block[offset];
    }
}*/
void Write_ramFDD144(unsigned char *ramFDD144, const char *path)
{
    FILE *fp;
    fp = fopen(path, "wb");
    fwrite(ramFDD144, 1, 1474560, fp);
    fclose(fp);
}