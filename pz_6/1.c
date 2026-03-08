#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct block_meta {
    size_t size;
    bool is_free;
    struct block_meta *next;
    struct block_meta *prev;
} block_meta_t;

#define META_SIZE sizeof(block_meta_t)

block_meta_t *global_base = NULL;

block_meta_t *find_free_block(block_meta_t **last, size_t size) {
    block_meta_t *current = global_base;
    while (current && !(current->is_free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}

block_meta_t *request_space(block_meta_t *last, size_t size) {
    block_meta_t *block = sbrk(0);
    void *request = sbrk(size + META_SIZE);
    
    if (request == (void*) -1) return NULL;
    
    if (last) {
        last->next = block;
        block->prev = last;
    } else {
        block->prev = NULL;
    }
    
    block->size = size;
    block->next = NULL;
    block->is_free = false;
    return block;
}

void *my_malloc(size_t size) {
    if (size <= 0) return NULL;
    
    block_meta_t *block;
    
    if (!global_base) {
        block = request_space(NULL, size);
        if (!block) return NULL;
        global_base = block;
    } else {
        block_meta_t *last = global_base;
        block = find_free_block(&last, size);
        if (!block) {
            block = request_space(last, size);
            if (!block) return NULL;
        } else {
            block->is_free = false;
        }
    }
    return (block + 1);
}

void merge_blocks(block_meta_t *block) {
    if (block->next && block->next->is_free) {
        block->size += block->next->size + META_SIZE;
        block->next = block->next->next;
        if (block->next) block->next->prev = block;
    }
    if (block->prev && block->prev->is_free) {
        block_meta_t *prev = block->prev;
        prev->size += block->size + META_SIZE;
        prev->next = block->next;
        if (block->next) block->next->prev = prev;
    }
}

void my_free(void *ptr) {
    if (!ptr) return;
    
    block_meta_t *block = (block_meta_t *)ptr - 1;
    block->is_free = true;
    merge_blocks(block);
}

int main() {
    printf("Starting custom allocator test...\n");
    
    void *p1 = my_malloc(128);
    void *p2 = my_malloc(256);
    void *p3 = my_malloc(512);
    
    printf("Allocated 3 blocks.\n");
    
    my_free(p2);
    printf("Freed middle block (no merge yet).\n");
    
    my_free(p1);
    printf("Freed first block (merges with middle).\n");
    
    my_free(p3);
    printf("Freed last block (all blocks coalesced).\n");
    
    return 0;
}
