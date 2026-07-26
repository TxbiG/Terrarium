#ifndef TERRARIUM_SYSTEM_BOOT_SYSCTL_H
#define TERRARIUM_SYSTEM_BOOT_SYSCTL_H

int terra_sysctl_get(const char *key, char *out_value, unsigned long out_size);
int terra_sysctl_set(const char *key, const char *value);

#endif
