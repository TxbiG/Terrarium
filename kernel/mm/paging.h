#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define PAGE_SIZE 4096U             // 4K
#define PAGE_SIZE_2M 0x200000U
#define PAGE_SIZE_1G 0x40000000ULL
#define PDPT_SIZE 0x8000000000U
#define PAGE_ENTRIES 512

// Page table entry flags
#define PTE_PRESENT   (1ULL << 0)
#define PTE_RW        (1ULL << 1)
#define PTE_USER      (1ULL << 2)
#define PTE_PWT       (1ULL << 3)
#define PTE_PCD       (1ULL << 4)
#define PTE_ACCESS    (1ULL << 5)
#define PTE_DIRTY     (1ULL << 6)
#define PTE_HUGE      (1ULL << 7)   // 1 = large page (2MB or 1GB)
#define PTE_GLOBAL    (1ULL << 8)
#define PTE_NOEXEC    (1ULL << 63)  // NX bit

typedef uint64_t page_entry_t;
typedef uint64_t page_dir_entry_t;
typedef uint64_t pdpt_entry_t;
typedef uint64_t pml4_entry_t;

typedef struct page_table {
    page_entry_t entries[PAGE_ENTRIES];
} page_table_t;

typedef struct {
    page_table_t* pml4;
    uint64_t pml4_phys_addr;
} paging_t;

// Initialize paging, set up PML4 and empty tables
void paging_init(paging_t* paging);

// Map a virtual address to physical address with given flags
void paging_map(paging_t* paging, uint64_t vaddr, uint64_t paddr, uint64_t flags);

// Unmap a virtual address
void paging_unmap(paging_t* paging, uint64_t vaddr);

// Translate virtual address to physical address (for debugging)
uint64_t paging_translate(paging_t* paging, uint64_t vaddr);

// Load page tables into CR3 and enable paging
void paging_enable(paging_t* paging);

paging_t* paging_create();

void paging_destroy(paging_t* paging);




//void initialize_paging(uint32_t total_frames, uint32_t ident_addr, uint32_t ident_len);

// Causes the specified page directory to be loaded into the CR3 register.
//void switch_page_directory(struct page_directory* new);

// Retrieves a pointer to the page required. If make == 1, if the page-table in which this page should reside isn't created, create it!
//struct page *get_page(uint32_t address, int make, struct page_directory *dir);

// Calls get_page with the kernel directory.
//struct page *get_kernel_page(uint32_t address, int make);

// Maps a kernel page. Returns NULL if it was unable to map.
//struct page *map_kernel_page(uint32_t address, int make);

// Unmaps a page from the kernel and returns its frame to the pool.
//void unmap_kernel_page(uint32_t address);
// Handler for page faults.
//void page_fault(registers_t regs);

#ifdef __cplusplus
}
#endif  // __cplusplus