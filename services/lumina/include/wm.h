#ifndef TERRARIUM_LUMINA_WM_H
#define TERRARIUM_LUMINA_WM_H

#include "compositor.h"

void lumina_wm_arrange(lumina_compositor_t *compositor);
int lumina_wm_focus(lumina_compositor_t *compositor, unsigned view_id);
int lumina_wm_focus_next(lumina_compositor_t *compositor);

#endif