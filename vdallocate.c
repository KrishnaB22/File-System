#include"vdallocate.h"

unsigned int free_disk_space = 0;


void add_file_helper(char *fname,char *outname,char *bitmap)
{
    int i;
    printf("ADD\n");
    i = add_file2(fname,outname,bitmap);
    
    if( i == 1)
    {
        printf("File Add sucessfully: %s\n",outname);
    }
    else
    {
        printf("Not file Add sucessfully: %s\n",outname);
    }
}

void read_file_helper(char *fname,char *outname)
{
    int i;
    printf("READ\n");
    i = read_file2(fname,outname);
    
    if( i == 1)
    {
        printf("File Read sucessfully: %s\n",fname);
    }
    else
    {
        printf("Not file Read sucessfully: %s\n",fname);
    }
}

void delete_file_helper(char *fname,char *bitmap)
{
    int i;
    printf("DELETE\n");
    i = delete_file2(fname,bitmap);
    
    if( i == 1)
    {
        printf("File Delete sucessfully: %s\n",fname);
    }
    else
    {
        printf("Not file Delete sucessfully: %s\n",fname);
    }
}

void insert_at_end_helper(char *fname,char *outname,char *bitmap)
{
    int i;
    i = insert_at_end(fname,outname,bitmap);
    if( i == 1)
    {
        printf("File INS_END sucessfully: %s\n",outname);
    }
    else
    {
        printf("Not file INS_END sucessfully: %s\n",outname);
    }
}

void read_bytes_helper(char *fname,char *outname, int start,int total)
{
    int i,j;
    char *out;
    out = (char *)malloc(total * sizeof(char));
    i = read_bytes(fname,outname,start,total,out);
    if(i == 1)
    {
        printf("File READ_BYTES sucessfully %s\n",fname);
    }
    else
    {
        printf("Not File READ_BYTES sucessfully %s\n",fname);
    }
}



void user_actions(char *ufname,char *bitmap)
{
    int i,j;
    FILE *fp = fopen(ufname,"r");

    char *action,*fname,*outname;
    action = (char *)malloc(10*sizeof(char));
    fname = (char *)malloc(200*sizeof(char));
    outname = (char *)malloc(200*sizeof(char));

    while(1)
    {

        fgets(action, 10, fp);
        if(strcmp(action,"end")== 0)
        {
            printf("Done\n");
            break;
        }
        fgets(fname, 200, fp);
        fgets(outname, 200, fp);

        fname[strcspn(fname, "\n")] = '\0';
        outname[strcspn(outname, "\n")] = '\0';
        action[strcspn(action,"\n")] = '\0';

        if(strcmp(action,"add")== 0)
        {
            add_file_helper(fname,outname,bitmap);
            continue;
        }

        if(strcmp(action,"read")== 0)
        {
            read_file_helper(fname,outname);
            continue;
        }

        if(strcmp(action,"delete")== 0)
        {
            delete_file_helper(fname,bitmap);
            continue;
        }

        if(strcmp(action,"insend")== 0)
        {
            insert_at_end_helper(fname,outname,bitmap);
            continue;
        }

    }

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
    outname = "out.txt";
    //displaybitmap(bitmap);
    user_actions(fname,bitmap);
    // printf("----------------------\n");
    // displaybitmap(bitmap);



    


    // i = add_file(fname,fname,bitmap);
    // if(i == 1)
    // {
    //     printf("file added succesfully\n");
    // }
    // else
    // {
    //     printf("Unable to add file \n");
    // }

    // //insert_at_end(fname,fname,bitmap);
    // read_file(fname,outname);

    // int s = 3,t= 9;

    // char *out;
    // out = (char *)malloc( t * sizeof(char));
    // memset(out,0,t);
    // read_bytes(fname,outname,s,t,out);
    // printf("bytes = %s\n",out);
    //

    // printf("free size on disk = %d\n",get_free_disk_size(bitmap));

    // displaybitmap(bitmap);
    // dis_file_meta();
    // display_diskmeta();

    write_bitmap_disk(bitmap);

    close(disk_fd);
}



