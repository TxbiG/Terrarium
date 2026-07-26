#include <Terra/app/app.h>
#include <hardware/hwinfo.h>
#include <runtime/process.h>
#include <runtime/service.h>
#include <status.h>

#include <stdio.h>
#include <string.h>

#define TASK_MANAGER_MAX_PROCESSES 64
#define TASK_MANAGER_MAX_SERVICES 32

typedef struct task_manager_process_row {
    terra_pid_t pid;
    terra_pid_t parent_pid;
    terra_process_state_t state;
    char name[64];
    int available;
} task_manager_process_row_t;

typedef struct task_manager_snapshot {
    terra_hwinfo_t hwinfo;
    int hwinfo_available;
    task_manager_process_row_t processes[TASK_MANAGER_MAX_PROCESSES];
    unsigned process_count;
    terra_service_info_t services[TASK_MANAGER_MAX_SERVICES];
    unsigned service_count;
    int process_backend_ready;
    int service_backend_ready;
    int memory_backend_ready;
    int scheduler_backend_ready;
} task_manager_snapshot_t;

static const char *process_state_name(terra_process_state_t state) {
    switch (state) {
    case TERRA_PROCESS_NEW:
        return "new";
    case TERRA_PROCESS_RUNNING:
        return "running";
    case TERRA_PROCESS_SLEEPING:
        return "sleeping";
    case TERRA_PROCESS_STOPPED:
        return "stopped";
    case TERRA_PROCESS_ZOMBIE:
        return "zombie";
    default:
        return "unknown";
    }
}

static const char *service_state_name(terra_service_state_t state) {
    switch (state) {
    case TERRA_SERVICE_STOPPED:
        return "stopped";
    case TERRA_SERVICE_STARTING:
        return "starting";
    case TERRA_SERVICE_RUNNING:
        return "running";
    case TERRA_SERVICE_FAILED:
        return "failed";
    case TERRA_SERVICE_BLOCKED:
        return "blocked";
    default:
        return "unknown";
    }
}

static void task_manager_collect(task_manager_snapshot_t *snapshot) {
    uint32_t count = 0;
    terra_process_info_t self;

    if (!snapshot)
        return;
    memset(snapshot, 0, sizeof(*snapshot));

    snapshot->hwinfo_available = terra_hwinfo_query(&snapshot->hwinfo) == TERRA_STATUS_OK;
    snapshot->process_backend_ready = terra_process_info(0, &self) == TERRA_STATUS_OK;
    if (snapshot->process_backend_ready) {
        snapshot->processes[0].pid = self.pid;
        snapshot->processes[0].parent_pid = self.parent_pid;
        snapshot->processes[0].state = self.state;
        snprintf(snapshot->processes[0].name, sizeof(snapshot->processes[0].name), "%s", self.name);
        snapshot->processes[0].available = 1;
        snapshot->process_count = 1;
    }

    snapshot->service_backend_ready = terra_service_list(snapshot->services, TASK_MANAGER_MAX_SERVICES, &count) == TERRA_STATUS_OK;
    if (snapshot->service_backend_ready)
        snapshot->service_count = count;

    snapshot->memory_backend_ready = snapshot->hwinfo_available;
    snapshot->scheduler_backend_ready = snapshot->process_backend_ready;
}

static void task_manager_render(const task_manager_snapshot_t *snapshot, const terra_app_t *app) {
    printf("Task Manager [%dx%d] frame=%u\n",
           app->window.view.rect.width,
           app->window.view.rect.height,
           app->frames);

    if (!snapshot)
        return;

    if (snapshot->hwinfo_available) {
        printf("hardware: machine=%s firmware=%s cpus=%u memory=%llu\n",
               snapshot->hwinfo.machine,
               snapshot->hwinfo.firmware,
               snapshot->hwinfo.cpu_count,
               (unsigned long long)snapshot->hwinfo.memory_bytes);
    } else {
        printf("hardware: backend not ready\n");
    }

    if (snapshot->process_backend_ready) {
        printf("processes: %u\n", snapshot->process_count);
        for (unsigned i = 0; i < snapshot->process_count; ++i) {
            printf("  %d %-16s %s parent=%d\n",
                   snapshot->processes[i].pid,
                   snapshot->processes[i].name,
                   process_state_name(snapshot->processes[i].state),
                   snapshot->processes[i].parent_pid);
        }
    } else {
        printf("processes: /proc and process-info syscall not ready\n");
    }

    if (snapshot->service_backend_ready) {
        printf("services: %u\n", snapshot->service_count);
        for (unsigned i = 0; i < snapshot->service_count; ++i)
            printf("  %-16s %s\n", snapshot->services[i].name, service_state_name(snapshot->services[i].state));
    } else {
        printf("services: service manager endpoint not ready\n");
    }
}

int main(int argc, char **argv) {
    task_manager_snapshot_t snapshot;
    terra_app_t app;
    terra_app_desc_t desc = {
        .id = "org.terrarium.TaskManager",
        .name = "Task Manager",
        .kind = TERRA_APP_KIND_SYSTEM,
        .width = 960,
        .height = 700,
    };
    (void)argc;
    (void)argv;

    if (terra_app_init(&app, &desc) != 0) {
        fprintf(stderr, "Task Manager: failed to initialize Terra app runtime\n");
        return 1;
    }

    task_manager_collect(&snapshot);
    terra_app_present(&app);
    task_manager_render(&snapshot, &app);
    terra_app_shutdown(&app);
    return 0;
}
