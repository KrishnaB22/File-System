#ifndef VDREAD_H
#define VDREAD_H

#include"diskdriver.h"
#include"filenode.h"
#include"bitio.h"
#include"diskfunc.h"
#include<string.h>

int read_file(char *fname,char *outname);

int read_bytes(char *fname,char *outname, int startnum,int tnum,char *out);

int read_file2(char *fname,char *outname);

void file_read_helper(int file_fd,int levels,int prev_block,int *level_data,int *size);

#endif