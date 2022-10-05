#ifndef BITIO_H
#define BITIO_H
#include<stdio.h>
#include"diskdriver.h"

void clear_bit(char *buf,int num);

void set_bit(char *buf,int num);

void write_bitmap_disk(char *bitmap);

int find_empty_block(char *bitmap);

#endif