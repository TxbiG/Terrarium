#include "module.h"

typedef struct terra_module_record {
    uint32_t id;
    terra_module_info_t info;
    terra_module_entry_t init;
    terra_module_exit_t exit;
    void *context;
    const char *dependencies[TERRARIUM_MODULE_DEPS_MAX];
    uint8_t active;
} terra_module_record_t;

typedef struct terra_module_symbol {
    uint32_t module_id;
    char name[TERRARIUM_MODULE_NAME_MAX];
    uintptr_t address;
    uint8_t active;
} terra_module_symbol_t;

static terra_module_t module_table[TERRARIUM_MAX_BOOT_MODULES];
static terra_module_record_t module_records[TERRARIUM_MAX_MODULES];
static terra_module_symbol_t module_symbols[TERRARIUM_MAX_MODULE_SYMBOLS];
static uint32_t modules_loaded;
static uint32_t next_module_id;
static int module_ready;

static int module_str_eq(const char *a, const char *b) {
    if (!a || !b)
        return 0;
    while (*a && *b) {
        if (*a != *b)
            return 0;
        ++a;
        ++b;
    }
    return *a == *b;
}

static void module_str_copy(char *dest, const char *src, size_t cap) {
    size_t i = 0;
    if (!cap)
        return;
    if (src) {
        for (; i + 1 < cap && src[i]; ++i)
            dest[i] = src[i];
    }
    dest[i] = 0;
}

static terra_module_record_t *module_record_by_id(uint32_t id) {
    for (uint32_t i = 0; i < TERRARIUM_MAX_MODULES; ++i) {
        if (module_records[i].active && module_records[i].id == id)
            return &module_records[i];
    }
    return 0;
}

static int dependencies_ready(const terra_module_record_t *record) {
    for (uint32_t i = 0; i < record->info.dependency_count; ++i) {
        uint32_t dep_id = 0;
        if (module_find(record->dependencies[i], &dep_id) != TERRA_MODULE_OK)
            return 0;
        terra_module_record_t *dep = module_record_by_id(dep_id);
        if (!dep || dep->info.state != TERRA_MODULE_ACTIVE)
            return 0;
    }
    return 1;
}

void module_init(const terra_boot_info_t *boot_info) {
    modules_loaded = 0;
    next_module_id = 1;

    for (uint32_t i = 0; i < TERRARIUM_MAX_MODULES; ++i) {
        module_records[i].id = 0;
        module_records[i].active = 0;
    }

    for (uint32_t i = 0; i < TERRARIUM_MAX_MODULE_SYMBOLS; ++i) {
        module_symbols[i].module_id = 0;
        module_symbols[i].name[0] = 0;
        module_symbols[i].address = 0;
        module_symbols[i].active = 0;
    }

    module_ready = 1;

    if (boot_info) {
        uint32_t count = boot_info->module_count;
        if (count > TERRA_BOOTLOADER_MAX_MODULES)
            count = TERRA_BOOTLOADER_MAX_MODULES;
        if (count > TERRARIUM_MAX_BOOT_MODULES)
            count = TERRARIUM_MAX_BOOT_MODULES;

        for (uint32_t i = 0; i < count; ++i) {
            module_table[i] = boot_info->modules[i];
            terra_module_descriptor_t descriptor = {
                .name = boot_info->modules[i].name,
                .version = 1,
                .capabilities = TERRA_CAP_NONE,
                .dependencies = {0},
                .init = 0,
                .exit = 0,
                .context = 0,
            };
            uint32_t id = 0;
            if (module_register(&descriptor, &id) == TERRA_MODULE_OK) {
                terra_module_record_t *record = module_record_by_id(id);
                if (record) {
                    record->info.image_base = boot_info->modules[i].base;
                    record->info.image_length = boot_info->modules[i].length;
                }
            }
        }

        modules_loaded = count;
    }

}

