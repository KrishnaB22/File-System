#include"bitio.h"

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

unsigned int *get_empty_blocks(char *bitmap,int num)
{
    unsigned int *emp_nos;
    emp_nos = (int *)malloc(num * sizeof(int));
    for(int i=0;i<num;i++)
    {
        emp_nos[i] = find_empty_block(bitmap);
    }
    return emp_nos;
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


int find_empty_block2(Heap *bitheap)
{

}

unsigned int *get_empty_blocks2(Heap *bitheap,int num)
{
    int i,j,size,start,temp;
    unsigned int *emp_nos;
    emp_nos = (int *)malloc(num * sizeof(int));
    i = 0;
    j = 0;
    while(num > 0)
    {
        if(heap_size == 0)
        {
            bitheap = build_heap();
            i = 0;
        }
        start = bitheap[i] . start;
        size = bitheap[i] . size;
        if(size < num)
        {
            delete_node(bitheap);
            heap_size--;
        }
        else
        {
            delete_node(bitheap);
            add_node(bitheap, (start + num),(size - num));
        }
        temp = 0
        while(temp < size)
        {
            if(j == num)
            {
                num = 0;
                break;
            }
            emp_nos[j] = start;
            start++;
            temp++;
            j++;
        }
        num -= size;
        i++;
    }
    return emp_nos;
}