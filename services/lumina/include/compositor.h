#ifndef TERRARIUM_LUMINA_COMPOSITOR_H
#define TERRARIUM_LUMINA_COMPOSITOR_H

#include <stddef.h>
#include <Terra/window/display_protocol.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LUMINA_MAX_OUTPUTS 8
#define LUMINA_MAX_VIEWS 64
#define LUMINA_EVENT_RING_SIZE 128
#define LUMINA_MAX_DAMAGE_RECTS 64

typedef enum lumina_layout {
    LUMINA_LAYOUT_TILE = 0,
    LUMINA_LAYOUT_MONOCLE,
    LUMINA_LAYOUT_FLOATING,
} lumina_layout_t;

typedef struct lumina_rect {
    int x;
    int y;
    int width;
    int height;
} lumina_rect_t;

typedef struct lumina_view {
    unsigned id;
    char title[64];
    lumina_rect_t rect;
    int mapped;
    int floating;
    int focused;
    unsigned pending_configure_serial;
    unsigned acked_configure_serial;
} lumina_view_t;

typedef struct lumina_output {
    unsigned id;
    char name[32];
    lumina_rect_t rect;
    int enabled;
} lumina_output_t;

typedef struct lumina_compositor {
    lumina_output_t outputs[LUMINA_MAX_OUTPUTS];
    lumina_view_t views[LUMINA_MAX_VIEWS];
    terra_display_event_t event_ring[LUMINA_EVENT_RING_SIZE];
    terra_display_rect_t damage[LUMINA_MAX_DAMAGE_RECTS];
    size_t output_count;
    size_t view_count;
    size_t event_ring_head;
    size_t event_ring_count;
    size_t damage_count;
    unsigned frame_callbacks_pending;
    int focused_view;
    lumina_layout_t layout;
    int running;
    unsigned frame_count;
    unsigned event_count;
    unsigned display_serial;
} lumina_compositor_t;

typedef lumina_compositor_t compositor_t;
typedef lumina_view_t view_t;
typedef lumina_output_t output_t;

lumina_compositor_t *lumina_compositor_create(void);
void lumina_compositor_run_once(lumina_compositor_t *compositor);
void lumina_compositor_emit(lumina_compositor_t *compositor, terra_display_event_type_t type, unsigned target_id);
void lumina_compositor_emit_event(lumina_compositor_t *compositor, terra_display_event_type_t type, unsigned target_id, unsigned value0, unsigned value1, unsigned serial);
unsigned lumina_compositor_next_serial(lumina_compositor_t *compositor);
void lumina_compositor_print_status(const lumina_compositor_t *compositor);
void lumina_compositor_destroy(lumina_compositor_t *compositor);

compositor_t *compositor_create(void);
void compositor_run(compositor_t *compositor);
void compositor_destroy(compositor_t *compositor);

#ifdef __cplusplus
}
#endif

#endif

