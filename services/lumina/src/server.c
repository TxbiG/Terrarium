#include "compositor.h"
#include "input.h"
#include "layout.h"
#include "output.h"
#include "scene.h"
#include "xdg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *event_name(terra_display_event_type_t type) {
    switch (type) {
    case TERRA_DISPLAY_EVENT_OUTPUT_ADDED:
        return "output-added";
    case TERRA_DISPLAY_EVENT_OUTPUT_REMOVED:
        return "output-removed";
    case TERRA_DISPLAY_EVENT_VIEW_CREATED:
        return "view-created";
    case TERRA_DISPLAY_EVENT_VIEW_DESTROYED:
        return "view-destroyed";
    case TERRA_DISPLAY_EVENT_VIEW_FOCUSED:
        return "view-focused";
    case TERRA_DISPLAY_EVENT_VIEW_CONFIGURE:
        return "view-configure";
    case TERRA_DISPLAY_EVENT_VIEW_CLOSE_REQUESTED:
        return "view-close-requested";
    case TERRA_DISPLAY_EVENT_KEY:
        return "key";
    case TERRA_DISPLAY_EVENT_POINTER:
        return "pointer";
    case TERRA_DISPLAY_EVENT_FRAME:
        return "frame";
    default:
        return "none";
    }
}

lumina_compositor_t *lumina_compositor_create(void) {
    lumina_compositor_t *compositor = calloc(1, sizeof(*compositor));
    if (!compositor) {
        return NULL;
    }

    compositor->focused_view = -1;
    compositor->layout = LUMINA_LAYOUT_TILE;
    compositor->running = 1;
    compositor->display_serial = 1;

    lumina_output_init_defaults(compositor);
    lumina_scene_init(compositor);
    lumina_input_init(compositor);
    lumina_xdg_init(compositor);
    lumina_layout_apply(compositor, compositor->layout);
    return compositor;
}

void lumina_compositor_emit_event(lumina_compositor_t *compositor, terra_display_event_type_t type, unsigned target_id, unsigned value0, unsigned value1, unsigned serial) {
    terra_display_event_t *event;

    if (!compositor)
        return;

    event = &compositor->event_ring[compositor->event_ring_head];
    event->type = type;
    event->target_id = target_id;
    event->value0 = value0;
    event->value1 = value1;
    event->serial = serial;

    compositor->event_ring_head = (compositor->event_ring_head + 1u) % LUMINA_EVENT_RING_SIZE;
    if (compositor->event_ring_count < LUMINA_EVENT_RING_SIZE)
        compositor->event_ring_count++;
    compositor->event_count++;
}

void lumina_compositor_emit(lumina_compositor_t *compositor, terra_display_event_type_t type, unsigned target_id) {
    unsigned serial = compositor ? compositor->display_serial : 0;
    lumina_compositor_emit_event(compositor, type, target_id, 0, 0, serial);
}

unsigned lumina_compositor_next_serial(lumina_compositor_t *compositor) {
    if (!compositor)
        return 0;

    compositor->display_serial++;
    if (compositor->display_serial == 0)
        compositor->display_serial = 1;
    return compositor->display_serial;
}

void lumina_compositor_run_once(lumina_compositor_t *compositor) {
    if (!compositor || !compositor->running) {
        return;
    }

    lumina_layout_apply(compositor, compositor->layout);
    lumina_scene_request_frame_callback(compositor);
    lumina_scene_present(compositor);
    compositor->frame_count++;
}

void lumina_compositor_print_status(const lumina_compositor_t *compositor) {
    size_t event_limit;

    if (!compositor) {
        return;
    }

    printf("terrarium-lumina: outputs=%zu views=%zu layout=%s running=%s frames=%u events=%u queued=%zu damage=%zu callbacks=%u\n",
           compositor->output_count,
           compositor->view_count,
           lumina_layout_name(compositor->layout),
           compositor->running ? "yes" : "no",
           compositor->frame_count,
           compositor->event_count,
           compositor->event_ring_count,
           compositor->damage_count,
           compositor->frame_callbacks_pending);

    for (size_t i = 0; i < compositor->output_count; ++i) {
        const lumina_output_t *output = &compositor->outputs[i];
        printf("  output %u %s %dx%d enabled=%s\n",
               output->id,
               output->name,
               output->rect.width,
               output->rect.height,
               output->enabled ? "yes" : "no");
    }

    for (size_t i = 0; i < compositor->view_count; ++i) {
        const lumina_view_t *view = &compositor->views[i];
        printf("  view %u mapped=%s focused=%s pending=%u acked=%u title=\"%s\" rect=%d,%d %dx%d\n",
               view->id,
               view->mapped ? "yes" : "no",
               view->focused ? "yes" : "no",
               view->pending_configure_serial,
               view->acked_configure_serial,
               view->title,
               view->rect.x,
               view->rect.y,
               view->rect.width,
               view->rect.height);
    }

    event_limit = compositor->event_ring_count < 8u ? compositor->event_ring_count : 8u;
    for (size_t i = 0; i < event_limit; ++i) {
        size_t newest = (compositor->event_ring_head + LUMINA_EVENT_RING_SIZE - event_limit + i) % LUMINA_EVENT_RING_SIZE;
        const terra_display_event_t *event = &compositor->event_ring[newest];
        printf("  event %s target=%u value=%u,%u serial=%u\n",
               event_name(event->type),
               event->target_id,
               event->value0,
               event->value1,
               event->serial);
    }
}

void lumina_compositor_destroy(lumina_compositor_t *compositor) {
    if (!compositor) {
        return;
    }

    compositor->running = 0;
    free(compositor);
}

compositor_t *compositor_create(void) {
    return lumina_compositor_create();
}

void compositor_run(compositor_t *compositor) {
    lumina_compositor_run_once(compositor);
}

void compositor_destroy(compositor_t *compositor) {
    lumina_compositor_destroy(compositor);
}

