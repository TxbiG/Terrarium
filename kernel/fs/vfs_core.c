#include "blockdev.h"
#include "diskfs.h"
#include "vfs_core.h"

static terra_fs_type_t fs_types[TERRA_FS_MAX_TYPES];
static terra_vfs_mount_t mounts[TERRA_FS_MAX_MOUNTS];
static terra_vfs_node_t nodes[TERRA_FS_MAX_NODES];
static terra_vfs_file_t handles[TERRA_FS_MAX_HANDLES];
static uint32_t fs_type_count_value;
static uint32_t next_node_id;

static size_t terra_strlen(const char *value) {
    size_t len = 0;
    if (!value)
        return 0;
    while (value[len])
        ++len;
    return len;
}

static int terra_streq(const char *left, const char *right) {
    size_t i = 0;
    if (!left || !right)
        return 0;
    while (left[i] && right[i]) {
        if (left[i] != right[i])
            return 0;
        ++i;
    }
    return left[i] == right[i];
}

static int terra_mem_copy(void *dst, const void *src, size_t len) {
    uint8_t *out = (uint8_t *)dst;
    const uint8_t *in = (const uint8_t *)src;
    size_t i;
    if (!dst || !src)
        return -1;
    for (i = 0; i < len; ++i)
        out[i] = in[i];
    return 0;
}

static void terra_mem_zero(void *dst, size_t len) {
    uint8_t *out = (uint8_t *)dst;
    size_t i;
    if (!dst)
        return;
    for (i = 0; i < len; ++i)
        out[i] = 0;
}

