#include "package.h"

#include <stdio.h>
#include <string.h>

#include "store.h"
#include "../status.h"

static void trim_line(char *line) {
    if (!line)
        return;
    size_t len = strlen(line);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r' || line[len - 1] == ' ' || line[len - 1] == '\t'))
        line[--len] = '\0';
}

int terra_package_query(const char *name, terra_package_info_t *out_info) {
    if (!name || !name[0] || !out_info)
        return TERRA_STATUS_INVAL;
    int index = terra_package_store_find_package(name);
    if (index < 0)
        return TERRA_STATUS_NOENT;
    *out_info = terra_package_store_get()->packages[index];
    return TERRA_STATUS_OK;
}

int terra_package_list(terra_package_info_t *packages, unsigned long max_packages) {
    if (!packages && max_packages != 0)
        return TERRA_STATUS_INVAL;
    terra_package_store_t *store = terra_package_store_get();
    unsigned long count = store->package_count < max_packages ? store->package_count : max_packages;
    for (unsigned long i = 0; i < count; ++i)
        packages[i] = store->packages[i];
    return (int)store->package_count;
}

int terra_package_read_manifest(const char *path, terra_package_manifest_t *out_manifest) {
    if (!path || !path[0] || !out_manifest)
        return TERRA_STATUS_INVAL;

    FILE *file = fopen_s(path, "rb");
    if (!file)
        return TERRA_STATUS_NOENT;

    memset(out_manifest, 0, sizeof(*out_manifest));
    out_manifest->format_version = TERRA_PACKAGE_FORMAT_VERSION;

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        trim_line(line);
        char *eq = strchr(line, '=');
        if (!eq || line[0] == '#')
            continue;
        *eq = '\0';
        const char *key = line;
        const char *value = eq + 1;
        if (strcmp(key, "name") == 0)
            snprintf(out_manifest->info.name, sizeof(out_manifest->info.name), "%s", value);
        else if (strcmp(key, "version") == 0)
            snprintf(out_manifest->info.version, sizeof(out_manifest->info.version), "%s", value);
        else if (strcmp(key, "architecture") == 0)
            snprintf(out_manifest->info.architecture, sizeof(out_manifest->info.architecture), "%s", value);
        else if (strcmp(key, "description") == 0)
            snprintf(out_manifest->info.description, sizeof(out_manifest->info.description), "%s", value);
        else if (strcmp(key, "hash") == 0)
            snprintf(out_manifest->info.content_hash, sizeof(out_manifest->info.content_hash), "%s", value);
        else if (strcmp(key, "signature") == 0)
            snprintf(out_manifest->signature_id, sizeof(out_manifest->signature_id), "%s", value);
        else if (strcmp(key, "rollback_group") == 0)
            snprintf(out_manifest->rollback_group, sizeof(out_manifest->rollback_group), "%s", value);
    }

    fclose(file);
    return terra_package_validate_manifest(out_manifest);
}

int terra_package_validate_manifest(const terra_package_manifest_t *manifest) {
    if (!manifest)
        return TERRA_STATUS_INVAL;
    if (manifest->format_version != TERRA_PACKAGE_FORMAT_VERSION)
        return TERRA_STATUS_INVAL;
    if (!manifest->info.name[0] || !manifest->info.version[0] || !manifest->info.architecture[0])
        return TERRA_STATUS_INVAL;
    if (manifest->info.dependency_count > 16)
        return TERRA_STATUS_INVAL;
    return TERRA_STATUS_OK;
}
