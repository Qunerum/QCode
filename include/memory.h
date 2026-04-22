#ifndef MEMORY_H
#define MEMORY_H

#ifdef _WIN32
#include <stddef.h> // Windows
#else
typedef unsigned long size_t;
#endif

typedef struct MemoryBlock {
    size_t size;
    int free;
    struct MemoryBlock* next;
} MemoryBlock;

void init_memory();
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif
