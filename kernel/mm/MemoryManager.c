#include "MemoryManager.h"

#include "heap.h"
#include "physical_mem.h"

static paging_t kernel_paging;

paging_t *current_paging = 0;

static uint64_t vm_to_pte_flags(uint64_t flags) {
    uint64_t pte = PTE_PRESENT;
    if (flags & TERRA_VM_WRITE)
        pte |= PTE_RW;
    if (flags & TERRA_VM_USER)
        pte |= PTE_USER;
    if (!(flags & TERRA_VM_EXEC))
        pte |= PTE_NOEXEC;
    return pte;
}

void memmgr_init(const terra_boot_info_t *boot_info) {
    phys_mem_init_from_boot(boot_info);
    paging_init(&kernel_paging);
    current_paging = &kernel_paging;
}

uintptr_t memmgr_alloc_phys_page(void) {
    return phys_alloc_page();
}

void memmgr_free_phys_page(uintptr_t paddr) {
    phys_free_page(paddr);
}

void *memmgr_alloc_virtual(size_t size) {
    return heap_malloc(size);
}

void memmgr_free_virtual(void *vaddr, size_t size) {
    (void)size;
    heap_free(vaddr);
}

int memmgr_map_page(uintptr_t phys_addr, void *virt_addr, uint64_t flags) {
    uintptr_t aligned_phys;
    uintptr_t aligned_virt;

    if (!current_paging || !virt_addr || phys_addr == 0)
        return TERRA_STATUS_INVAL;

    aligned_phys = phys_addr & ~(uintptr_t)(PAGE_SIZE - 1u);
    aligned_virt = (uintptr_t)virt_addr & ~(uintptr_t)(PAGE_SIZE - 1u);
    paging_map(current_paging, aligned_virt, aligned_phys, vm_to_pte_flags(flags));
    return paging_translate(current_paging, aligned_virt) == aligned_phys ? TERRA_STATUS_OK : TERRA_STATUS_IO;
}

int memmgr_unmap_page(void *virt_addr) {
    if (!current_paging || !virt_addr)
        return TERRA_STATUS_INVAL;

    paging_unmap(current_paging, (uintptr_t)virt_addr);
    return TERRA_STATUS_OK;
}

int memmgr_map_range(uintptr_t phys_addr, void *virt_addr, size_t size, uint64_t flags) {
    uintptr_t phys;
    uintptr_t virt;
    size_t offset;

    if (!virt_addr || size == 0)
        return TERRA_STATUS_INVAL;

    phys = phys_addr & ~(uintptr_t)(PAGE_SIZE - 1u);
    virt = (uintptr_t)virt_addr & ~(uintptr_t)(PAGE_SIZE - 1u);
    for (offset = 0; offset < size; offset += PAGE_SIZE) {
        int mapped = memmgr_map_page(phys + offset, (void *)(virt + offset), flags);
        if (mapped != TERRA_STATUS_OK)
            return mapped;
    }

    return TERRA_STATUS_OK;
}

int memmgr_unmap_range(void *virt_addr, size_t size) {
    uintptr_t virt;
    size_t offset;

    if (!virt_addr || size == 0)
        return TERRA_STATUS_INVAL;

    virt = (uintptr_t)virt_addr & ~(uintptr_t)(PAGE_SIZE - 1u);
    for (offset = 0; offset < size; offset += PAGE_SIZE) {
        int unmapped = memmgr_unmap_page((void *)(virt + offset));
        if (unmapped != TERRA_STATUS_OK)
            return unmapped;
    }

    return TERRA_STATUS_OK;
}

uintptr_t memmgr_get_phys_for_virt(void *virt_addr) {
    if (!current_paging || !virt_addr)
        return 0;

    return paging_translate(current_paging, (uintptr_t)virt_addr);
}