#include "swap.h"

#define SWAP_MAX_PAGES 4096u

static terra_swap_state_t current_state;
static uint8_t *swap_store;
static size_t swap_store_size;
static size_t swap_page_size;
static size_t swap_pages;
static uint8_t swap_bitmap[SWAP_MAX_PAGES];
static size_t swap_used;

void swap_init(void) {
    current_state = TERRA_SWAP_DISABLED;
    swap_store = 0;
    swap_store_size = 0;
    swap_page_size = 0;
    swap_pages = 0;
    swap_used = 0;

    for (size_t i = 0; i < SWAP_MAX_PAGES; ++i)
        swap_bitmap[i] = 0;
}

int swap_enable(void *backing_store, size_t backing_store_size, size_t page_size) {
    if (!backing_store || backing_store_size == 0 || page_size == 0)
        return -1;

    size_t pages = backing_store_size / page_size;
    if (pages == 0 || pages > SWAP_MAX_PAGES)
        return -1;

    swap_init();
    swap_store = (uint8_t *)backing_store;
    swap_store_size = backing_store_size;
    swap_page_size = page_size;
    swap_pages = pages;
    current_state = TERRA_SWAP_READY;
    return 0;
}

void swap_disable(void) {
    swap_init();
}

terra_swap_state_t swap_state(void) {
    return current_state;
}

size_t swap_total_pages(void) {
    return swap_pages;
}

size_t swap_used_pages(void) {
    return swap_used;
}

int swap_reserve_page(uintptr_t *out_slot) {
    if (current_state != TERRA_SWAP_READY || !out_slot)
        return -1;

    for (size_t i = 0; i < swap_pages; ++i) {
        if (!swap_bitmap[i]) {
            swap_bitmap[i] = 1;
            ++swap_used;
            *out_slot = (uintptr_t)(swap_store + (i * swap_page_size));
            return 0;
        }
    }

    return -1;
}

int swap_release_page(uintptr_t slot) {
    if (current_state != TERRA_SWAP_READY || slot < (uintptr_t)swap_store)
        return -1;

    uintptr_t offset = slot - (uintptr_t)swap_store;
    if (offset >= swap_store_size || (offset % swap_page_size) != 0)
        return -1;

    size_t index = offset / swap_page_size;
    if (index >= swap_pages || !swap_bitmap[index])
        return -1;

    swap_bitmap[index] = 0;
    --swap_used;
    return 0;
}
