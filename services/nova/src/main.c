#include "server.h"
#include <stdio.h>
#include <string.h>

static recti_t nova_launcher_rect(void) {
    recti_t rect;
    rect.x = 8;
    rect.y = 4;
    rect.width = 120;
    rect.height = terra_taskbar_height() - 8;
    return rect;
}

int nova_server_init(nova_server_t *server, int width, int height) {
    if (!server) {
        return -1;
    }

    memset(server, 0, sizeof(*server));
    terra_desktop_init(&server->desktop, width, height);
    terra_taskbar_init(&server->taskbar, TERRA_TASKBAR_BOTTOM, width, height);
    terra_taskbar_set_blur(&server->taskbar, 1, 0.35f);
    server->launcher_button = terra_widget_create(NOVA_LAUNCHER_WIDGET_ID, nova_launcher_rect(), "Launch");

    nova_statusbar_init(&server->statusbar);
    nova_network_tray_init(&server->network_tray);
    nova_workspace_manager_init(&server->workspaces, NOVA_MAX_WORKSPACES);
    nova_protocol_client_init(&server->protocol);
    nova_protocol_client_connect(&server->protocol, "lumina");
    server->running = 1;
    return 0;
}

void nova_server_run_once(nova_server_t *server) {
    if (!server || !server->running) {
        return;
    }

    nova_statusbar_update(&server->statusbar);
    nova_network_tray_update(&server->network_tray);
}

void nova_server_print_status(const nova_server_t *server) {
    char status[128];
    char workspaces[128];
    const nova_workspace_t *workspace;

    if (!server) {
        return;
    }

    workspace = nova_workspace_current(&server->workspaces);
    nova_statusbar_summary(&server->statusbar, status, sizeof(status));
    nova_workspace_list(&server->workspaces, workspaces, sizeof(workspaces));

    printf("%s: %dx%d\n", NOVA_SERVICE_NAME, server->desktop.work_area.width, server->desktop.work_area.height);
    printf("taskbar: %s, launcher=%s\n", server->taskbar.visible ? "visible" : "hidden", server->launcher_button.label);
    printf("status: %s\n", status);
    printf("network: %s on %s\n", nova_network_state_name(server->network_tray.state), server->network_tray.interface_name);
    printf("workspace: %s\n", workspace ? workspace->name : "none");
    printf("compositor: %s protocol=%u connected=%s\n",
           server->protocol.compositor_name,
           server->protocol.protocol_version,
           nova_protocol_client_is_connected(&server->protocol) ? "yes" : "no");
    printf("workspaces: %s\n", workspaces);
}

void nova_server_shutdown(nova_server_t *server) {
    if (server) {
        nova_protocol_client_disconnect(&server->protocol);
        server->running = 0;
    }
}

static void print_usage(const char *program) {
    printf("usage: %s [--status|--once|--run]\n", program);
}

int main(int argc, char **argv) {
    nova_server_t server;
    const char *mode = argc > 1 ? argv[1] : "--once";

    if (nova_server_init(&server, NOVA_DEFAULT_WIDTH, NOVA_DEFAULT_HEIGHT) != 0) {
        fprintf(stderr, "%s: failed to initialize\n", NOVA_SERVICE_NAME);
        return 1;
    }

    if (strcmp(mode, "--status") == 0 || strcmp(mode, "--once") == 0) {
        nova_server_run_once(&server);
        nova_server_print_status(&server);
        nova_server_shutdown(&server);
        return 0;
    }

    if (strcmp(mode, "--run") == 0) {
        printf("%s: desktop shell started\n", NOVA_SERVICE_NAME);
        while (server.running) {
            nova_server_run_once(&server);
            nova_server_print_status(&server);
            nova_server_shutdown(&server);
        }
        return 0;
    }

    print_usage(argv[0]);
    nova_server_shutdown(&server);
    return 2;
}

