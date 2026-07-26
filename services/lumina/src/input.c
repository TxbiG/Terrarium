#include "input.h"
#include "keybinds.h"

void lumina_input_init(lumina_compositor_t *compositor) {
    (void)compositor;
}

void lumina_input_handle_key(lumina_compositor_t *compositor, unsigned keycode, int pressed) {
    if (compositor)
        lumina_compositor_emit_event(compositor, TERRA_DISPLAY_EVENT_KEY, 0, keycode, pressed ? 1u : 0u, compositor->display_serial);
    lumina_keybinds_handle(compositor, keycode, pressed);
}
