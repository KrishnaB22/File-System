#ifndef VDADD_H
#define VDADD_H

#include"diskdriver.h"
#include"filenode.h"
#include"bitio.h"
#include"diskfunc.h"
#include<string.h>
#include<math.h>

int add_file(char *fname,char *outname,char *bitmap);

int insert_at_end(char *fname,char *fname1,char *bitmap);

void file_add_helper(int file_fd,char *bitmap,int level,int prev_block,int *level_data);

int add_file2(char *fname,char *outname,char *bitmap);

#endif