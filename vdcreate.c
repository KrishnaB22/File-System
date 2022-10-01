#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>
#include<limits.h>
#include<errno.h>
#include"diskinfo.h"

int main(int argc,char **argv)
{
    disk_info *disk;
    disk = (disk_info *)malloc(sizeof(disk_info));
    int i,j,disk_size;
    if(argc != 3)
    {
        printf("Enter filename and file size\n");
        exit(0);
    }

    char *fname,*fsize;
    fname = (char *)malloc(256*sizeof(char));
    fsize = (char *)malloc(256*sizeof(char));
    fname = argv[1];
    fsize = argv[2];

    int file_size;
    file_size = atoi(fsize);
    disk -> disk_size = file_size;
    disk -> blk_size = BLOCK_SIZE;
    disk -> total_files = MAX_FILE_ALLOWED;    

    int disk_fd;
    disk_fd = open(fname,O_CREAT|O_WRONLY | O_TRUNC,0666);
    if(disk_fd == -1)
    {
        printf("Unable to create disk \n");
        exit(EXIT_FAILURE);
    }

    char buf[BLOCK_SIZE]="";
    memset(buf,0,BLOCK_SIZE);
    memcpy(buf,disk,BLOCK_SIZE);
    write(disk_fd,buf,BLOCK_SIZE);


    int no_of_blks = disk->disk_size / BLOCK_SIZE ;
    int total_bit_char = no_of_blks / 8;
    int total_blocks_req_bits = (no_of_blks / 8)/BLOCK_SIZE;
    
    int file_meta_per_block = disk -> blk_size /sizeof(file_info);
    int total_file_meta_blocks = disk -> total_files / file_meta_per_block;
    
    total_blocks_req_bits += total_file_meta_blocks;
    int max_val_char = total_blocks_req_bits /8;
    int bits_to_set = (total_blocks_req_bits*disk->blk_size) + 1;
    int all_done,bits_to_add;
    if (bits_to_set % 8 == 0)
    {
        all_done=0;
    }
    else
    {
        all_done = 1;
        bits_to_add = bits_to_set % 8;
        printf("max val char = %d\n",bits_to_set);
        printf("bits to add = %d\n",bits_to_add);
    }

    printf("size of disk data block = %ld\n",sizeof(disk));
    printf("size of file data block = %ld\n",sizeof(file_info));
    printf("total integers required for bitmaps = %d\n",total_bit_char);
    printf("total blocks required for bits = %d\n",total_blocks_req_bits);
    printf("total bit ints = %d\n",total_bit_char);


    unsigned char bitmap;
    unsigned char *temp;
    for(i=0;i<total_bit_char;i++)
    {
        if(i < max_val_char)
        {
            bitmap = 0x00;
        }
        else if((i == max_val_char) && (all_done == 1))
        {
            bitmap = 0xff;
            bitmap = bitmap >> bits_to_add;
        }
        else
        {
            bitmap = 0xff;
        }
        temp = (unsigned char *)&bitmap;
        write(disk_fd,temp,1);
    }
    
    // memset(buf,0,BLOCK_SIZE);
    // for(j=0;j<total_file_meta_blocks;j++)
    // {
    //     write(disk_fd,buf,BLOCK_SIZE);
    // }


    i = 4;
    j = 0;
    memset(buf,0,BLOCK_SIZE);
    file_info file_meta ={'\0',"\0",0,0};
    while(j < MAX_FILE_ALLOWED)
    {
        memcpy(buf,&file_meta,sizeof(file_info));
        write(disk_fd,buf,sizeof(file_info));
        memset(buf,0,BLOCK_SIZE);
        j++;
    }

    printf("no of file meta per block= %d\n",file_meta_per_block);

    lseek(disk_fd,0,SEEK_SET);
    lseek(disk_fd,(file_size-1),SEEK_SET);        
    write(disk_fd,buf,1);
    close(disk_fd);

    return 0;
}
