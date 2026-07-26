#ifndef TERRARIUM_SYSTEM_TEXT_YAML_H
#define TERRARIUM_SYSTEM_TEXT_YAML_H

int terra_yaml_validate(const char *text);
int terra_yaml_get_string(const char *text, const char *path, char *out_value, unsigned long out_size);

#endif
