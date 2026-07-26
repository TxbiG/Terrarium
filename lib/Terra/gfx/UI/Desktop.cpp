#include "Desktop.h"

void terra_desktop_init(terra_desktop_t *desktop, int width, int height) {
    if (!desktop)
        return;

    desktop->work_area.x = 0;
    desktop->work_area.y = 0;
    desktop->work_area.width = width;
    desktop->work_area.height = height;
    desktop->background.r = 0.0f;
    desktop->background.g = 0.0f;
    desktop->background.b = 0.0f;
    desktop->background.a = 1.0f;
    desktop->wallpaper_mode = TERRA_WALLPAPER_SOLID;
    desktop->wallpaper_path = 0;
    desktop->workspace_rows = 1;
    desktop->workspace_columns = 1;
}

void terra_desktop_set_background(terra_desktop_t *desktop, color_t color) {
    if (desktop)
        desktop->background = color;
}

void terra_desktop_set_wallpaper(terra_desktop_t *desktop, const char *path, terra_wallpaper_mode_t mode) {
    if (!desktop)
        return;

    desktop->wallpaper_path = path;
    desktop->wallpaper_mode = mode;
}

void terra_desktop_set_workspaces(terra_desktop_t *desktop, unsigned rows, unsigned columns) {
    if (!desktop)
        return;

    desktop->workspace_rows = rows ? rows : 1;
    desktop->workspace_columns = columns ? columns : 1;
}
