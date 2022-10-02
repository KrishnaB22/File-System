#include"diskdriver.h"

int write_block(char *buf,int blk)
{
    int i;
    if(blk <= 0 )
    {
        printf("Block index out of bounds to write");
        return -1;
    }

    if( lseek(disk_fd,blk*BLOCK_SIZE,SEEK_SET) < 0 )
    {
        perror("Write Unable to seek ");
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
        perror("Read Unable to seek ");
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
