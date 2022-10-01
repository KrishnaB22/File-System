#include<stdio.h>

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

