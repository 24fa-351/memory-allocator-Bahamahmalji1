#include "memory_manager.h"
#include <stdio.h>
#include <string.h>

void test_allocation() {
    void* ptr = mm_malloc(64);
    if (ptr) printf("Allocation test passed\n");
    else printf("Allocation test failed\n");
    mm_free(ptr);
}

void test_reallocation() {
    void* ptr = mm_malloc(64);
    memset(ptr, 'A', 64);
    ptr = mm_realloc(ptr, 128);
    if (ptr) printf("Reallocation test passed\n");
    else printf("Reallocation test failed\n");
    mm_free(ptr);
}

void test_free() {
    void* ptr = mm_malloc(64);
    mm_free(ptr);
    printf("Free test passed\n");
}

int main() {
    mm_init(1024);
    test_allocation();
    test_reallocation();
    test_free();
    mm_destroy();
    return 0;
}
