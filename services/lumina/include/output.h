#ifndef TERRARIUM_LUMINA_OUTPUT_H
#define TERRARIUM_LUMINA_OUTPUT_H

#include "compositor.h"

int lumina_output_add(lumina_compositor_t *compositor, const char *name, int width, int height);
void lumina_output_init_defaults(lumina_compositor_t *compositor);

#endif
