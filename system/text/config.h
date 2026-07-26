#ifndef TERRARIUM_SYSTEM_TEXT_CONFIG_H
#define TERRARIUM_SYSTEM_TEXT_CONFIG_H

typedef struct terra_config {
    void *opaque;
} terra_config_t;

int terra_config_load(const char *path, terra_config_t *out_config);
int terra_config_get(const terra_config_t *config, const char *key, char *out_value, unsigned long out_size);
void terra_config_close(terra_config_t *config);

#endif
