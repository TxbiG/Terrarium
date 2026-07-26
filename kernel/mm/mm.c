#include "mm.h"

#include "heap.h"
#include "MemoryManager.h"
#include "physical_mem.h"
#include "swap.h"

#include <stddef.h>
#include <stdint.h>

#define BOOT_HEAP_SIZE (256u * 1024u)

static uint8_t boot_heap[BOOT_HEAP_SIZE];
static int mm_ready;
static size_t mm_total_memory;
static size_t mm_available_memory;

void mm_init(const terra_boot_info_t *boot_info) {
    heap_init(boot_heap, sizeof(boot_heap));
    memmgr_init(boot_info);
    swap_init();

    mm_total_memory = phys_mem_total_pages() * PAGE_SIZE;
    mm_available_memory = phys_mem_free_pages() * PAGE_SIZE;
    if (mm_total_memory == 0) {
        mm_total_memory = sizeof(boot_heap);
        mm_available_memory = heap_free_bytes();
    }
    mm_ready = 1;
}

void mm_terminate(void) {
    mm_ready = 0;
}

int mm_is_ready(void) {
    return mm_ready;
}

size_t mm_total_memory_bytes(void) {
    return mm_total_memory;
}

size_t mm_available_memory_bytes(void) {
    if (phys_mem_is_ready())
        return phys_mem_free_pages() * PAGE_SIZE;
    return heap_free_bytes();
}
