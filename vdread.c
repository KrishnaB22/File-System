#include"vdread.h"

int read_file(char *fname,char *outname)
{
    int i,j,k;
    i = get_file_node(fname); 
    if(i == 0)
    {
        printf("file not found %s\n",fname);
        return 0;
    }
    // display_meta(file_meta);
    int file_fd;
    file_fd = open(outname,O_CREAT |O_WRONLY|O_TRUNC,0666);
    if(file_fd == -1)
    {
        printf("Unable to create file to write\n");
        return 0;
    }
    k = file_meta.ptr_to_blk;


    int count = file_meta.file_size;
    int arrsize = disk_meta . blk_size / sizeof(unsigned int);
    unsigned int *blk_nos;
    blk_nos = (unsigned int *)malloc(arrsize*sizeof(unsigned int));
    for(i=0;i<arrsize;i++)
        blk_nos[i] = 0;

    read_block(buf,k);
    memcpy(blk_nos,buf,disk_meta.blk_size);
    memset(buf,0,disk_meta.blk_size);

    while(count > 0)
    {
        for(i =0 ; i< (arrsize - 1) ;i++)
        {
            if(blk_nos[i] > 0)
            {
                read_block(buf,blk_nos[i]);
                if(count < disk_meta.blk_size)
                {
                    write(file_fd,buf,count);
                    count = 0;
                    break;
                }
                write(file_fd,buf,disk_meta.blk_size);
                count = count - disk_meta.blk_size;
            }
        }
        if(count == 0)
        {
            break;
        }
        k = arrsize -1;
        if(blk_nos[k] > 0)
        {
            memset(buf, 0, disk_meta.blk_size);
            read_block(buf,blk_nos[k]);
            memcpy(blk_nos, buf, disk_meta.blk_size);
        }

    }
    return 1;
    close(file_fd);
}


int read_bytes(char *fname,char *outname, int startnum,int tnum,char *out)
{
    int i,j,k;
    i = get_file_node(fname); 
    if(i == 0)
    {
        printf("file not found\n");
        return 0;
    }
    display_meta(file_meta);
    int file_fd;
    file_fd = open(outname,O_CREAT |O_WRONLY|O_TRUNC,0666);
    if(file_fd == -1)
    {
        printf("Unable to create file to write\n");
        return 0;
    }
    k = file_meta.ptr_to_blk;


    int count = file_meta.file_size;
    int arrsize = disk_meta . blk_size / sizeof(unsigned int);
    unsigned int *blk_nos;
    blk_nos = (unsigned int *)malloc(arrsize*sizeof(unsigned int));
    for(i=0;i<arrsize;i++)
        blk_nos[i] = 0;

    read_block(buf,k);
    memcpy(blk_nos,buf,disk_meta.blk_size);
    memset(buf,0,disk_meta.blk_size);

    int start = 0;
    int fblock = startnum / disk_meta . blk_size;
    int end_blkno = (startnum + tnum) / disk_meta . blk_size;

    char *temp;
    if(fblock == end_blkno)
    {
        temp = malloc(disk_meta .blk_size * sizeof(char));
    }
    else
    {
        int t = end_blkno - fblock;
        temp = malloc((t* disk_meta.blk_size) * sizeof(char));
    }
    
    j = 0;
    while(count > 0)
    {
        for(i=0 ; i< (arrsize - 1) ;i++)
        {
            if(blk_nos[i] > 0)
            {
                if(start >= fblock && start <= end_blkno)
                {
                    read_block(buf,blk_nos[i]);
                    j = j + disk_meta . blk_size;
                    memcpy((temp + j),buf,disk_meta .blk_size);
                }
                if(count < disk_meta.blk_size)
                {
                    count = 0;
                    break;
                }
            }
            start++;
            count = count - disk_meta.blk_size;
        }
        if(start > end_blkno)
        {
            break;
        }
        if(count == 0)
        {
            break;
        }
        k = arrsize -1;
        if(blk_nos[k] > 0)
        {
            memset(buf, 0, disk_meta.blk_size);
            read_block(buf,blk_nos[k]);
            memcpy(blk_nos, buf, disk_meta.blk_size);
        } 
    }
    
    k = startnum % disk_meta.blk_size;
    memset(out,0,tnum);
    memcpy(out,(temp + k),tnum);

    return 1;

}
