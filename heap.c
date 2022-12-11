#include"heap.h"

void swap(Heap a, Heap b)
{
    Heap temp = b;
    b = a;
    a = temp;
}


void heapify(Heap *bitheap, int size, int i)
{
    if (size == 1)
    {
        return;
    }
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < size && bitheap[l] . size > bitheap[largest] . size)
        largest = l;
    if (r < size && bitheap[r] . size > bitheap[largest] . size)
        largest = r;
    if (largest != i)
    {
        swap(bitheap[i], bitheap[largest]);
        heapify(bitheap, size, largest);
    }
}

void add_node(Heap *bitheap,int start, int size)
{
    int i,j;
    // Heap *node;
    // node =(Heap *) malloc(sizeof(Heap));
    Heap node;
    node . start = start;
    node . size = size;
    if (heap_size == 0)
    {
        bitheap[0] = node;
        heap_size += 1;
    }
    else
    {
        bitheap[(heap_total_size - 1)] = node;
        heap_size += 1;
        for (int i = heap_total_size / 2 - 1; i >= 0; i--)
        {
            heapify(bitheap, heap_total_size, i);
        }
    }
}


Heap *build_heap()
{
    int i,j,k,size,n,num,start;
    Heap *bitheap;
    heap_total_size = 16;
    bitheap = (Heap *) malloc(heap_total_size * sizeof(Heap));
    heap_size = 0;

    char bitmap[disk_meta.blk_size], bit;
    size = 0;
    start = 0;
    n = 0;
    j = 1;
    while(j <= total_blocks_req_bits)
    {   
        read_block(buf,j);
        k = 0;
        while(k < disk_meta . blk_size)
        {
            bitmap[i] = buf[k];
            i++;
            k++;
        }
        k = 0;
        while(k < disk_meta . blk_size)
        {
            if(bitmap[k] == 0)
            {
                continue;
            }
            bit = bitmap[k];
            for(i=0;i<8;i++)
            {
                n = (bit<<i) & (0x80);
                if(n == 0x80)
                {
                    num = i + 1;
                    if(start == 0)
                        start = (j*k*8)+num;
                    size++;
                }
                else
                {
                    if(start != 0)
                    {
                        add_node(bitheap,start,size);
                        start = 0;
                        size = 0;
                        n++;
                    }
                    if(n == (heap_total_size - 1))
                    {
                        return bitheap;
                    }
                }
            }
        }
        j++;
    }
    return bitheap;
}
