#include <Terra/app/app.h>
#include <hardware/hwinfo.h>
#include <net/net.h>
#include <runtime/service.h>
#include <status.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SETTINGS_MAX_SERVICES 8u
#define SETTINGS_MAX_INTERFACES 8u

typedef struct settings_probe {
    int hardware_status;
    int service_status;
    int network_status;
    terra_hwinfo_t hardware;
    terra_service_info_t services[SETTINGS_MAX_SERVICES];
    uint32_t service_count;
    terra_net_interface_t interfaces[SETTINGS_MAX_INTERFACES];
    int interface_count;
} settings_probe_t;

static const char *settings_sections[] = {
    "System overview",
    "Display",
    "Sound",
    "Power & battery",
    "Storage",
    "Devices and drivers",
    "Personalisation",
    "Applications and permissions",
    "Time, language, and region",
    "Network and internet",
    "Accounts",
    "Privacy and security",
    "Accessibility",
    "Developer options",
};

static const char *status_label(int status) {
    switch (status) {
    case TERRA_STATUS_OK:
        return "ready";
    case TERRA_STATUS_NOSYS:
        return "not implemented";
    case TERRA_STATUS_NOTREADY:
        return "not ready";
    case TERRA_STATUS_INVAL:
        return "invalid request";
    default:
        return status < 0 ? "backend error" : "ready";
    }
}

static const char *net_state_label(terra_net_state_t state) {
    switch (state) {
    case TERRA_NET_STATE_UP:
        return "up";
    case TERRA_NET_STATE_CONFIGURING:
        return "configuring";
    case TERRA_NET_STATE_DOWN:
    default:
        return "down";
    }
}

static void settings_probe_system(settings_probe_t *probe) {
    if (!probe)
        return;

    memset(probe, 0, sizeof(*probe));
    probe->hardware_status = terra_hwinfo_query(&probe->hardware);
    probe->service_status = terra_service_list(probe->services, SETTINGS_MAX_SERVICES, &probe->service_count);
    probe->network_status = terra_net_list_interfaces(probe->interfaces, SETTINGS_MAX_INTERFACES);
    probe->interface_count = probe->network_status >= 0 ? probe->network_status : 0;
}

static void settings_print_dashboard(const terra_app_t *app, const settings_probe_t *probe) {
    if (!app || !probe)
        return;

    printf("%s\n", app->desc.name);
    printf("  sections:\n");
    for (size_t i = 0; i < sizeof(settings_sections) / sizeof(settings_sections[0]); ++i)
        printf("    - %s\n", settings_sections[i]);

    printf("  hardware API: %s\n", status_label(probe->hardware_status));
    if (probe->hardware_status == TERRA_STATUS_OK) {
        printf("    machine=%s firmware=%s cpus=%u memory=%llu\n",
               probe->hardware.machine,
               probe->hardware.firmware,
               probe->hardware.cpu_count,
               (unsigned long long)probe->hardware.memory_bytes);
    }

    printf("  service API: %s\n", status_label(probe->service_status));
    if (probe->service_status == TERRA_STATUS_OK) {
        for (uint32_t i = 0; i < probe->service_count && i < SETTINGS_MAX_SERVICES; ++i)
            printf("    service[%u]=%s state=%u required=%u\n",
                   i,
                   probe->services[i].name,
                   (unsigned)probe->services[i].state,
                   probe->services[i].required);
    }

    printf("  network API: %s\n", status_label(probe->network_status));
    for (int i = 0; i < probe->interface_count && i < (int)SETTINGS_MAX_INTERFACES; ++i)
        printf("    net[%d]=%s state=%s rx=%llu tx=%llu\n",
               i,
               probe->interfaces[i].name,
               net_state_label(probe->interfaces[i].state),
               (unsigned long long)probe->interfaces[i].rx_bytes,
               (unsigned long long)probe->interfaces[i].tx_bytes);

    printf("  controls: pending settings daemon, policy store, per-panel schemas, and privileged apply/rollback APIs\n");
}

int main(void) {
    settings_probe_t probe;
    settings_probe_system(&probe);

    terra_app_t app;
    const terra_app_desc_t desc = {
        .id = "org.terrarium.Settings",
        .name = "Settings",
        .kind = TERRA_APP_KIND_SYSTEM,
        .width = 960,
        .height = 640,
    };

    if (terra_app_init(&app, &desc) != 0) {
        fprintf(stderr, "Settings: compositor connection unavailable; running system readiness checks only.\n");
        memset(&app, 0, sizeof(app));
        app.desc = desc;
    }

    terra_app_present(&app);
    settings_print_dashboard(&app, &probe);
    terra_app_request_close(&app);
    terra_app_shutdown(&app);

    return 0;
}
