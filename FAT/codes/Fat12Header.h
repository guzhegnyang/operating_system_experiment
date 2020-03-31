struct Fatl2Header
{
    char BS_OEMName[8];
    unsigned short BPB_BytsPerSec;
    unsigned char BPB_SecPerClus;
    unsigned short BPB_RsvdSecCnt;
    unsigned char BPB_NumFATs;
    unsigned short BPB_RootEntCnt;
    unsigned short BPB_TotSecl6;
    unsigned char BPB_Media;
    unsigned short BPB_FATSzl6;
    unsigned short BPB_SecPerTrk;
    unsigned short BPB_NumHeads;
    unsigned int BPB_HiddSec;
    unsigned int BPB_TotSec32;
    unsigned char BS_DrvNum;
    unsigned char BS_Reservedl;
    unsigned char BS_BootSig;
    unsigned int BS_VolID;
    char BS_VolLab[11];
    char BS_FileSysType[8];
};
int check()
{
    
}