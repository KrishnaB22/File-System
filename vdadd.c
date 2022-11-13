#include"vdadd.h"

extern int free_disk_space;

int add_file(char *fname,char *outname,char *bitmap)
{
    int file_fd,i,j,k;
    file_fd = open(outname,O_RDONLY);
    if(file_fd == -1)
    {
        printf("Unable to open file %s\n",outname);
        return 0;
    }

    int file_size = lseek(file_fd,0,SEEK_END);
    lseek(file_fd,0,SEEK_SET);
    if(file_size <= 0)
    {
        printf("Unable to seek file ");
        return 0;
    }

    i = get_empty_node(fname);
    if(i == 0)
    {
        printf("Disk file limit reached\n");
        return 0;
    }
    if(i==2)
    {
        return 0;
    }
    int total_space = ceil((ceil( (double)file_size / (double)disk_meta . blk_size)) / (((double)disk_meta .blk_size / (double)sizeof(int)) -1)) * disk_meta .blk_size;
    total_space = total_space + file_size ;
    int free_space = get_free_disk_size(bitmap);
    if(free_space == 0)
    {
        printf("Disk full\n");
        return 0;
    }

    if(free_space < total_space)
    {
        printf("Not enough space on disk\n");
        return 0;
    }

    //writing file meta to disk

    file_meta.is_free = 0x01;
    strcpy(file_meta.file_name,fname);
    file_meta.file_size = file_size;
    file_meta.ptr_to_blk = find_empty_block(bitmap);
    clear_bit(bitmap,file_meta.ptr_to_blk);

    read_block(buf,bno);
    memcpy((buf+(mno*sizeof(file_info))), &file_meta, sizeof(file_info));
    write_block(buf,bno);
    memset(buf,0,disk_meta.blk_size);
    // -----------------------------------------

    // writing data to disk
    int arrsize = disk_meta . blk_size / sizeof(unsigned int);
    unsigned int *blk_nos;
    blk_nos = (unsigned int *)malloc(arrsize*sizeof(unsigned int));
    for(i=0;i<arrsize;i++)
        blk_nos[i] = 0;

    int count = 0;
    int prev_no = file_meta.ptr_to_blk;
    j =0;
    while(count < file_size)
    {
        if(j == (arrsize - 1))
        {
            blk_nos[j] = find_empty_block(bitmap);
            clear_bit(bitmap,blk_nos[j]);
            memcpy(buf,blk_nos,disk_meta.blk_size);

            write_block(buf , prev_no);
            prev_no = blk_nos[j];


            for( k =0; k < arrsize ; k++)
            {
                blk_nos[k] = 0;
            }
            j = 0;
            continue;
        }

        memset(buf,0,disk_meta .blk_size);
        i = find_empty_block(bitmap);
        clear_bit(bitmap,i);


        k = read(file_fd,buf,disk_meta.blk_size);    
        if(k < 0)
        {
            perror("Unable to read block ");
            exit(EXIT_FAILURE);
        }

        k = write_block(buf,i);
        if(k < disk_meta . blk_size)
        {
            printf("error\n");
            exit(EXIT_FAILURE);
        }

        blk_nos[j] = i;
        j++;

        count = count + disk_meta.blk_size;
    }

    if(j < (arrsize -1))
    {
        memcpy(buf,blk_nos,disk_meta.blk_size);
        k = write_block(buf,prev_no);
        if(k < disk_meta . blk_size)
        {
            printf("error\n");
            exit(EXIT_FAILURE);
        }
    }

    free_disk_space = free_disk_space - file_size;
    close(file_fd);
    return 1;

}

