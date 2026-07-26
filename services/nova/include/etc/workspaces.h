#ifndef TERRARIUM_NOVA_WORKSPACES_H
#define TERRARIUM_NOVA_WORKSPACES_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nova_workspace {
    int id;
    char name[24];
    unsigned window_count;
} nova_workspace_t;

typedef struct nova_workspace_manager {
    nova_workspace_t workspaces[4];
    int current;
    int count;
} nova_workspace_manager_t;

void nova_workspace_manager_init(nova_workspace_manager_t *manager, int count);
int nova_workspace_switch(nova_workspace_manager_t *manager, int id);
const nova_workspace_t *nova_workspace_current(const nova_workspace_manager_t *manager);
size_t nova_workspace_list(const nova_workspace_manager_t *manager, char *buffer, size_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif
