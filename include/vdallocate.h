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


char *disk_init();

int get_free_disk_size(char *bitmap);

int add_file(char *fname,char *bitmap);

void read_file(char *fname);

#endif