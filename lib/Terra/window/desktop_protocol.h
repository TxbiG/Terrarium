#ifndef TERRA_DESKTOP_PROTOCOL_H
#define TERRA_DESKTOP_PROTOCOL_H

#include <stdint.h>
#include "display_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_DESKTOP_PROTOCOL_VERSION 1u
#define TERRA_DESKTOP_MIME_MAX 64u
#define TERRA_DESKTOP_TEXT_MAX 256u

typedef enum terra_shell_surface_role {
    TERRA_SHELL_SURFACE_NORMAL = 0,
    TERRA_SHELL_SURFACE_PANEL,
    TERRA_SHELL_SURFACE_LAUNCHER,
    TERRA_SHELL_SURFACE_NOTIFICATION,
    TERRA_SHELL_SURFACE_LOCK_SCREEN,
    TERRA_SHELL_SURFACE_BACKGROUND
} terra_shell_surface_role_t;

typedef enum terra_shell_edge {
    TERRA_SHELL_EDGE_NONE = 0,
    TERRA_SHELL_EDGE_TOP = 1u << 0,
    TERRA_SHELL_EDGE_RIGHT = 1u << 1,
    TERRA_SHELL_EDGE_BOTTOM = 1u << 2,
    TERRA_SHELL_EDGE_LEFT = 1u << 3
} terra_shell_edge_t;

typedef enum terra_capture_kind {
    TERRA_CAPTURE_SCREENSHOT = 0,
    TERRA_CAPTURE_RECORDING
} terra_capture_kind_t;

typedef struct terra_capture_request {
    terra_capture_kind_t kind;
    uint32_t output_id;
    terra_display_rect_t region;
    uint32_t include_cursor;
    uint32_t requester_pid;
} terra_capture_request_t;

typedef enum terra_clipboard_action {
    TERRA_CLIPBOARD_SET = 0,
    TERRA_CLIPBOARD_GET,
    TERRA_CLIPBOARD_CLEAR
} terra_clipboard_action_t;

typedef struct terra_clipboard_request {
    terra_clipboard_action_t action;
    char mime[TERRA_DESKTOP_MIME_MAX];
    uint32_t bytes;
    uint32_t owner_view;
} terra_clipboard_request_t;

typedef enum terra_drag_action {
    TERRA_DRAG_NONE = 0,
    TERRA_DRAG_COPY = 1u << 0,
    TERRA_DRAG_MOVE = 1u << 1,
    TERRA_DRAG_LINK = 1u << 2
} terra_drag_action_t;

typedef struct terra_drag_request {
    uint32_t source_view;
    uint32_t target_view;
    terra_drag_action_t actions;
    char mime[TERRA_DESKTOP_MIME_MAX];
} terra_drag_request_t;

typedef enum terra_accessibility_role {
    TERRA_A11Y_ROLE_UNKNOWN = 0,
    TERRA_A11Y_ROLE_WINDOW,
    TERRA_A11Y_ROLE_BUTTON,
    TERRA_A11Y_ROLE_TEXT,
    TERRA_A11Y_ROLE_LIST,
    TERRA_A11Y_ROLE_IMAGE
} terra_accessibility_role_t;

typedef struct terra_accessibility_node {
    uint32_t id;
    uint32_t parent;
    terra_accessibility_role_t role;
    terra_display_rect_t bounds;
    char label[TERRA_DESKTOP_TEXT_MAX];
} terra_accessibility_node_t;

typedef enum terra_notification_urgency {
    TERRA_NOTIFICATION_LOW = 0,
    TERRA_NOTIFICATION_NORMAL,
    TERRA_NOTIFICATION_CRITICAL
} terra_notification_urgency_t;

typedef struct terra_notification_request {
    uint32_t app_id;
    terra_notification_urgency_t urgency;
    char title[TERRA_DESKTOP_TEXT_MAX];
    char body[TERRA_DESKTOP_TEXT_MAX];
    uint32_t timeout_ms;
} terra_notification_request_t;

typedef struct terra_layer_surface_request {
    uint32_t view_id;
    terra_shell_surface_role_t role;
    terra_shell_edge_t anchor;
    terra_display_rect_t exclusive_zone;
    uint32_t keyboard_interactive;
} terra_layer_surface_request_t;

#ifdef __cplusplus
}
#endif

#endif
