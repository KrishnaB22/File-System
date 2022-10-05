#ifndef VDALLOCATE_H
#define VDALLOCATE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>

//----------------------
#include"diskdriver.h"
#include"bitmap.h"
#include"disk_util.h"
#include"filenode.h"
#include"diskfunc.h"

int add_file(char *fname,char *bitmap);

void read_file(char *fname,char *outname);

int delete_file(char *fname,char *bitmap);

int insert_at_end(char *fname,char *fname1,char *bitmap);

int delete_at_end(char *fname,int delnum,char *bitmap);

#endif