int module_is_ready(void) {
    return module_ready;
}

void module_shutdown(void) {
    for (uint32_t i = 0; i < TERRARIUM_MAX_MODULES; ++i) {
        if (module_records[i].active && module_records[i].exit &&
            module_records[i].info.state == TERRA_MODULE_ACTIVE)
            module_records[i].exit(module_records[i].context);
        module_records[i].active = 0;
    }

    for (uint32_t i = 0; i < TERRARIUM_MAX_MODULE_SYMBOLS; ++i)
        module_symbols[i].active = 0;

    modules_loaded = 0;
    module_ready = 0;
}

uint32_t module_count(void) {
    return modules_loaded;
}

const terra_module_t *module_get(uint32_t index) {
    if (index >= modules_loaded)
        return 0;

    return &module_table[index];
}

terra_module_status_t module_register(const terra_module_descriptor_t *descriptor,
                                      uint32_t *out_id) {
    if (!module_ready)
        return TERRA_MODULE_ERR_NOT_READY;
    if (!descriptor || !descriptor->name || !out_id)
        return TERRA_MODULE_ERR_INVALID;

    terra_security_context_t kernel = {
        .subject_id = 0,
        .subject_type = TERRA_SECURITY_SUBJECT_KERNEL,
        .caps = security_kernel_caps(),
    };
    if (security_check(&kernel,
                       TERRA_SECURITY_ACTION_MODULE_LOAD,
                       next_module_id,
                       TERRA_CAP_MODULE_LOAD) != TERRA_SECURITY_ALLOW)
        return TERRA_MODULE_ERR_DENIED;

    for (uint32_t i = 0; i < TERRARIUM_MAX_MODULES; ++i) {
        if (module_records[i].active &&
            module_str_eq(module_records[i].info.name, descriptor->name))
            return TERRA_MODULE_ERR_BUSY;
    }

    for (uint32_t i = 0; i < TERRARIUM_MAX_MODULES; ++i) {
        if (module_records[i].active)
            continue;

        terra_module_record_t *record = &module_records[i];
        record->id = next_module_id++;
        record->active = 1;
        record->init = descriptor->init;
        record->exit = descriptor->exit;
        record->context = descriptor->context;
        record->info.id = record->id;
        record->info.version = descriptor->version;
        record->info.state = TERRA_MODULE_LOADED;
        record->info.capabilities = descriptor->capabilities;
        record->info.refcount = 0;
        record->info.image_base = 0;
        record->info.image_length = 0;
        module_str_copy(record->info.name, descriptor->name, TERRARIUM_MODULE_NAME_MAX);

        record->info.dependency_count = 0;
        for (uint32_t dep = 0; dep < TERRARIUM_MODULE_DEPS_MAX; ++dep) {
            record->dependencies[dep] = descriptor->dependencies[dep];
            if (descriptor->dependencies[dep])
                record->info.dependency_count++;
        }

        *out_id = record->id;
        return TERRA_MODULE_OK;
    }

    return TERRA_MODULE_ERR_NO_SPACE;
}

terra_module_status_t module_activate(uint32_t id) {
    terra_module_record_t *record = module_record_by_id(id);
    if (!record)
        return TERRA_MODULE_ERR_NOT_FOUND;
    if (!dependencies_ready(record))
        return TERRA_MODULE_ERR_DEPENDENCY;
    if (record->info.state == TERRA_MODULE_ACTIVE)
        return TERRA_MODULE_OK;

    if (record->init && record->init(record->context) != 0) {
        record->info.state = TERRA_MODULE_FAILED;
        return TERRA_MODULE_ERR_INVALID;
    }

    record->info.state = TERRA_MODULE_ACTIVE;
    return TERRA_MODULE_OK;
}

