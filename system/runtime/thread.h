#ifndef TERRARIUM_SYSTEM_RUNTIME_THREAD_H
#define TERRARIUM_SYSTEM_RUNTIME_THREAD_H

#include <stdint.h>

typedef uint64_t terra_tid_t;
typedef int (*terra_thread_entry_t)(void *user);

terra_tid_t terra_thread_create(terra_thread_entry_t entry, void *user);
int terra_thread_join(terra_tid_t tid, int *out_status);
void terra_thread_yield(void);
terra_tid_t terra_thread_current(void);

#endif
