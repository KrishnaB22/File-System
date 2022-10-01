#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>


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