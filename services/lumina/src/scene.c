#include "scene.h"

static terra_display_rect_t rect_from_view(const lumina_view_t *view) {
    terra_display_rect_t rect;
    rect.x = view ? view->rect.x : 0;
    rect.y = view ? view->rect.y : 0;
    rect.width = view ? view->rect.width : 0;
    rect.height = view ? view->rect.height : 0;
    return rect;
}

void lumina_scene_init(lumina_compositor_t *compositor) {
    if (!compositor)
        return;
    compositor->damage_count = 0;
    compositor->frame_callbacks_pending = 0;
}

void lumina_scene_damage_rect(lumina_compositor_t *compositor, terra_display_rect_t rect) {
    if (!compositor || rect.width <= 0 || rect.height <= 0)
        return;

    if (compositor->damage_count < LUMINA_MAX_DAMAGE_RECTS) {
        compositor->damage[compositor->damage_count++] = rect;
    } else if (compositor->output_count > 0) {
        compositor->damage[0].x = compositor->outputs[0].rect.x;
        compositor->damage[0].y = compositor->outputs[0].rect.y;
        compositor->damage[0].width = compositor->outputs[0].rect.width;
        compositor->damage[0].height = compositor->outputs[0].rect.height;
        compositor->damage_count = 1;
    }
}

void lumina_scene_damage_view(lumina_compositor_t *compositor, unsigned view_id) {
    if (!compositor)
        return;

    for (size_t i = 0; i < compositor->view_count; ++i) {
        if (compositor->views[i].id == view_id) {
            lumina_scene_damage_rect(compositor, rect_from_view(&compositor->views[i]));
            return;
        }
    }
}

void lumina_scene_request_frame_callback(lumina_compositor_t *compositor) {
    if (compositor)
        compositor->frame_callbacks_pending++;
}

void lumina_scene_present(lumina_compositor_t *compositor) {
    if (!compositor)
        return;

    if (compositor->damage_count > 0 || compositor->frame_callbacks_pending > 0) {
        lumina_compositor_emit_event(compositor,
                                     TERRA_DISPLAY_EVENT_FRAME,
                                     0,
                                     (unsigned)compositor->damage_count,
                                     compositor->frame_callbacks_pending,
                                     compositor->display_serial);
    }

    compositor->damage_count = 0;
    compositor->frame_callbacks_pending = 0;
}
