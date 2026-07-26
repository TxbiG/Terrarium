#include "compositor.h"

int lumina_surface_is_visible(const lumina_view_t *view) {
    return view && view->mapped;
}
