#include"filenode.h"

int get_empty_node(char *fname)
{
    int i,j,k;
    int free_meta_found = 0;
    int first_block_meta = total_blocks_req_bits + 1;
    int last_block_meta = first_block_meta + (disk_meta.total_files / (disk_meta .blk_size / sizeof(file_info)));
    int file_meta_per_block = disk_meta .blk_size / sizeof(file_info);
    
    for(i=first_block_meta;i<=last_block_meta;i++)
    {
        read_block(buf,i);
        for(j =0; j<file_meta_per_block; j++)
        {
            memcpy(&file_meta, (buf+(j*sizeof(file_info))), sizeof(file_info));
            if((strcmp(file_meta.file_name,fname) == 0) && (file_meta.is_free == 0x01))
            {
                    printf("Same name file already exists\n");
                    return 2;
            }
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

int get_file_node(char *fname)
{
    int i,j,k;
    int free_meta_found = 0;
    int first_block_meta = total_blocks_req_bits + 1;
    int last_block_meta = first_block_meta + (disk_meta.total_files / (disk_meta .blk_size / sizeof(file_info)));
    int file_meta_per_block = disk_meta .blk_size / sizeof(file_info);
    
    for(i=first_block_meta;i<=last_block_meta;i++)
    {
        read_block(buf,i);
        for(j =0; j<file_meta_per_block; j++)
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

void display_meta(file_info file_meta)
{
    printf("---- Reading file meta data ----------\n");
    printf("is free = %x\n",file_meta.is_free);
    printf("filename = %s\n",file_meta.file_name);
    printf("file size = %d\n",file_meta.file_size);
    printf("ptr to blk = %d\n",file_meta.ptr_to_blk); 
    printf("\n");
}