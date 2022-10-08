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
    if(argc != 5)
    {
        printf("Enter filename and file size\n");
        exit(0);
    }

    char *fname,*fsize,*block,*tfno;
    fname = (char *)malloc(256*sizeof(char));
    fsize = (char *)malloc(256*sizeof(char));
    block = (char *)malloc(256*sizeof(char));
    tfno = (char *)malloc(256*sizeof(char));


    fname = argv[1];
    fsize = argv[2];
    block = argv[3];
    tfno = argv[4];

    int file_size,b_size,t_fno;
    
    file_size = atoi(fsize);
    b_size = atoi(block);
    t_fno = atoi(tfno);

    disk -> disk_size = file_size;
    disk -> blk_size = b_size;
    disk -> total_files = file_size / t_fno;    

    int disk_fd;
    disk_fd = open(fname,O_CREAT|O_WRONLY | O_TRUNC,0666);
    if(disk_fd == -1)
    {
        printf("Unable to create disk \n");
        exit(EXIT_FAILURE);
    }

    unsigned char *buf;
    buf = (unsigned char *)malloc(b_size*sizeof(char));
    memset(buf,0,b_size);
    memcpy(buf,disk,b_size);
    write(disk_fd,buf,b_size);


    int no_of_blks = disk->disk_size / b_size ;
    int total_bit_char = no_of_blks / 8;
    int total_blocks_req_bits = total_bit_char / b_size;
    
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
        // printf("max val char = %d\n",bits_to_set);
        // printf("bits to add = %d\n",bits_to_add);
    }


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




/////////////////////////////
// Writing file metadata
    
    // i = 4;
    // j = 0;
    // memset(buf,0,b_size);
    // file_info file_meta ={'\0',"\0",0,0};
    // while(j < disk -> total_files)
    // {
    //     memcpy(buf,&file_meta,sizeof(file_info));
    //     write(disk_fd,buf,sizeof(file_info));
    //     memset(buf,0,b_size);
    //     j++;
    // }



    memset(buf,0,b_size);
    i =0;
    j =0;
    file_info file_meta ={'\0',"\0",0,0};
    int meta_per_block = disk_meta .blk_size / sizeof(file_info);
    while(i < disk -> total_files)
    {
        while(j < meta_per_block)
        {
            memcpy((buf+(j*sizeof(file_info))), &file_meta, sizeof(file_info));
            j++;
        }
        write(disk_fd, buf, b_size);
        i++;
        j = 0;
    }

    
    memset(buf,0,b_size);
    lseek(disk_fd,0,SEEK_SET);
    lseek(disk_fd,(file_size-1),SEEK_SET);        
    write(disk_fd,buf,1);
    close(disk_fd);

    printf("size of disk data block = %ld\n",sizeof(disk));
    printf("size of file data block = %ld\n",sizeof(file_info));
    printf("total char required for bitmaps = %d\n",total_bit_char);
    printf("total blocks set by bits = %d\n",total_blocks_req_bits);
    printf("total bit chars = %d\n",total_bit_char);
    printf("no of file meta per block= %d\n",file_meta_per_block);
    return 0;
}
