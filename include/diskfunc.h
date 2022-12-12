#ifndef DISKFUNC_H
#define DISKFUNC_H

#include"diskdriver.h"
#include"heap.h"
#include<string.h>
#include<math.h>

Heap *disk_init();

int get_free_disk_size(char *bitmap);

int get_free_disk_size2();

int get_levels(int *level_data , unsigned int file_size);

#endif