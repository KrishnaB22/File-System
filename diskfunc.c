#include"diskfunc.h"

extern unsigned int free_disk_space;

char *disk_init()
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

int get_free_disk_size(char *bitmap)
{
    int i,j,k;
    int count=0;
    if(free_disk_space > 0)
    {
        return free_disk_space;
    }
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