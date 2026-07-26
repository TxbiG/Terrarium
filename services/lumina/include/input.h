#ifndef TERRARIUM_LUMINA_INPUT_H
#define TERRARIUM_LUMINA_INPUT_H

#include "compositor.h"

void lumina_input_init(lumina_compositor_t *compositor);
void lumina_input_handle_key(lumina_compositor_t *compositor, unsigned keycode, int pressed);

#endif
