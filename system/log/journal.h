#ifndef TERRARIUM_SYSTEM_LOG_JOURNAL_H
#define TERRARIUM_SYSTEM_LOG_JOURNAL_H

#include <stdint.h>
#include "log.h"

typedef struct terra_journal_entry {
    uint64_t timestamp_ns;
    terra_log_level_t level;
    char component[64];
    char message[256];
} terra_journal_entry_t;

int terra_journal_append(const terra_journal_entry_t *entry);
int terra_journal_read(uint64_t cursor, terra_journal_entry_t *out_entry);

#endif
