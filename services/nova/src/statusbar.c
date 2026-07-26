#include "etc/statusbar.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void nova_statusbar_init(nova_statusbar_t *bar) {
    if (!bar) {
        return;
    }

    memset(bar, 0, sizeof(*bar));
    bar->battery_percent = 100;
    bar->visible = 1;
    snprintf(bar->session_text, sizeof(bar->session_text), "Nova desktop");
    nova_statusbar_update(bar);
}

void nova_statusbar_update(nova_statusbar_t *bar) {
    time_t now;
    struct tm *local;

    if (!bar) {
        return;
    }

    now = time(NULL);
    local = localtime(&now);
    if (!local || strftime(bar->clock_text, sizeof(bar->clock_text), "%H:%M:%S", local) == 0) {
        snprintf(bar->clock_text, sizeof(bar->clock_text), "--:--:--");
    }
}

const char *nova_statusbar_summary(const nova_statusbar_t *bar, char *buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) {
        return "";
    }

    if (!bar) {
        snprintf(buffer, buffer_size, "unavailable");
        return buffer;
    }

    snprintf(buffer, buffer_size, "%s, battery %d%%, %s",
             bar->clock_text,
             bar->battery_percent,
             bar->visible ? "visible" : "hidden");
    return buffer;
}
