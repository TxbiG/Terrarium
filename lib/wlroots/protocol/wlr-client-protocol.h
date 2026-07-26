#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <wayland-client.h>
#include <wayland-client-protocol.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct wl_drm wl_drm;
typedef struct zwlr_input_method zwlr_input_method;
typedef struct zwlr_idle_timeout zwlr_idle_timeout;    // underlying Wayland proxy
typedef struct zwlr_idle zwlr_idle; // underlying Wayland proxy
typedef struct zwlr_input_popup_surface zwlr_input_popup_surface;
typedef struct zwlr_input_method_keyboard_grab zwlr_input_method_keyboard_grab;
typedef struct zwlr_input_method_manager zwlr_input_method_manager;
typedef struct zwlr_server_decoration_manager zwlr_server_decoration_manager;
typedef struct zwlr_server_decoration zwlr_server_decoration;
typedef struct zwlr_export_manager zwlr_export_manager;
typedef struct zwlr_export_frame zwlr_export_frame;
typedef struct zwlr_virtual_keyboard zwlr_virtual_keyboard;
typedef struct zwlr_virtual_keyboard_manager zwlr_virtual_keyboard_manager;
typedef struct zwlr_data_control_manager zwlr_data_control_manager;
typedef struct zwlr_data_control_device zwlr_data_control_device;
typedef struct zwlr_data_control_source zwlr_data_control_source;
typedef struct zwlr_data_control_offer zwlr_data_control_offer;
typedef struct zwlr_foreign_toplevel_manager zwlr_foreign_toplevel_manager;
typedef struct zwlr_foreign_toplevel_handle zwlr_foreign_toplevel_handle;
typedef struct zwlr_gamma_control_manager zwlr_gamma_control_manager;
typedef struct zwlr_gamma_control zwlr_gamma_control;
typedef struct zwlr_input_inhibit_manager zwlr_input_inhibit_manager;
typedef struct zwlr_input_inhibitor zwlr_input_inhibitor;
typedef struct zwlr_layer_shell zwlr_layer_shell;
typedef struct zwlr_layer_surface zwlr_layer_surface;
typedef struct zwlr_output_manager zwlr_output_manager;
typedef struct zwlr_output_head zwlr_output_head;
typedef struct zwlr_output_mode zwlr_output_mode;
typedef struct zwlr_output_configuration zwlr_output_configuration;
typedef struct zwlr_output_configuration_head zwlr_output_configuration_head;
typedef struct zwlr_output_power_manager zwlr_output_power_manager;
typedef struct zwlr_output_power zwlr_output_power;
typedef struct zwlr_screencopy_manager zwlr_screencopy_manager;
typedef struct zwlr_screencopy_frame zwlr_screencopy_frame;
typedef struct zwlr_virtual_pointer zwlr_virtual_pointer;
typedef struct zwlr_virtual_pointer_manager zwlr_virtual_pointer_manager;

enum zwlr_server_decoration_mode {
    zwlr_SERVER_DECORATION_MODE_NONE = 0,    // Undecorated
    zwlr_SERVER_DECORATION_MODE_CLIENT = 1,  // Client-side decoration
    zwlr_SERVER_DECORATION_MODE_SERVER = 2   // Server-side decoration
};

// DRM format codes (subset for brevity)
enum wl_drm_format {
    // 8-bit formats
    WL_DRM_FORMAT_C8          = 0x20203843, // 8-bit color index
    WL_DRM_FORMAT_RGB332      = 0x38424752, // 8-bit RGB 3-3-2
    WL_DRM_FORMAT_BGR233      = 0x38524742, // 8-bit BGR 2-3-3

    // 16-bit formats
    WL_DRM_FORMAT_XRGB4444    = 0x32315258,
    WL_DRM_FORMAT_XBGR4444    = 0x32314258,
    WL_DRM_FORMAT_RGBX4444    = 0x32315852,
    WL_DRM_FORMAT_BGRX4444    = 0x32315842,
    WL_DRM_FORMAT_ARGB4444    = 0x32315241,
    WL_DRM_FORMAT_ABGR4444    = 0x32314241,
    WL_DRM_FORMAT_RGBA4444    = 0x32314152,
    WL_DRM_FORMAT_BGRA4444    = 0x32314142,