static void terra_copy_string(char *dst, size_t cap, const char *src) {
    size_t i = 0;
    if (!dst || cap == 0)
        return;
    if (!src) {
        dst[0] = '\0';
        return;
    }
    while (src[i] && i + 1 < cap) {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
}

static int normalize_path(const char *path, char *out, size_t cap) {
    size_t in_i = 0;
    size_t out_i = 0;
    int last_slash = 0;

    if (!path || !out || cap < 2)
        return -1;

    out[out_i++] = '/';
    if (path[0] == '/')
        in_i = 1;

    while (path[in_i] && out_i + 1 < cap) {
        char ch = path[in_i++];
        if (ch == '/') {
            if (last_slash)
                continue;
            last_slash = 1;
        } else {
            last_slash = 0;
        }
        out[out_i++] = ch;
    }

    if (out_i > 1 && out[out_i - 1] == '/')
        --out_i;
    out[out_i] = '\0';
    return path[in_i] ? -1 : 0;
}

static terra_vfs_node_t *node_by_id(uint32_t id) {
    size_t i;
    for (i = 0; i < TERRA_FS_MAX_NODES; ++i) {
        if (nodes[i].used && nodes[i].id == id)
            return &nodes[i];
    }
    return 0;
}

static terra_vfs_node_t *alloc_node(uint32_t parent, const char *name, terra_vfs_node_kind_t kind, uint32_t fs_type, uint32_t mode) {
    size_t i;
    for (i = 0; i < TERRA_FS_MAX_NODES; ++i) {
        if (!nodes[i].used) {
            terra_mem_zero(&nodes[i], sizeof(nodes[i]));
            nodes[i].used = 1;
            nodes[i].id = next_node_id++;
            nodes[i].parent = parent;
            nodes[i].kind = kind;
            nodes[i].fs_type = fs_type;
            nodes[i].mode = mode;
            terra_copy_string(nodes[i].name, sizeof(nodes[i].name), name);
            return &nodes[i];
        }
    }
    return 0;
}

static terra_vfs_node_t *find_child(uint32_t parent, const char *name) {
    size_t i;
    for (i = 0; i < TERRA_FS_MAX_NODES; ++i) {
        if (nodes[i].used && nodes[i].parent == parent && terra_streq(nodes[i].name, name))
            return &nodes[i];
    }
    return 0;
}

static int node_has_children(uint32_t parent) {
    size_t i;
    for (i = 0; i < TERRA_FS_MAX_NODES; ++i) {
        if (nodes[i].used && nodes[i].parent == parent)
            return 1;
    }
    return 0;
}

static int node_has_open_handle(uint32_t node_id) {
    size_t i;
    for (i = 0; i < TERRA_FS_MAX_HANDLES; ++i) {
        if (handles[i].used && handles[i].node == node_id)
            return 1;
    }
    return 0;
}

static int node_has_ancestor(uint32_t node_id, uint32_t ancestor_id) {
    terra_vfs_node_t *node = node_by_id(node_id);
    while (node) {
        if (node->id == ancestor_id)
            return 1;
        if (node->parent == 0)
            break;
        node = node_by_id(node->parent);
    }
    return 0;
}

static int node_is_readonly(const terra_vfs_node_t *node) {
    size_t i;
    const terra_vfs_mount_t *best = 0;

    if (!node)
        return 1;

    for (i = 0; i < TERRA_FS_MAX_MOUNTS; ++i) {
        if (!mounts[i].used)
            continue;
        if (node_has_ancestor(node->id, mounts[i].root_node))
            best = &mounts[i];
    }

    return best ? best->readonly : 0;
}

static int mount_index_for_node(uint32_t node_id) {
    size_t i;
    for (i = 0; i < TERRA_FS_MAX_MOUNTS; ++i) {
        if (mounts[i].used && mounts[i].root_node == node_id)
            return (int)i;
    }
    return -1;
}

static int split_parent(const char *path, char *parent, size_t parent_cap, char *name, size_t name_cap) {
    char normalized[TERRA_FS_MAX_PATH];
    size_t len;
    size_t slash = 0;
    size_t i;

    if (normalize_path(path, normalized, sizeof(normalized)) != 0)
        return -1;

    len = terra_strlen(normalized);
    if (len == 1)
        return -1;

    for (i = 1; i < len; ++i) {
        if (normalized[i] == '/')
            slash = i;
    }

    if (slash == 0) {
        terra_copy_string(parent, parent_cap, "/");
        terra_copy_string(name, name_cap, normalized + 1);
        return 0;
    }

    if (slash + 1 >= len)
        return -1;

    for (i = 0; i < slash && i + 1 < parent_cap; ++i)
        parent[i] = normalized[i];
    parent[i] = '\0';
    terra_copy_string(name, name_cap, normalized + slash + 1);
    return 0;
}

static terra_vfs_node_t *resolve_node(const char *path) {
    char normalized[TERRA_FS_MAX_PATH];
    char segment[TERRA_FS_MAX_NAME];
    terra_vfs_node_t *current;
    size_t i = 1;
    size_t s;

    if (normalize_path(path, normalized, sizeof(normalized)) != 0)
        return 0;

    current = node_by_id(1);
    if (!current)
        return 0;
    if (terra_streq(normalized, "/"))
        return current;

    while (normalized[i]) {
        s = 0;
        while (normalized[i] && normalized[i] != '/') {
            if (s + 1 >= sizeof(segment))
                return 0;
            segment[s++] = normalized[i++];
        }
        segment[s] = '\0';
        current = find_child(current->id, segment);
        if (!current)
            return 0;
        if (normalized[i] == '/')
            ++i;
    }

    return current;
}

static void node_set_data(terra_vfs_node_t *node, const char *text) {
    size_t len = terra_strlen(text);

    if (!node)
        return;
    if (len > TERRA_FS_INLINE_DATA)
        len = TERRA_FS_INLINE_DATA;
    if (len > 0)
        terra_mem_copy(node->data, text, len);
    node->size = len;
}

static terra_vfs_node_t *create_child_internal(const char *parent_path, const char *name, terra_vfs_node_kind_t kind, const char *type_name, uint32_t mode) {
    terra_vfs_node_t *parent = resolve_node(parent_path);
    const terra_fs_type_t *type = terra_fs_type_by_name(type_name);
    terra_vfs_node_t *existing;

    if (!parent || !type)
        return 0;

    existing = find_child(parent->id, name);
    if (existing)
        return existing;

    return alloc_node(parent->id, name, kind, type->id, mode);
}

static void populate_proc_files(void) {
    terra_vfs_node_t *filesystems = create_child_internal("/proc", "filesystems", TERRA_VFS_NODE_FILE, "procfs", 0444);
    terra_vfs_node_t *mount_list = create_child_internal("/proc", "mounts", TERRA_VFS_NODE_FILE, "procfs", 0444);

    node_set_data(filesystems,
                  "tmpfs rw\n"
                  "devfs rw\n"
                  "procfs ro\n"
                  "initrd ro\n"
                  "fat32 disk-ro\n"
                  "ext2 disk-ro\n"
                  "exfat disk-ro\n"
                  "ext4 disk-ro\n"
                  "ntfs3 disk-ro\n"
                  "xfs disk-ro\n");
    node_set_data(mount_list,
                  "tmpfs / rw\n"
                  "devfs /dev rw\n"
                  "procfs /proc ro\n");
}

static void populate_dev_files(void) {
    create_child_internal("/dev", "null", TERRA_VFS_NODE_DEVICE, "devfs", 0666);
    create_child_internal("/dev", "zero", TERRA_VFS_NODE_DEVICE, "devfs", 0666);
    create_child_internal("/dev", "full", TERRA_VFS_NODE_DEVICE, "devfs", 0666);
    create_child_internal("/dev", "console", TERRA_VFS_NODE_DEVICE, "devfs", 0600);
}

void terra_vfs_reset(void) {
    terra_mem_zero(fs_types, sizeof(fs_types));
    terra_mem_zero(mounts, sizeof(mounts));
    terra_mem_zero(nodes, sizeof(nodes));
    terra_mem_zero(handles, sizeof(handles));
    fs_type_count_value = 0;
    next_node_id = 1;
}

int terra_fs_register_type(const char *name, terra_fs_status_t status, uint32_t flags) {
    terra_fs_type_t *type;

    if (!name)
        return TERRA_FS_ERR_INVAL;
    if (fs_type_count_value >= TERRA_FS_MAX_TYPES)
        return TERRA_FS_ERR_NOSPC;
    if (terra_fs_type_by_name(name))
        return TERRA_FS_OK;

    type = &fs_types[fs_type_count_value];
    type->id = fs_type_count_value + 1;
    type->name = name;
    type->status = status;
    type->flags = flags;
    fs_type_count_value++;
    return TERRA_FS_OK;
}

const terra_fs_type_t *terra_fs_type_by_name(const char *name) {
    uint32_t i;
    for (i = 0; i < fs_type_count_value; ++i) {
        if (terra_streq(fs_types[i].name, name))
            return &fs_types[i];
    }
    return TERRA_FS_OK;
}

const terra_fs_type_t *terra_fs_type_by_id(uint32_t id) {
    uint32_t i;
    for (i = 0; i < fs_type_count_value; ++i) {
        if (fs_types[i].id == id)
            return &fs_types[i];
    }
    return 0;
}

uint32_t terra_fs_type_count(void) {
    return fs_type_count_value;
}

int terra_vfs_bootstrap(void) {
    const terra_fs_type_t *tmpfs = terra_fs_type_by_name("tmpfs");
    if (!tmpfs)
        return TERRA_FS_ERR_UNSUPPORTED;

    if (!alloc_node(0, "/", TERRA_VFS_NODE_DIR, tmpfs->id, 0755))
        return TERRA_FS_ERR_NOSPC;

    if (terra_vfs_mount("/", "tmpfs", 0) != 0)
        return TERRA_FS_ERR_INVAL;
    terra_vfs_mkdir("/dev", 0755);
    terra_vfs_mkdir("/proc", 0555);
    terra_vfs_mkdir("/boot", 0555);
    terra_vfs_mkdir("/mnt", 0755);
    terra_vfs_mount("/dev", "devfs", 0);
    terra_vfs_mount("/proc", "procfs", 1);
    populate_dev_files();
    populate_proc_files();
    return 0;
}

int terra_vfs_mount(const char *path, const char *type_name, int readonly) {
    char normalized[TERRA_FS_MAX_PATH];
    const terra_fs_type_t *type = terra_fs_type_by_name(type_name);
    terra_vfs_node_t *node;
    size_t i;

    if (!type || type->status == TERRA_FS_STATUS_UNSUPPORTED)
        return TERRA_FS_ERR_UNSUPPORTED;
    if (!(type->flags & TERRA_FS_TYPE_PSEUDO))
        return TERRA_FS_ERR_INVAL;
    if (normalize_path(path, normalized, sizeof(normalized)) != 0)
        return TERRA_FS_ERR_INVAL;

    node = resolve_node(normalized);
    if (!node)
        return TERRA_FS_ERR_NOENT;
    if (node->kind != TERRA_VFS_NODE_DIR)
        return TERRA_FS_ERR_NOTDIR;

    for (i = 0; i < TERRA_FS_MAX_MOUNTS; ++i) {
        if (!mounts[i].used) {
            mounts[i].used = 1;
            terra_copy_string(mounts[i].path, sizeof(mounts[i].path), normalized);
            mounts[i].fs_type = type->id;
            mounts[i].root_node = node->id;
            mounts[i].block_device = 0;
            mounts[i].readonly = readonly || type->status == TERRA_FS_STATUS_READONLY;
            node->fs_type = type->id;
            return TERRA_FS_OK;
        }
    }

    return TERRA_FS_ERR_NOSPC;
}

int terra_vfs_mount_block(const char *path, const char *type_name, uint32_t block_device, int readonly) {
    char normalized[TERRA_FS_MAX_PATH];
    const char *selected_type = type_name;
    const char *detected_type;
    const terra_fs_type_t *type;
    const terra_diskfs_driver_t *driver;
    terra_vfs_node_t *node;
    size_t i;

    if (!selected_type || terra_streq(selected_type, "auto")) {
        selected_type = terra_diskfs_detect(block_device);
        if (!selected_type)
            return TERRA_FS_ERR_UNSUPPORTED;
    }

    type = terra_fs_type_by_name(selected_type);
    if (!type || type->status == TERRA_FS_STATUS_UNSUPPORTED)
        return TERRA_FS_ERR_UNSUPPORTED;
    if (!(type->flags & TERRA_FS_TYPE_DISK))
        return TERRA_FS_ERR_INVAL;
    if (!terra_blockdev_get(block_device))
        return TERRA_FS_ERR_NOENT;
    if (normalize_path(path, normalized, sizeof(normalized)) != 0)
        return TERRA_FS_ERR_INVAL;

    detected_type = terra_diskfs_detect(block_device);
    if (detected_type) {
        if (!terra_streq(detected_type, selected_type))
            return TERRA_FS_ERR_INVAL;
    }

    driver = terra_diskfs_driver_by_name(selected_type);
    if (driver) {
        if (driver->context_size > TERRA_FS_DISK_CONTEXT_SIZE)
            return TERRA_FS_ERR_NOSPC;
    }

    node = resolve_node(normalized);
    if (!node)
        return TERRA_FS_ERR_NOENT;
    if (node->kind != TERRA_VFS_NODE_DIR)
        return TERRA_FS_ERR_NOTDIR;

    for (i = 0; i < TERRA_FS_MAX_MOUNTS; ++i) {
        if (!mounts[i].used) {
            int mounted = TERRA_FS_OK;
            mounts[i].used = 1;
            terra_copy_string(mounts[i].path, sizeof(mounts[i].path), normalized);
            mounts[i].fs_type = type->id;
            mounts[i].root_node = node->id;
            mounts[i].block_device = block_device;
            mounts[i].disk_driver = driver;
            terra_mem_zero(mounts[i].disk_context, sizeof(mounts[i].disk_context));
            mounts[i].readonly = readonly || type->status == TERRA_FS_STATUS_READONLY;
            if (driver)
                mounted = driver->mount(block_device, mounts[i].disk_context, sizeof(mounts[i].disk_context));
            if (mounted != TERRA_FS_OK) {
                terra_mem_zero(&mounts[i], sizeof(mounts[i]));
                return mounted;
            }
            node->fs_type = type->id;
            if (driver && driver->describe) {
                terra_vfs_node_t *volume = create_child_internal(normalized, ".volume", TERRA_VFS_NODE_FILE, selected_type, 0444);
                if (volume) {
                    int described = driver->describe(mounts[i].disk_context, (char *)volume->data, sizeof(volume->data));
                    volume->size = described > 0 ? (uint64_t)described : 0;
                }
            }
            return TERRA_FS_OK;
        }
    }

    return TERRA_FS_ERR_NOSPC;
}

int terra_vfs_mkdir(const char *path, uint32_t mode) {
    return terra_vfs_create(path, TERRA_VFS_NODE_DIR, mode);
}

int terra_vfs_create(const char *path, terra_vfs_node_kind_t kind, uint32_t mode) {
    char parent_path[TERRA_FS_MAX_PATH];
    char name[TERRA_FS_MAX_NAME];
    terra_vfs_node_t *parent;

    if (split_parent(path, parent_path, sizeof(parent_path), name, sizeof(name)) != 0)
        return TERRA_FS_ERR_INVAL;

    parent = resolve_node(parent_path);
    if (!parent || parent->kind != TERRA_VFS_NODE_DIR)
        return TERRA_FS_ERR_NOTDIR;
    if (node_is_readonly(parent))
        return TERRA_FS_ERR_RDONLY;
    if (find_child(parent->id, name))
        return TERRA_FS_OK;

    return alloc_node(parent->id, name, kind, parent->fs_type, mode) ? TERRA_FS_OK : TERRA_FS_ERR_NOSPC;
}

int terra_vfs_unlink(const char *path) {
    terra_vfs_node_t *node = resolve_node(path);

    if (!node || node->id == 1)
        return TERRA_FS_ERR_NOENT;
    if (node_is_readonly(node))
        return TERRA_FS_ERR_RDONLY;
    if (mount_index_for_node(node->id) >= 0)
        return TERRA_FS_ERR_BUSY;
    if (node_has_open_handle(node->id))
        return TERRA_FS_ERR_BUSY;
    if (node->kind == TERRA_VFS_NODE_DIR && node_has_children(node->id))
        return TERRA_FS_ERR_BUSY;

    terra_mem_zero(node, sizeof(*node));
    return TERRA_FS_OK;
}

int terra_vfs_rename(const char *old_path, const char *new_path) {
    char parent_path[TERRA_FS_MAX_PATH];
    char name[TERRA_FS_MAX_NAME];
    terra_vfs_node_t *node = resolve_node(old_path);
    terra_vfs_node_t *parent;

    if (!node || node->id == 1)
        return TERRA_FS_ERR_NOENT;
    if (node_is_readonly(node))
        return TERRA_FS_ERR_RDONLY;
    if (split_parent(new_path, parent_path, sizeof(parent_path), name, sizeof(name)) != 0)
        return TERRA_FS_ERR_INVAL;
    parent = resolve_node(parent_path);
    if (!parent || parent->kind != TERRA_VFS_NODE_DIR)
        return TERRA_FS_ERR_NOTDIR;
    if (node_is_readonly(parent))
        return TERRA_FS_ERR_RDONLY;
    if (find_child(parent->id, name))
        return TERRA_FS_ERR_EXIST;

    node->parent = parent->id;
    terra_copy_string(node->name, sizeof(node->name), name);
    return TERRA_FS_OK;
}

int terra_vfs_open(const char *path, uint32_t flags) {
    terra_vfs_node_t *node = resolve_node(path);
    int existed = node != 0;
    size_t i;

    if (existed && (flags & TERRA_VFS_O_EXCL) && (flags & TERRA_VFS_O_CREAT))
        return TERRA_FS_ERR_EXIST;

    if (!node && (flags & TERRA_VFS_O_CREAT)) {
        int created = terra_vfs_create(path, TERRA_VFS_NODE_FILE, 0644);
        if (created != TERRA_FS_OK)
            return created;
        node = resolve_node(path);
    }

    if (!node)
        return TERRA_FS_ERR_NOENT;
    if ((flags & TERRA_VFS_O_DIRECTORY) && node->kind != TERRA_VFS_NODE_DIR)
        return TERRA_FS_ERR_NOTDIR;
    if (node->kind == TERRA_VFS_NODE_DIR && !(flags & TERRA_VFS_O_DIRECTORY))
        return TERRA_FS_ERR_ISDIR;
    if ((flags & TERRA_VFS_O_TRUNC) && node->kind == TERRA_VFS_NODE_FILE) {
        if (node_is_readonly(node))
            return TERRA_FS_ERR_RDONLY;
        node->size = 0;
    }

    for (i = 0; i < TERRA_FS_MAX_HANDLES; ++i) {
        if (!handles[i].used) {
            handles[i].used = 1;
            handles[i].node = node->id;
            handles[i].offset = (flags & TERRA_VFS_O_APPEND) ? node->size : 0;
            handles[i].flags = flags;
            return (int)i;
        }
    }

    return TERRA_FS_ERR_NOSPC;
}

int terra_vfs_close(int fd) {
    if (fd < 0 || fd >= (int)TERRA_FS_MAX_HANDLES || !handles[fd].used)
        return TERRA_FS_ERR_BADF;

    terra_mem_zero(&handles[fd], sizeof(handles[fd]));
    return TERRA_FS_OK;
}

ssize_t terra_vfs_read(int fd, void *buffer, size_t size) {
    terra_vfs_file_t *file;
    terra_vfs_node_t *node;
    size_t available;

    if (fd < 0 || fd >= (int)TERRA_FS_MAX_HANDLES)
        return TERRA_FS_ERR_BADF;
    if (!buffer)
        return TERRA_FS_ERR_INVAL;
    file = &handles[fd];
    if (!file->used)
        return TERRA_FS_ERR_BADF;
    node = node_by_id(file->node);
    if (!node)
        return TERRA_FS_ERR_NOENT;
    if (node->kind == TERRA_VFS_NODE_DEVICE && terra_streq(node->name, "null"))
        return 0;
    if (node->kind == TERRA_VFS_NODE_DEVICE &&
        (terra_streq(node->name, "zero") || terra_streq(node->name, "full"))) {
        terra_mem_zero(buffer, size);
        return (ssize_t)size;
    }
    if (file->offset >= node->size)
        return 0;

    available = (size_t)(node->size - file->offset);
    if (size > available)
        size = available;
    terra_mem_copy(buffer, node->data + file->offset, size);
    file->offset += size;
    return (ssize_t)size;
}

ssize_t terra_vfs_write(int fd, const void *buffer, size_t size) {
    terra_vfs_file_t *file;
    terra_vfs_node_t *node;

    if (fd < 0 || fd >= (int)TERRA_FS_MAX_HANDLES)
        return TERRA_FS_ERR_BADF;
    if (!buffer)
        return TERRA_FS_ERR_INVAL;
    file = &handles[fd];
    if (!file->used)
        return TERRA_FS_ERR_BADF;
    node = node_by_id(file->node);
    if (node && node->kind == TERRA_VFS_NODE_DEVICE && terra_streq(node->name, "full"))
        return TERRA_FS_ERR_NOSPC;
    if (node && node->kind == TERRA_VFS_NODE_DEVICE && terra_streq(node->name, "null"))
        return (ssize_t)size;
    if (node && node->kind == TERRA_VFS_NODE_DEVICE && terra_streq(node->name, "console"))
        return (ssize_t)size;
    if (!node)
        return TERRA_FS_ERR_NOENT;
    if (node->kind != TERRA_VFS_NODE_FILE)
        return TERRA_FS_ERR_INVAL;
    if (node_is_readonly(node))
        return TERRA_FS_ERR_RDONLY;
    if (file->offset >= TERRA_FS_INLINE_DATA)
        return TERRA_FS_ERR_NOSPC;
    if (size > TERRA_FS_INLINE_DATA - file->offset)
        size = (size_t)(TERRA_FS_INLINE_DATA - file->offset);

    terra_mem_copy(node->data + file->offset, buffer, size);
    file->offset += size;
    if (file->offset > node->size)
        node->size = file->offset;
    return (ssize_t)size;
}

int terra_vfs_seek(int fd, uint64_t offset) {
    if (fd < 0 || fd >= (int)TERRA_FS_MAX_HANDLES || !handles[fd].used)
        return TERRA_FS_ERR_BADF;
    handles[fd].offset = offset;
    return TERRA_FS_OK;
}

int terra_vfs_truncate(const char *path, uint64_t size) {
    terra_vfs_node_t *node = resolve_node(path);

    if (!node)
        return TERRA_FS_ERR_NOENT;
    if (node->kind != TERRA_VFS_NODE_FILE)
        return TERRA_FS_ERR_INVAL;
    if (node_is_readonly(node))
        return TERRA_FS_ERR_RDONLY;
    if (size > TERRA_FS_INLINE_DATA)
        return TERRA_FS_ERR_NOSPC;

    if (size < node->size)
        node->size = size;
    else {
        while (node->size < size)
            node->data[node->size++] = 0;
    }

    return TERRA_FS_OK;
}

int terra_vfs_stat(const char *path, terra_vfs_stat_t *out_stat) {
    terra_vfs_node_t *node = resolve_node(path);
    if (!out_stat)
        return TERRA_FS_ERR_INVAL;
    if (!node)
        return TERRA_FS_ERR_NOENT;

    out_stat->kind = node->kind;
    out_stat->mode = node->mode;
    out_stat->size = node->size;
    out_stat->fs_type = node->fs_type;
    return TERRA_FS_OK;
}

int terra_vfs_list(const char *path, char *buffer, size_t size) {
    terra_vfs_node_t *dir = resolve_node(path);
    size_t i;
    size_t used = 0;

    if (!buffer || size == 0)
        return TERRA_FS_ERR_INVAL;
    if (!dir)
        return TERRA_FS_ERR_NOENT;
    if (dir->kind != TERRA_VFS_NODE_DIR)
        return TERRA_FS_ERR_NOTDIR;

    buffer[0] = '\0';
    for (i = 0; i < TERRA_FS_MAX_NODES; ++i) {
        size_t n;
        if (!nodes[i].used || nodes[i].parent != dir->id)
            continue;
        n = terra_strlen(nodes[i].name);
        if (used + n + 2 >= size)
            break;
        if (used > 0)
            buffer[used++] = '\n';
        terra_mem_copy(buffer + used, nodes[i].name, n);
        used += n;
        buffer[used] = '\0';
    }

    return (int)used;
}
