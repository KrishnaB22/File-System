#include"vdallocate.h"

unsigned int free_disk_space = 0;


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
    printf("total blocks required for bits= %d\n",total_blocks_req_bits);

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


void read_file(char *fname)
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
    file_fd = open("outfromdisk.txt",O_CREAT |O_WRONLY|O_TRUNC,0666);
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


int main(int argc,char **argv)
{
    int i,j,k;
    char *bitmap;
    bitmap = disk_init();
    
    char *fname;
    fname = (char *)malloc(200*sizeof(char));
    fname = argv[1];
    
    i = add_file(fname,bitmap);
    if(i == 1)
    {
        printf("file added succesfully\n");
    }
    else
    {
        printf("Unable to add file \n");
    }
    
    
    read_file(fname);
    delete_file(fname,bitmap);
    

    
    

    // printf("free size on disk = %d\n",get_free_disk_size(bitmap));
    
    // displaybitmap(bitmap);
    // dis_file_meta();
    // display_diskmeta();
    
    write_bitmap_disk(bitmap);
    close(disk_fd);
}