    WL_DRM_FORMAT_XRGB1555    = 0x38315258,
    WL_DRM_FORMAT_XBGR1555    = 0x38314258,
    WL_DRM_FORMAT_RGBX5551    = 0x38315852,
    WL_DRM_FORMAT_BGRX5551    = 0x38315842,
    WL_DRM_FORMAT_ARGB1555    = 0x38315241,
    WL_DRM_FORMAT_ABGR1555    = 0x38314241,
    WL_DRM_FORMAT_RGBA5551    = 0x38314152,
    WL_DRM_FORMAT_BGRA5551    = 0x38314142,

    WL_DRM_FORMAT_RGB565      = 0x36314752,
    WL_DRM_FORMAT_BGR565      = 0x36314742,

    // 24/32-bit formats
    WL_DRM_FORMAT_XRGB8888    = 0x34325258,
    WL_DRM_FORMAT_XBGR8888    = 0x34324258,
    WL_DRM_FORMAT_RGBX8888    = 0x34325852,
    WL_DRM_FORMAT_BGRX8888    = 0x34325842,
    WL_DRM_FORMAT_ARGB8888    = 0x34325241,
    WL_DRM_FORMAT_ABGR8888    = 0x34324241,
    WL_DRM_FORMAT_RGBA8888    = 0x34324152,
    WL_DRM_FORMAT_BGRA8888    = 0x34324142,

    // 10-bit packed
    WL_DRM_FORMAT_XRGB2101010 = 0x30335258,
    WL_DRM_FORMAT_XBGR2101010 = 0x30334258,
    WL_DRM_FORMAT_RGBX1010102 = 0x30335852,
    WL_DRM_FORMAT_BGRX1010102 = 0x30335842,
    WL_DRM_FORMAT_ARGB2101010 = 0x30335241,
    WL_DRM_FORMAT_ABGR2101010 = 0x30334241,
    WL_DRM_FORMAT_RGBA1010102 = 0x30334152,
    WL_DRM_FORMAT_BGRA1010102 = 0x30334142,

    // YUV / planar formats
    WL_DRM_FORMAT_NV12        = 0x3231564e, // YUV 4:2:0 planar
    WL_DRM_FORMAT_NV21        = 0x3132564e,
    WL_DRM_FORMAT_YUV420      = 0x32315559,
    WL_DRM_FORMAT_YVU420      = 0x32315659,
    WL_DRM_FORMAT_YUYV        = 0x56595559, // packed YUYV
    WL_DRM_FORMAT_YVYU        = 0x55595659, 
    WL_DRM_FORMAT_UYVY        = 0x59565955,
    WL_DRM_FORMAT_VYUY        = 0x59555956,
    
    // Misc / indexed formats
    WL_DRM_FORMAT_XBGR8888_sRGB = 0x34324258, // same fourcc, sRGB usage
    WL_DRM_FORMAT_ARGB8888_sRGB = 0x34325241
};

// wl_drm capability bitmask
enum wl_drm_capability { WL_DRM_CAP_PRIME = 1 };

// Frame flags
enum zwlr_export_frame_flags { ZWLR_EXPORT_FRAME_FLAG_TRANSIENT = 0x1 };

// Cancel reasons
enum zwlr_export_frame_cancel_reason {
    ZWLR_EXPORT_FRAME_CANCEL_REASON_TEMPORARY = 0,
    ZWLR_EXPORT_FRAME_CANCEL_REASON_PERMANENT = 1,
    ZWLR_EXPORT_FRAME_CANCEL_REASON_RESIZING = 2
};

// State enum
enum zwlr_foreign_toplevel_handle_state {
    ZWLR_FOREIGN_TOPLEVEL_HANDLE_STATE_MAXIMIZED = 0,
    ZWLR_FOREIGN_TOPLEVEL_HANDLE_STATE_MINIMIZED = 1,
    ZWLR_FOREIGN_TOPLEVEL_HANDLE_STATE_ACTIVATED = 2,
    ZWLR_FOREIGN_TOPLEVEL_HANDLE_STATE_FULLSCREEN = 3
};

