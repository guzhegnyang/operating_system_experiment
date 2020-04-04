void modify_next_item(unsigned short p, unsigned short nxtp, unsigned char fat1[], unsigned char fat2[])
{
    if (!p % 2)
    {
        fat1[(unsigned short)(p * 1.5)] &= 0xF000;
        fat1[(unsigned short)(p * 1.5)] |= nxtp;
        fat2[(unsigned short)(p * 1.5)] &= 0xF000;
        fat2[(unsigned short)(p * 1.5)] |= nxtp;
    }
    else
    {
        fat1[(unsigned short)(p * 1.5)] &= 0x000F;
        fat1[(unsigned short)(p * 1.5)] |= nxtp << 4;
        fat2[(unsigned short)(p * 1.5)] &= 0x000F;
        fat2[(unsigned short)(p * 1.5)] |= nxtp << 4;
    }
}
unsigned short next_item(unsigned short p, unsigned char fat[])
{
    if (!p % 2)
    {
        return fat[(unsigned short)(p * 1.5)] & 0x0FFF;
    }
    else
    {
        return fat[(unsigned short)(p * 1.5)] >> 4;
    }
}
unsigned short top = 0xFFF;
unsigned short item_alloc(unsigned char fat1[], unsigned char fat2[])
{
    unsigned short p;
    if (top == 0xFFF)
    {
        p = top;
        top = next_item(top, fat1);
        modify_next_item(p, 0xFFF, fat1, fat2);
        return p;
    }
    for (p = 2; next_item(p, fat1); p = next_item(p, fat1))
        ;
    modify_next_item(p, 0xFFF, fat1, fat2);
    return p;
}
void item_delete(unsigned short DIR_FstClus, unsigned char fat1[], unsigned char fat2[])
{
    unsigned short p = DIR_FstClus, nxtp;
    p = next_item(DIR_FstClus, fat1);
    if (p == 0xFFF)
    {
        return;
    }
    nxtp = next_item(p, fat1);
    while (nxtp != 0xFFF)
    {
        p = nxtp;
        nxtp = next_item(nxtp, fat1);
    }
    modify_next_item(p, top, fat1, fat2);
    top = next_item(DIR_FstClus, fat1);
}