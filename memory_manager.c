#include "memory_manager.h"
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

typedef struct Block {
    size_t size;
    int free;
    struct Block* next;
} Block;

static void* heap_start = NULL;
static size_t heap_size = 0;
static Block* free_list = NULL;

void mm_init(size_t size) {
    heap_size = size;
    heap_start = mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (heap_start == MAP_FAILED) {
        heap_start = NULL;
        return;
    }
    free_list = (Block*)heap_start;
    free_list->size = heap_size - sizeof(Block);
    free_list->free = 1;
    free_list->next = NULL;
}

void mm_destroy() {
    if (heap_start) {
        munmap(heap_start, heap_size);
        heap_start = NULL;
        free_list = NULL;
    }
}

static Block* find_free_block(size_t size) {
    Block* curr = free_list;
    while (curr) {
        if (curr->free && curr->size >= size) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

static void split_block(Block* block, size_t size) {
    Block* new_block = (Block*)((char*)block + sizeof(Block) + size);
    new_block->size = block->size - size - sizeof(Block);
    new_block->free = 1;
    new_block->next = block->next;
    block->size = size;
    block->free = 0;
    block->next = new_block;
}

void* mm_malloc(size_t size) {
    size = ALIGN(size);
    Block* block = find_free_block(size);
    if (block) {
        if (block->size > size + sizeof(Block)) {
            split_block(block, size);
        } else {
            block->free = 0;
        }
        return (void*)((char*)block + sizeof(Block));
    }
    return NULL;
}

void mm_free(void* ptr) {
    if (!ptr) return;
    Block* block = (Block*)((char*)ptr - sizeof(Block));
    block->free = 1;
    Block* curr = free_list;
    while (curr && curr->next) {
        if (curr->free && curr->next->free) {
            curr->size += sizeof(Block) + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

void* mm_realloc(void* ptr, size_t size) {
    if (!ptr) return mm_malloc(size);
    if (size == 0) {
        mm_free(ptr);
        return NULL;
    }
    Block* block = (Block*)((char*)ptr - sizeof(Block));
    if (block->size >= size) return ptr;
    void* new_ptr = mm_malloc(size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, block->size);
        mm_free(ptr);
    }
    return new_ptr;
}
