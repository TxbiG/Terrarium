#ifndef TERRARIUM_DRIVERS_NVME_H
#define TERRARIUM_DRIVERS_NVME_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>
#include "storage.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_NVME_MAX_NAMESPACES 16u

typedef uint32_t terra_nvme_id_t;

typedef int (*terra_nvme_rw_fn_t)(terra_nvme_id_t id,
                                  uint64_t lba,
                                  void *buffer,
                                  size_t blocks,
                                  void *context);
typedef int (*terra_nvme_flush_fn_t)(terra_nvme_id_t id, void *context);

typedef struct terra_nvme_namespace_config {
    const char *name;
    uint64_t controller_mmio;
    uint32_t namespace_id;
    uint64_t block_count;
    uint32_t block_size;
    terra_nvme_rw_fn_t read;
    terra_nvme_rw_fn_t write;
    terra_nvme_flush_fn_t flush;
    void *context;
} terra_nvme_namespace_config_t;

typedef struct terra_nvme_namespace_info {
    terra_nvme_id_t id;
    terra_storage_id_t storage_id;
    uint64_t controller_mmio;
    uint32_t namespace_id;
    uint64_t block_count;
    uint32_t block_size;
    int active;
} terra_nvme_namespace_info_t;

void nvme_init(const terra_boot_info_t *boot_info);
void nvme_shutdown(void);
int nvme_register_namespace(const terra_nvme_namespace_config_t *config, terra_nvme_id_t *out_id);
int nvme_unregister_namespace(terra_nvme_id_t id);
int nvme_info(terra_nvme_id_t id, terra_nvme_namespace_info_t *out_info);

#ifdef __cplusplus
}
#endif

#endif
