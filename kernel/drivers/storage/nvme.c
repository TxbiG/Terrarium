#include "nvme.h"

#include "../drivers.h"

typedef struct terra_nvme_record {
    terra_nvme_namespace_info_t info;
    terra_nvme_rw_fn_t read;
    terra_nvme_rw_fn_t write;
    terra_nvme_flush_fn_t flush;
    void *context;
} terra_nvme_record_t;

static terra_nvme_record_t nvme_namespaces[TERRA_NVME_MAX_NAMESPACES];
static terra_nvme_id_t next_nvme_id;
static int nvme_ready;

static terra_nvme_record_t *nvme_find(terra_nvme_id_t id) {
    for (uint32_t i = 0; i < TERRA_NVME_MAX_NAMESPACES; ++i) {
        if (nvme_namespaces[i].info.active && nvme_namespaces[i].info.id == id)
            return &nvme_namespaces[i];
    }
    return 0;
}

static int nvme_storage_read(terra_storage_id_t storage_id,
                             uint64_t lba,
                             void *buffer,
                             size_t blocks,
                             void *context) {
    terra_nvme_record_t *record = (terra_nvme_record_t *)context;
    (void)storage_id;
    if (!record || !record->read)
        return -1;
    return record->read(record->info.id, lba, buffer, blocks, record->context);
}

static int nvme_storage_write(terra_storage_id_t storage_id,
                              uint64_t lba,
                              const void *buffer,
                              size_t blocks,
                              void *context) {
    terra_nvme_record_t *record = (terra_nvme_record_t *)context;
    (void)storage_id;
    if (!record || !record->write)
        return -1;
    return record->write(record->info.id, lba, (void *)buffer, blocks, record->context);
}

static int nvme_storage_flush(terra_storage_id_t storage_id, void *context) {
    terra_nvme_record_t *record = (terra_nvme_record_t *)context;
    (void)storage_id;
    if (!record || !record->flush)
        return 0;
    return record->flush(record->info.id, record->context);
}

static int nvme_driver_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    return nvme_ready ? 0 : -1;
}

void nvme_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    for (uint32_t i = 0; i < TERRA_NVME_MAX_NAMESPACES; ++i)
        nvme_namespaces[i].info.active = 0;
    next_nvme_id = 1;
    nvme_ready = 1;
    terra_driver_t driver = {"nvme", TERRA_DRIVER_STORAGE, nvme_driver_init, nvme_shutdown, 1};
    drivers_register(&driver);
}

void nvme_shutdown(void) {
    for (uint32_t i = 0; i < TERRA_NVME_MAX_NAMESPACES; ++i) {
        if (nvme_namespaces[i].info.active)
            storage_unregister_device(nvme_namespaces[i].info.storage_id);
        nvme_namespaces[i].info.active = 0;
    }
    nvme_ready = 0;
}

int nvme_register_namespace(const terra_nvme_namespace_config_t *config, terra_nvme_id_t *out_id) {
    if (!nvme_ready || !config || !config->name || !config->read || !out_id)
        return -1;
    if (!config->block_count || !config->block_size || !config->namespace_id)
        return -1;

    for (uint32_t i = 0; i < TERRA_NVME_MAX_NAMESPACES; ++i) {
        if (nvme_namespaces[i].info.active)
            continue;

        nvme_namespaces[i].info.id = next_nvme_id++;
        nvme_namespaces[i].info.controller_mmio = config->controller_mmio;
        nvme_namespaces[i].info.namespace_id = config->namespace_id;
        nvme_namespaces[i].info.block_count = config->block_count;
        nvme_namespaces[i].info.block_size = config->block_size;
        nvme_namespaces[i].info.active = 1;
        nvme_namespaces[i].read = config->read;
        nvme_namespaces[i].write = config->write;
        nvme_namespaces[i].flush = config->flush;
        nvme_namespaces[i].context = config->context;

        terra_storage_config_t storage = {
            .name = config->name,
            .block_count = config->block_count,
            .block_size = config->block_size,
            .flags = 0,
            .read = nvme_storage_read,
            .write = config->write ? nvme_storage_write : 0,
            .flush = nvme_storage_flush,
            .context = &nvme_namespaces[i],
        };

        if (storage_register_device(&storage, &nvme_namespaces[i].info.storage_id) != 0) {
            nvme_namespaces[i].info.active = 0;
            return -1;
        }

        *out_id = nvme_namespaces[i].info.id;
        return 0;
    }

    return -1;
}

int nvme_unregister_namespace(terra_nvme_id_t id) {
    terra_nvme_record_t *record = nvme_find(id);
    if (!record)
        return -1;
    storage_unregister_device(record->info.storage_id);
    record->info.active = 0;
    return 0;
}

int nvme_info(terra_nvme_id_t id, terra_nvme_namespace_info_t *out_info) {
    terra_nvme_record_t *record = nvme_find(id);
    if (!record || !out_info)
        return -1;
    *out_info = record->info;
    return 0;
}
