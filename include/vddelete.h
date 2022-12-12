#ifndef VDDELETE_H
#define VDDELETE_H

#include"diskdriver.h"
#include"filenode.h"
#include"bitio.h"
#include"diskfunc.h"

int delete_file(char *fname,char *bitmap);

int delete_at_end(char *fname,int delnum,char *bitmap);

int delete_file2(char *fname,Heap *bitmap);

void file_delete_helper(char *bitmap,int levels,int prev_block,int *level_data,int *size);
#endif