// Layer enum
enum zwlr_layer_shell_layer {
    ZWLR_LAYER_SHELL_LAYER_BACKGROUND = 0,
    ZWLR_LAYER_SHELL_LAYER_BOTTOM = 1,
    ZWLR_LAYER_SHELL_LAYER_TOP = 2,
    ZWLR_LAYER_SHELL_LAYER_OVERLAY = 3
};


// Anchor bitfield
enum zwlr_layer_surface_anchor {
    ZWLR_LAYER_SURFACE_ANCHOR_TOP = 1,
    ZWLR_LAYER_SURFACE_ANCHOR_BOTTOM = 2,
    ZWLR_LAYER_SURFACE_ANCHOR_LEFT = 4,
    ZWLR_LAYER_SURFACE_ANCHOR_RIGHT = 8
};

// Keyboard interactivity enum
enum zwlr_layer_surface_keyboard_interactivity {
    ZWLR_LAYER_SURFACE_KEYBOARD_INTERACTIVITY_NONE = 0,
    ZWLR_LAYER_SURFACE_KEYBOARD_INTERACTIVITY_EXCLUSIVE = 1,
    ZWLR_LAYER_SURFACE_KEYBOARD_INTERACTIVITY_ON_DEMAND = 2
};


// Enums
enum zwlr_output_power_mode {
    ZWLR_OUTPUT_POWER_MODE_OFF = 0,
    ZWLR_OUTPUT_POWER_MODE_ON = 1
};

enum zwlr_screencopy_frame_flags { ZWLR_SCREENCOPY_FRAME_FLAGS_Y_INVERT = 1 };

// Requests
/*! @brief X */
void zwlr_idle_timeout_release(struct zwlr_idle_timeout *timeout);

/*! @brief X */
void zwlr_idle_timeout_simulate_user_activity(struct zwlr_idle_timeout *timeout);

/*! @brief Event listener registration */
void zwlr_idle_timeout_add_listener(struct zwlr_idle_timeout *timeout, const struct zwlr_idle_timeout_listener *listener, void *data);

// Requests
/*! @brief X */
struct zwlr_idle_timeout* zwlr_idle_get_idle_timeout(struct zwlr_idle *idle, struct wl_seat *seat, uint32_t timeout_ms);

/*! @brief Authenticate with a DRM magic number */
void wl_drm_authenticate(struct wl_drm *drm, uint32_t magic);

/*! @brief Create a standard DRM buffer */
struct wl_buffer* wl_drm_create_buffer(struct wl_drm *drm, uint32_t name, int32_t width, int32_t height, uint32_t stride, uint32_t format);

/*! @brief Create a planar DRM buffer */
struct wl_buffer* wl_drm_create_planar_buffer(struct wl_drm *drm, uint32_t name, int32_t width,  int32_t height, uint32_t format, int32_t offset0, 
    int32_t stride0, int32_t offset1, int32_t stride1, int32_t offset2, int32_t stride2);

/*! @brief create a buffer from a prime fd */
struct wl_buffer* wl_drm_create_prime_buffer(struct wl_drm *drm, int fd, int32_t width, int32_t height, uint32_t format, int32_t offset0, int32_t stride0, 
    int32_t offset1, int32_t stride1, int32_t offset2, int32_t stride2);

/*! @brief Add listener */
void wl_drm_add_listener(struct wl_drm *drm, const struct wl_drm_listener *listener, void *data);

// Requests
/*! @brief X */
void zwlr_input_method_commit_string(struct zwlr_input_method *im, const char *text);

/*! @brief X */
void zwlr_input_method_set_preedit_string(struct zwlr_input_method *im, const char *text, int32_t cursor_begin, int32_t cursor_end);

/*! @brief X */
void zwlr_input_method_delete_surrounding_text(struct zwlr_input_method *im, uint32_t before_length, uint32_t after_length);

/*! @brief X */
void zwlr_input_method_commit(struct zwlr_input_method *im, uint32_t serial);

/*! @brief X */
struct zwlr_input_popup_surface *zwlr_input_method_get_input_popup_surface( struct zwlr_input_method *im, struct wl_surface *surface);

/*! @brief X */
struct zwlr_input_method_keyboard_grab *zwlr_input_method_grab_keyboard( struct zwlr_input_method *im);

/*! @brief X */
void zwlr_input_method_destroy(struct zwlr_input_method *im);

