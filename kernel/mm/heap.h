#ifndef TERRARIUM_KERNEL_HEAP_H
#define TERRARIUM_KERNEL_HEAP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void heap_init(void *heap_start, size_t heap_size);
void *heap_malloc(size_t size);
void heap_free(void *ptr);
size_t heap_total_bytes(void);
size_t heap_free_bytes(void);

#ifdef __cplusplus
}
#endif

#endif
