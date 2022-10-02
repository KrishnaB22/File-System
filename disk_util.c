#include"disk_util.h"


void dis_file_meta()
{
    int i,j,k;
    int free_meta_found = 0;
    int first_block_meta = total_blocks_req_bits + 1;
    int last_block_meta = first_block_meta + (disk_meta.total_files / (disk_meta .blk_size / sizeof(file_info)));
    int file_meta_per_block = disk_meta .blk_size / sizeof(file_info);
    
    for(i=first_block_meta;i<=last_block_meta;i++)
    {
        read_block(buf,i);
        for(j =0;j<file_meta_per_block;j++)
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


void displaybitmap(char *bitmap)
{
    int i,j,k;
    for(i=0;i<total_arr_size;i++)
    {
        printf(" %d - bitmap value = %x\n",i,bitmap[i]);
    }

}


void display_diskmeta()
{
    printf("-------------------------------\n");
    printf("Disk size = %d\n",disk_meta.disk_size);
    printf("Block size = %d\n",disk_meta.blk_size);
    printf("Total files = %d\n",disk_meta.total_files);
    printf("\n");  
}