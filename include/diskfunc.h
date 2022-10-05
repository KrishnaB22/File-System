#ifndef DISKFUNC_H
#define DISKFUNC_H

#include"diskdriver.h"
#include<string.h>

char *disk_init();

int get_free_disk_size(char *bitmap);

#endif