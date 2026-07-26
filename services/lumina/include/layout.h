#ifndef TERRARIUM_LUMINA_LAYOUT_H
#define TERRARIUM_LUMINA_LAYOUT_H

#include "compositor.h"

void lumina_layout_apply(lumina_compositor_t *compositor, lumina_layout_t layout);
const char *lumina_layout_name(lumina_layout_t layout);

#endif
