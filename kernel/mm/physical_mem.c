#include "physical_mem.h"

static phys_mem_manager_t phys_mem_mgr;

static uintptr_t align_up(uintptr_t value, uintptr_t align) {
    return (value + align - 1u) & ~(align - 1u);
}

static uintptr_t align_down(uintptr_t value, uintptr_t align) {
    return value & ~(align - 1u);
}

static void mem_zero(void *dst, size_t len) {
    uint8_t *out = (uint8_t *)dst;
    size_t i;
    for (i = 0; i < len; ++i)
        out[i] = 0;
}

static void mem_set(void *dst, uint8_t value, size_t len) {
    uint8_t *out = (uint8_t *)dst;
    size_t i;
    for (i = 0; i < len; ++i)
        out[i] = value;
}

static void bitmap_set(size_t bit) {
    phys_mem_mgr.bitmap[bit / 8u] |= (uint8_t)(1u << (bit % 8u));
}

static void bitmap_clear(size_t bit) {
    phys_mem_mgr.bitmap[bit / 8u] &= (uint8_t)~(1u << (bit % 8u));
}

static int bitmap_test(size_t bit) {
    return (phys_mem_mgr.bitmap[bit / 8u] >> (bit % 8u)) & 1u;
}

static void mark_page_used(size_t page) {
    if (page >= phys_mem_mgr.total_pages || bitmap_test(page))
        return;
    bitmap_set(page);
    ++phys_mem_mgr.used_pages;
}

static void mark_page_free(size_t page) {
    if (page >= phys_mem_mgr.total_pages || !bitmap_test(page))
        return;
    bitmap_clear(page);
    if (phys_mem_mgr.used_pages)
        --phys_mem_mgr.used_pages;
}

static void mark_range_free(uintptr_t start, uintptr_t end) {
    uintptr_t page_start;
    uintptr_t page_end;
    size_t first;
    size_t last;
    size_t p;

    if (end <= start || !phys_mem_mgr.total_pages)
        return;

    page_start = align_up(start, PAGE_SIZE);
    page_end = align_down(end, PAGE_SIZE);
    if (page_end <= page_start)
        return;
    if (page_end <= phys_mem_mgr.mem_start || page_start >= phys_mem_mgr.mem_end)
        return;
    if (page_start < phys_mem_mgr.mem_start)
        page_start = phys_mem_mgr.mem_start;
    if (page_end > phys_mem_mgr.mem_end)
        page_end = phys_mem_mgr.mem_end;

    first = (size_t)((page_start - phys_mem_mgr.mem_start) / PAGE_SIZE);
    last = (size_t)((page_end - phys_mem_mgr.mem_start) / PAGE_SIZE);
    if (last > phys_mem_mgr.total_pages)
        last = phys_mem_mgr.total_pages;

    for (p = first; p < last; ++p) {
        if (bitmap_test(p))
            ++phys_mem_mgr.usable_pages;
        mark_page_free(p);
    }
}

static void mark_range_used(uintptr_t start, uintptr_t end) {
    uintptr_t page_start;
    uintptr_t page_end;
    size_t first;
    size_t last;
    size_t p;

    if (end <= start || !phys_mem_mgr.total_pages)
        return;

    page_start = align_down(start, PAGE_SIZE);
    page_end = align_up(end, PAGE_SIZE);
    if (page_end <= page_start)
        return;
    if (page_end <= phys_mem_mgr.mem_start || page_start >= phys_mem_mgr.mem_end)
        return;
    if (page_start < phys_mem_mgr.mem_start)
        page_start = phys_mem_mgr.mem_start;
    if (page_end > phys_mem_mgr.mem_end)
        page_end = phys_mem_mgr.mem_end;

    first = page_start <= phys_mem_mgr.mem_start
        ? 0
        : (size_t)((page_start - phys_mem_mgr.mem_start) / PAGE_SIZE);
    last = (size_t)((page_end - phys_mem_mgr.mem_start) / PAGE_SIZE);
    if (last > phys_mem_mgr.total_pages)
        last = phys_mem_mgr.total_pages;

    for (p = first; p < last; ++p)
        mark_page_used(p);
}

void phys_mem_init(phys_mem_region_t* regions, size_t region_count) {
    size_t i;
    uintptr_t min_addr = UINTPTR_MAX;
    uintptr_t max_addr = 0;

    mem_zero(&phys_mem_mgr, sizeof(phys_mem_mgr));
    if (!regions || region_count == 0)
        return;

    if (region_count > TERRA_PHYS_MAX_REGIONS)
        region_count = TERRA_PHYS_MAX_REGIONS;

    for (i = 0; i < region_count; ++i) {
        uintptr_t start = align_down(regions[i].start, PAGE_SIZE);
        uintptr_t end = align_up(regions[i].start + regions[i].length, PAGE_SIZE);
        if (regions[i].length == 0)
            continue;
        if (start < min_addr)
            min_addr = start;
        if (end > max_addr)
            max_addr = end;
        phys_mem_mgr.regions[phys_mem_mgr.region_count++] = regions[i];
    }

    if (max_addr <= min_addr)
        return;

    phys_mem_mgr.mem_start = min_addr;
    phys_mem_mgr.mem_end = max_addr;
    phys_mem_mgr.total_pages = (max_addr - min_addr) / PAGE_SIZE;
    if (phys_mem_mgr.total_pages > TERRA_PHYS_MAX_PAGES)
        phys_mem_mgr.total_pages = TERRA_PHYS_MAX_PAGES;

    phys_mem_mgr.bitmap_size = (phys_mem_mgr.total_pages + 7u) / 8u;
    mem_set(phys_mem_mgr.bitmap, 0xffu, phys_mem_mgr.bitmap_size);
    phys_mem_mgr.used_pages = phys_mem_mgr.total_pages;

    for (i = 0; i < phys_mem_mgr.region_count; ++i)
        mark_range_free(phys_mem_mgr.regions[i].start, phys_mem_mgr.regions[i].start + phys_mem_mgr.regions[i].length);

    phys_mem_mgr.ready = 1;
}

