#ifndef HEAP_H
#define HEAP_H

#include"diskdriver.h"
#include<string.h>
#include<math.h>

typedef struct Heap
{
    unsigned long start;
    unsigned long size;
}Heap;

Heap *build_heap();
void delete_node(Heap *bitheap);
void add_node(Heap *bitheap,int start, int size);
void display_heap(Heap *bitheap);

#endif