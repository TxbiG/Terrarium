#include <stdint.h>
#include "wlr-client-protocol.h"

typedef struct wl_drm { struct wl_proxy *proxy; } wl_drm;
typedef struct zwlr_input_method { struct wl_proxy *proxy; } zwlr_input_method;
typedef struct zwlr_idle_timeout { struct wl_proxy *proxy; } zwlr_idle_timeout;    // underlying Wayland proxy
typedef struct zwlr_idle { struct wl_proxy *proxy; } zwlr_idle; // underlying Wayland proxy
typedef struct zwlr_input_popup_surface { struct wl_proxy *proxy; } zwlr_input_popup_surface;
typedef struct zwlr_input_method_keyboard_grab { struct wl_proxy *proxy; } zwlr_input_method_keyboard_grab;
typedef struct zwlr_input_method_manager { struct wl_proxy *proxy; } zwlr_input_method_manager;
typedef struct zwlr_server_decoration_manager { struct wl_proxy* proxy; } zwlr_server_decoration_manager;
typedef struct zwlr_server_decoration { struct wl_proxy *proxy; } zwlr_server_decoration;
typedef struct zwlr_export_manager { struct wl_proxy *proxy; } zwlr_export_manager;
typedef struct zwlr_export_frame { struct wl_proxy *proxy; } zwlr_export_frame;
typedef struct zwlr_virtual_keyboard { struct wl_proxy *proxy; } zwlr_virtual_keyboard;
typedef struct zwlr_virtual_keyboard_manager { struct wl_proxy *proxy; } zwlr_virtual_keyboard_manager;
typedef struct zwlr_data_control_manager { struct wl_proxy *proxy; } zwlr_data_control_manager;
typedef struct zwlr_data_control_device { struct wl_proxy *proxy; } zwlr_data_control_device;
typedef struct zwlr_data_control_source { struct wl_proxy *proxy; } zwlr_data_control_source;
typedef struct zwlr_data_control_offer { struct wl_proxy *proxy; } zwlr_data_control_offer;
typedef struct zwlr_foreign_toplevel_manager { struct wl_proxy *proxy; } zwlr_foreign_toplevel_manager;
typedef struct zwlr_foreign_toplevel_handle { struct wl_proxy *proxy; } zwlr_foreign_toplevel_handle;
typedef struct zwlr_gamma_control_manager { struct wl_proxy *proxy; } zwlr_gamma_control_manager;
typedef struct zwlr_gamma_control { struct wl_proxy *proxy; } zwlr_gamma_control;
typedef struct zwlr_input_inhibit_manager { struct wl_proxy *proxy; } zwlr_input_inhibit_manager;
typedef struct zwlr_input_inhibitor { struct wl_proxy *proxy; } zwlr_input_inhibitor;
typedef struct zwlr_layer_shell { struct wl_proxy *proxy; } zwlr_layer_shell;
typedef struct zwlr_layer_surface { struct wl_proxy *proxy; } zwlr_layer_surface;
typedef struct zwlr_output_manager { struct wl_proxy *proxy; } zwlr_output_manager;
typedef struct zwlr_output_head { struct wl_proxy *proxy; } zwlr_output_head;
typedef struct zwlr_output_mode { struct wl_proxy *proxy; } zwlr_output_mode;
typedef struct zwlr_output_configuration { struct wl_proxy *proxy; } zwlr_output_configuration;
typedef struct zwlr_output_configuration_head { struct wl_proxy *proxy; } zwlr_output_configuration_head;
typedef struct zwlr_output_power_manager { struct wl_proxy *proxy; } zwlr_output_power_manager;
typedef struct zwlr_output_power { struct wl_proxy *proxy; } zwlr_output_power;
typedef struct zwlr_screencopy_manager { struct wl_proxy *proxy; } zwlr_screencopy_manager;
typedef struct zwlr_screencopy_frame { struct wl_proxy *proxy; } zwlr_screencopy_frame;
typedef struct zwlr_virtual_pointer { struct wl_proxy *proxy; } zwlr_virtual_pointer;
typedef struct zwlr_virtual_pointer_manager { struct wl_proxy *proxy; } zwlr_virtual_pointer_manager;

// Event listeners
typedef void (*zwlr_idle_timeout_idle_handler)(void *data, struct zwlr_idle_timeout *timeout);

typedef void (*zwlr_idle_timeout_resumed_handler)(void *data, struct zwlr_idle_timeout *timeout);

typedef void (*wl_drm_device_handler)(void *data, struct wl_drm *drm, const char *name);

typedef void (*wl_drm_format_handler)(void *data, struct wl_drm *drm, uint32_t format);

