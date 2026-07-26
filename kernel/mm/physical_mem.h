#ifndef TERRARIUM_PHYSICAL_MEM_H
#define TERRARIUM_PHYSICAL_MEM_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>

#include "paging.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_PHYS_MAX_REGIONS 128
#define TERRA_PHYS_MAX_PAGES 1048576u

typedef struct phys_mem_region {
    uintptr_t start;    // physical start address of memory region
    size_t length;      // length in bytes
} phys_mem_region_t;

typedef struct phys_mem_manager {
    phys_mem_region_t regions[TERRA_PHYS_MAX_REGIONS];
    size_t region_count;

    uint8_t bitmap[TERRA_PHYS_MAX_PAGES / 8u];
    size_t bitmap_size;     // bitmap size in bytes

    uintptr_t mem_start;    // physical address of first managed page
    uintptr_t mem_end;      // end physical address
    size_t total_pages;     // total pages managed
    size_t usable_pages;
    size_t used_pages;
    int ready;
} phys_mem_manager_t;

void phys_mem_init(phys_mem_region_t* regions, size_t region_count);
void phys_mem_init_from_boot(const terra_boot_info_t *boot_info);
int phys_mem_is_ready(void);

uintptr_t phys_alloc_page(void);
void phys_free_page(uintptr_t paddr);
uintptr_t phys_alloc_pages(size_t n);
void phys_free_pages(uintptr_t paddr, size_t n);
size_t phys_mem_total_pages(void);
size_t phys_mem_free_pages(void);
size_t phys_mem_used_pages(void);
uintptr_t phys_mem_start(void);
uintptr_t phys_mem_end(void);

#ifdef __cplusplus
}
#endif

#endif
