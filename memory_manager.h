#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>

void* mm_malloc(size_t size);
void mm_free(void* ptr);
void* mm_realloc(void* ptr, size_t size);
void mm_init(size_t heap_size);
void mm_destroy();

#endif
