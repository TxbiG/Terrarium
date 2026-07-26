#ifndef TERRA_DISPLAY_PROTOCOL_H
#define TERRA_DISPLAY_PROTOCOL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_DISPLAY_PROTOCOL_VERSION 1u
#define TERRA_DISPLAY_NAME_MAX 64u
#define TERRA_DISPLAY_TITLE_MAX 128u

typedef enum terra_display_event_type {
    TERRA_DISPLAY_EVENT_NONE = 0,
    TERRA_DISPLAY_EVENT_OUTPUT_ADDED,
    TERRA_DISPLAY_EVENT_OUTPUT_REMOVED,
    TERRA_DISPLAY_EVENT_VIEW_CREATED,
    TERRA_DISPLAY_EVENT_VIEW_DESTROYED,
    TERRA_DISPLAY_EVENT_VIEW_FOCUSED,
    TERRA_DISPLAY_EVENT_VIEW_CONFIGURE,
    TERRA_DISPLAY_EVENT_VIEW_CLOSE_REQUESTED,
    TERRA_DISPLAY_EVENT_KEY,
    TERRA_DISPLAY_EVENT_POINTER,
    TERRA_DISPLAY_EVENT_FRAME
} terra_display_event_type_t;

typedef struct terra_display_rect {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
} terra_display_rect_t;

typedef struct terra_display_output_info {
    uint32_t id;
    char name[TERRA_DISPLAY_NAME_MAX];
    terra_display_rect_t rect;
    uint32_t enabled;
} terra_display_output_info_t;

typedef struct terra_display_view_info {
    uint32_t id;
    char title[TERRA_DISPLAY_TITLE_MAX];
    terra_display_rect_t rect;
    uint32_t mapped;
    uint32_t focused;
} terra_display_view_info_t;

typedef struct terra_display_event {
    terra_display_event_type_t type;
    uint32_t target_id;
    uint32_t value0;
    uint32_t value1;
    uint32_t serial;
} terra_display_event_t;

#ifdef __cplusplus
}
#endif

#endif
