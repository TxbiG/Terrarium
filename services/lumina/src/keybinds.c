#include "keybinds.h"
#include "layout.h"
#include "wm.h"

void lumina_keybinds_handle(lumina_compositor_t *compositor, unsigned keycode, int pressed) {
    if (!compositor || !pressed) {
        return;
    }

    if (keycode == 24 && compositor->focused_view >= 0) {
        compositor->views[compositor->focused_view].mapped = 0;
        lumina_wm_arrange(compositor);
    }

    if (keycode == 65) {
        compositor->layout = compositor->layout == LUMINA_LAYOUT_TILE
            ? LUMINA_LAYOUT_FLOATING
            : LUMINA_LAYOUT_TILE;
        lumina_wm_arrange(compositor);
    }
}
