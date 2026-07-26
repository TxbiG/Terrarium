#include "paging.h"

#include "heap.h"

static page_table_t *paging_alloc_table(void) {
    uintptr_t raw = (uintptr_t)heap_malloc(sizeof(page_table_t) + PAGE_SIZE);
    if (!raw)
        return 0;

    page_table_t *table = (page_table_t *)((raw + PAGE_SIZE - 1u) & ~(uintptr_t)(PAGE_SIZE - 1u));

    for (size_t i = 0; i < PAGE_ENTRIES; ++i)
        table->entries[i] = 0;

    return table;
}

static size_t pml4_index(uint64_t vaddr) {
    return (size_t)((vaddr >> 39) & 0x1ffu);
}

static size_t pdpt_index(uint64_t vaddr) {
    return (size_t)((vaddr >> 30) & 0x1ffu);
}

static size_t pd_index(uint64_t vaddr) {
    return (size_t)((vaddr >> 21) & 0x1ffu);
}

static size_t pt_index(uint64_t vaddr) {
    return (size_t)((vaddr >> 12) & 0x1ffu);
}

static page_table_t *entry_table(page_entry_t entry) {
    return (page_table_t *)(uintptr_t)(entry & ~0xfffull);
}

static page_entry_t table_entry(page_table_t *table, uint64_t flags) {
    return ((uint64_t)(uintptr_t)table & ~0xfffull) | flags | PTE_PRESENT | PTE_RW;
}

static page_table_t *ensure_next_table(page_table_t *table, size_t index, uint64_t flags) {
    if (!(table->entries[index] & PTE_PRESENT)) {
        page_table_t *next = paging_alloc_table();
        if (!next)
            return 0;

        table->entries[index] = table_entry(next, flags);
    }

    return entry_table(table->entries[index]);
}

void paging_init(paging_t *paging) {
    if (!paging)
        return;

    paging->pml4 = paging_alloc_table();
    paging->pml4_phys_addr = (uint64_t)(uintptr_t)paging->pml4;
}

void paging_map(paging_t *paging, uint64_t vaddr, uint64_t paddr, uint64_t flags) {
    if (!paging || !paging->pml4 || paddr == 0)
        return;

    vaddr &= ~0xfffull;
    paddr &= ~0xfffull;

    page_table_t *pml4 = paging->pml4;
    size_t pml4i = pml4_index(vaddr);
    page_table_t *pdpt = ensure_next_table(pml4, pml4i, flags);
    if (!pdpt)
        return;

    size_t pdpti = pdpt_index(vaddr);
    page_table_t *pd = ensure_next_table(pdpt, pdpti, flags);
    if (!pd)
        return;

    size_t pdi = pd_index(vaddr);
    page_table_t *pt = ensure_next_table(pd, pdi, flags);
    if (!pt)
        return;

    pt->entries[pt_index(vaddr)] = (paddr & ~0xfffull) | flags | PTE_PRESENT;
}

void paging_unmap(paging_t *paging, uint64_t vaddr) {
    page_entry_t pml4e;
    page_entry_t pdpte;
    page_entry_t pde;

    if (!paging || !paging->pml4)
        return;

    pml4e = paging->pml4->entries[pml4_index(vaddr)];
    if (!(pml4e & PTE_PRESENT))
        return;

    page_table_t *pdpt = entry_table(pml4e);
    if (!pdpt)
        return;

    pdpte = pdpt->entries[pdpt_index(vaddr)];
    if (!(pdpte & PTE_PRESENT))
        return;

    page_table_t *pd = entry_table(pdpte);
    if (!pd)
        return;

    pde = pd->entries[pd_index(vaddr)];
    if (!(pde & PTE_PRESENT))
        return;

    page_table_t *pt = entry_table(pde);
    if (!pt)
        return;

    pt->entries[pt_index(vaddr)] = 0;
}

uint64_t paging_translate(paging_t *paging, uint64_t vaddr) {
    if (!paging || !paging->pml4)
        return 0;

    page_entry_t pml4e = paging->pml4->entries[pml4_index(vaddr)];
    if (!(pml4e & PTE_PRESENT))
        return 0;

    page_table_t *pdpt = entry_table(pml4e);
    page_entry_t pdpte = pdpt->entries[pdpt_index(vaddr)];
    if (!(pdpte & PTE_PRESENT))
        return 0;

    page_table_t *pd = entry_table(pdpte);
    page_entry_t pde = pd->entries[pd_index(vaddr)];
    if (!(pde & PTE_PRESENT))
        return 0;

    page_table_t *pt = entry_table(pde);
    page_entry_t pte = pt->entries[pt_index(vaddr)];
    if (!(pte & PTE_PRESENT))
        return 0;

    return (pte & ~0xfffull) | (vaddr & 0xfffull);
}

void paging_enable(paging_t *paging) {
    (void)paging;
}

paging_t *paging_create(void) {
    paging_t *paging = (paging_t *)heap_malloc(sizeof(paging_t));
    if (!paging)
        return 0;

    paging_init(paging);
    if (!paging->pml4) {
        heap_free(paging);
        return 0;
    }

    return paging;
}

void paging_destroy(paging_t *paging) {
    if (!paging)
        return;

    heap_free(paging);
}
