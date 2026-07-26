#include "service_manager.h"

#include <stdio.h>
#include <string.h>

#define LINE_MAX_CHARS 256u

typedef struct service_storage {
    terra_service_unit_t unit;
    char name[TERRA_SERVICE_NAME_MAX];
    char exec[TERRA_SERVICE_EXEC_MAX];
    char args[TERRA_SERVICE_ARGS_MAX];
    char description[TERRA_SERVICE_DESCRIPTION_MAX];
    char dep_names[TERRA_SERVICE_MAX_DEPS][TERRA_SERVICE_NAME_MAX];
} service_storage_t;

static service_storage_t service_storage[TERRA_SERVICE_MAX_UNITS];
static terra_service_state_t service_states[TERRA_SERVICE_MAX_UNITS];
static size_t service_count;

static const terra_service_unit_t fallback_units[] = {
    {.name = "logd", .exec = "/sbin/terrarium-logd", .description = "kernel and service log collector", .restart = TERRA_SERVICE_RESTART_ON_FAILURE, .required = 1},
    {.name = "fsd", .exec = "/sbin/terrarium-fsd", .description = "filesystem and mount coordinator", .deps = {"logd"}, .restart = TERRA_SERVICE_RESTART_ON_FAILURE, .required = 1},
    {.name = "inputd", .exec = "/sbin/terrarium-inputd", .description = "input event router", .deps = {"logd"}, .restart = TERRA_SERVICE_RESTART_ON_FAILURE, .required = 1},
    {.name = "timed", .exec = "/sbin/terrarium-timed", .description = "wall-clock and timezone service", .deps = {"logd"}, .restart = TERRA_SERVICE_RESTART_ON_FAILURE, .required = 1},
    {.name = "netd", .exec = "/sbin/terrarium-netd", .description = "network manager", .deps = {"logd", "fsd"}, .restart = TERRA_SERVICE_RESTART_ON_FAILURE, .required = 0},
    {.name = "dhcpd", .exec = "/sbin/terrarium-dhcpd", .description = "DHCP client/server boundary", .deps = {"netd"}, .restart = TERRA_SERVICE_RESTART_ON_FAILURE, .required = 0},
    {.name = "audiod", .exec = "/sbin/terrarium-audiod", .description = "audio service", .deps = {"logd", "fsd"}, .restart = TERRA_SERVICE_RESTART_ON_FAILURE, .required = 0},
    {.name = "launcherd", .exec = "/sbin/terrarium-launcherd", .description = "application and desktop launcher", .deps = {"logd", "fsd"}, .restart = TERRA_SERVICE_RESTART_ON_FAILURE, .required = 1},
    {.name = "login", .exec = "/sbin/terrarium-login", .description = "text login and session entry", .deps = {"logd", "fsd", "inputd", "launcherd"}, .restart = TERRA_SERVICE_RESTART_ALWAYS, .required = 1},
};

static size_t fallback_count(void) {
    return sizeof(fallback_units) / sizeof(fallback_units[0]);
}

