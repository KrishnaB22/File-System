#include"bitmap.h"

void clear_bit(char *buf,int num)
{
    int i,j;
    i = num  /8;
    j = num % 8;
    //printf("i = %d -- j =%d\n",i,j);
    if (j == 0)
    {
        i--;
        buf[i] = 0x00;
        return;
    }
    buf[i] = buf[i] & ~(1<<(8-j));
}

void set_bit(char *buf,int num)
{
    int i,j;
    i = num  / 8;
    j = num % 8;
    //printf("i = %d -- j =%d\n",i,j);
    if (j == 0)
    {
        i--;
        buf[i] = buf[i] | 1;
        return;
    }
    buf[i] = buf[i] | (1<<(8-j));
}

void write_bitmap_disk(char *bitmap)
{
    int i,j,k;
    i = 0;
    k = 0;
    while(i< total_blocks_req_bits)
    {
        j =0;
        while(j < disk_meta.blk_size)
        {
            buf[j] = bitmap[k];
            j++;
            k++;
        }
        write_block(buf,(i+1));
        i++;
    }
}

int find_empty_block(char *bitmap)
{
    int i,j=0,k=0,num=0;
    int found = 0;
    for(i= 0;i<total_arr_size;i++)
    {
        if(bitmap[i] == 0)
        {
            continue;
        }
        else
        {
            j =i;
            found = 1;
            break;
        }
    }
    if(found == 0)
    {
        printf("DISK FULL BITMAP\n");
        exit(EXIT_FAILURE);
    }
    char bit = bitmap[j];
    k = 0;
    for(i=0;i<8;i++)
    {
        k = (bit<<i) & (0x80);
        if(k==128)
        {
            num = i + 1;
            break;
        }
    }
    num = (j*8)+num;
    return num;
}
