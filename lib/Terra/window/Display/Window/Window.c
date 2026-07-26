#include <Terra/window/window.h>

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wayland-client.h>
#include "xdg-shell-client-protocol.h"

struct Window {
    char *title;
    int width;
    int height;
    int pending_width;
    int pending_height;
    bool open;
    bool configured;
    bool redraw_pending;
    WindowFlags flags;

    struct wl_surface *surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
};

typedef struct terra_wayland_state {
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct xdg_wm_base *wm_base;
    uint32_t compositor_name;
    uint32_t wm_base_name;
} terra_wayland_state_t;

static terra_wayland_state_t g_wayland;

static char *terra_window_strdup(const char *text) {
    size_t len;
    char *copy;

    if (!text)
        text = "Terrarium Window";

    len = strlen(text) + 1u;
    copy = (char *)malloc(len);
    if (!copy)
        return NULL;
    memcpy(copy, text, len);
    return copy;
}

static void xdg_wm_base_ping(void *data, struct xdg_wm_base *wm_base, uint32_t serial) {
    (void)data;
    xdg_wm_base_pong(wm_base, serial);
}

static const struct xdg_wm_base_listener wm_base_listener = {
    xdg_wm_base_ping
};

static void registry_global(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
    terra_wayland_state_t *state = (terra_wayland_state_t *)data;
    uint32_t bind_version;

    if (!state || !interface)
        return;

    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        bind_version = version < 4u ? version : 4u;
        state->compositor = (struct wl_compositor *)wl_registry_bind(registry, name, &wl_compositor_interface, bind_version);
        state->compositor_name = name;
        return;
    }

    if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        bind_version = version < 1u ? version : 1u;
        state->wm_base = (struct xdg_wm_base *)wl_registry_bind(registry, name, &xdg_wm_base_interface, bind_version);
        state->wm_base_name = name;
        xdg_wm_base_add_listener(state->wm_base, &wm_base_listener, state);
    }
}

static void registry_global_remove(void *data, struct wl_registry *registry, uint32_t name) {
    terra_wayland_state_t *state = (terra_wayland_state_t *)data;
    (void)registry;

    if (!state)
        return;

    if (name == state->compositor_name) {
        state->compositor = NULL;
        state->compositor_name = 0;
    }

    if (name == state->wm_base_name) {
        state->wm_base = NULL;
        state->wm_base_name = 0;
    }
}

static const struct wl_registry_listener registry_listener = {
    registry_global,
    registry_global_remove
};

static int ensure_wayland_connected(void) {
    if (g_wayland.display)
        return 0;

    g_wayland.display = wl_display_connect(NULL);
    if (!g_wayland.display) {
        fprintf(stderr, "terrarium-window: failed to connect to Wayland display\n");
        return -1;
    }

    g_wayland.registry = wl_display_get_registry(g_wayland.display);
    if (!g_wayland.registry) {
        wl_display_disconnect(g_wayland.display);
        memset(&g_wayland, 0, sizeof(g_wayland));
        return -1;
    }

    wl_registry_add_listener(g_wayland.registry, &registry_listener, &g_wayland);
    wl_display_roundtrip(g_wayland.display);
    wl_display_roundtrip(g_wayland.display);

    if (!g_wayland.compositor || !g_wayland.wm_base) {
        fprintf(stderr, "terrarium-window: compositor or xdg_wm_base unavailable\n");
        return -1;
    }

    return 0;
}

static void xdg_surface_configure(void *data, struct xdg_surface *surface, uint32_t serial) {
    Window *window = (Window *)data;

    xdg_surface_ack_configure(surface, serial);
    if (!window)
        return;

    if (window->pending_width > 0)
        window->width = window->pending_width;
    if (window->pending_height > 0)
        window->height = window->pending_height;
    window->pending_width = 0;
    window->pending_height = 0;
    window->configured = true;
    window->redraw_pending = true;
}

static const struct xdg_surface_listener xdg_surface_listener = {
    xdg_surface_configure
};

static void xdg_toplevel_configure(void *data, struct xdg_toplevel *toplevel, int32_t width, int32_t height, struct wl_array *states) {
    Window *window = (Window *)data;
    (void)toplevel;
    (void)states;

    if (!window)
        return;

    if (width > 0)
        window->pending_width = width;
    if (height > 0)
        window->pending_height = height;
}

static void xdg_toplevel_close(void *data, struct xdg_toplevel *toplevel) {
    Window *window = (Window *)data;
    (void)toplevel;

    if (window)
        window->open = false;
}

static void xdg_toplevel_configure_bounds(void *data, struct xdg_toplevel *toplevel, int32_t width, int32_t height) {
    Window *window = (Window *)data;
    (void)toplevel;

    if (!window)
        return;
    if (width > 0 && window->width > width)
        window->pending_width = width;
    if (height > 0 && window->height > height)
        window->pending_height = height;
}

static const struct xdg_toplevel_listener xdg_toplevel_listener = {
    xdg_toplevel_configure,
    xdg_toplevel_close,
    xdg_toplevel_configure_bounds
};

