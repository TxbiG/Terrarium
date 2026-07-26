#include "app.h"

#include <string.h>

int terra_app_init(terra_app_t *app, const terra_app_desc_t *desc) {
    if (!app || !desc || !desc->name)
        return -1;

    memset(app, 0, sizeof(*app));
    app->desc = *desc;
    terra_window_client_init(&app->window, desc->name, desc->width > 0 ? desc->width : 800, desc->height > 0 ? desc->height : 600);
    if (terra_window_client_connect(&app->window, "lumina") != 0)
        return -1;
    app->running = 1;
    return 0;
}

void terra_app_handle_display_event(terra_app_t *app, const terra_display_event_t *event) {
    if (!app || !event)
        return;
    terra_window_client_handle_event(&app->window, event);
    if (terra_window_client_should_close(&app->window))
        app->running = 0;
}

void terra_app_present(terra_app_t *app) {
    if (!app || !app->running)
        return;
    app->frames++;
}

void terra_app_request_close(terra_app_t *app) {
    if (!app)
        return;
    terra_window_client_request_close(&app->window);
    app->running = 0;
}

void terra_app_shutdown(terra_app_t *app) {
    if (!app)
        return;
    terra_window_client_disconnect(&app->window);
    app->running = 0;
}