/*! @brief X */
void zwlr_input_popup_surface_destroy(struct zwlr_input_popup_surface *popup);

/*! @brief X */
void zwlr_input_method_keyboard_grab_release( struct zwlr_input_method_keyboard_grab *grab);

/*! @brief X */
struct zwlr_input_method* zwlr_input_method_manager_get_input_method(struct zwlr_input_method_manager *mgr, struct wl_seat *seat);

/*! @brief X */
void zwlr_input_method_manager_destroy(struct zwlr_input_method_manager *mgr);

/*! @brief X */
struct zwlr_server_decoration* zwlr_server_decoration_manager_create( struct zwlr_server_decoration_manager *mgr, struct wl_surface *surface);

/*! @brief X */
void zwlr_server_decoration_release(struct zwlr_server_decoration *decoration);

/*! @brief X */
void zwlr_server_decoration_request_mode(struct zwlr_server_decoration *decoration, uint32_t mode);

/*! @brief X */
struct zwlr_export_frame* zwlr_export_manager_capture_output(struct zwlr_export_manager *manager, int overlay_cursor, struct wl_output *output);

/*! @brief X */
void zwlr_export_manager_destroy(struct zwlr_export_manager *manager);

/*! @brief X */
void zwlr_export_frame_destroy(struct zwlr_export_frame *frame);

/*! @brief X */
void zwlr_virtual_keyboard_keymap(struct zwlr_virtual_keyboard *keyboard, uint32_t format, int fd, uint32_t size);

/*! @brief X */
void zwlr_virtual_keyboard_key(struct zwlr_virtual_keyboard *keyboard, uint32_t time, uint32_t key, uint32_t state);

/*! @brief X */
void zwlr_virtual_keyboard_modifiers(struct zwlr_virtual_keyboard *keyboard, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group);

/*! @brief X */
void zwlr_virtual_keyboard_destroy(struct zwlr_virtual_keyboard *keyboard);

/*! @brief X */
struct zwlr_virtual_keyboard* zwlr_virtual_keyboard_manager_create_virtual_keyboard(struct zwlr_virtual_keyboard_manager *manager, struct wl_seat *seat);

/*! @brief X */
struct zwlr_data_control_source* zwlr_data_control_manager_create_data_source(struct zwlr_data_control_manager *manager);

/*! @brief X */
struct zwlr_data_control_device* zwlr_data_control_manager_get_data_device(struct zwlr_data_control_manager *manager, struct wl_seat *seat);

/*! @brief X */
void zwlr_data_control_manager_destroy(struct zwlr_data_control_manager *manager);

/*! @brief X */
void zwlr_data_control_device_set_selection(struct zwlr_data_control_device *device, struct zwlr_data_control_source *source);

/*! @brief X */
void zwlr_data_control_device_set_primary_selection(struct zwlr_data_control_device *device, struct zwlr_data_control_source *source);

/*! @brief X */
void zwlr_data_control_device_destroy(struct zwlr_data_control_device *device);

/*! @brief X */
void zwlr_data_control_source_offer(struct zwlr_data_control_source *source, const char *mime_type);

/*! @brief X */
void zwlr_data_control_source_destroy(struct zwlr_data_control_source *source);

/*! @brief X */
void zwlr_data_control_offer_receive(struct zwlr_data_control_offer *offer, const char *mime_type, int fd);

/*! @brief X */
void zwlr_data_control_offer_destroy(struct zwlr_data_control_offer *offer);

/*! @brief X */
void zwlr_foreign_toplevel_manager_stop(struct zwlr_foreign_toplevel_manager *manager);

/*! @brief X */
void zwlr_foreign_toplevel_handle_set_maximized(struct zwlr_foreign_toplevel_handle *toplevel);

/*! @brief X */
void zwlr_foreign_toplevel_handle_unset_maximized(struct zwlr_foreign_toplevel_handle *toplevel);

/*! @brief X */
void zwlr_foreign_toplevel_handle_set_minimized(struct zwlr_foreign_toplevel_handle *toplevel);

/*! @brief X */
void zwlr_foreign_toplevel_handle_unset_minimized(struct zwlr_foreign_toplevel_handle *toplevel);

