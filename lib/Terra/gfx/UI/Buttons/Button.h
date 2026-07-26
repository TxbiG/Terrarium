#ifndef TERRARIUM_UI_BUTTON_H
#define TERRARIUM_UI_BUTTON_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum terra_button_action {
    TERRA_BUTTON_RELEASED = 0,
    TERRA_BUTTON_PRESSED = 1,
} terra_button_action_t;

typedef enum terra_button_mask {
    TERRA_BUTTON_LEFT = 1 << 0,
    TERRA_BUTTON_RIGHT = 1 << 1,
    TERRA_BUTTON_MIDDLE = 1 << 2,
} terra_button_mask_t;

typedef struct terra_button {
    const char *label;
    bool disabled;
    bool pressed;
    bool hovered;
    unsigned mask;
} terra_button_t;

static inline terra_button_t terra_button_create(const char *label) {
    terra_button_t button;
    button.label = label;
    button.disabled = false;
    button.pressed = false;
    button.hovered = false;
    button.mask = TERRA_BUTTON_LEFT;
    return button;
}

static inline bool terra_button_activate(terra_button_t *button, terra_button_action_t action) {
    if (!button || button->disabled)
        return false;

    button->pressed = action == TERRA_BUTTON_PRESSED;
    return true;
}

#ifdef __cplusplus
}
#endif

#endif
