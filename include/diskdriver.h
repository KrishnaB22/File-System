#ifndef DRIVER_H
#define DRIVER_H

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<errno.h>
#include"diskinfo.h"


int write_block(char *buf,int blk);

int read_block(char *buf,int blk);

#endif