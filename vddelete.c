#include"vddelete.h"

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

int delete_file2(char *fname,Heap *bitmap)
{
    int i,j,k;

    i = get_file_node(fname); 
    if(i == 0)
    {
        printf("file not found\n");
        return 0;
    }
    k = file_meta.file_size;
    
    file_meta.is_free = 0x00;
    read_block(buf,bno);
    memcpy((buf+(mno*sizeof(file_info))), &file_meta, sizeof(file_info));
    write_block(buf,bno);

    unsigned int * level_data;
    level_data = (int *)malloc(10 * sizeof(int));
    int levels = get_levels(level_data, file_meta.file_size);

    file_delete_helper(bitmap,levels,file_meta.ptr_to_blk, level_data,&k);
    set_bit(bitmap,file_meta.ptr_to_blk);
    free_disk_space -= file_meta.file_size;
    
    return 1;
}


void file_delete_helper(char *bitmap, int levels,int prev_block,int *level_data,int *size)
{
    int i,j,n;
    int k = level_data[levels];
    int temp = disk_meta.blk_size/ sizeof(int);
    unsigned int *empty_nos;
    empty_nos = (int *) malloc((disk_meta.blk_size/sizeof(int)) * sizeof(int));

    while(level_data[levels])
    {
        if(level_data[levels] > temp)
            n = temp;
        else
            n = level_data[levels];
        
        memset(buf, 0, disk_meta.blk_size);
        read_block(buf,prev_block);
        memcpy(empty_nos,buf,disk_meta.blk_size);
        i =0;
        while(i < n)
        {
            memset(buf, 0, disk_meta.blk_size);
            if(levels > 0)
            {   
                file_delete_helper(bitmap,(levels -1),empty_nos[i],level_data,size);
            }
            else
            {
                if(*size < disk_meta.blk_size)
                {
                    set_bit(bitmap,empty_nos[i]);
                    break;
                }
                set_bit(bitmap,empty_nos[i]);
                *size -= disk_meta.blk_size;
            }
            i++;
        }
        level_data[levels] -= n;
    }
}