/*! @brief X */
void zwlr_foreign_toplevel_handle_activate(struct zwlr_foreign_toplevel_handle *toplevel, struct wl_seat *seat);

/*! @brief X */
void zwlr_foreign_toplevel_handle_close(struct zwlr_foreign_toplevel_handle *toplevel);

/*! @brief X */
void zwlr_foreign_toplevel_handle_set_rectangle(struct zwlr_foreign_toplevel_handle *toplevel, struct wl_surface *surface, int32_t x, int32_t y, int32_t width, int32_t height);

/*! @brief X */
void zwlr_foreign_toplevel_handle_set_fullscreen(struct zwlr_foreign_toplevel_handle *toplevel, struct wl_output *output);

/*! @brief X */
void zwlr_foreign_toplevel_handle_unset_fullscreen(struct zwlr_foreign_toplevel_handle *toplevel);

/*! @brief X */
void zwlr_foreign_toplevel_handle_destroy(struct zwlr_foreign_toplevel_handle *toplevel);

/*! @brief X */
void zwlr_gamma_control_manager_get_gamma_control(struct zwlr_gamma_control_manager *manager, struct zwlr_gamma_control *id, struct wl_output *output);
/*! @brief X */
void zwlr_gamma_control_manager_destroy(struct zwlr_gamma_control_manager *manager);

/*! @brief X */
void zwlr_gamma_control_set_gamma(struct zwlr_gamma_control *control, int fd);

/*! @brief X */
void zwlr_gamma_control_destroy( struct zwlr_gamma_control *control);


/*! @brief X */
void zwlr_input_inhibit_manager_get_inhibitor(struct zwlr_input_inhibit_manager *manager, struct zwlr_input_inhibitor *id);


/*! @brief X */
void zwlr_input_inhibitor_destroy(struct zwlr_input_inhibitor *inhibitor);

/*! @brief X */
void zwlr_layer_shell_get_layer_surface(struct zwlr_layer_shell *shell, struct zwlr_layer_surface *id, struct wl_surface *surface, struct wl_output *output, uint32_t layer, const char *namespace_);

/*! @brief X */
void zwlr_layer_shell_destroy(struct zwlr_layer_shell *shell);

/*! @brief X */
void zwlr_layer_surface_set_size(struct zwlr_layer_surface *surface, uint32_t width, uint32_t height);

/*! @brief X */
void zwlr_layer_surface_set_anchor( struct zwlr_layer_surface *surface, uint32_t anchor);

/*! @brief X */
void zwlr_layer_surface_set_exclusive_zone(struct zwlr_layer_surface *surface, int32_t zone);

/*! @brief X */
void zwlr_layer_surface_set_margin(struct zwlr_layer_surface *surface, int32_t top, int32_t right, int32_t bottom, int32_t left);

/*! @brief X */
void zwlr_layer_surface_set_keyboard_interactivity(struct zwlr_layer_surface *surface, uint32_t keyboard_interactivity);

/*! @brief X */
void zwlr_layer_surface_get_popup(struct zwlr_layer_surface *surface, struct xdg_popup *popup);

/*! @brief X */
void zwlr_layer_surface_ack_configure( struct zwlr_layer_surface *surface, uint32_t serial);

/*! @brief X */
void zwlr_layer_surface_destroy(struct zwlr_layer_surface *surface);

/*! @brief X */
void zwlr_layer_surface_set_layer(struct zwlr_layer_surface *surface, uint32_t layer);

/*! @brief X */
void zwlr_output_manager_create_configuration(struct zwlr_output_manager *manager, struct zwlr_output_configuration *id, uint32_t serial);

/*! @brief X */
void zwlr_output_manager_stop(struct zwlr_output_manager *manager);

/*! @brief X */
void zwlr_output_manager_destroy(struct zwlr_output_manager *manager);

/*! @brief X */
void zwlr_output_configuration_enable_head(struct zwlr_output_configuration *config, struct zwlr_output_configuration_head *id, struct zwlr_output_head *head);

/*! @brief X */
void zwlr_output_configuration_disable_head(struct zwlr_output_configuration *config, struct zwlr_output_head *head);

/*! @brief X */
void zwlr_output_configuration_apply(struct zwlr_output_configuration *config);

