#include"vdallocate.h"

unsigned int free_disk_space = 0;


int add_file(char *fname,char *bitmap)
{
    int file_fd,i,j,k;
    file_fd = open(fname,O_RDONLY);
    if(file_fd == -1)
    {
        printf("Unable to open file %s\n",fname);
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

void read_file(char *fname,char *outname)
{
    int i,j,k;
    i = get_file_node(fname); 
    if(i == 0)
    {
        printf("file not found\n");
        return;
    }
    display_meta(file_meta);
    int file_fd;
    file_fd = open(outname,O_CREAT |O_WRONLY|O_TRUNC,0666);
    if(file_fd == -1)
    {
        printf("Unable to create file to write\n");
        return;
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

    close(file_fd);
}

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
    
    memcpy(out,(temp + k),tnum);

    return 1;

}


int main(int argc,char **argv)
{
    int i,j,k;
    char *bitmap;
    bitmap = disk_init();

    char *fname,*outname;
    fname = (char *)malloc(200*sizeof(char));
    outname = (char *)malloc(200*sizeof(char));

    fname = argv[1];
    outname = argv[2];

    i = add_file(fname,bitmap);
    if(i == 1)
    {
        printf("file added succesfully\n");
    }
    else
    {
        printf("Unable to add file \n");
    }

    insert_at_end(fname,fname,bitmap);
    read_file(fname,outname);
    


    printf("free size on disk = %d\n",get_free_disk_size(bitmap));

    // displaybitmap(bitmap);
    // dis_file_meta();
    // display_diskmeta();

    write_bitmap_disk(bitmap);

    close(disk_fd);
}

