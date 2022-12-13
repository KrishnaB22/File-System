#include"diskfunc.h"

Heap *disk_init()
{
    int i,j,k,e;

    disk_fd = open("mydisk",O_RDWR);
    if(disk_fd == -1)
    {
        printf("Unable to access disk \n");
        exit(EXIT_FAILURE);
    }
    
    e = read_block(buf,0);
    if(e == -1)
    {
        printf("read error \n");
        exit(EXIT_FAILURE);
    }
    memcpy(&disk_meta,buf,sizeof(disk_info));

    no_of_blks = disk_meta . disk_size / disk_meta . blk_size;
    total_blocks_req_bits = (no_of_blks / 8)/disk_meta . blk_size;
    // printf("total blocks required for bits= %d\n",total_blocks_req_bits);

    total_arr_size = total_blocks_req_bits * disk_meta . blk_size;
    unsigned char *bitmap;
    bitmap = (unsigned char *)malloc(total_arr_size * sizeof(unsigned char));

    i = 0;
    j = 1;
    while(j <= total_blocks_req_bits)
    {   
        read_block(buf,j);
        k = 0;
        while(k < disk_meta . blk_size)
        {
            bitmap[i] = buf[k];
            i++;
            k++;
        }
        j++;
    }
    printf("here\n");
    Heap *bitheap = build_heap();
    return bitheap;
}

int get_free_disk_size(char *bitmap)
{
    int i,j,k;
    int count=0;
    // if(free_disk_space > 0)
    // {
    //     return free_disk_space;
    // }
    for(i=0;i<total_arr_size;i++)
    {
        if(bitmap[i] == 0)
        {
            continue;
        }
        for(j=0;j<8;j++)
        {
            k = (bitmap[i]<<j) & (0x80);
            if(k==128)
            {
                count++;
            }
        }
    }
    free_disk_space = count*disk_meta.blk_size;
    return free_disk_space;
}

unsigned long get_free_disk_size2()
{
    int i,j,k,n;
    char bit;
    unsigned long size = 0;
    j = 1;
    while(j <= total_blocks_req_bits)
    {   
        read_block(buf,j);
        k = 0;
        while(k < disk_meta . blk_size)
        {
            if(buf[k] == 0x00)
            {
                k++;
                continue;
            }
            bit = buf[k];
            for(i=0;i<8;i++)
            {
                n = (bit<<i) & (0x80);
                if(n == 128)
                {
                    size++;
                }
            }
            k++;
        }
        j++;
    }

    return size * disk_meta . blk_size;
}

int get_levels(int *level_data , unsigned int file_size)
{
    int i,j,temp;
    temp = disk_meta.blk_size / sizeof(int) ;
    file_size = ceil((double)file_size / (double)disk_meta.blk_size);

    i = 0;
    j = 0;
    while(file_size != 1) 
    {
        level_data[j] = file_size;
        file_size = ceil((double)file_size / (double) temp);
        i++;
        j++;
    }
    if(i == 0)
    {
        level_data[i] = 1;
    }
    return i;
}


char *disk_init2()
{
    int i,j,k,e;

    disk_fd = open("mydisk",O_RDWR);
    if(disk_fd == -1)
    {
        printf("Unable to access disk \n");
        exit(EXIT_FAILURE);
    }
    
    e = read_block(buf,0);
    if(e == -1)
    {
        printf("read error \n");
        exit(EXIT_FAILURE);
    }
    memcpy(&disk_meta,buf,sizeof(disk_info));

    no_of_blks = disk_meta . disk_size / disk_meta . blk_size;
    total_blocks_req_bits = (no_of_blks / 8)/disk_meta . blk_size;
    // printf("total blocks required for bits= %d\n",total_blocks_req_bits);

    total_arr_size = total_blocks_req_bits * disk_meta . blk_size;
    unsigned char *bitmap;
    bitmap = (unsigned char *)malloc(total_arr_size * sizeof(unsigned char));

    i = 0;
    j = 1;
    while(j <= total_blocks_req_bits)
    {   
        read_block(buf,j);
        k = 0;
        while(k < disk_meta . blk_size)
        {
            bitmap[i] = buf[k];
            i++;
            k++;
        }
        j++;
    }
    return bitmap;
}