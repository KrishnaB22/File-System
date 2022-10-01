#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>
// #include"diskinfo.h"

#include"diskdriver.c"
#include"bitio.c"

disk_info disk_meta;
file_info file_meta;
char buf[BLOCK_SIZE];
int total_arr_size,no_of_blks;
int total_blocks_req_bits;
int free_disk_space = 0;
int bno,mno;



void dis_file_meta()
{
    int i,j,k;
    int free_meta_found = 0;
    int first_block_meta = total_blocks_req_bits + 1;
    int last_block_meta = first_block_meta + (disk_meta.total_files / 4);
    
    for(i=first_block_meta;i<=last_block_meta;i++)
    {
        read_block(buf,i);
        for(j =0;j<4;j++)
        {
            memcpy(&file_meta, (buf+(j*sizeof(file_info))), sizeof(file_info));
            if(file_meta.is_free == 0x01)
            {
                printf("-------------------------------\n");
                printf("is free = %x\n",file_meta.is_free);
                printf("filename = %s\n",file_meta.file_name);
                printf("file size = %d\n",file_meta.file_size);
                printf("ptr to blk = %d\n",file_meta.ptr_to_blk); 
                printf("\n");  
            }
        }
    }
}

void write_bitmap_disk(char *bitmap)
{
    int i,j,k;
    i = 0;
    k = 0;
    while(i< total_blocks_req_bits)
    {
        j =0;
        while(j < disk_meta.blk_size)
        {
            buf[j] = bitmap[k];
            j++;
            k++;
        }
        write_block(buf,(i+1));
        i++;
    }
}

void displaybitmap(char *bitmap)
{
    int i,j,k;
    for(i=0;i<total_arr_size;i++)
    {
        printf(" %d - bitmap value = %x\n",i,bitmap[i]);
    }

}

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
    //printf("total blocks required for bits= %d\n",total_blocks_req_bits);

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

int find_empty_block(char *bitmap)
{
    int i,j=0,k=0,num=0;
    for(i= 0;i<total_arr_size;i++)
    {
        if(bitmap[i] == 0)
        {
            continue;
        }
        else
        {
            j =i;
            break;
        }
    }
    char bit = bitmap[j];
    k = 0;
    for(i=0;i<8;i++)
    {
        k = (bit<<i) & (0x80);
        if(k==128)
        {
            num = i + 1;
            break;
        }
    }
    num = (j*8)+num;
    return num;
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

int get_empty_node(char *fname)
{
    int i,j,k;
    int free_meta_found = 0;
    int first_block_meta = total_blocks_req_bits + 1;
    int last_block_meta = first_block_meta + (disk_meta.total_files / 4);
    
    for(i=first_block_meta;i<=last_block_meta;i++)
    {
        read_block(buf,i);
        for(j =0;j<4;j++)
        {
            memcpy(&file_meta, (buf+(j*sizeof(file_info))), sizeof(file_info));
            // if((strcmp(file_meta.file_name,fname) == 0) && (file_meta.is_free = 0x01))
            // {
            //         printf("Same name file already exists\n");
            //         return 2;
            // }
            if(file_meta.is_free == 0x00)
            {
                free_meta_found = 1;
                bno = i;
                mno = j;
                return 1;
            }
        }
    }
    if(free_meta_found == 0)
        return 0;
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
    if(file_size < 0)
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

    file_meta.is_free = 0x01;
    strcpy(file_meta.file_name,fname);
    file_meta.file_size = file_size;
    file_meta.ptr_to_blk = find_empty_block(bitmap);
    clear_bit(bitmap,file_meta.ptr_to_blk);

    read_block(buf,bno);
    memcpy((buf+(mno*sizeof(file_info))), &file_meta,sizeof(file_info));
    write_block(buf,bno);

    int blk_nos[255]={0};
    int count = 0;
    j =0;
    while(count < file_size)
    {
        memset(buf,0,disk_meta.blk_size);
        i = find_empty_block(bitmap);
        clear_bit(bitmap,i);
        blk_nos[j] = i;
        j++;
        k = read(file_fd,buf,disk_meta.blk_size);    
        if(k < 0)
        {
            perror("Unable to read block ");
            return 0;
        }
        write_block(buf,i);

        count = count + disk_meta.blk_size;
    }
    
    memcpy(buf,blk_nos,disk_meta.blk_size);
    write_block(buf,file_meta.ptr_to_blk);
    for(i=0;i<255;i++)
    {
        if(blk_nos[i] > 0)
        printf("blk no = %d\n",blk_nos[i]);
    }
    free_disk_space = free_disk_space - file_size;
    close(file_fd);
    return 1;
    
}


int get_file_node(char *fname)
{
    int i,j,k;
    int free_meta_found = 0;
    int first_block_meta = total_blocks_req_bits + 1;
    int last_block_meta = first_block_meta + (disk_meta.total_files / 4);
    
    for(i=first_block_meta;i<=last_block_meta;i++)
    {
        read_block(buf,i);
        for(j =0;j<4;j++)
        {
            memcpy(&file_meta, (buf+(j*sizeof(file_info))), sizeof(file_info));
            if(file_meta.is_free == 0x01)
            {
                if(strcmp(file_meta.file_name,fname)==0)
                {
                    bno = i;
                    mno =j;
                    return 1;
                } 
            }
        }
    }
    return 0;
}

void read_file(char *fname)
{
    int i,j,k;
    i = get_file_node(fname);
    printf("---------------     ------------\n");
    printf("is free = %x\n",file_meta.is_free);
    printf("filename = %s\n",file_meta.file_name);
    printf("file size = %d\n",file_meta.file_size);
    printf("ptr to blk = %d\n",file_meta.ptr_to_blk); 
    printf("\n"); 
    if(i == 0)
    {
        printf("file not found\n");
        return;
    }

    int file_fd;
    file_fd = open("outfromdisk.txt",O_CREAT |O_WRONLY,0666);
    if(file_fd == -1)
    {
        printf("Unable to create file to write\n");
        return;
    }
    k = file_meta.ptr_to_blk;
    read_block(buf,k);
    
    int blk_nos[255];
    memcpy(blk_nos,buf,disk_meta.blk_size);

    for(i=0;i<255;i++)
    {
        if(blk_nos[i] > 0)
        {
            printf("blk no = %d\n",blk_nos[i]);
        }
    }

    for(i=0;i<254;i++)
    {
        if(i > 0)
        {
            read_block(buf,blk_nos[i]);
            write(file_fd,buf,1024);
        }
    }

    close(file_fd);
}


int main()
{
    int i,j,k;
    char *bitmap;
    bitmap = disk_init();
    //displaybitmap(bitmap);
    // i = find_empty_block(bitmap);
    // printf("block found = %d\n",i);
    char *fname = "sample_test.txt";
    i = add_file(fname,bitmap);
    printf("value of i = %d\n",i);
    //read_file(fname);
    write_bitmap_disk(bitmap);

    dis_file_meta();
    close(disk_fd);
}