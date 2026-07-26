#ifndef TERRARIUM_SYSTEM_PACKAGE_PACKAGE_H
#define TERRARIUM_SYSTEM_PACKAGE_PACKAGE_H

#include <stdint.h>

#define TERRA_PACKAGE_NAME_MAX 64u
#define TERRA_PACKAGE_VERSION_MAX 32u
#define TERRA_PACKAGE_ARCH_MAX 32u
#define TERRA_PACKAGE_DESCRIPTION_MAX 256u
#define TERRA_PACKAGE_DEP_MAX 64u
#define TERRA_PACKAGE_HASH_MAX 65u
#define TERRA_PACKAGE_FORMAT_MAGIC "TPKG"
#define TERRA_PACKAGE_FORMAT_VERSION 1u

typedef enum terra_package_kind {
    TERRA_PACKAGE_KIND_APP = 0,
    TERRA_PACKAGE_KIND_SERVICE,
    TERRA_PACKAGE_KIND_DRIVER,
    TERRA_PACKAGE_KIND_LIBRARY,
    TERRA_PACKAGE_KIND_SYSTEM
} terra_package_kind_t;

typedef struct terra_package_dependency {
    char name[TERRA_PACKAGE_NAME_MAX];
    char min_version[TERRA_PACKAGE_VERSION_MAX];
    char max_version[TERRA_PACKAGE_VERSION_MAX];
    uint32_t optional;
} terra_package_dependency_t;

typedef struct terra_package_info {
    char name[TERRA_PACKAGE_NAME_MAX];
    char version[TERRA_PACKAGE_VERSION_MAX];
    char architecture[TERRA_PACKAGE_ARCH_MAX];
    char description[TERRA_PACKAGE_DESCRIPTION_MAX];
    terra_package_kind_t kind;
    char content_hash[TERRA_PACKAGE_HASH_MAX];
    uint64_t installed_size;
    uint32_t dependency_count;
} terra_package_info_t;

typedef struct terra_package_manifest {
    uint32_t format_version;
    terra_package_info_t info;
    terra_package_dependency_t dependencies[16];
    char signature_id[64];
    char rollback_group[64];
} terra_package_manifest_t;

int terra_package_query(const char *name, terra_package_info_t *out_info);
int terra_package_list(terra_package_info_t *packages, unsigned long max_packages);
int terra_package_read_manifest(const char *path, terra_package_manifest_t *out_manifest);
int terra_package_validate_manifest(const terra_package_manifest_t *manifest);

#endif
