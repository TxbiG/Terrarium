
#include "heap.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct heap_block {
    size_t size;
    bool free;
    uint32_t magic;
    struct heap_block* next;
} heap_block_t;

#define HEAP_BLOCK_MAGIC 0x48454150u
#define HEAP_ALIGN 16u

static heap_block_t* heap_head = NULL;
static uintptr_t heap_base;
static uintptr_t heap_limit;
static size_t heap_total;

static size_t align_size(size_t size) {
    return (size + (HEAP_ALIGN - 1u)) & ~(size_t)(HEAP_ALIGN - 1u);
}

void heap_init(void* heap_start, size_t heap_size) {
    if (!heap_start || heap_size <= sizeof(heap_block_t)) {
        heap_head = NULL;
        heap_base = 0;
        heap_limit = 0;
        heap_total = 0;
        return;
    }

    heap_base = ((uintptr_t)heap_start + (HEAP_ALIGN - 1u)) & ~(uintptr_t)(HEAP_ALIGN - 1u);
    heap_limit = ((uintptr_t)heap_start + heap_size) & ~(uintptr_t)(HEAP_ALIGN - 1u);
    if (heap_limit <= heap_base + sizeof(heap_block_t)) {
        heap_head = NULL;
        heap_base = 0;
        heap_limit = 0;
        heap_total = 0;
        return;
    }

    heap_total = (size_t)(heap_limit - heap_base);
    heap_head = (heap_block_t*)heap_base;
    heap_head->size = heap_total - sizeof(heap_block_t);
    heap_head->free = true;
    heap_head->magic = HEAP_BLOCK_MAGIC;
    heap_head->next = NULL;
}

void* heap_malloc(size_t size) {
    if (size == 0 || !heap_head) return NULL;
    size = align_size(size);

    heap_block_t* current = heap_head;

    while (current) {
        if (current->magic != HEAP_BLOCK_MAGIC)
            return NULL;
        if (current->free && current->size >= size) {
            size_t remaining = current->size - size;

            if (remaining > sizeof(heap_block_t) + HEAP_ALIGN) {
                heap_block_t* next_block = (heap_block_t*)((uint8_t*)current + sizeof(heap_block_t) + size);
                next_block->size = remaining - sizeof(heap_block_t);
                next_block->free = true;
                next_block->magic = HEAP_BLOCK_MAGIC;
                next_block->next = current->next;

                current->size = size;
                current->next = next_block;
            }

            current->free = false;
            return (void*)((uint8_t*)current + sizeof(heap_block_t));
        }
        current = current->next;
    }

    return NULL; // No suitable block found
}

void heap_free(void* ptr) {
    if (!ptr) return;
    if ((uintptr_t)ptr < heap_base + sizeof(heap_block_t) || (uintptr_t)ptr >= heap_limit)
        return;

    heap_block_t* block = (heap_block_t*)((uint8_t*)ptr - sizeof(heap_block_t));
    if (block->magic != HEAP_BLOCK_MAGIC)
        return;
    block->free = true;

    // Coalesce adjacent free blocks
    heap_block_t* current = heap_head;
    while (current && current->next) {
        if (current->magic != HEAP_BLOCK_MAGIC || current->next->magic != HEAP_BLOCK_MAGIC)
            return;
        if (current->free && current->next->free) {
            current->size += sizeof(heap_block_t) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

size_t heap_total_bytes(void) {
    return heap_total;
}

size_t heap_free_bytes(void) {
    size_t total = 0;
    heap_block_t* current = heap_head;
    while (current) {
        if (current->magic != HEAP_BLOCK_MAGIC)
            break;
        if (current->free)
            total += current->size;
        current = current->next;
    }
    return total;
}
