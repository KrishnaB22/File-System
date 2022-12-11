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

#endif