static void copy_text(char *dst, size_t cap, const char *src) {
    size_t i = 0;

    if (!dst || cap == 0)
        return;
    if (!src)
        src = "";

    while (src[i] && i + 1 < cap) {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
}

static char *trim(char *text) {
    char *end;

    while (*text == ' ' || *text == '\t' || *text == '\r' || *text == '\n')
        ++text;

    end = text + strlen(text);
    while (end > text && (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\r' || end[-1] == '\n'))
        *--end = '\0';

    return text;
}

static int name_eq(const char *left, const char *right) {
    return left && right && strcmp(left, right) == 0;
}

static int find_unit(const char *name) {
    for (size_t i = 0; i < service_count; ++i) {
        if (name_eq(service_storage[i].unit.name, name))
            return (int)i;
    }
    return -1;
}

static const char *state_name(terra_service_state_t state) {
    switch (state) {
    case TERRA_SERVICE_STOPPED: return "stopped";
    case TERRA_SERVICE_STARTING: return "starting";
    case TERRA_SERVICE_RUNNING: return "running";
    case TERRA_SERVICE_FAILED: return "failed";
    case TERRA_SERVICE_BLOCKED: return "blocked";
    }
    return "unknown";
}

static terra_service_restart_t parse_restart(const char *value) {
    if (name_eq(value, "always"))
        return TERRA_SERVICE_RESTART_ALWAYS;
    if (name_eq(value, "on-failure"))
        return TERRA_SERVICE_RESTART_ON_FAILURE;
    return TERRA_SERVICE_RESTART_NEVER;
}

static int parse_bool(const char *value) {
    return name_eq(value, "true") || name_eq(value, "yes") || name_eq(value, "1");
}

static void finalize_unit(service_storage_t *storage) {
    storage->unit.name = storage->name;
    storage->unit.exec = storage->exec;
    storage->unit.description = storage->description;
    storage->unit.args = storage->args;
    for (size_t i = 0; i < TERRA_SERVICE_MAX_DEPS; ++i)
        storage->unit.deps[i] = storage->dep_names[i][0] ? storage->dep_names[i] : 0;
}

static int add_unit(const terra_service_unit_t *unit) {
    service_storage_t *storage;

    if (!unit || !unit->name || !unit->exec || service_count >= TERRA_SERVICE_MAX_UNITS)
        return -1;
    if (find_unit(unit->name) >= 0)
        return -2;

    storage = &service_storage[service_count];
    memset(storage, 0, sizeof(*storage));
    copy_text(storage->name, sizeof(storage->name), unit->name);
    copy_text(storage->exec, sizeof(storage->exec), unit->exec);
    copy_text(storage->description, sizeof(storage->description), unit->description);
    copy_text(storage->args, sizeof(storage->args), unit->args);
    storage->unit.restart = unit->restart;
    storage->unit.required = unit->required;

    for (size_t i = 0; i < TERRA_SERVICE_MAX_DEPS && unit->deps[i]; ++i)
        copy_text(storage->dep_names[i], sizeof(storage->dep_names[i]), unit->deps[i]);

    finalize_unit(storage);
    service_states[service_count] = TERRA_SERVICE_STOPPED;
    ++service_count;
    return 0;
}

static void parse_deps(service_storage_t *storage, char *value) {
    size_t index = 0;
    char *cursor = value;

    while (cursor && *cursor && index < TERRA_SERVICE_MAX_DEPS) {
        char *comma = strchr(cursor, ',');
        if (comma)
            *comma = '\0';
        copy_text(storage->dep_names[index], sizeof(storage->dep_names[index]), trim(cursor));
        ++index;
        cursor = comma ? comma + 1 : 0;
    }
}

static int parse_unit_file(const char *path) {
    FILE *file;
    char line[LINE_MAX_CHARS];
    service_storage_t parsed;

    if (!path || service_count >= TERRA_SERVICE_MAX_UNITS)
        return -1;

    file = fopen(path, "r");
    if (!file)
        return -2;

    memset(&parsed, 0, sizeof(parsed));
    parsed.unit.restart = TERRA_SERVICE_RESTART_NEVER;

    while (fgets(line, sizeof(line), file)) {
        char *key;
        char *value;
        char *equals;

        key = trim(line);
        if (!*key || *key == '#')
            continue;

        equals = strchr(key, '=');
        if (!equals)
            continue;

        *equals = '\0';
        value = trim(equals + 1);
        key = trim(key);

        if (name_eq(key, "name"))
            copy_text(parsed.name, sizeof(parsed.name), value);
        else if (name_eq(key, "exec"))
            copy_text(parsed.exec, sizeof(parsed.exec), value);
        else if (name_eq(key, "description"))
            copy_text(parsed.description, sizeof(parsed.description), value);
        else if (name_eq(key, "args"))
            copy_text(parsed.args, sizeof(parsed.args), value);
        else if (name_eq(key, "after"))
            parse_deps(&parsed, value);
        else if (name_eq(key, "restart"))
            parsed.unit.restart = parse_restart(value);
        else if (name_eq(key, "required"))
            parsed.unit.required = parse_bool(value);
    }

    fclose(file);

    finalize_unit(&parsed);
    return add_unit(&parsed.unit);
}

static void manifest_base_dir(const char *manifest_path, char *out, size_t out_cap) {
    const char *slash = 0;

    for (const char *p = manifest_path; p && *p; ++p) {
        if (*p == '/' || *p == '\\')
            slash = p;
    }

    if (!slash) {
        copy_text(out, out_cap, ".");
        return;
    }

    size_t len = (size_t)(slash - manifest_path);
    if (len + 1 > out_cap)
        len = out_cap - 1;
    memcpy(out, manifest_path, len);
    out[len] = '\0';
}

static void join_path(char *out, size_t out_cap, const char *base, const char *leaf) {
    size_t len;

    copy_text(out, out_cap, base);
    len = strlen(out);
    if (len + 1 < out_cap && len > 0 && out[len - 1] != '/' && out[len - 1] != '\\') {
        out[len++] = '/';
        out[len] = '\0';
    }
    copy_text(out + len, out_cap - len, leaf);
}

void terra_service_manager_reset(void) {
    for (size_t i = 0; i < TERRA_SERVICE_MAX_UNITS; ++i)
        service_states[i] = TERRA_SERVICE_STOPPED;
}

int terra_service_manager_load_defaults(void) {
    service_count = 0;
    terra_service_manager_reset();

    for (size_t i = 0; i < fallback_count(); ++i) {
        if (add_unit(&fallback_units[i]) != 0)
            return -1;
    }

    return 0;
}

int terra_service_manager_load_manifest(const char *manifest_path) {
    FILE *manifest;
    char base[256];
    char line[LINE_MAX_CHARS];
    size_t loaded = 0;

    if (!manifest_path)
        return -1;

    manifest = fopen(manifest_path, "r");
    if (!manifest)
        return -2;

    service_count = 0;
    terra_service_manager_reset();
    manifest_base_dir(manifest_path, base, sizeof(base));

    while (fgets(line, sizeof(line), manifest)) {
        char full_path[512];
        char *entry = trim(line);

        if (!*entry || *entry == '#')
            continue;

        if (strchr(entry, ':') || entry[0] == '/' || entry[0] == '\\')
            copy_text(full_path, sizeof(full_path), entry);
        else
            join_path(full_path, sizeof(full_path), base, entry);

        if (parse_unit_file(full_path) == 0)
            ++loaded;
    }

    fclose(manifest);

    if (loaded == 0) {
        terra_service_manager_load_defaults();
        return -3;
    }

    return 0;
}

size_t terra_service_manager_count(void) {
    return service_count;
}

const terra_service_unit_t *terra_service_manager_unit(size_t index) {
    if (index >= service_count)
        return 0;
    return &service_storage[index].unit;
}

terra_service_state_t terra_service_manager_state(size_t index) {
    if (index >= service_count)
        return TERRA_SERVICE_FAILED;
    return service_states[index];
}

int terra_service_manager_start(const char *name, int dry_run) {
    int index = find_unit(name);

    if (index < 0)
        return -1;
    if (service_states[index] == TERRA_SERVICE_RUNNING)
        return 0;
    if (service_states[index] == TERRA_SERVICE_STARTING) {
        service_states[index] = TERRA_SERVICE_BLOCKED;
        return -2;
    }

    service_states[index] = TERRA_SERVICE_STARTING;

    for (size_t i = 0; i < TERRA_SERVICE_MAX_DEPS && service_storage[index].unit.deps[i]; ++i) {
        if (terra_service_manager_start(service_storage[index].unit.deps[i], dry_run) != 0) {
            service_states[index] = TERRA_SERVICE_BLOCKED;
            return -3;
        }
    }

    printf("%s %s (%s%s%s)\n", dry_run ? "would start" : "starting", service_storage[index].unit.name, service_storage[index].unit.exec, service_storage[index].unit.args && service_storage[index].unit.args[0] ? " " : "", service_storage[index].unit.args && service_storage[index].unit.args[0] ? service_storage[index].unit.args : "");

    /* Real Phase 6 process spawning will replace this dry transition with
     * terra_process_spawn plus supervision, restart, and status IPC. */
    service_states[index] = TERRA_SERVICE_RUNNING;
    return 0;
}


int terra_service_manager_stop(const char *name, int dry_run) {
    int index = find_unit(name);

    if (index < 0)
        return -1;
    if (service_states[index] == TERRA_SERVICE_STOPPED)
        return 0;

    printf("%s %s\n", dry_run ? "would stop" : "stopping", service_storage[index].unit.name);
    service_states[index] = TERRA_SERVICE_STOPPED;
    return 0;
}

int terra_service_manager_restart(const char *name, int dry_run) {
    int stop_status;

    if (find_unit(name) < 0)
        return -1;

    stop_status = terra_service_manager_stop(name, dry_run);
    if (stop_status != 0)
        return stop_status;

    return terra_service_manager_start(name, dry_run);
}

int terra_service_manager_start_all(int dry_run) {
    int result = 0;

    for (size_t i = 0; i < service_count; ++i) {
        int status = terra_service_manager_start(service_storage[i].unit.name, dry_run);
        if (status != 0 && service_storage[i].unit.required)
            result = status;
    }

    return result;
}


int terra_service_manager_list(void) {
    for (size_t i = 0; i < service_count; ++i) {
        printf("%s\n", service_storage[i].unit.name);
    }
    return 0;
}

int terra_service_manager_status(void) {
    for (size_t i = 0; i < service_count; ++i) {
        printf("%-10s %-8s %s\n", service_storage[i].unit.name, state_name(service_states[i]), service_storage[i].unit.description ? service_storage[i].unit.description : "");
    }
    return 0;
}