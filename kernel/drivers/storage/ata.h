#ifndef TERRARIUM_DRIVERS_ATA_H
#define TERRARIUM_DRIVERS_ATA_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>
#include "storage.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_ATA_MAX_DEVICES 8u
#define TERRA_ATA_SECTOR_SIZE 512u

typedef uint32_t terra_ata_id_t;

typedef int (*terra_ata_rw_fn_t)(terra_ata_id_t id,
                                 uint64_t lba,
                                 void *buffer,
                                 size_t sectors,
                                 void *context);
typedef int (*terra_ata_flush_fn_t)(terra_ata_id_t id, void *context);

typedef struct terra_ata_device_config {
    const char *name;
    uint64_t sectors;
    uint16_t io_base;
    uint16_t control_base;
    uint8_t drive_select;
    terra_ata_rw_fn_t read;
    terra_ata_rw_fn_t write;
    terra_ata_flush_fn_t flush;
    void *context;
} terra_ata_device_config_t;

typedef struct terra_ata_device_info {
    terra_ata_id_t id;
    terra_storage_id_t storage_id;
    uint64_t sectors;
    uint16_t io_base;
    uint16_t control_base;
    uint8_t drive_select;
    int active;
} terra_ata_device_info_t;

void ata_init(const terra_boot_info_t *boot_info);
void ata_shutdown(void);
int ata_register_device(const terra_ata_device_config_t *config, terra_ata_id_t *out_id);
int ata_unregister_device(terra_ata_id_t id);
int ata_info(terra_ata_id_t id, terra_ata_device_info_t *out_info);

#ifdef __cplusplus
}
#endif

#endif