typedef void (*wl_drm_authenticated_handler)(void *data, struct wl_drm *drm);

typedef void (*wl_drm_capabilities_handler)(void *data, struct wl_drm *drm, uint32_t value);

typedef void (*zwlr_input_method_activate_handler)(void *data, struct zwlr_input_method *input_method);

typedef void (*zwlr_input_method_deactivate_handler)(void *data, struct zwlr_input_method *input_method);

typedef void (*zwlr_input_method_surrounding_text_handler)(void *data, struct zwlr_input_method *input_method, const char *text, uint32_t cursor, uint32_t anchor);

typedef void (*zwlr_input_method_text_change_cause_handler)(void *data, struct zwlr_input_method *input_method, uint32_t cause);

typedef void (*zwlr_input_method_content_type_handler)(void *data, struct zwlr_input_method *input_method, uint32_t hint, uint32_t purpose);

typedef void (*zwlr_input_method_done_handler)(void *data, struct zwlr_input_method *input_method);

typedef void (*zwlr_input_method_unavailable_handler)(void *data, struct zwlr_input_method *input_method);
           
typedef void (*zwlr_input_popup_surface_text_input_rectangle_handler)(void *data, struct zwlr_input_popup_surface *popup, int32_t x, int32_t y, int32_t width, int32_t height);
    
typedef void (*zwlr_input_method_keyboard_grab_keymap_handler)(void *data, struct zwlr_input_method_keyboard_grab *grab, uint32_t format, int fd, uint32_t size);

typedef void (*zwlr_input_method_keyboard_grab_key_handler)(void *data, struct zwlr_input_method_keyboard_grab *grab, uint32_t serial, uint32_t time, uint32_t key, uint32_t state);

typedef void (*zwlr_input_method_keyboard_grab_modifiers_handler)(void *data, struct zwlr_input_method_keyboard_grab *grab, uint32_t serial, 
    uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group);

typedef void (*zwlr_input_method_keyboard_grab_repeat_info_handler)(void *data, struct zwlr_input_method_keyboard_grab *grab, int32_t rate, int32_t delay);

typedef void (*zwlr_server_decoration_manager_default_mode_handler)(void *data, struct zwlr_server_decoration_manager *mgr, uint32_t mode);

typedef void (*zwlr_server_decoration_mode_handler)(void *data, struct zwlr_server_decoration *decoration, uint32_t mode);

typedef void (*zwlr_export_frame_frame_handler)(void *data, struct zwlr_export_frame *frame, uint32_t width, uint32_t height, uint32_t offset_x, uint32_t offset_y,
    uint32_t buffer_flags, uint32_t flags, uint32_t format, uint32_t mod_high, uint32_t mod_low, uint32_t num_objects);

typedef void (*zwlr_export_frame_object_handler)(void *data, struct zwlr_export_frame *frame, uint32_t index, int fd, uint32_t size, uint32_t offset, uint32_t stride, uint32_t plane_index);

typedef void (*zwlr_export_frame_ready_handler)(void *data, struct zwlr_export_frame *frame, uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec);

typedef void (*zwlr_export_frame_cancel_handler)(void *data, struct zwlr_export_frame *frame, uint32_t reason);

typedef void (*zwlr_output_manager_head_handler)(void *data, struct zwlr_output_manager *manager, struct zwlr_output_head *head);

typedef void (*zwlr_output_manager_done_handler)(void *data, struct zwlr_output_manager *manager, uint32_t serial);

typedef void (*zwlr_output_manager_finished_handler)(void *data,  struct zwlr_output_manager *manager);

typedef void (*zwlr_output_head_name_handler)(void *data, struct zwlr_output_head *head, const char *name);

typedef void (*zwlr_output_head_description_handler)(void *data, struct zwlr_output_head *head, const char *description);

typedef void (*zwlr_output_head_physical_size_handler)(void *data, struct zwlr_output_head *head, int32_t width, int32_t height);

typedef void (*zwlr_output_head_mode_handler)(void *data, struct zwlr_output_head *head, struct zwlr_output_mode *mode);

typedef void (*zwlr_output_head_enabled_handler)(void *data, struct zwlr_output_head *head, int32_t enabled);

typedef void (*zwlr_output_head_current_mode_handler)(void *data, struct zwlr_output_head *head, struct zwlr_output_mode *mode);

typedef void (*zwlr_output_head_position_handler)(void *data, struct zwlr_output_head *head, int32_t x,  int32_t y);

typedef void (*zwlr_output_head_transform_handler)(void *data, struct zwlr_output_head *head, int32_t transform);

