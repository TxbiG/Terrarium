#ifndef TERRARIUM_SYSTEM_BOOT_CMDLINE_H
#define TERRARIUM_SYSTEM_BOOT_CMDLINE_H

const char *terra_cmdline_get(void);
int terra_cmdline_get_value(const char *key, char *out_value, unsigned long out_size);
int terra_cmdline_has_flag(const char *flag);

#endif
