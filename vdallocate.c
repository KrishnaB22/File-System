#include"vdallocate.h"

unsigned int free_disk_space = 0;


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
            printf("adding file\n");
            add_file(fname,outname,bitmap);
            continue;
        }

        if(strcmp(action,"read")== 0)
        {
            printf("reading file\n");
            read_file(fname,outname);
            continue;
        }

        if(strcmp(action,"delete")== 0)
        {
            delete_file(fname,bitmap);
            continue;
        }

        if(strcmp(action,"insend")== 0)
        {
            insert_at_end(fname,outname,bitmap);
            continue;
        }

    }

}

int main(int argc,char **argv)
{
    int i,j,k;
    char *bitmap;
    bitmap = disk_init();
    //displaybitmap(bitmap);

    char *fname,*outname;
    fname = (char *)malloc(200*sizeof(char));
    outname = (char *)malloc(200*sizeof(char));

    fname = argv[1];
    outname = "out.txt";

    user_actions(fname,bitmap);




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
    


    // printf("free size on disk = %d\n",get_free_disk_size(bitmap));

    // displaybitmap(bitmap);
    // dis_file_meta();
    // display_diskmeta();

    write_bitmap_disk(bitmap);

    close(disk_fd);
}



