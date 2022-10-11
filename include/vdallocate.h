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
#include"bitio.h"
#include"disk_util.h"
#include"filenode.h"
#include"diskfunc.h"
#include"vdadd.h"
#include"vdread.h"





int delete_file(char *fname,char *bitmap);

int delete_at_end(char *fname,int delnum,char *bitmap);

#endif