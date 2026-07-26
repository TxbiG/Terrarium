#include "etc/workspaces.h"
#include <stdio.h>
#include <string.h>

void nova_workspace_manager_init(nova_workspace_manager_t *manager, int count) {
    int i;

    if (!manager) {
        return;
    }

    memset(manager, 0, sizeof(*manager));
    if (count < 1) {
        count = 1;
    }
    if (count > 4) {
        count = 4;
    }

    manager->count = count;
    manager->current = 0;
    for (i = 0; i < count; ++i) {
        manager->workspaces[i].id = i;
        snprintf(manager->workspaces[i].name, sizeof(manager->workspaces[i].name), "Workspace %d", i + 1);
        manager->workspaces[i].window_count = 0;
    }
}

int nova_workspace_switch(nova_workspace_manager_t *manager, int id) {
    if (!manager || id < 0 || id >= manager->count) {
        return -1;
    }

    manager->current = id;
    return 0;
}

const nova_workspace_t *nova_workspace_current(const nova_workspace_manager_t *manager) {
    if (!manager || manager->current < 0 || manager->current >= manager->count) {
        return NULL;
    }

    return &manager->workspaces[manager->current];
}

size_t nova_workspace_list(const nova_workspace_manager_t *manager, char *buffer, size_t buffer_size) {
    size_t used = 0;
    int i;

    if (!buffer || buffer_size == 0) {
        return 0;
    }

    buffer[0] = '\0';
    if (!manager) {
        return 0;
    }

    for (i = 0; i < manager->count; ++i) {
        int written = snprintf(buffer + used,
                               buffer_size - used,
                               "%s%d:%u",
                               used == 0 ? "" : " ",
                               i + 1,
                               manager->workspaces[i].window_count);
        if (written < 0) {
            break;
        }
        if ((size_t)written >= buffer_size - used) {
            used = buffer_size - 1;
            break;
        }
        used += (size_t)written;
    }

    return used;
}
