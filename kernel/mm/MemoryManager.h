#ifndef TERRARIUM_MEMORY_MANAGER_H
#define TERRARIUM_MEMORY_MANAGER_H

#include <stddef.h>
#include <stdint.h>
#include <bootloader.h>
#include <status.h>

#include "paging.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_VM_READ  (1ull << 0)
#define TERRA_VM_WRITE (1ull << 1)
#define TERRA_VM_EXEC  (1ull << 2)
#define TERRA_VM_USER  (1ull << 3)

extern paging_t *current_paging;

void memmgr_init(const terra_boot_info_t *boot_info);
uintptr_t memmgr_alloc_phys_page(void);
void memmgr_free_phys_page(uintptr_t paddr);
void *memmgr_alloc_virtual(size_t size);
void memmgr_free_virtual(void *vaddr, size_t size);
int memmgr_map_page(uintptr_t phys_addr, void *virt_addr, uint64_t flags);
int memmgr_unmap_page(void *virt_addr);
int memmgr_map_range(uintptr_t phys_addr, void *virt_addr, size_t size, uint64_t flags);
int memmgr_unmap_range(void *virt_addr, size_t size);
uintptr_t memmgr_get_phys_for_virt(void *virt_addr);

#ifdef __cplusplus
}
#endif

#endif