void phys_mem_init_from_boot(const terra_boot_info_t *boot_info) {
    phys_mem_region_t regions[TERRA_PHYS_MAX_REGIONS];
    size_t count = 0;
    uint32_t i;

    if (!boot_info || boot_info->memory_map_count == 0) {
        phys_mem_init(0, 0);
        return;
    }

    for (i = 0; i < boot_info->memory_map_count && count < TERRA_PHYS_MAX_REGIONS; ++i) {
        const terra_memory_map_entry_t *entry = &boot_info->memory_map[i];
        if (entry->type != TERRA_MEMORY_USABLE || entry->length == 0)
            continue;
        regions[count].start = (uintptr_t)entry->base;
        regions[count].length = (size_t)entry->length;
        ++count;
    }

    phys_mem_init(regions, count);
    if (!phys_mem_mgr.ready)
        return;

    mark_range_used((uintptr_t)boot_info->kernel_phys_base, (uintptr_t)boot_info->kernel_phys_end);
    for (i = 0; i < boot_info->module_count && i < TERRA_BOOTLOADER_MAX_MODULES; ++i)
        mark_range_used((uintptr_t)boot_info->modules[i].base, (uintptr_t)(boot_info->modules[i].base + boot_info->modules[i].length));
    if (boot_info->framebuffer.address && boot_info->framebuffer.pitch && boot_info->framebuffer.height) {
        uintptr_t fb_start = (uintptr_t)boot_info->framebuffer.address;
        uintptr_t fb_end = fb_start + ((uintptr_t)boot_info->framebuffer.pitch * boot_info->framebuffer.height);
        mark_range_used(fb_start, fb_end);
    }
}

int phys_mem_is_ready(void) {
    return phys_mem_mgr.ready;
}

uintptr_t phys_alloc_page(void) {
    size_t page;
    if (!phys_mem_mgr.ready)
        return 0;

    for (page = 0; page < phys_mem_mgr.total_pages; ++page) {
        if (!bitmap_test(page)) {
            mark_page_used(page);
            return phys_mem_mgr.mem_start + (page * PAGE_SIZE);
        }
    }
    return 0;
}

void phys_free_page(uintptr_t paddr) {
    size_t page;
    if (!phys_mem_mgr.ready || paddr < phys_mem_mgr.mem_start || paddr >= phys_mem_mgr.mem_end)
        return;
    if ((paddr & (PAGE_SIZE - 1u)) != 0)
        return;

    page = (size_t)((paddr - phys_mem_mgr.mem_start) / PAGE_SIZE);
    if (page >= phys_mem_mgr.total_pages)
        return;
    mark_page_free(page);
}

uintptr_t phys_alloc_pages(size_t n) {
    size_t consecutive = 0;
    size_t start_page = 0;
    size_t i;
    size_t p;

    if (!phys_mem_mgr.ready || n == 0 || n > phys_mem_mgr.total_pages)
        return 0;

    for (i = 0; i < phys_mem_mgr.total_pages; ++i) {
        if (!bitmap_test(i)) {
            if (consecutive == 0)
                start_page = i;
            ++consecutive;
            if (consecutive == n) {
                for (p = start_page; p < start_page + n; ++p)
                    mark_page_used(p);
                return phys_mem_mgr.mem_start + start_page * PAGE_SIZE;
            }
        } else {
            consecutive = 0;
        }
    }
    return 0;
}

void phys_free_pages(uintptr_t paddr, size_t n) {
    size_t i;
    for (i = 0; i < n; ++i)
        phys_free_page(paddr + (i * PAGE_SIZE));
}

size_t phys_mem_total_pages(void) {
    return phys_mem_mgr.usable_pages;
}

size_t phys_mem_free_pages(void) {
    size_t count = 0;
    size_t i;
    if (!phys_mem_mgr.ready)
        return 0;
    for (i = 0; i < phys_mem_mgr.total_pages; ++i) {
        if (!bitmap_test(i))
            ++count;
    }
    return count;
}

size_t phys_mem_used_pages(void) {
    return phys_mem_mgr.used_pages;
}

uintptr_t phys_mem_start(void) {
    return phys_mem_mgr.mem_start;
}

uintptr_t phys_mem_end(void) {
    return phys_mem_mgr.mem_end;
}
