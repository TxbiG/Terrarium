#ifndef TERRARIUM_WINDOW_H
#define TERRARIUM_WINDOW_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum WindowFlags {
    WINDOW_FLAGS_NONE = 0,
    WINDOW_FLAGS_NO_TITLEBAR = 1u << 0,
    WINDOW_FLAGS_RESIZE_DISABLED = 1u << 1,
    WINDOW_FLAGS_ALWAYS_ON_TOP = 1u << 2,
    WINDOW_FLAGS_TRANSPARENT = 1u << 3,
    WINDOW_FLAGS_NO_FOCUS = 1u << 4,
    WINDOW_FLAGS_POPUP = 1u << 5,
    WINDOW_FLAGS_BORDERLESS = 1u << 6,
    WINDOW_FLAGS_RESIZABLE = 1u << 7,
    WINDOW_FLAGS_VISIBLE = 1u << 8,
    WINDOW_FLAGS_HIDDEN = 1u << 9,
    WINDOW_FLAGS_FULLSCREEN = 1u << 10,
    WINDOW_FLAGS_EXCLUSIVE_FULLSCREEN = 1u << 11,
    WINDOW_FLAGS_MAXIMIZED = 1u << 12,
    WINDOW_FLAGS_SKIP_TASKBAR = 1u << 13,
    WINDOW_FLAGS_UTILITY = 1u << 14,
    WINDOW_FLAGS_TOOLTIP = 1u << 15,
    WINDOW_FLAGS_INPUT_FOCUS = 1u << 16,
    WINDOW_FLAGS_MOUSE_FOCUS = 1u << 17,
    WINDOW_FLAGS_ALLOW_HIGHDPI = 1u << 18
} WindowFlags;

typedef enum PopupBoxFlags {
    POPUP_BOX_INFO = 1u << 0,
    POPUP_BOX_WARNING = 1u << 1,
    POPUP_BOX_ERROR = 1u << 2,
    POPUP_BOX_MODAL = 1u << 3
} PopupBoxFlags;

typedef struct Window Window;

Window *CreateWindow(const char *title, int width, int height, WindowFlags flags, Window *share);
Window *CreateConsole(const char *title, int width, int height, WindowFlags flags, Window *share);
bool ShowPopup(const char *title, const char *message, PopupBoxFlags flags, Window *parent);

void DestroyWindow(Window *window);
void CloseWindow(Window *window);
bool WindowShouldClose(const Window *window);

void PollEvents(void);
void WaitEvents(void);
void IntervalPollEvents(double seconds);
int DispatchWindowEvents(void);
void RedrawWindow(Window *window);

void SetWindowTitle(Window *window, const char *title);
void SetWindowSize(Window *window, int width, int height);
void SetWindowMaximized(Window *window, bool maximized);
void SetWindowFullscreen(Window *window, bool fullscreen);
void ShowWindow(Window *window);
void HideWindow(Window *window);

#ifdef __cplusplus
}
#endif

#endif /* TERRARIUM_WINDOW_H */
