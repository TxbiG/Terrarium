#ifndef TERRARIUM_UI_DESKTOP_H
#define TERRARIUM_UI_DESKTOP_H

#include "../gfx.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TERRARIUM_DESKTOP_VERSION 0x0200u

typedef enum terra_wallpaper_mode {
    TERRA_WALLPAPER_SOLID = 0,
    TERRA_WALLPAPER_CENTER,
    TERRA_WALLPAPER_STRETCH,
    TERRA_WALLPAPER_TILE,
} terra_wallpaper_mode_t;

typedef struct terra_desktop {
    recti_t work_area;
    color_t background;
    terra_wallpaper_mode_t wallpaper_mode;
    const char *wallpaper_path;
    unsigned workspace_rows;
    unsigned workspace_columns;
} terra_desktop_t;

void terra_desktop_init(terra_desktop_t *desktop, int width, int height);
void terra_desktop_set_background(terra_desktop_t *desktop, color_t color);
void terra_desktop_set_wallpaper(terra_desktop_t *desktop, const char *path, terra_wallpaper_mode_t mode);
void terra_desktop_set_workspaces(terra_desktop_t *desktop, unsigned rows, unsigned columns);

#ifdef __cplusplus
}
#endif

#endif
