#ifndef TERRA_APP_H
#define TERRA_APP_H

#include <Terra/window/window_client.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum terra_app_kind {
    TERRA_APP_KIND_UTILITY = 0,
    TERRA_APP_KIND_DESKTOP,
    TERRA_APP_KIND_GAME,
    TERRA_APP_KIND_SYSTEM
} terra_app_kind_t;

typedef struct terra_app_desc {
    const char *id;
    const char *name;
    terra_app_kind_t kind;
    int width;
    int height;
} terra_app_desc_t;

typedef struct terra_app {
    terra_app_desc_t desc;
    terra_window_client_t window;
    int running;
    unsigned frames;
} terra_app_t;

int terra_app_init(terra_app_t *app, const terra_app_desc_t *desc);
void terra_app_handle_display_event(terra_app_t *app, const terra_display_event_t *event);
void terra_app_present(terra_app_t *app);
void terra_app_request_close(terra_app_t *app);
void terra_app_shutdown(terra_app_t *app);

#ifdef __cplusplus
}
#endif

#endif
