#ifndef TERRARIUM_SYSTEM_DEV_SYMBOLS_H
#define TERRARIUM_SYSTEM_DEV_SYMBOLS_H

int terra_symbols_lookup(void *address, char *out_name, unsigned long out_size);
int terra_symbols_load(const char *path);
int terra_symbols_register(void *address, const char *name);
int terra_symbols_count(void);

#endif
