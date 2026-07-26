#include "storage.h"

#include "../drivers.h"

typedef struct terra_storage_record {
    terra_storage_info_t info;
    terra_storage_read_fn_t read;
    terra_storage_write_fn_t write;
    terra_storage_flush_fn_t flush;
    void *context;
} terra_storage_record_t;

static terra_storage_record_t storage_devices[TERRA_STORAGE_MAX_DEVICES];
static terra_storage_id_t next_storage_id;
static int storage_ready;

static void storage_copy_name(char *dest, const char *src) {
    uint32_t i = 0;
    if (src) {
        for (; i + 1 < TERRA_STORAGE_NAME_MAX && src[i]; ++i)
            dest[i] = src[i];
    }
    dest[i] = 0;
}

static terra_storage_record_t *storage_find(terra_storage_id_t id) {
    for (uint32_t i = 0; i < TERRA_STORAGE_MAX_DEVICES; ++i) {
        if (storage_devices[i].info.active && storage_devices[i].info.id == id)
            return &storage_devices[i];
    }
    return 0;
}

static int storage_driver_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    return storage_ready ? 0 : -1;
}

void storage_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    for (uint32_t i = 0; i < TERRA_STORAGE_MAX_DEVICES; ++i)
        storage_devices[i].info.active = 0;
    next_storage_id = 1;
    storage_ready = 1;
    terra_driver_t driver = {"storage-core", TERRA_DRIVER_STORAGE, storage_driver_init, storage_shutdown, 1};
    drivers_register(&driver);
}

void storage_shutdown(void) {
    for (uint32_t i = 0; i < TERRA_STORAGE_MAX_DEVICES; ++i)
        storage_devices[i].info.active = 0;
    storage_ready = 0;
}

int storage_register_device(const terra_storage_config_t *config, terra_storage_id_t *out_id) {
    if (!storage_ready || !config || !config->name || !config->read || !out_id)
        return -1;
    if (!config->block_count || !config->block_size)
        return -1;

    for (uint32_t i = 0; i < TERRA_STORAGE_MAX_DEVICES; ++i) {
        if (storage_devices[i].info.active)
            continue;
        storage_devices[i].info.id = next_storage_id++;
        storage_copy_name(storage_devices[i].info.name, config->name);
        storage_devices[i].info.block_count = config->block_count;
        storage_devices[i].info.block_size = config->block_size;
        storage_devices[i].info.flags = config->flags;
        storage_devices[i].info.reads = 0;
        storage_devices[i].info.writes = 0;
        storage_devices[i].info.active = 1;
        storage_devices[i].read = config->read;
        storage_devices[i].write = config->write;
        storage_devices[i].flush = config->flush;
        storage_devices[i].context = config->context;
        *out_id = storage_devices[i].info.id;
        return 0;
    }
    return -1;
}

int storage_unregister_device(terra_storage_id_t id) {
    terra_storage_record_t *record = storage_find(id);
    if (!record)
        return -1;
    record->info.active = 0;
    return 0;
}

int storage_info(terra_storage_id_t id, terra_storage_info_t *out_info) {
    terra_storage_record_t *record = storage_find(id);
    if (!record || !out_info)
        return -1;
    *out_info = record->info;
    return 0;
}

int storage_read(terra_storage_id_t id, uint64_t lba, void *buffer, size_t blocks) {
    terra_storage_record_t *record = storage_find(id);
    if (!record || !buffer || !blocks || lba + blocks > record->info.block_count)
        return -1;
    int result = record->read(id, lba, buffer, blocks, record->context);
    if (result >= 0)
        record->info.reads += blocks;
    return result;
}

int storage_write(terra_storage_id_t id, uint64_t lba, const void *buffer, size_t blocks) {
    terra_storage_record_t *record = storage_find(id);
    if (!record || !record->write || !buffer || !blocks || lba + blocks > record->info.block_count)
        return -1;
    int result = record->write(id, lba, buffer, blocks, record->context);
    if (result >= 0)
        record->info.writes += blocks;
    return result;
}

int storage_flush(terra_storage_id_t id) {
    terra_storage_record_t *record = storage_find(id);
    if (!record)
        return -1;
    if (!record->flush)
        return 0;
    return record->flush(id, record->context);
}