/*! @brief X */
void zwlr_output_configuration_test(struct zwlr_output_configuration *config);

/*! @brief X */
void zwlr_output_configuration_destroy(struct zwlr_output_configuration *config);

/*! @brief X */
void zwlr_output_configuration_head_set_mode(struct zwlr_output_configuration_head *head, struct zwlr_output_mode *mode);

/*! @brief X */
void zwlr_output_configuration_head_set_custom_mode(struct zwlr_output_configuration_head *head, int32_t width, int32_t height, int32_t refresh);

/*! @brief X */
void zwlr_output_configuration_head_set_position(struct zwlr_output_configuration_head *head, int32_t x, int32_t y);

/*! @brief X */
void zwlr_output_configuration_head_set_transform(struct zwlr_output_configuration_head *head, int32_t transform);

/*! @brief X */
void zwlr_output_configuration_head_set_scale(struct zwlr_output_configuration_head *head, wl_fixed_t scale);

/*! @brief X */
void zwlr_output_power_manager_get_output_power(struct zwlr_output_power_manager *manager, struct zwlr_output_power *id, struct wl_output *output);

/*! @brief X */
void zwlr_output_power_manager_destroy(struct zwlr_output_power_manager *manager);

/*! @brief X */
void zwlr_output_power_set_mode(struct zwlr_output_power *power, uint32_t mode);

/*! @brief X */
void zwlr_output_power_destroy(struct zwlr_output_power *power);

/*! @brief X */
void zwlr_screencopy_manager_capture_output(struct zwlr_screencopy_manager *manager, struct zwlr_screencopy_frame *frame, int overlay_cursor, struct wl_output *output);

/*! @brief X */
void zwlr_screencopy_manager_capture_output_region(struct zwlr_screencopy_manager *manager, struct zwlr_screencopy_frame *frame, int overlay_cursor, struct wl_output *output,  int x, int y, int width, int height);

/*! @brief X */
void zwlr_screencopy_manager_destroy(struct zwlr_screencopy_manager *manager);

/*! @brief X */
void zwlr_screencopy_frame_copy(struct zwlr_screencopy_frame *frame, struct wl_buffer *buffer);

/*! @brief X */
void zwlr_screencopy_frame_copy_with_damage(struct zwlr_screencopy_frame *frame, struct wl_buffer *buffer);

/*! @brief X */
void zwlr_screencopy_frame_destroy(struct zwlr_screencopy_frame *frame);

/*! @brief X */
void zwlr_virtual_pointer_motion(struct zwlr_virtual_pointer *pointer, uint32_t time, wl_fixed_t dx, wl_fixed_t dy);

/*! @brief X */
void zwlr_virtual_pointer_motion_absolute(struct zwlr_virtual_pointer *pointer, uint32_t time, uint32_t x, uint32_t y, uint32_t x_extent, uint32_t y_extent);

/*! @brief X */
void zwlr_virtual_pointer_button(struct zwlr_virtual_pointer *pointer, uint32_t time, uint32_t button, uint32_t state);

/*! @brief X */
void zwlr_virtual_pointer_axis(struct zwlr_virtual_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value);

/*! @brief X */
void zwlr_virtual_pointer_frame( struct zwlr_virtual_pointer *pointer);

/*! @brief X */
void zwlr_virtual_pointer_axis_source( struct zwlr_virtual_pointer *pointer, uint32_t axis_source);

/*! @brief X */
void zwlr_virtual_pointer_axis_stop(struct zwlr_virtual_pointer *pointer, uint32_t time, uint32_t axis);

/*! @brief X */
void zwlr_virtual_pointer_axis_discrete(struct zwlr_virtual_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value, int32_t discrete);

/*! @brief X */
void zwlr_virtual_pointer_destroy(struct zwlr_virtual_pointer *pointer);

/*! @brief X */
void zwlr_virtual_pointer_manager_create_virtual_pointer(struct zwlr_virtual_pointer_manager *manager, struct wl_seat *seat, struct zwlr_virtual_pointer *id);

/*! @brief X */
void zwlr_virtual_pointer_manager_create_virtual_pointer_with_output(struct zwlr_virtual_pointer_manager *manager, struct wl_seat *seat, struct wl_output *output, struct zwlr_virtual_pointer *id);

