#ifndef TERRARIUM_BOOT_GUI_H
#define TERRARIUM_BOOT_GUI_H

#include <stdint.h>
#include "../../include/boot_config.h"

typedef struct terra_boot_framebuffer {
    uint32_t *pixels;
    uint32_t width;
    uint32_t height;
    uint32_t pitch_pixels;
} terra_boot_framebuffer_t;

typedef enum terra_boot_key {
    TERRA_BOOT_KEY_NONE = 0,
    TERRA_BOOT_KEY_UP,
    TERRA_BOOT_KEY_DOWN,
    TERRA_BOOT_KEY_ENTER,
    TERRA_BOOT_KEY_ESCAPE
} terra_boot_key_t;

typedef terra_boot_key_t (*terra_boot_key_reader_t)(void *user);

typedef struct terra_boot_menu {
    const terra_boot_config_t *config;
    uint32_t selected;
    terra_boot_framebuffer_t fb;
    terra_boot_key_reader_t read_key;
    void *key_user;
} terra_boot_menu_t;

void terra_boot_menu_init(
    terra_boot_menu_t *menu,
    const terra_boot_config_t *config,
    terra_boot_framebuffer_t framebuffer,
    terra_boot_key_reader_t read_key,
    void *key_user);

const terra_boot_entry_t *terra_boot_menu_run(terra_boot_menu_t *menu);
void terra_boot_menu_draw(const terra_boot_menu_t *menu);

#endif
