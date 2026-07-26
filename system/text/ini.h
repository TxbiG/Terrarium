#ifndef TERRARIUM_SYSTEM_TEXT_INI_H
#define TERRARIUM_SYSTEM_TEXT_INI_H

typedef int (*terra_ini_callback_t)(const char *section, const char *key, const char *value, void *user);

int terra_ini_parse_file(const char *path, terra_ini_callback_t callback, void *user);
int terra_ini_parse_string(const char *text, terra_ini_callback_t callback, void *user);

#endif
