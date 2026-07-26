#ifndef TERRARIUM_LUMINA_XDG_H
#define TERRARIUM_LUMINA_XDG_H

#include "compositor.h"

int lumina_xdg_add_view(lumina_compositor_t *compositor, const char *title);
unsigned lumina_xdg_configure_view(lumina_compositor_t *compositor, unsigned view_id, int width, int height);
int lumina_xdg_ack_configure(lumina_compositor_t *compositor, unsigned view_id, unsigned serial);
int lumina_xdg_request_close(lumina_compositor_t *compositor, unsigned view_id);
int lumina_xdg_close_view(lumina_compositor_t *compositor, unsigned view_id);
void lumina_xdg_init(lumina_compositor_t *compositor);

#endif
