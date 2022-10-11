#include"vddelete.h"

extern int free_disk_space;

int delete_file(char *fname,char *bitmap)
{
    int i,j,k;

    i = get_file_node(fname); 
    if(i == 0)
    {
        printf("file not found\n");
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
    set_bit(bitmap,k);
    memcpy(blk_nos,buf,disk_meta.blk_size);
    memset(buf,0,disk_meta.blk_size);

    while(count > 0)
    {
        for(i =0 ; i< (arrsize - 1) ;i++)
        {
            if(blk_nos[i] > 0)
            {
                set_bit(bitmap,blk_nos[i]);
                if(count < disk_meta.blk_size)
                {
                    count = 0;
                    break;
                }
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
            set_bit(bitmap,blk_nos[k]);
            memcpy(blk_nos, buf, disk_meta.blk_size);
        }

    }
    file_meta.is_free = 0x00;

    read_block(buf,bno);
    memcpy((buf+(mno*sizeof(file_info))), &file_meta, sizeof(file_info));
    write_block(buf,bno);
    free_disk_space = free_disk_space + file_meta.file_size;
    return 1;

}

// in progress
/*int delete_at_end(char *fname,int delnum,char *bitmap)
{
    int i,j,k;

    i = get_file_node(fname); 
    if(i == 0)
    {
        printf("file not found\n");
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
    set_bit(bitmap,k);
    memcpy(blk_nos,buf,disk_meta.blk_size);
    memset(buf,0,disk_meta.blk_size);

    int end_blkno = 0, end_listno = k;

    while(count > 0)
    {
        for(i=0 ; i< (arrsize - 1) ;i++)
        {
            if(blk_nos[i] > 0)
            {
                if(count < disk_meta.blk_size)
                {
                    count = 0;
                    end_blkno = blk_nos[i];
                    break;
                }
            }
            if(blk_nos[i] == 0)
            {
                end_blkno = blk_nos[(i-1)];
                break;
            }
            count = count - disk_meta.blk_size;
        }
        if(end_blkno > 0)
        {
            break;
        }
        k = arrsize -1;
        if(blk_nos[k] > 0)
        {
            end_listno = blk_nos[k];
            memset(buf, 0, disk_meta.blk_size);
            read_block(buf,blk_nos[k]);
            memcpy(blk_nos, buf, disk_meta.blk_size);
        } 
    }

    int temp = file_meta .file_size - delnum;

    if(temp % disk_meta == 0)
    {
        k = delnum / disk_meta . blk_size;
    }

}*/