Window *CreateWindow(const char *title, int width, int height, WindowFlags flags, Window *share) {
    Window *window;
    (void)share;

    if (ensure_wayland_connected() != 0)
        return NULL;

    window = (Window *)calloc(1, sizeof(*window));
    if (!window)
        return NULL;

    window->title = terra_window_strdup(title);
    if (!window->title) {
        free(window);
        return NULL;
    }

    window->width = width > 0 ? width : 640;
    window->height = height > 0 ? height : 480;
    window->open = true;
    window->flags = flags;

    window->surface = wl_compositor_create_surface(g_wayland.compositor);
    if (!window->surface)
        goto fail;

    window->xdg_surface = xdg_wm_base_get_xdg_surface(g_wayland.wm_base, window->surface);
    if (!window->xdg_surface)
        goto fail;
    xdg_surface_add_listener(window->xdg_surface, &xdg_surface_listener, window);

    window->xdg_toplevel = xdg_surface_get_toplevel(window->xdg_surface);
    if (!window->xdg_toplevel)
        goto fail;
    xdg_toplevel_add_listener(window->xdg_toplevel, &xdg_toplevel_listener, window);
    xdg_toplevel_set_title(window->xdg_toplevel, window->title);
    xdg_toplevel_set_app_id(window->xdg_toplevel, "terrarium.app");

    if ((flags & WINDOW_FLAGS_RESIZE_DISABLED) != 0)
        xdg_toplevel_set_min_size(window->xdg_toplevel, window->width, window->height);
    if ((flags & WINDOW_FLAGS_MAXIMIZED) != 0)
        xdg_toplevel_set_maximized(window->xdg_toplevel);
    if ((flags & WINDOW_FLAGS_FULLSCREEN) != 0 || (flags & WINDOW_FLAGS_EXCLUSIVE_FULLSCREEN) != 0)
        xdg_toplevel_set_fullscreen(window->xdg_toplevel, NULL);

    wl_surface_commit(window->surface);
    wl_display_roundtrip(g_wayland.display);
    return window;

fail:
    DestroyWindow(window);
    return NULL;
}

Window *CreateConsole(const char *title, int width, int height, WindowFlags flags, Window *share) {
    return CreateWindow(title ? title : "Terrarium Console", width, height, flags, share);
}

bool ShowPopup(const char *title, const char *message, PopupBoxFlags flags, Window *parent) {
    (void)flags;
    (void)parent;
    fprintf(stderr, "terrarium-window popup: %s: %s\n", title ? title : "Message", message ? message : "");
    return true;
}

void DestroyWindow(Window *window) {
    if (!window)
        return;

    if (window->xdg_toplevel)
        xdg_toplevel_destroy(window->xdg_toplevel);
    if (window->xdg_surface)
        xdg_surface_destroy(window->xdg_surface);
    if (window->surface)
        wl_surface_destroy(window->surface);
    free(window->title);
    free(window);
}

void CloseWindow(Window *window) {
    if (window)
        window->open = false;
}

bool WindowShouldClose(const Window *window) {
    return !window || !window->open;
}

int DispatchWindowEvents(void) {
    if (!g_wayland.display)
        return 0;
    return wl_display_dispatch_pending(g_wayland.display);
}

void PollEvents(void) {
    if (!g_wayland.display)
        return;
    wl_display_dispatch_pending(g_wayland.display);
    wl_display_flush(g_wayland.display);
}

void WaitEvents(void) {
    if (!g_wayland.display)
        return;
    wl_display_dispatch(g_wayland.display);
    wl_display_flush(g_wayland.display);
}

void IntervalPollEvents(double seconds) {
    struct timespec ts;

    if (seconds < 0.0)
        seconds = 0.0;
    ts.tv_sec = (time_t)seconds;
    ts.tv_nsec = (long)((seconds - (double)ts.tv_sec) * 1000000000.0);
    nanosleep(&ts, NULL);
    PollEvents();
}

void RedrawWindow(Window *window) {
    if (!window || !window->surface)
        return;

    window->redraw_pending = false;
    wl_surface_commit(window->surface);
    if (g_wayland.display)
        wl_display_flush(g_wayland.display);
}

void SetWindowTitle(Window *window, const char *title) {
    char *copy;

    if (!window)
        return;
    copy = terra_window_strdup(title);
    if (!copy)
        return;
    free(window->title);
    window->title = copy;
    if (window->xdg_toplevel)
        xdg_toplevel_set_title(window->xdg_toplevel, window->title);
}

void SetWindowSize(Window *window, int width, int height) {
    if (!window)
        return;
    if (width > 0)
        window->width = width;
    if (height > 0)
        window->height = height;
    window->redraw_pending = true;
}

void SetWindowMaximized(Window *window, bool maximized) {
    if (!window || !window->xdg_toplevel)
        return;
    if (maximized)
        xdg_toplevel_set_maximized(window->xdg_toplevel);
    else
        xdg_toplevel_unset_maximized(window->xdg_toplevel);
    wl_surface_commit(window->surface);
}

void SetWindowFullscreen(Window *window, bool fullscreen) {
    if (!window || !window->xdg_toplevel)
        return;
    if (fullscreen)
        xdg_toplevel_set_fullscreen(window->xdg_toplevel, NULL);
    else
        xdg_toplevel_unset_fullscreen(window->xdg_toplevel);
    wl_surface_commit(window->surface);
}

void ShowWindow(Window *window) {
    if (!window)
        return;
    window->flags &= ~WINDOW_FLAGS_HIDDEN;
    window->flags |= WINDOW_FLAGS_VISIBLE;
    RedrawWindow(window);
}

void HideWindow(Window *window) {
    if (!window)
        return;
    window->flags &= ~WINDOW_FLAGS_VISIBLE;
    window->flags |= WINDOW_FLAGS_HIDDEN;
}

