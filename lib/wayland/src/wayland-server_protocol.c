#include "wayland-server-protocol.h"

#define TERRA_WL_INTERFACE(name) \
    const struct wl_interface name = { #name, 1, 0, 0, 0, 0 }

TERRA_WL_INTERFACE(wl_display_interface);
TERRA_WL_INTERFACE(wl_registry_interface);
TERRA_WL_INTERFACE(wl_callback_interface);
TERRA_WL_INTERFACE(wl_compositor_interface);
TERRA_WL_INTERFACE(wl_shm_interface);
TERRA_WL_INTERFACE(wl_buffer_interface);
TERRA_WL_INTERFACE(wl_data_offer_interface);
TERRA_WL_INTERFACE(wl_data_source_interface);
TERRA_WL_INTERFACE(wl_data_device_interface);
TERRA_WL_INTERFACE(wl_data_device_manager_interface);
TERRA_WL_INTERFACE(wl_shell_interface);
TERRA_WL_INTERFACE(wl_shell_surface_interface);
TERRA_WL_INTERFACE(wl_surface_interface);
TERRA_WL_INTERFACE(wl_seat_interface);
TERRA_WL_INTERFACE(wl_pointer_interface);
TERRA_WL_INTERFACE(wl_keyboard_interface);
TERRA_WL_INTERFACE(wl_touch_interface);
TERRA_WL_INTERFACE(wl_output_interface);
TERRA_WL_INTERFACE(wl_region_interface);
TERRA_WL_INTERFACE(wl_subcompositor_interface);
TERRA_WL_INTERFACE(wl_subsurface_interface);
