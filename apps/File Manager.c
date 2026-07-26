#include <Terra/app/app.h>
#include <fs/vfs.h>
#include <fs/mount.h>
#include <fs/block.h>
#include <utils/disk_util.h>
#include <status.h>

#include <stdio.h>
#include <string.h>

#define FILE_MANAGER_MAX_ENTRIES 128
#define FILE_MANAGER_NAME_MAX 128
#define FILE_MANAGER_PATH_MAX 512
#define FILE_MANAGER_MAX_MOUNTS 16
#define FILE_MANAGER_MAX_BLOCKS 16

typedef enum file_manager_entry_type {
    FILE_MANAGER_ENTRY_FILE = 0,
    FILE_MANAGER_ENTRY_DIRECTORY,
    FILE_MANAGER_ENTRY_MOUNT,
    FILE_MANAGER_ENTRY_DEVICE
} file_manager_entry_type_t;

typedef struct file_manager_entry {
    char name[FILE_MANAGER_NAME_MAX];
    file_manager_entry_type_t type;
    uint64_t size;
    uint32_t mode;
} file_manager_entry_t;

typedef struct file_manager_state {
    char cwd[FILE_MANAGER_PATH_MAX];
    file_manager_entry_t entries[FILE_MANAGER_MAX_ENTRIES];
    unsigned entry_count;
    unsigned selected_index;
    unsigned scroll_offset;
    int vfs_ready;
    int mount_ready;
    int block_ready;
    int thumbnails_ready;
    int removable_ready;
} file_manager_state_t;

static const char *entry_type_name(file_manager_entry_type_t type) {
    switch (type) {
    case FILE_MANAGER_ENTRY_DIRECTORY:
        return "dir";
    case FILE_MANAGER_ENTRY_MOUNT:
        return "mount";
    case FILE_MANAGER_ENTRY_DEVICE:
        return "device";
    case FILE_MANAGER_ENTRY_FILE:
    default:
        return "file";
    }
}

static void file_manager_add_entry(file_manager_state_t *state, const char *name, file_manager_entry_type_t type, uint64_t size, uint32_t mode) {
    file_manager_entry_t *entry;
    if (!state || !name || state->entry_count >= FILE_MANAGER_MAX_ENTRIES)
        return;
    entry = &state->entries[state->entry_count++];
    snprintf(entry->name, sizeof(entry->name), "%s", name);
    entry->type = type;
    entry->size = size;
    entry->mode = mode;
}

static void file_manager_load(file_manager_state_t *state, const char *path) {
    terra_vfs_stat_t stat;
    terra_mount_entry_t mounts[FILE_MANAGER_MAX_MOUNTS];
    terra_block_device_t blocks[FILE_MANAGER_MAX_BLOCKS];
    int mount_count;
    int block_count;

    if (!state)
        return;
    memset(state, 0, sizeof(*state));
    snprintf(state->cwd, sizeof(state->cwd), "%s", path ? path : "/");

    state->vfs_ready = terra_vfs_stat(state->cwd, &stat) == TERRA_STATUS_OK;
    state->mount_ready = (mount_count = terra_mount_list(mounts, FILE_MANAGER_MAX_MOUNTS)) >= 0;
    state->block_ready = (block_count = terra_block_list(blocks, FILE_MANAGER_MAX_BLOCKS)) >= 0;
    state->thumbnails_ready = 0;
    state->removable_ready = state->block_ready;

    file_manager_add_entry(state, "..", FILE_MANAGER_ENTRY_DIRECTORY, 0, 0755);

    if (state->mount_ready) {
        for (int i = 0; i < mount_count && state->entry_count < FILE_MANAGER_MAX_ENTRIES; ++i)
            file_manager_add_entry(state, mounts[i].target, FILE_MANAGER_ENTRY_MOUNT, 0, 0);
    } else {
        file_manager_add_entry(state, "mount metadata unavailable", FILE_MANAGER_ENTRY_MOUNT, 0, 0);
    }

    if (state->block_ready) {
        for (int i = 0; i < block_count && state->entry_count < FILE_MANAGER_MAX_ENTRIES; ++i)
            file_manager_add_entry(state, blocks[i].name, FILE_MANAGER_ENTRY_DEVICE, blocks[i].bytes, blocks[i].removable ? 1u : 0u);
    } else {
        file_manager_add_entry(state, "removable devices unavailable", FILE_MANAGER_ENTRY_DEVICE, 0, 0);
    }

    if (!state->vfs_ready)
        file_manager_add_entry(state, "directory listing backend not implemented", FILE_MANAGER_ENTRY_FILE, 0, 0);
}

static void file_manager_render(const file_manager_state_t *state, const terra_app_t *app) {
    printf("File Manager [%dx%d] path=%s frame=%u\n",
           app->window.view.rect.width,
           app->window.view.rect.height,
           state ? state->cwd : "/",
           app->frames);

    if (!state)
        return;

    printf("backends: vfs=%s mounts=%s blocks=%s thumbnails=%s removable=%s\n",
           state->vfs_ready ? "ready" : "not-ready",
           state->mount_ready ? "ready" : "not-ready",
           state->block_ready ? "ready" : "not-ready",
           state->thumbnails_ready ? "ready" : "not-ready",
           state->removable_ready ? "ready" : "not-ready");

    for (unsigned i = 0; i < state->entry_count; ++i) {
        char size_text[32];
        terra_disk_human_size(state->entries[i].size, size_text, sizeof(size_text));
        printf("  %c %-36s %-7s %s\n",
               i == state->selected_index ? '*' : ' ',
               state->entries[i].name,
               entry_type_name(state->entries[i].type),
               size_text);
    }
}

int main(int argc, char **argv) {
    file_manager_state_t state;
    terra_app_t app;
    terra_app_desc_t desc = {
        .id = "org.terrarium.FileManager",
        .name = "File Manager",
        .kind = TERRA_APP_KIND_SYSTEM,
        .width = 920,
        .height = 680,
    };
    const char *path = argc > 1 ? argv[1] : "/";

    if (terra_app_init(&app, &desc) != 0) {
        fprintf(stderr, "File Manager: failed to initialize Terra app runtime\n");
        return 1;
    }

    file_manager_load(&state, path);
    terra_app_present(&app);
    file_manager_render(&state, &app);
    terra_app_shutdown(&app);
    return 0;
}