terra_module_status_t module_unload(uint32_t id) {
    terra_module_record_t *record = module_record_by_id(id);
    if (!record)
        return TERRA_MODULE_ERR_NOT_FOUND;
    if (record->info.refcount)
        return TERRA_MODULE_ERR_BUSY;

    terra_security_context_t kernel = {
        .subject_id = 0,
        .subject_type = TERRA_SECURITY_SUBJECT_KERNEL,
        .caps = security_kernel_caps(),
    };
    if (security_check(&kernel,
                       TERRA_SECURITY_ACTION_MODULE_UNLOAD,
                       id,
                       TERRA_CAP_MODULE_UNLOAD) != TERRA_SECURITY_ALLOW)
        return TERRA_MODULE_ERR_DENIED;

    if (record->exit && record->info.state == TERRA_MODULE_ACTIVE)
        record->exit(record->context);

    for (uint32_t i = 0; i < TERRARIUM_MAX_MODULE_SYMBOLS; ++i) {
        if (module_symbols[i].active && module_symbols[i].module_id == id)
            module_symbols[i].active = 0;
    }

    record->info.state = TERRA_MODULE_STOPPED;
    record->active = 0;
    return TERRA_MODULE_OK;
}

terra_module_status_t module_info(uint32_t id, terra_module_info_t *out_info) {
    terra_module_record_t *record = module_record_by_id(id);
    if (!record)
        return TERRA_MODULE_ERR_NOT_FOUND;
    if (!out_info)
        return TERRA_MODULE_ERR_INVALID;
    *out_info = record->info;
    return TERRA_MODULE_OK;
}

terra_module_status_t module_find(const char *name, uint32_t *out_id) {
    if (!name || !out_id)
        return TERRA_MODULE_ERR_INVALID;
    for (uint32_t i = 0; i < TERRARIUM_MAX_MODULES; ++i) {
        if (module_records[i].active && module_str_eq(module_records[i].info.name, name)) {
            *out_id = module_records[i].id;
            return TERRA_MODULE_OK;
        }
    }
    return TERRA_MODULE_ERR_NOT_FOUND;
}

terra_module_status_t module_export_symbol(uint32_t module_id,
                                           const char *name,
                                           uintptr_t address) {
    if (!name || !address)
        return TERRA_MODULE_ERR_INVALID;
    if (!module_record_by_id(module_id))
        return TERRA_MODULE_ERR_NOT_FOUND;

    terra_security_context_t kernel = {
        .subject_id = module_id,
        .subject_type = TERRA_SECURITY_SUBJECT_MODULE,
        .caps = security_kernel_caps(),
    };
    if (security_check(&kernel,
                       TERRA_SECURITY_ACTION_MODULE_EXPORT,
                       module_id,
                       TERRA_CAP_MODULE_EXPORT) != TERRA_SECURITY_ALLOW)
        return TERRA_MODULE_ERR_DENIED;

    for (uint32_t i = 0; i < TERRARIUM_MAX_MODULE_SYMBOLS; ++i) {
        if (!module_symbols[i].active)
            continue;
        if (module_str_eq(module_symbols[i].name, name))
            return TERRA_MODULE_ERR_BUSY;
    }

    for (uint32_t i = 0; i < TERRARIUM_MAX_MODULE_SYMBOLS; ++i) {
        if (module_symbols[i].active)
            continue;
        module_symbols[i].active = 1;
        module_symbols[i].module_id = module_id;
        module_symbols[i].address = address;
        module_str_copy(module_symbols[i].name, name, TERRARIUM_MODULE_NAME_MAX);
        return TERRA_MODULE_OK;
    }

    return TERRA_MODULE_ERR_NO_SPACE;
}

uintptr_t module_resolve_symbol(const char *name) {
    if (!name)
        return 0;
    for (uint32_t i = 0; i < TERRARIUM_MAX_MODULE_SYMBOLS; ++i) {
        if (module_symbols[i].active && module_str_eq(module_symbols[i].name, name))
            return module_symbols[i].address;
    }
    return 0;
}
