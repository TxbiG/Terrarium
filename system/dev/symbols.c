#include "symbols.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../status.h"

#define TERRA_SYMBOL_MAX 128u

typedef struct terra_symbol_entry {
    uintptr_t address;
    char name[128];
} terra_symbol_entry_t;

static terra_symbol_entry_t g_symbols[TERRA_SYMBOL_MAX];
static unsigned g_symbol_count;

int terra_symbols_register(void *address, const char *name) {
    if (!address || !name || !name[0])
        return TERRA_STATUS_INVAL;
    uintptr_t addr = (uintptr_t)address;
    for (unsigned i = 0; i < g_symbol_count; ++i) {
        if (g_symbols[i].address == addr) {
            snprintf(g_symbols[i].name, sizeof(g_symbols[i].name), "%s", name);
            return TERRA_STATUS_OK;
        }
    }
    if (g_symbol_count >= TERRA_SYMBOL_MAX)
        return TERRA_STATUS_NOSPC;
    g_symbols[g_symbol_count].address = addr;
    snprintf(g_symbols[g_symbol_count].name, sizeof(g_symbols[g_symbol_count].name), "%s", name);
    ++g_symbol_count;
    return TERRA_STATUS_OK;
}

int terra_symbols_lookup(void *address, char *out_name, unsigned long out_size) {
    if (!address || !out_name || out_size == 0)
        return TERRA_STATUS_INVAL;
    uintptr_t addr = (uintptr_t)address;
    int best = -1;
    for (unsigned i = 0; i < g_symbol_count; ++i) {
        if (g_symbols[i].address <= addr && (best < 0 || g_symbols[i].address > g_symbols[best].address))
            best = (int)i;
    }
    if (best < 0) {
        out_name[0] = '\0';
        return TERRA_STATUS_NOENT;
    }
    snprintf(out_name, out_size, "%s", g_symbols[best].name);
    return TERRA_STATUS_OK;
}

int terra_symbols_load(const char *path) {
    if (!path || !path[0])
        return TERRA_STATUS_INVAL;
    FILE *file = fopen(path, "rb");
    if (!file)
        return TERRA_STATUS_NOENT;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        unsigned long long addr = 0;
        char name[128];
        if (sscanf(line, "%llx %127s", &addr, name) == 2)
            terra_symbols_register((void *)(uintptr_t)addr, name);
    }
    fclose(file);
    return TERRA_STATUS_OK;
}

int terra_symbols_count(void) {
    return (int)g_symbol_count;
}
