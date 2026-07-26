#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef enum terra_swap_state {
    TERRA_SWAP_DISABLED = 0,
    TERRA_SWAP_READY = 1,
} terra_swap_state_t;

void swap_init(void);
int swap_enable(void *backing_store, size_t backing_store_size, size_t page_size);
void swap_disable(void);
terra_swap_state_t swap_state(void);
size_t swap_total_pages(void);
size_t swap_used_pages(void);
int swap_reserve_page(uintptr_t *out_slot);
int swap_release_page(uintptr_t slot);

#ifdef __cplusplus
}
#endif  // __cplusplus
