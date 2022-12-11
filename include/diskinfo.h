#define DISK_SIZE 134217728 //1073741824 - 1gb //134217728 - 128mb
#define DISK_BLOCKS 100
#define BLOCK_SIZE 1024
#define MAX_FILE_ALLOWED 128

typedef struct disk_info{
    unsigned int disk_size;
    unsigned int blk_size;
    unsigned int total_files;
}disk_info;


typedef struct file_info{
    char is_free;
    char file_name[247];
    int file_size;
    int ptr_to_blk;
}file_info;

int disk_fd;

disk_info disk_meta;
file_info file_meta;
char buf[BLOCK_SIZE];
int total_arr_size,no_of_blks;
int total_blocks_req_bits;

int bno,mno;

int heap_total_size = 16;
int heap_size;