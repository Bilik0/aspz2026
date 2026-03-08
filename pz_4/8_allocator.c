#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#define PROT_READ      0x1
#define PROT_WRITE     0x2
#define MAP_PRIVATE    0x02
#define MAP_ANONYMOUS  0x20
#define MAP_FAILED     ((void *) -1)

extern void *mmap(void *addr, size_t length, int prot, int flags, int fd, long int offset);
extern int munmap(void *addr, size_t length);

#define MMAP_THRESHOLD (128 * 1024)

typedef struct block_meta {
    size_t size;
    bool is_free;
    bool is_mmaped;
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
    block->is_mmaped = false;
    return block;
}

void split_block(block_meta_t *block, size_t size) {
    if (block->size >= size + META_SIZE + 1) {
        block_meta_t *new_block = (block_meta_t *)((char *)block + META_SIZE + size);
        new_block->size = block->size - size - META_SIZE;
        new_block->is_free = true;
        new_block->is_mmaped = false;
        new_block->next = block->next;
        new_block->prev = block;
        if (block->next) block->next->prev = new_block;
        block->next = new_block;
        block->size = size;
        printf("[brk] Splitting %zu-byte block at %p\n", new_block->size + size + META_SIZE, (void*)(block + 1));
    }
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
        block = prev;
    }
    printf("[merge] Free block is now %zu bytes\n", block->size);
}

void *my_malloc(size_t size) {
    if (size <= 0) return NULL;
    block_meta_t *block;

    if (size >= MMAP_THRESHOLD) {
        block = mmap(NULL, size + META_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (block == MAP_FAILED) return NULL;
        block->size = size;
        block->is_free = false;
        block->is_mmaped = true;
        printf("[mmap] Allocated %zu bytes at %p\n", size, (void*)(block + 1));
        return (block + 1);
    }

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
            split_block(block, size);
        }
    }
    printf("[brk] Allocated %zu bytes at %p\n", size, (void*)(block + 1));
    return (block + 1);
}

void my_free(void *ptr) {
    if (!ptr) return;
    block_meta_t *block = (block_meta_t *)ptr - 1;

    if (block->is_mmaped) {
        printf("[munmap] Freed %zu bytes at %p\n", block->size, ptr);
        munmap(block, block->size + META_SIZE);
    } else {
        block->is_free = true;
        printf("[free] Freed %zu bytes at %p\n", block->size, ptr);
        merge_blocks(block);
    }
}

int main() {
    void *p1 = my_malloc(1024);
    void *p2 = my_malloc(512);
    void *p3 = my_malloc(200000); 
    
    my_free(p1); 
    void *p4 = my_malloc(256);
    
    my_free(p2);
    my_free(p4);
    my_free(p3);
    return 0;
}
