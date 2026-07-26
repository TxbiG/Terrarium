#ifndef TERRARIUM_MODULE_H
#define TERRARIUM_MODULE_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>
#include "../security/security.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TERRARIUM_MAX_BOOT_MODULES 32u
#define TERRARIUM_MAX_MODULES 64u
#define TERRARIUM_MAX_MODULE_SYMBOLS 256u
#define TERRARIUM_MODULE_NAME_MAX 64u
#define TERRARIUM_MODULE_DEPS_MAX 4u

typedef enum terra_module_state {
    TERRA_MODULE_EMPTY = 0,
    TERRA_MODULE_DISCOVERED,
    TERRA_MODULE_LOADED,
    TERRA_MODULE_ACTIVE,
    TERRA_MODULE_STOPPED,
    TERRA_MODULE_FAILED
} terra_module_state_t;

typedef enum terra_module_status {
    TERRA_MODULE_OK = 0,
    TERRA_MODULE_ERR_NOT_READY = -1,
    TERRA_MODULE_ERR_INVALID = -2,
    TERRA_MODULE_ERR_NO_SPACE = -3,
    TERRA_MODULE_ERR_NOT_FOUND = -4,
    TERRA_MODULE_ERR_DENIED = -5,
    TERRA_MODULE_ERR_BUSY = -6,
    TERRA_MODULE_ERR_DEPENDENCY = -7
} terra_module_status_t;

typedef int (*terra_module_entry_t)(void *context);
typedef void (*terra_module_exit_t)(void *context);

typedef struct terra_module_descriptor {
    const char *name;
    uint32_t version;
    terra_security_caps_t capabilities;
    const char *dependencies[TERRARIUM_MODULE_DEPS_MAX];
    terra_module_entry_t init;
    terra_module_exit_t exit;
    void *context;
} terra_module_descriptor_t;

typedef struct terra_module_info {
    uint32_t id;
    char name[TERRARIUM_MODULE_NAME_MAX];
    uint32_t version;
    terra_module_state_t state;
    terra_security_caps_t capabilities;
    uint32_t dependency_count;
    uint32_t refcount;
    uint64_t image_base;
    uint64_t image_length;
} terra_module_info_t;

void module_init(const terra_boot_info_t *boot_info);
int module_is_ready(void);
void module_shutdown(void);
uint32_t module_count(void);
const terra_module_t *module_get(uint32_t index);
terra_module_status_t module_register(const terra_module_descriptor_t *descriptor,
                                      uint32_t *out_id);
terra_module_status_t module_activate(uint32_t id);
terra_module_status_t module_unload(uint32_t id);
terra_module_status_t module_info(uint32_t id, terra_module_info_t *out_info);
terra_module_status_t module_find(const char *name, uint32_t *out_id);
terra_module_status_t module_export_symbol(uint32_t module_id,
                                           const char *name,
                                           uintptr_t address);
uintptr_t module_resolve_symbol(const char *name);

#ifdef __cplusplus
}
#endif

#endif