int insert_at_end(char *fname,char *fname1,char *bitmap)
{
    int i,j,k;
    int file_fd;

    file_fd = open(fname,O_RDONLY);
    if(file_fd == -1)
    {
        printf("unable to open file to add\n");
        return 0;
    }

    int file_size = lseek(file_fd,0,SEEK_END);
    lseek(file_fd,0,SEEK_SET);
    if(file_size <= 0)
    {
        printf("Unable to seek file to add at end\n ");
        return 0;
    }
    int free_space = get_free_disk_size(bitmap);
    if(free_space == 0)
    {
        printf("Disk full\n");
        return 0;
    }

    if(free_space < file_size)
    {
        printf("Not enough space on disk\n");
        return 0;
    }

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

    int partial_fill_size = file_meta . file_size % disk_meta . blk_size;
    count = 0;
    if(partial_fill_size > 0)
    {
        char temp_buf[BLOCK_SIZE];
        memset(buf,0,disk_meta.blk_size);
        read_block(buf,end_blkno);

        int read_from_file = disk_meta . blk_size - partial_fill_size ;
        read(file_fd, temp_buf, read_from_file);
        memcpy((buf + partial_fill_size) , temp_buf, read_from_file);

        write_block(buf,end_blkno);
        count = count + read_from_file;
    }
    
    int prev_no = end_listno;
    j = 0;
    for(i=0; i< arrsize; i++)
    {
        if(blk_nos[i] == 0)
        {
            j = i;
            break;
        }
    }

    if(j == (arrsize -1))
    {
        blk_nos[j] = find_empty_block(bitmap);
        
        clear_bit(bitmap,blk_nos[j]);
        memcpy(buf,blk_nos,disk_meta.blk_size);
        
        write_block(buf , prev_no);
        prev_no = blk_nos[j];
        
        for(k=0; k < arrsize ; k++)
        {
            blk_nos[k] = 0;
        }
        j = 0;
    }
    
    while(count < file_size)
    {
        if(j == (arrsize - 1))
        {
            blk_nos[j] = find_empty_block(bitmap);
            clear_bit(bitmap,blk_nos[j]);
            memcpy(buf,blk_nos,disk_meta.blk_size);

            write_block(buf , prev_no);
            prev_no = blk_nos[j];


            for( k =0; k < arrsize ; k++)
            {
                blk_nos[k] = 0;
            }
            j = 0;
            continue;
        }

        memset(buf,0,disk_meta .blk_size);
        i = find_empty_block(bitmap);
        clear_bit(bitmap,i);


        k = read(file_fd,buf,disk_meta.blk_size);    
        if(k < 0)
        {
            perror("Unable to read block ");
            exit(EXIT_FAILURE);
        }

        k = write_block(buf,i);
        if(k < disk_meta . blk_size)
        {
            printf("error\n");
            exit(EXIT_FAILURE);
        }

        blk_nos[j] = i;
        j++;

        count = count + disk_meta.blk_size;
    }

    if(j < (arrsize -1))
    {
        memcpy(buf,blk_nos,disk_meta.blk_size);
        k = write_block(buf,prev_no);
        if(k < disk_meta . blk_size)
        {
            printf("error\n");
            exit(EXIT_FAILURE);
        }
    }

    file_meta . file_size = file_meta . file_size + file_size;

    read_block(buf,bno);
    memcpy((buf+(mno*sizeof(file_info))), &file_meta, sizeof(file_info));
    write_block(buf,bno);
    memset(buf,0,disk_meta.blk_size);
    
    free_disk_space = free_disk_space - file_size;
    close(file_fd);
    return 1;
}


int add_file2(char *fname,char *outname,char *bitmap)
{
    int file_fd,i,j,k;
    file_fd = open(outname,O_RDONLY);
    if(file_fd == -1)
    {
        printf("Unable to open file %s\n",outname);
        return 0;
    }

    int file_size = lseek(file_fd,0,SEEK_END);
    lseek(file_fd,0,SEEK_SET);
    if(file_size <= 0)
    {
        printf("Unable to seek file ");
        return 0;
    }

    i = get_empty_node(fname);
    if(i == 0)
    {
        printf("Disk file limit reached\n");
        return 0;
    }
    if(i==2)
    {
        return 0;
    }
    int total_blocks = ceil((ceil( (double)file_size / (double)disk_meta . blk_size)) / (((double)disk_meta .blk_size / (double)sizeof(int)) -1))
    int total_space = total_blocks * disk_meta .blk_size;
    total_space = total_space + file_size ;
    int free_space = get_free_disk_size(bitmap);
    if(free_space == 0)
    {
        printf("Disk full\n");
        return 0;
    }

    if(free_space < total_space)
    {
        printf("Not enough space on disk\n");
        return 0;
    }

    //writing file meta to disk
    unsigned int *emp_nos;
    int n = 0;
    file_meta.is_free = 0x01;
    strcpy(file_meta.file_name,fname);
    file_meta.file_size = file_size;
    file_meta.ptr_to_blk = find_empty_block(bitmap);
    clear_bit(bitmap,file_meta.ptr_to_blk);

    read_block(buf,bno);
    memcpy((buf+(mno*sizeof(file_info))), &file_meta, sizeof(file_info));
    write_block(buf,bno);
    memset(buf,0,disk_meta.blk_size);

}