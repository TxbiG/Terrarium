#include "GUI.h"

#include <stddef.h>

#define TERRA_COLOR_BG 0x101820u
#define TERRA_COLOR_PANEL 0x182430u
#define TERRA_COLOR_PANEL_HI 0x2B6CB0u
#define TERRA_COLOR_TEXT 0xE8F0F8u
#define TERRA_COLOR_MUTED 0x9FB3C8u
#define TERRA_COLOR_ACCENT 0x7DD3FCu

static void put_pixel(const terra_boot_framebuffer_t *fb, uint32_t x, uint32_t y, uint32_t color) {
    if (!fb || !fb->pixels || x >= fb->width || y >= fb->height)
        return;
    fb->pixels[y * fb->pitch_pixels + x] = color;
}

static void draw_rect(const terra_boot_framebuffer_t *fb, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    for (uint32_t yy = y; yy < y + h && yy < fb->height; ++yy) {
        for (uint32_t xx = x; xx < x + w && xx < fb->width; ++xx)
            put_pixel(fb, xx, yy, color);
    }
}

static void draw_char_block(const terra_boot_framebuffer_t *fb, uint32_t x, uint32_t y, char c, uint32_t color) {
    if (c == ' ')
        return;

    uint8_t pattern = (uint8_t)c;
    for (uint32_t row = 0; row < 8; ++row) {
        for (uint32_t col = 0; col < 6; ++col) {
            uint32_t edge = (row == 0 || row == 7 || col == 0 || col == 5);
            uint32_t bit = (pattern >> ((row + col) & 7)) & 1u;
            if (edge || bit)
                put_pixel(fb, x + col, y + row, color);
        }
    }
}

static void draw_text(const terra_boot_framebuffer_t *fb, uint32_t x, uint32_t y, const char *text, uint32_t color) {
    while (text && *text) {
        draw_char_block(fb, x, y, *text, color);
        x += 8;
        ++text;
    }
}

static void draw_entry(const terra_boot_menu_t *menu, uint32_t index, uint32_t x, uint32_t y, uint32_t w) {
    const terra_boot_entry_t *entry = &menu->config->entries[index];
    uint32_t selected = index == menu->selected;
    uint32_t bg = selected ? TERRA_COLOR_PANEL_HI : TERRA_COLOR_PANEL;
    uint32_t fg = selected ? 0xFFFFFFu : TERRA_COLOR_TEXT;

    draw_rect(&menu->fb, x, y, w, 42, bg);
    draw_text(&menu->fb, x + 16, y + 8, entry->name, fg);
    draw_text(&menu->fb, x + 16, y + 24, terra_boot_arch_name(entry->arch), TERRA_COLOR_MUTED);
    draw_text(&menu->fb, x + 112, y + 24, terra_boot_protocol_name(entry->protocol), TERRA_COLOR_MUTED);
}

void terra_boot_menu_init(
    terra_boot_menu_t *menu,
    const terra_boot_config_t *config,
    terra_boot_framebuffer_t framebuffer,
    terra_boot_key_reader_t read_key,
    void *key_user) {
    if (!menu)
        return;

    menu->config = config;
    menu->selected = config ? config->default_entry : 0;
    menu->fb = framebuffer;
    menu->read_key = read_key;
    menu->key_user = key_user;
}

void terra_boot_menu_draw(const terra_boot_menu_t *menu) {
    uint32_t panel_w;
    uint32_t panel_x;
    uint32_t panel_y = 116;

    if (!menu || !menu->config || !menu->fb.pixels)
        return;

    panel_w = menu->fb.width > 760 ? 640 : menu->fb.width;
    if (panel_w > 96)
        panel_w -= 48;
    panel_x = (menu->fb.width - panel_w) / 2;

    draw_rect(&menu->fb, 0, 0, menu->fb.width, menu->fb.height, TERRA_COLOR_BG);
    draw_text(&menu->fb, panel_x, 48, "TerrariumOS Boot", TERRA_COLOR_TEXT);
    draw_text(&menu->fb, panel_x, 72, "Use arrow keys and Enter", TERRA_COLOR_MUTED);

    for (uint32_t i = 0; i < menu->config->entry_count; ++i)
        draw_entry(menu, i, panel_x, panel_y + i * 52, panel_w);

    draw_text(&menu->fb, panel_x, menu->fb.height - 40, "Single boot skips this menu. Multi boot shows configured entries.", TERRA_COLOR_ACCENT);
}

const terra_boot_entry_t *terra_boot_menu_run(terra_boot_menu_t *menu) {
    if (!menu || !menu->config || menu->config->entry_count == 0)
        return NULL;

    terra_boot_menu_draw(menu);

    while (menu->read_key) {
        terra_boot_key_t key = menu->read_key(menu->key_user);

        if (key == TERRA_BOOT_KEY_UP && menu->selected > 0) {
            --menu->selected;
            terra_boot_menu_draw(menu);
        } else if (key == TERRA_BOOT_KEY_DOWN && menu->selected + 1 < menu->config->entry_count) {
            ++menu->selected;
            terra_boot_menu_draw(menu);
        } else if (key == TERRA_BOOT_KEY_ENTER) {
            return &menu->config->entries[menu->selected];
        } else if (key == TERRA_BOOT_KEY_ESCAPE) {
            return terra_boot_select_entry(menu->config);
        }
    }

    return terra_boot_select_entry(menu->config);
}
