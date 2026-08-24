#include "../common.h"

#include <runtime/service.h>
#include <status.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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

static int service_backend_result(const char *command, int status) {
    if (status == TERRA_STATUS_NOSYS || status == TERRA_STATUS_NOTREADY)
        return util_backend_missing("service", "/run/services/control", "services manager endpoint is not available");
    if (status < 0) {
        fprintf(stderr, "service: %s failed: %d\n", command, status);
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    const char *command;
    const char *name = 0;

    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("service", "<list|start|stop|restart|status> [name]");
        return argc < 2 ? 2 : 0;
    }

    command = argv[1];
    if (argc > 2)
        name = argv[2];

    if (strcmp(command, "list") == 0) {
        terra_service_info_t services[64];
        uint32_t count = 0;
        int status = terra_service_list(services, 64, &count);
        if (status < 0)
            return service_backend_result(command, status);
        for (uint32_t i = 0; i < count; ++i)
            printf("%-16s %-8s %s\n", services[i].name, state_name(services[i].state), services[i].description);
        return 0;
    }

    if (!name) {
        fprintf(stderr, "service: %s requires a service name\n", command);
        return 2;
    }

    if (strcmp(command, "start") == 0)
        return service_backend_result(command, terra_service_start(name));

    if (strcmp(command, "stop") == 0)
        return service_backend_result(command, terra_service_stop(name));

    if (strcmp(command, "restart") == 0) {
        int stop_status = terra_service_stop(name);
        if (stop_status < 0 && stop_status != TERRA_STATUS_NOSYS && stop_status != TERRA_STATUS_NOTREADY)
            return service_backend_result(command, stop_status);
        return service_backend_result(command, terra_service_start(name));
    }

    if (strcmp(command, "status") == 0) {
        terra_service_info_t info;
        int status = terra_service_status(name, &info);
        if (status < 0)
            return service_backend_result(command, status);
        printf("%-16s %-8s %s\n", info.name, state_name(info.state), info.description);
        return 0;
    }

    fprintf(stderr, "service: unknown command %s\n", command);
    return 2;
}
