#include "ata.h"

#include "../drivers.h"

typedef struct terra_ata_record {
    terra_ata_device_info_t info;
    terra_ata_rw_fn_t read;
    terra_ata_rw_fn_t write;
    terra_ata_flush_fn_t flush;
    void *context;
} terra_ata_record_t;

static terra_ata_record_t ata_devices[TERRA_ATA_MAX_DEVICES];
static terra_ata_id_t next_ata_id;
static int ata_ready;

static terra_ata_record_t *ata_find(terra_ata_id_t id) {
    for (uint32_t i = 0; i < TERRA_ATA_MAX_DEVICES; ++i) {
        if (ata_devices[i].info.active && ata_devices[i].info.id == id)
            return &ata_devices[i];
    }
    return 0;
}

static int ata_storage_read(terra_storage_id_t storage_id,
                            uint64_t lba,
                            void *buffer,
                            size_t blocks,
                            void *context) {
    terra_ata_record_t *record = (terra_ata_record_t *)context;
    (void)storage_id;
    if (!record || !record->read)
        return -1;
    return record->read(record->info.id, lba, buffer, blocks, record->context);
}

static int ata_storage_write(terra_storage_id_t storage_id,
                             uint64_t lba,
                             const void *buffer,
                             size_t blocks,
                             void *context) {
    terra_ata_record_t *record = (terra_ata_record_t *)context;
    (void)storage_id;
    if (!record || !record->write)
        return -1;
    return record->write(record->info.id, lba, (void *)buffer, blocks, record->context);
}

static int ata_storage_flush(terra_storage_id_t storage_id, void *context) {
    terra_ata_record_t *record = (terra_ata_record_t *)context;
    (void)storage_id;
    if (!record || !record->flush)
        return 0;
    return record->flush(record->info.id, record->context);
}

static int ata_driver_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    return ata_ready ? 0 : -1;
}

void ata_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    for (uint32_t i = 0; i < TERRA_ATA_MAX_DEVICES; ++i)
        ata_devices[i].info.active = 0;
    next_ata_id = 1;
    ata_ready = 1;
    terra_driver_t driver = {"ata", TERRA_DRIVER_STORAGE, ata_driver_init, ata_shutdown, 1};
    drivers_register(&driver);
}

void ata_shutdown(void) {
    for (uint32_t i = 0; i < TERRA_ATA_MAX_DEVICES; ++i) {
        if (ata_devices[i].info.active)
            storage_unregister_device(ata_devices[i].info.storage_id);
        ata_devices[i].info.active = 0;
    }
    ata_ready = 0;
}

int ata_register_device(const terra_ata_device_config_t *config, terra_ata_id_t *out_id) {
    if (!ata_ready || !config || !config->name || !config->read || !out_id)
        return -1;
    if (!config->sectors)
        return -1;

    for (uint32_t i = 0; i < TERRA_ATA_MAX_DEVICES; ++i) {
        if (ata_devices[i].info.active)
            continue;

        ata_devices[i].info.id = next_ata_id++;
        ata_devices[i].info.sectors = config->sectors;
        ata_devices[i].info.io_base = config->io_base;
        ata_devices[i].info.control_base = config->control_base;
        ata_devices[i].info.drive_select = config->drive_select;
        ata_devices[i].info.active = 1;
        ata_devices[i].read = config->read;
        ata_devices[i].write = config->write;
        ata_devices[i].flush = config->flush;
        ata_devices[i].context = config->context;

        terra_storage_config_t storage = {
            .name = config->name,
            .block_count = config->sectors,
            .block_size = TERRA_ATA_SECTOR_SIZE,
            .flags = 0,
            .read = ata_storage_read,
            .write = config->write ? ata_storage_write : 0,
            .flush = ata_storage_flush,
            .context = &ata_devices[i],
        };

        if (storage_register_device(&storage, &ata_devices[i].info.storage_id) != 0) {
            ata_devices[i].info.active = 0;
            return -1;
        }

        *out_id = ata_devices[i].info.id;
        return 0;
    }

    return -1;
}

int ata_unregister_device(terra_ata_id_t id) {
    terra_ata_record_t *record = ata_find(id);
    if (!record)
        return -1;
    storage_unregister_device(record->info.storage_id);
    record->info.active = 0;
    return 0;
}

int ata_info(terra_ata_id_t id, terra_ata_device_info_t *out_info) {
    terra_ata_record_t *record = ata_find(id);
    if (!record || !out_info)
        return -1;
    *out_info = record->info;
    return 0;
}
