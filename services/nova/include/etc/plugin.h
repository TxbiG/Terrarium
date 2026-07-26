#ifndef TERRARIUM_NOVA_PLUGIN_H
#define TERRARIUM_NOVA_PLUGIN_H

typedef struct nova_plugin {
    const char *name;
    int (*start)(void *context);
    void (*stop)(void *context);
} nova_plugin_t;

#endif