typedef void (*zwlr_output_head_scale_handler)(void *data, struct zwlr_output_head *head, wl_fixed_t scale);

typedef void (*zwlr_output_head_finished_handler)(void *data, struct zwlr_output_head *head);

typedef void (*zwlr_output_head_make_handler)(void *data, struct zwlr_output_head *head, const char *make);

typedef void (*zwlr_output_head_model_handler)(void *data, struct zwlr_output_head *head, const char *model);

typedef void (*zwlr_output_head_serial_number_handler)(void *data, struct zwlr_output_head *head, const char *serial_number);

typedef void (*zwlr_output_mode_size_handler)(void *data, struct zwlr_output_mode *mode, int32_t width, int32_t height);

typedef void (*zwlr_output_mode_refresh_handler)(void *data, struct zwlr_output_mode *mode, int32_t refresh);

typedef void (*zwlr_output_mode_preferred_handler)(void *data, struct zwlr_output_mode *mode);

typedef void (*zwlr_output_mode_finished_handler)(void *data, struct zwlr_output_mode *mode);

typedef void (*zwlr_output_configuration_succeeded_handler)(void *data, struct zwlr_output_configuration *config);

typedef void (*zwlr_output_configuration_failed_handler)(void *data, struct zwlr_output_configuration *config);

typedef void (*zwlr_output_configuration_cancelled_handler)(void *data, struct zwlr_output_configuration *config);

typedef void (*zwlr_output_power_mode_handler)(void *data, struct zwlr_output_power *power, uint32_t mode);

typedef void (*zwlr_output_power_failed_handler)(void *data, struct zwlr_output_power *power);

typedef void (*zwlr_screencopy_frame_buffer_handler)(void *data, struct zwlr_screencopy_frame *frame, uint32_t format, uint32_t width, uint32_t height, uint32_t stride);

typedef void (*zwlr_screencopy_frame_flags_handler)(void *data, struct zwlr_screencopy_frame *frame, uint32_t flags);

typedef void (*zwlr_screencopy_frame_ready_handler)(void *data, struct zwlr_screencopy_frame *frame, uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec);

typedef void (*zwlr_screencopy_frame_failed_handler)(void *data, struct zwlr_screencopy_frame *frame);

typedef void (*zwlr_screencopy_frame_damage_handler)(void *data, struct zwlr_screencopy_frame *frame, uint32_t x, uint32_t y, uint32_t width, uint32_t height);

typedef void (*zwlr_screencopy_frame_linux_handler)(void *data, struct zwlr_screencopy_frame *frame, uint32_t format, uint32_t width, uint32_t height);

typedef void (*zwlr_screencopy_frame_buffer_done_handler)(void *data, struct zwlr_screencopy_frame *frame);


struct zwlr_idle_timeout_listener { 
    zwlr_idle_timeout_idle_handler idle;
    zwlr_idle_timeout_resumed_handler resumed;
};

struct wl_drm_listener {
    wl_drm_device_handler device;
    wl_drm_format_handler format;
    wl_drm_authenticated_handler authenticated;
    wl_drm_capabilities_handler capabilities;
};

struct zwlr_input_method_listener {
    zwlr_input_method_activate_handler activate;
    zwlr_input_method_deactivate_handler deactivate;
    zwlr_input_method_surrounding_text_handler surrounding_text;
    zwlr_input_method_text_change_cause_handler text_change_cause;
    zwlr_input_method_content_type_handler content_type;
    zwlr_input_method_done_handler done;
    zwlr_input_method_unavailable_handler unavailable;
};

struct zwlr_input_popup_surface_listener {
    zwlr_input_popup_surface_text_input_rectangle_handler text_input_rectangle;
};

struct zwlr_input_method_keyboard_grab_listener {
    zwlr_input_method_keyboard_grab_keymap_handler keymap;
    zwlr_input_method_keyboard_grab_key_handler key;
    zwlr_input_method_keyboard_grab_modifiers_handler modifiers;
    zwlr_input_method_keyboard_grab_repeat_info_handler repeat_info;
};

struct zwlr_server_decoration_manager_listener {
    zwlr_server_decoration_manager_default_mode_handler default_mode;
};

struct zwlr_server_decoration_listener { zwlr_server_decoration_mode_handler mode; };

struct zwlr_export_frame_listener {
    zwlr_export_frame_frame_handler frame;
    zwlr_export_frame_object_handler object;
    zwlr_export_frame_ready_handler ready;
    zwlr_export_frame_cancel_handler cancel;
};

// number of elements in a ramp
struct zwlr_gamma_control_event_gamma_size { uint32_t size; };