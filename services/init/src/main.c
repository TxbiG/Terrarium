#include "service_manager.h"

#include <stdio.h>
#include <string.h>

#define DEFAULT_UNIT_MANIFEST "/etc/terrarium/services/manifest.txt"

static void usage(const char *argv0) {
    printf("usage: %s [--units MANIFEST] [--dry-run] [--status] [--list] [--start NAME] [--stop NAME] [--restart NAME]\n", argv0);
}

typedef enum init_action {
    INIT_ACTION_START_ALL = 0,
    INIT_ACTION_STATUS,
    INIT_ACTION_LIST,
    INIT_ACTION_START_ONE,
    INIT_ACTION_STOP_ONE,
    INIT_ACTION_RESTART_ONE
} init_action_t;

int main(int argc, char **argv) {
    int dry_run = 0;
    const char *service_name = 0;
    const char *manifest = DEFAULT_UNIT_MANIFEST;
    init_action_t action = INIT_ACTION_START_ALL;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--dry-run") == 0) {
            dry_run = 1;
        } else if (strcmp(argv[i], "--units") == 0 && i + 1 < argc) {
            manifest = argv[++i];
        } else if (strcmp(argv[i], "--status") == 0) {
            action = INIT_ACTION_STATUS;
        } else if (strcmp(argv[i], "--list") == 0) {
            action = INIT_ACTION_LIST;
        } else if (strcmp(argv[i], "--start") == 0 && i + 1 < argc) {
            action = INIT_ACTION_START_ONE;
            service_name = argv[++i];
        } else if (strcmp(argv[i], "--stop") == 0 && i + 1 < argc) {
            action = INIT_ACTION_STOP_ONE;
            service_name = argv[++i];
        } else if (strcmp(argv[i], "--restart") == 0 && i + 1 < argc) {
            action = INIT_ACTION_RESTART_ONE;
            service_name = argv[++i];
        } else {
            usage(argv[0]);
            return 2;
        }
    }

    if (terra_service_manager_load_manifest(manifest) != 0)
        terra_service_manager_load_defaults();

    switch (action) {
    case INIT_ACTION_STATUS:
        return terra_service_manager_status();
    case INIT_ACTION_LIST:
        return terra_service_manager_list();
    case INIT_ACTION_START_ONE:
        return terra_service_manager_start(service_name, dry_run) == 0 ? 0 : 1;
    case INIT_ACTION_STOP_ONE:
        return terra_service_manager_stop(service_name, dry_run) == 0 ? 0 : 1;
    case INIT_ACTION_RESTART_ONE:
        return terra_service_manager_restart(service_name, dry_run) == 0 ? 0 : 1;
    case INIT_ACTION_START_ALL:
        return terra_service_manager_start_all(dry_run) == 0 ? 0 : 1;
    }

    return 1;
}