/*! @brief X */
void zwlr_virtual_pointer_manager_destroy(struct zwlr_virtual_pointer_manager *manager);

// ----------------------------------------------------------------
// Enums
// ----------------------------------------------------------------
enum wl_drm_error {
    WL_DRM_ERROR_AUTHENTICATE_FAIL = 0,
    WL_DRM_ERROR_INVALID_FORMAT    = 1,
    WL_DRM_ERROR_INVALID_NAME      = 2
};
enum zwlr_input_method_error {
    zwlr_INPUT_METHOD_ERROR_ROLE = 0
};
enum zwlr_data_control_device_error {
    ZWLR_DATA_CONTROL_DEVICE_ERROR_USED_SOURCE = 1
};
enum zwlr_virtual_keyboard_error {
    zwlr_VIRTUAL_KEYBOARD_ERROR_NO_KEYMAP = 0
};
enum zwlr_virtual_keyboard_manager_error {
    zwlr_VIRTUAL_KEYBOARD_MANAGER_ERROR_UNAUTHORIZED = 0
};
enum zwlr_data_control_source_error {
    ZWLR_DATA_CONTROL_SOURCE_ERROR_INVALID_OFFER = 1
};
enum zwlr_foreign_toplevel_handle_error {
    ZWLR_FOREIGN_TOPLEVEL_HANDLE_ERROR_INVALID_RECTANGLE = 0
};

enum zwlr_gamma_control_error {
    ZWLR_GAMMA_CONTROL_ERROR_INVALID_GAMMA = 1
};
enum zwlr_input_inhibit_manager_error {
    ZWLR_INPUT_INHIBIT_MANAGER_ERROR_ALREADY_INHIBITED = 0
};
enum zwlr_layer_shell_error {
    ZWLR_LAYER_SHELL_ERROR_ROLE = 0,
    ZWLR_LAYER_SHELL_ERROR_INVALID_LAYER = 1,
    ZWLR_LAYER_SHELL_ERROR_ALREADY_CONSTRUCTED = 2
};
enum zwlr_layer_surface_error {
    ZWLR_LAYER_SURFACE_ERROR_INVALID_SURFACE_STATE = 0,
    ZWLR_LAYER_SURFACE_ERROR_INVALID_SIZE = 1,
    ZWLR_LAYER_SURFACE_ERROR_INVALID_ANCHOR = 2,
    ZWLR_LAYER_SURFACE_ERROR_INVALID_KEYBOARD_INTERACTIVITY = 3
};
enum zwlr_output_configuration_error {
    ZWLR_OUTPUT_CONFIGURATION_ERROR_ALREADY_CONFIGURED_HEAD = 1,
    ZWLR_OUTPUT_CONFIGURATION_ERROR_UNCONFIGURED_HEAD = 2,
    ZWLR_OUTPUT_CONFIGURATION_ERROR_ALREADY_USED = 3
};

enum zwlr_output_configuration_head_error {
    ZWLR_OUTPUT_CONFIGURATION_HEAD_ERROR_ALREADY_SET = 1,
    ZWLR_OUTPUT_CONFIGURATION_HEAD_ERROR_INVALID_MODE = 2,
    ZWLR_OUTPUT_CONFIGURATION_HEAD_ERROR_INVALID_CUSTOM_MODE = 3,
    ZWLR_OUTPUT_CONFIGURATION_HEAD_ERROR_INVALID_TRANSFORM = 4,
    ZWLR_OUTPUT_CONFIGURATION_HEAD_ERROR_INVALID_SCALE = 5
};
enum zwlr_output_power_error {
    ZWLR_OUTPUT_POWER_ERROR_INVALID_MODE = 1
};
enum zwlr_screencopy_frame_error {
    ZWLR_SCREENCOPY_FRAME_ERROR_ALREADY_USED = 0,
    ZWLR_SCREENCOPY_FRAME_ERROR_INVALID_BUFFER = 1
};

enum zwlr_virtual_pointer_error {
    ZWLR_VIRTUAL_POINTER_ERROR_INVALID_AXIS = 0,
    ZWLR_VIRTUAL_POINTER_ERROR_INVALID_AXIS_SOURCE = 1
};

#ifdef __cplusplus
}
#endif