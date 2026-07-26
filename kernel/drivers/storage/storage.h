#ifndef TERRARIUM_DRIVERS_STORAGE_H
#define TERRARIUM_DRIVERS_STORAGE_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_STORAGE_MAX_DEVICES 32u
#define TERRA_STORAGE_NAME_MAX 32u
#define TERRA_STORAGE_UUID_BYTES 16u

typedef uint32_t terra_storage_id_t;
typedef int (*terra_storage_read_fn_t)(terra_storage_id_t id, uint64_t lba, void *buffer, size_t blocks, void *context);
typedef int (*terra_storage_write_fn_t)(terra_storage_id_t id, uint64_t lba, const void *buffer, size_t blocks, void *context);
typedef int (*terra_storage_flush_fn_t)(terra_storage_id_t id, void *context);

typedef struct terra_storage_config {
    const char *name;
    uint64_t block_count;
    uint32_t block_size;
    uint32_t flags;
    terra_storage_read_fn_t read;
    terra_storage_write_fn_t write;
    terra_storage_flush_fn_t flush;
    void *context;
} terra_storage_config_t;

typedef struct terra_storage_info {
    terra_storage_id_t id;
    char name[TERRA_STORAGE_NAME_MAX];
    uint64_t block_count;
    uint32_t block_size;
    uint32_t flags;
    uint64_t reads;
    uint64_t writes;
    int active;
} terra_storage_info_t;

void storage_init(const terra_boot_info_t *boot_info);
void storage_shutdown(void);
int storage_register_device(const terra_storage_config_t *config, terra_storage_id_t *out_id);
int storage_unregister_device(terra_storage_id_t id);
int storage_info(terra_storage_id_t id, terra_storage_info_t *out_info);
int storage_read(terra_storage_id_t id, uint64_t lba, void *buffer, size_t blocks);
int storage_write(terra_storage_id_t id, uint64_t lba, const void *buffer, size_t blocks);
int storage_flush(terra_storage_id_t id);

#ifdef __cplusplus
}
#endif

#endif
