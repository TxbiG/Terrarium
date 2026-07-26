#include "login.h"
#include <wlr/types/wlr_scene.h>
#include <wlr/util/log.h>
#include <string.h>
#include <stdio.h>

struct login_t *login_create(struct compositor_t *comp) {
    struct login_t *gui = calloc(1, sizeof(*gui));
    gui->comp = comp;

    // Create fullscreen scene tree for login window
    gui->window = wlr_scene_tree_create(comp->scene);

    // TODO: draw rectangle for login box
    wlr_scene_rect_create(gui->window, 400, 200, (float[]){0.2,0.2,0.2,1.0});
    wlr_scene_node_set_position(&gui->window->node, (comp->scene->width - 400)/2, (comp->scene->height - 200)/2);

    return gui;
}

void login_run(struct login_t *gui) {
    // Minimal event loop
    while (1) {
        // TODO: handle keyboard input
        // - Type username
        // - Select profile
        // - Enter key triggers authentication
    }
}

void login_destroy(struct login_t *gui) {
    if (!gui) return;
    wlr_scene_node_destroy(&gui->window->node);
    free(gui);
}