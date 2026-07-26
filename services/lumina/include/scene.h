#ifndef TERRARIUM_LUMINA_SCENE_H
#define TERRARIUM_LUMINA_SCENE_H

#include "compositor.h"

void lumina_scene_init(lumina_compositor_t *compositor);
void lumina_scene_damage_rect(lumina_compositor_t *compositor, terra_display_rect_t rect);
void lumina_scene_damage_view(lumina_compositor_t *compositor, unsigned view_id);
void lumina_scene_request_frame_callback(lumina_compositor_t *compositor);
void lumina_scene_present(lumina_compositor_t *compositor);

#endif
