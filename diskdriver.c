#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<errno.h>
#include"diskinfo.h"

int write_block(char *buf,int blk)
{
    int i;
    if(blk < 0 )
    {
        printf("Block index out of bounds to write");
        return -1;
    }

    if( lseek(disk_fd,blk*BLOCK_SIZE,SEEK_SET) < 0 )
    {
        perror("Unable to seek ");
        return -1;
    }

    i = write(disk_fd,buf,BLOCK_SIZE);
    if( i < 0)
    {
        perror("Unable to write block ");
        return -1;
    }
    return i;
}

int read_block(char *buf,int blk)
{
    int i;
    if(blk < 0 )
    {
        printf("Block index out of bounds to read ");
        return -1;
    }

    if( lseek(disk_fd,blk*BLOCK_SIZE,SEEK_SET) < 0 )
    {
        perror("Unable to seek ");
        return -1;
    }
    i = read(disk_fd,buf,BLOCK_SIZE);
    if( i < 0)
    {
        perror("Unable to read block ");
        return -1;
    }
    return i;
}
