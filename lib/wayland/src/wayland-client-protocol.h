#ifndef FLORA_PROTOCALS_H
#define FLORA_PROTOCALS_H

#include <stdint.h>
#include "wayland-client-core.h"

#ifdef  __cplusplus
extern "C" {
#endif

// Forward declare core Wayland protocol interfaces
typedef struct wl_display wl_display;                           // Core global object
typedef struct wl_registry wl_registry;                         // Global registry object
typedef struct wl_compositor wl_compositor;                     // The compositor singleton
typedef struct wl_surface wl_surface;                           // An onscreen surface
typedef struct wl_callback wl_callback;                         // Callback object
typedef struct wl_shm wl_shm;                                   // shared memory support
typedef struct wl_shm_pool wl_shm_pool;                         // A shared memory pool
typedef struct wl_buffer wl_buffer;                             // content for a wl_surface
typedef struct wl_output wl_output;                             // compositor output region
typedef struct wl_seat wl_seat;                                 // Group of input devices
typedef struct wl_keyboard wl_keyboard;                         // Keyboard input device
typedef struct wl_pointer wl_pointer;                           // Pointer input device
typedef struct wl_touch wl_touch;                               // Touchscreen input device
typedef struct wl_data_device_manager wl_data_device_manager;   // data transfer interface
typedef struct wl_data_device wl_data_device;                   // data transfer interface
typedef struct wl_data_offer wl_data_offer;                     // offer to transfer data
typedef struct wl_data_source wl_data_source;                   // offer to transfer data
typedef struct wl_shell wl_shell;                               // create desktop-style surfaces
typedef struct wl_shell_surface wl_shell_surface;               // desktop-style metadata interface
typedef struct wl_region wl_region;                             // region interface [Done]
typedef struct wl_subcompositor wl_subcompositor;               // subcompositor interface
typedef struct wl_subsurface wl_subsurface;                     // subsurface interface

struct wl_interface;
struct wl_proxy;


/* wl_keyboard interface object */
struct wl_keyboard;
typedef int32_t wl_fixed_t;
/* wl_touch interface version 10 */
struct wl_touch;
/* wl_output interface version 4 */
struct wl_output;
/* Opaque struct representing the wl_seat object */
typedef struct wl_seat wl_seat;

enum wl_surface_opcode {
    WL_SURFACE_DESTROY = 0,
    WL_SURFACE_ATTACH = 1,
    WL_SURFACE_DAMAGE = 2,
    WL_SURFACE_FRAME = 3,
    WL_SURFACE_SET_OPAQUE_REGION = 4,
    WL_SURFACE_SET_INPUT_REGION = 5,
    WL_SURFACE_COMMIT = 6,
    WL_SURFACE_SET_BUFFER_TRANSFORM = 7,
    WL_SURFACE_SET_BUFFER_SCALE = 8,
    WL_SURFACE_DAMAGE_BUFFER = 9,
    WL_SURFACE_OFFSET = 10
};

enum wl_surface_error {
    WL_SURFACE_ERROR_INVALID_SCALE = 0,
    WL_SURFACE_ERROR_INVALID_TRANSFORM = 1,
    WL_SURFACE_ERROR_INVALID_SIZE = 2,
    WL_SURFACE_ERROR_INVALID_OFFSET = 3,
    WL_SURFACE_ERROR_DEFUNCT_ROLE_OBJECT = 4
};

enum wl_surface_event {
    WL_SURFACE_ENTER = 0,
    WL_SURFACE_LEAVE = 1,
    WL_SURFACE_PREFERRED_BUFFER_SCALE = 2,      // since v6
    WL_SURFACE_PREFERRED_BUFFER_TRANSFORM = 3   // since v6
};

typedef enum {
    WL_SURFACE_TRANSFORM_NORMAL = 0,
    WL_SURFACE_TRANSFORM_90,
    WL_SURFACE_TRANSFORM_180,
    WL_SURFACE_TRANSFORM_270,
    WL_SURFACE_TRANSFORM_FLIPPED,
    WL_SURFACE_TRANSFORM_FLIPPED_90,
    WL_SURFACE_TRANSFORM_FLIPPED_180,
    WL_SURFACE_TRANSFORM_FLIPPED_270,
} wl_surface_transform;

enum wl_display_error {
    WL_DISPLAY_ERROR_INVALID_OBJECT = 0,
    WL_DISPLAY_ERROR_INVALID_METHOD = 1,
    WL_DISPLAY_ERROR_NO_MEMORY = 2,
    WL_DISPLAY_ERROR_IMPLEMENTATION = 3,
};

typedef enum {
    WL_SHM_ERROR_INVALID_FORMAT = 0,  // buffer format is not known
    WL_SHM_ERROR_INVALID_STRIDE = 1,  // invalid size or stride during pool or buffer creation
    WL_SHM_ERROR_INVALID_FD = 2       // mmapping the file descriptor failed
} wl_shm_error_t;

// wl_shm pixel formats (values correspond to DRM fourcc or custom codes)
typedef enum {
    WL_SHM_FORMAT_ARGB8888 = 0,           // 32-bit ARGB format, A:R:G:B 8:8:8:8 little endian
    WL_SHM_FORMAT_XRGB8888 = 1,           // 32-bit RGB format, x:R:G:B 8:8:8:8 little endian
    WL_SHM_FORMAT_C8       = 0x20203843,  // 8-bit color index format, [7:0] C
    WL_SHM_FORMAT_RGB332   = 0x38424752,  // 8-bit RGB format, R:G:B 3:3:2
    WL_SHM_FORMAT_BGR233   = 0x38524742,  // 8-bit BGR format, B:G:R 2:3:3
    WL_SHM_FORMAT_XRGB4444 = 0x32315258,  // 16-bit xRGB format, x:R:G:B 4:4:4:4 little endian
    WL_SHM_FORMAT_XBGR4444 = 0x32314258,  // 16-bit xBGR format, x:B:G:R 4:4:4:4 little endian
    WL_SHM_FORMAT_RGBX4444 = 0x32315852,  // 16-bit RGBx format, R:G:B:x 4:4:4:4 little endian
    WL_SHM_FORMAT_BGRX4444 = 0x32315842,  // 16-bit BGRx format, B:G:R:x 4:4:4:4 little endian
    WL_SHM_FORMAT_ARGB4444 = 0x32315241,  // 16-bit ARGB format, A:R:G:B 4:4:4:4 little endian
    WL_SHM_FORMAT_ABGR4444 = 0x32314241,  // 16-bit ABGR format, A:B:G:R 4:4:4:4 little endian
    WL_SHM_FORMAT_RGBA4444 = 0x32314152,  // 16-bit RGBA format, R:G:B:A 4:4:4:4 little endian
    WL_SHM_FORMAT_BGRA4444 = 0x32314142,  // 16-bit BGRA format, B:G:R:A 4:4:4:4 little endian
    WL_SHM_FORMAT_XRGB1555 = 0x35315258,  // 16-bit xRGB format, x:R:G:B 1:5:5:5 little endian
    WL_SHM_FORMAT_XBGR1555 = 0x35314258,  // 16-bit xBGR 1555 format, x:B:G:R 1:5:5:5 little endian
    WL_SHM_FORMAT_RGBX5551 = 0x35315852,  // 16-bit RGBx 5551 format, R:G:B:x 5:5:5:1 little endian
    WL_SHM_FORMAT_BGRX5551 = 0x35315842,  // 16-bit BGRx 5551 format, B:G:R:x 5:5:5:1 little endian
    WL_SHM_FORMAT_ARGB1555 = 0x35315241,  // 16-bit ARGB 1555 format, A:R:G:B 1:5:5:5 little endian
    WL_SHM_FORMAT_ABGR1555 = 0x35314241,  // 16-bit ABGR 1555 format, A:B:G:R 1:5:5:5 little endian
    WL_SHM_FORMAT_RGBA5551 = 0x35314152,  // 16-bit RGBA 5551 format, R:G:B:A 5:5:5:1 little endian
    WL_SHM_FORMAT_BGRA5551 = 0x35314142,  // 16-bit BGRA 5551 format, B:G:R:A 5:5:5:1 little endian
    WL_SHM_FORMAT_RGB565   = 0x36314752,  // 16-bit RGB 565 format, R:G:B 5:6:5 little endian
    WL_SHM_FORMAT_BGR565   = 0x36314742,  // 16-bit BGR 565 format, B:G:R 5:6:5 little endian
    WL_SHM_FORMAT_RGB888   = 0x34324752,  // 24-bit RGB format, R:G:B little endian
    WL_SHM_FORMAT_BGR888   = 0x34324742,  // 24-bit BGR format, B:G:R little endian
    WL_SHM_FORMAT_XBGR8888 = 0x34324258,  // 32-bit xBGR format, x:B:G:R 8:8:8:8 little endian
    WL_SHM_FORMAT_RGBX8888 = 0x34325852,  // 32-bit RGBx format, R:G:B:x 8:8:8:8 little endian
    WL_SHM_FORMAT_BGRX8888 = 0x34325842,  // 32-bit BGRx format, B:G:R:x 8:8:8:8 little endian
    WL_SHM_FORMAT_ABGR8888 = 0x34324241,  // 32-bit ABGR format, A:B:G:R 8:8:8:8 little endian
    WL_SHM_FORMAT_RGBA8888 = 0x34324152,  // 32-bit RGBA format, R:G:B:A 8:8:8:8 little endian
    WL_SHM_FORMAT_BGRA8888 = 0x34324142,  // 32-bit BGRA format, B:G:R:A 8:8:8:8 little endian
    WL_SHM_FORMAT_XRGB2101010 = 0x30335258, // 32-bit xRGB 2:10:10:10 little endian
    WL_SHM_FORMAT_XBGR2101010 = 0x30334258, // 32-bit xBGR 2:10:10:10 little endian
    WL_SHM_FORMAT_RGBX1010102 = 0x30335852, // 32-bit RGBx 10:10:10:2 little endian
    WL_SHM_FORMAT_BGRX1010102 = 0x30335842, // 32-bit BGRx 10:10:10:2 little endian
    WL_SHM_FORMAT_ARGB2101010 = 0x30335241, // 32-bit ARGB 2:10:10:10 little endian
    WL_SHM_FORMAT_ABGR2101010 = 0x30334241, // 32-bit ABGR 2:10:10:10 little endian
    WL_SHM_FORMAT_RGBA1010102 = 0x30334152, // 32-bit RGBA 10:10:10:2 little endian
    WL_SHM_FORMAT_BGRA1010102 = 0x30334142, // 32-bit BGRA 10:10:10:2 little endian
} wl_shm_format_t;

enum wl_data_offer_error {
    WL_DATA_OFFER_ERROR_INVALID_FINISH = 0,    /* finish request was called untimely */
    WL_DATA_OFFER_ERROR_INVALID_ACTION_MASK = 1, /* action mask contains invalid values */
    WL_DATA_OFFER_ERROR_INVALID_ACTION = 2,      /* action argument has an invalid value */
    WL_DATA_OFFER_ERROR_INVALID_OFFER = 3,       /* offer doesn't accept this request */
};
enum wl_data_source_error {
    WL_DATA_SOURCE_ERROR_INVALID_ACTION_MASK = 0, /* action mask contains invalid values */
    WL_DATA_SOURCE_ERROR_INVALID_SOURCE = 1,      /* source doesn't accept this request */
};
enum wl_data_device_error {
    WL_DATA_DEVICE_ERROR_ROLE = 0,        /* given wl_surface has another role */
    WL_DATA_DEVICE_ERROR_USED_SOURCE = 1, /* source has already been used */
};
/* wl_data_device_manager drag-and-drop actions (bitmask, since version 3) */
enum wl_data_device_manager_dnd_action {
    WL_DATA_DEVICE_MANAGER_DND_ACTION_NONE = 0,
    WL_DATA_DEVICE_MANAGER_DND_ACTION_COPY = 1,
    WL_DATA_DEVICE_MANAGER_DND_ACTION_MOVE = 2,
    WL_DATA_DEVICE_MANAGER_DND_ACTION_ASK  = 4,
};

enum wl_shell_error {
    WL_SHELL_ERROR_ROLE = 0, /* given wl_surface has another role */
};
/**
 * wl_shell_surface - desktop-style metadata interface
 *
 * Implemented by a wl_surface for desktop-style user interfaces.
 * Provides requests to treat surfaces as toplevel, fullscreen,
 * popup, etc., and to move, resize, maximize, set metadata, etc.
 */

// wl_shell_surface resize edges (bitfield)
enum wl_shell_surface_resize {
    WL_SHELL_SURFACE_RESIZE_NONE          = 0,
    WL_SHELL_SURFACE_RESIZE_TOP           = 1,
    WL_SHELL_SURFACE_RESIZE_BOTTOM        = 2,
    WL_SHELL_SURFACE_RESIZE_LEFT          = 4,
    WL_SHELL_SURFACE_RESIZE_TOP_LEFT      = 5,
    WL_SHELL_SURFACE_RESIZE_BOTTOM_LEFT   = 6,
    WL_SHELL_SURFACE_RESIZE_RIGHT         = 8,
    WL_SHELL_SURFACE_RESIZE_TOP_RIGHT     = 9,
    WL_SHELL_SURFACE_RESIZE_BOTTOM_RIGHT  = 10,
};

// wl_shell_surface transient flags (bitfield)
enum wl_shell_surface_transient {
    WL_SHELL_SURFACE_TRANSIENT_INACTIVE = 0x1, /* do not set keyboard focus */
};

//* wl_shell_surface fullscreen methods
enum wl_shell_surface_fullscreen_method {
    WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT = 0,
    WL_SHELL_SURFACE_FULLSCREEN_METHOD_SCALE   = 1,
    WL_SHELL_SURFACE_FULLSCREEN_METHOD_DRIVER  = 2,
    WL_SHELL_SURFACE_FULLSCREEN_METHOD_FILL    = 3,
};

typedef enum wl_seat_capability {
    WL_SEAT_CAPABILITY_POINTER  = 1 << 0, /* the seat has pointer devices */
    WL_SEAT_CAPABILITY_KEYBOARD = 1 << 1, /* the seat has one or more keyboards */
    WL_SEAT_CAPABILITY_TOUCH    = 1 << 2  /* the seat has touch devices */
} wl_seat_capability_t;

// wl_seat_error
typedef enum wl_seat_error {
    WL_SEAT_ERROR_MISSING_CAPABILITY = 0 /* get_pointer, get_keyboard or get_touch called on seat without matching capability */
} wl_seat_error_t;

//
typedef enum {
    WL_POINTER_ERROR_ROLE = 0, /* given wl_surface has another role */
} wl_pointer_error_t;

// button_state
typedef enum {
    WL_POINTER_BUTTON_STATE_RELEASED = 0,
    WL_POINTER_BUTTON_STATE_PRESSED = 1,
} wl_pointer_button_state_t;

// Enum: axis
typedef enum {
    WL_POINTER_AXIS_VERTICAL_SCROLL = 0,
    WL_POINTER_AXIS_HORIZONTAL_SCROLL = 1,
} wl_pointer_axis_t;

// axis_source
typedef enum {
    WL_POINTER_AXIS_SOURCE_WHEEL = 0,
    WL_POINTER_AXIS_SOURCE_FINGER = 1,
    WL_POINTER_AXIS_SOURCE_CONTINUOUS = 2,
    WL_POINTER_AXIS_SOURCE_WHEEL_TILT = 3, /* since version 6 */
} wl_pointer_axis_source_t;

// axis_relative_direction
typedef enum {
    WL_POINTER_AXIS_RELATIVE_DIRECTION_IDENTICAL = 0,
    WL_POINTER_AXIS_RELATIVE_DIRECTION_INVERTED = 1,
} wl_pointer_axis_relative_direction_t;
// Keymap format enum
enum wl_keyboard_keymap_format {
    WL_KEYBOARD_KEYMAP_FORMAT_NO_KEYMAP = 0, /* no keymap; client must understand how to interpret the raw keycode */
    WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1 = 1     /* libxkbcommon compatible, null-terminated string */
};

/* Key state enum */
enum wl_keyboard_key_state {
    WL_KEYBOARD_KEY_STATE_RELEASED = 0, /* key is not pressed */
    WL_KEYBOARD_KEY_STATE_PRESSED = 1,  /* key is pressed */
    WL_KEYBOARD_KEY_STATE_REPEATED = 2  /* key was repeated (since version 10) */
};
//
enum wl_touch_events {
    WL_TOUCH_DOWN,
    WL_TOUCH_UP,
    WL_TOUCH_MOTION,
    WL_TOUCH_FRAME,
    WL_TOUCH_CANCEL,
    WL_TOUCH_SHAPE,       /* since version 6 */
    WL_TOUCH_ORIENTATION  /* since version 6 */
};

// wl_output subpixel enum
enum wl_output_subpixel {
    WL_OUTPUT_SUBPIXEL_UNKNOWN = 0,
    WL_OUTPUT_SUBPIXEL_NONE = 1,
    WL_OUTPUT_SUBPIXEL_HORIZONTAL_RGB = 2,
    WL_OUTPUT_SUBPIXEL_HORIZONTAL_BGR = 3,
    WL_OUTPUT_SUBPIXEL_VERTICAL_RGB = 4,
    WL_OUTPUT_SUBPIXEL_VERTICAL_BGR = 5
};

// wl_output transform enum
enum wl_output_transform {
    WL_OUTPUT_TRANSFORM_NORMAL = 0,
    WL_OUTPUT_TRANSFORM_90 = 1,
    WL_OUTPUT_TRANSFORM_180 = 2,
    WL_OUTPUT_TRANSFORM_270 = 3,
    WL_OUTPUT_TRANSFORM_FLIPPED = 4,
    WL_OUTPUT_TRANSFORM_FLIPPED_90 = 5,
    WL_OUTPUT_TRANSFORM_FLIPPED_180 = 6,
    WL_OUTPUT_TRANSFORM_FLIPPED_270 = 7
};

// wl_output mode flags (bitfield)
enum wl_output_mode_flags {
    WL_OUTPUT_MODE_CURRENT = 0x1,
    WL_OUTPUT_MODE_PREFERRED = 0x2
};

// wl_output events enum
enum wl_output_events {
    WL_OUTPUT_GEOMETRY,
    WL_OUTPUT_MODE,
    WL_OUTPUT_DONE,       /* since version 2 */
    WL_OUTPUT_SCALE,      /* since version 2 */
    WL_OUTPUT_NAME,       /* since version 4 */
    WL_OUTPUT_DESCRIPTION /* since version 4 */
};

// Subcompositor errors.
enum wl_subcompositor_error {
    WL_SUBCOMPOSITOR_ERROR_BAD_SURFACE = 0, /* the to-be sub-surface is invalid */
    WL_SUBCOMPOSITOR_ERROR_BAD_PARENT = 1   /* the to-be sub-surface parent is invalid */
};

// Subsurface errors. wl_surface is not a sibling or the parent
enum wl_subsurface_error { WL_SUBSURFACE_ERROR_BAD_SURFACE = 0 };

/*! @brief */
struct wl_callback *wl_display_sync(struct wl_display *display);

/*! @brief */
struct wl_registry *wl_display_get_registry(struct wl_display *display);

/*! @brief */
int wl_display_add_listener(struct wl_display *display, const struct wl_display_listener *listener, void *data);

/*! @brief */
struct wl_proxy *wl_registry_bind(struct wl_registry *registry, uint32_t name, const struct wl_interface *interface, uint32_t version);

/*! @brief Add listener for wl_registry events */
int wl_registry_add_listener(struct wl_registry *registry, const struct wl_registry_listener *listener, void *data);

/*! @brief Destroy wl_callback proxy */
void wl_callback_destroy(struct wl_callback *callback);

/*! @brief Add listener for wl_callback events */
int wl_callback_add_listener(struct wl_callback *callback, const struct wl_callback_listener *listener, void *data);

/*! @brief */
struct wl_surface *wl_compositor_create_surface(struct wl_compositor *compositor);

/*! @brief */
struct wl_region *wl_compositor_create_region(struct wl_compositor *compositor);

/*! @brief Create a wl_buffer from this shm pool */
struct wl_buffer *wl_shm_pool_create_buffer(struct wl_shm_pool *pool, int32_t offset, int32_t width, int32_t height, int32_t stride, uint32_t format);

/*! @brief Destroy the shm pool */
void wl_shm_pool_destroy(struct wl_shm_pool *pool);

/*! @brief Resize the shm pool */
void wl_shm_pool_resize(struct wl_shm_pool *pool, int32_t size);

/*!
 * @brief start_drag - start drag-and-drop operation
 * @param source: data source for the eventual transfer (nullable) @param origin: surface where the drag originates @param icon: drag-and-drop icon surface (nullable) 
 * @param serial: serial number of the implicit grab on the origin
 *
 * Asks the compositor to start a drag-and-drop operation on behalf of the client. If source is NULL, enter/leave/motion events sent only to initiating client.
 * Icon surface is optional and can be moved with wl_surface.offset. The icon surface receives the drag-and-drop icon role. Reusing a source that was used before will cause an error. */
void wl_data_device_start_drag(void *data, void *source, void *origin, void *icon, uint32_t serial);

/*!
 * set_selection - copy data to the selection
 * @param source: data source for the selection (nullable) @param serial: serial number of event that triggered this request
 * Ask compositor to set selection to data from source. To unset selection, pass NULL as source. Reusing a source that was used before will cause an error. */
void wl_data_device_set_selection(void *data, void *source, uint32_t serial);

/*! @brief destroy data device. */
void wl_data_device_release(void *data);

/*! @brief introduce a new wl_data_offer */
void wl_data_device_data_offer(void *data, void *id);

/*! @brief enter - initiate drag-and-drop session
 * @param serial: serial number of the enter event @param surface: client surface entered @param x: surface-local x coordinate (fixed point) 
 * @param y: surface-local y coordinate (fixed point) @param id: source data_offer object (nullable) */
void wl_data_device_enter(void *data, uint32_t serial, void *surface, int32_t x_fixed, int32_t y_fixed, void *id);

/*! @brief leave - end drag-and-drop session */
void wl_data_device_leave(void *data);

/*! @brief motion - drag-and-drop pointer motion * @param time: timestamp (ms granularity) @param x: surface-local x coordinate (fixed point) @param y: surface-local y coordinate (fixed point) */
void wl_data_device_motion(void *data, uint32_t time, int32_t x_fixed, int32_t y_fixed);

/*! @brief drop - end drag-and-drop session successfully Sent when drag-and-drop ends because implicit grab is removed. */
void wl_data_device_drop(void *data);

/**
 * @brief selection - advertise new selection
 * @param id: selection data_offer object (nullable)
 *
 * Sent to notify client of new wl_data_offer for selection.
 */
/*! @brief */
void wl_data_device_selection(void *data, void *id /* wl_data_offer* or NULL */);

/*! @brief create_data_source - create a new data source @param id: data source to create (new_id) */
void wl_data_device_manager_create_data_source(void *data, void *id);

/*! @brief get_data_device - create a new data device for a given seat @param id data device to create (new_id) @param seat seat associated with the data device */
void wl_data_device_manager_get_data_device(void *data, void *id, void *seat);

/*!
 * @brief get_shell_surface - create a shell surface from a surface
 * @param id: new wl_shell_surface object @param surface: wl_surface to be given the shell surface role
 * Creates a shell surface for an existing wl_surface. This assigns the shell surface role to the wl_surface. If the surface already
 * has another role, a protocol error is raised. Only one shell surface can be associated with a given surface. */
void wl_shell_get_shell_surface(void *data, void *id /* wl_shell_surface* */, void *surface /* wl_surface* */);

/*! @brief pong - respond to a ping event
 * @param serial: serial number of the ping event */
void wl_shell_surface_pong(void *data, uint32_t serial);

/*! @brief move - start an interactive move
 * @param seat: wl_seat whose pointer is used. @param serial: serial number of implicit grab on pointer
 * Must be used in response to a button press event. Server may ignore depending on surface state. */
void wl_shell_surface_move(void *data, void *seat, uint32_t serial);

/*! @brief resize - start an interactive resize
 * @param seat: wl_seat whose pointer is used @param serial: serial number of implicit grab on pointer @param edges: which edge or corner is dragged (bitmask)
 * Must be used in response to a button press event. Server may ignore depending on surface state. */
void wl_shell_surface_resize(void *data, void *seat, uint32_t serial, uint32_t edges);

/*! @brief set_toplevel - make surface a toplevel surface A toplevel surface is not fullscreen, maximized or transient. */
void wl_shell_surface_set_toplevel(void *data);

/*! @brief set_transient - make surface a transient surface @param parent: parent wl_surface @param x: x coordinate relative to parent surface @param y: y coordinate relative to parent surface
 * @param flags: transient surface behavior flags (bitmask) */
void wl_shell_surface_set_transient(void *data, void *parent, int32_t x, int32_t y, uint32_t flags);

/*! @brief set_fullscreen - make surface fullscreen @param method: method for resolving size conflict @param framerate: preferred framerate in mHz (0 = no preference) 
 * @param output: output on which surface is fullscreen (nullable). Server must respond with configure event. */
void wl_shell_surface_set_fullscreen(void *data, uint32_t method, uint32_t framerate, void *output);

/*! @brief set_popup - make surface a popup surface @param seat: wl_seat whose pointer is used @param serial: serial number of implicit grab on pointer @param parent: parent wl_surface 
 * @param x: x coordinate relative to parent surface @param y: y coordinate relative to parent surface @param flags: transient surface behavior flags (bitmask) */
void wl_shell_surface_set_popup(void *data, void *seat, uint32_t serial, void *parent, int32_t x, int32_t y, uint32_t flags);

/*! @brief set_maximized - make surface maximized @output: output on which surface is maximized (nullable) Server replies with configure event with new size. */
void wl_shell_surface_set_maximized(void *data, void *output /* wl_output* or NULL */);
/*! @brief set_title - set surface title @title: UTF-8 string title */
void wl_shell_surface_set_title(void *data, const char *title);

/*! @brief set_class - set surface class @class_: UTF-8 string class */
void wl_shell_surface_set_class(void *data, const char *class_);

/*! @brief ping - ping client to check responsiveness @serial: serial number of the ping event */
void wl_shell_surface_ping(void *data, uint32_t serial);

/*! @brief configure - suggest resize to client @param edges hint for resize (bitmask) @param width suggested width in surface-local coordinates @param height suggested height in surface-local coordinates */
void wl_shell_surface_configure(void *data, uint32_t edges, int32_t width, int32_t height);

/*! @brief popup_done - popup interaction done (popup grab broken) */
void wl_shell_surface_popup_done(void *data);

/*! @brief Destory the wl_surface object. */
void wl_surface_destroy(struct wl_surface *surface);

/*! @brief Attach a wl_buffer to the surface. x, y must be zero for version >= 5, else error invalid_offset. */
void wl_surface_attach(struct wl_surface *surface, struct wl_buffer *buffer, int32_t x, int32_t y);

/*! @brief Damage a rectangular area of the surface in surface-local coordinates. */
void wl_surface_damage(struct wl_surface *surface, int32_t x, int32_t y, int32_t width, int32_t height);

/*! @brief Request a frame callback. Returns a new wl_callback object for notification. */
struct wl_callback *wl_surface_frame(struct wl_surface *surface);

/*! @brief Set the opaque region of the surface. Passing NULL clears the opaque region. */
void wl_surface_set_opaque_region(struct wl_surface *surface, struct wl_region *region);

/*! Set the input region of the surface. Passing NULL sets the input region to infinite. */
void wl_surface_set_input_region(struct wl_surface *surface, struct wl_region *region);

/*! @brief Commit the pending surface state. */
void wl_surface_commit(struct wl_surface *surface);

/*! @brief Set buffer transform (since version 2). */
void wl_surface_set_buffer_transform(struct wl_surface *surface, int32_t transform);

/*! @brief Set buffer scale (since version 3). */
void wl_surface_set_buffer_scale(struct wl_surface *surface, int32_t scale);

/*! @brief Damage part of the surface in buffer coordinates (since version 4). */
void wl_surface_damage_buffer(struct wl_surface *surface, int32_t x, int32_t y, int32_t width, int32_t height);

/*! @brief Set surface offset */
void wl_surface_offset(struct wl_surface *surface, int32_t x, int32_t y);

/*! @brief Add a listener for wl_surface events. */
int wl_surface_add_listener(struct wl_surface *surface, const struct wl_surface_listener *listener, void *data);

/*! @brief  Create a wl_pointer object for this seat. Must only be called if seat has pointer capability or had it in the past. Returns NULL or error if seat lacks pointer capability. */
wl_pointer* wl_seat_get_pointer(wl_seat *seat);

/*! @brief Create a wl_keyboard object for this seat. Must only be called if seat has keyboard capability or had it in the past. Returns NULL or error if seat lacks keyboard capability. */
wl_keyboard* wl_seat_get_keyboard(wl_seat *seat);

/*! @brief Create a wl_touch object for this seat. Must only be called if seat has touch capability or had it in the past. Returns NULL or error if seat lacks touch capability. */
wl_touch* wl_seat_get_touch(wl_seat *seat);

/*! @brief Release the wl_seat object. Client indicates it no longer needs this seat object. */
void wl_seat_release(wl_seat *seat);

/* @brief Function to add a listener to wl_seat events */
void wl_seat_add_listener(wl_seat *seat, const wl_seat_listener *listener, void *data);

/*! @brief Set the pointer surface (cursor) with hotspot offset. @param serial Serial number of the enter event to match. @param surface Pointer surface, can be NULL to hide cursor. 
  * @param hotspot_x Surface-local x coordinate of hotspot. @param hotspot_y Surface-local y coordinate of hotspot. */
void wl_pointer_set_cursor(uint32_t serial, struct wl_surface *surface, int32_t hotspot_x, int32_t hotspot_y);

/*! @brief Release the pointer object. Destroys the wl_pointer proxy object. Client must not call wl_pointer_destroy() after this. */
void wl_pointer_release(void);

/*! @brief Release the keyboard object. */
void wl_keyboard_release(struct wl_keyboard *keyboard);

/*! @brief wl_touch requests */
void wl_touch_release(struct wl_touch *touch);

/*! @brief wl_output requests. */
void wl_output_release(struct wl_output *output);

/*! @brief Destroy the region object. This invalidates the object ID. */
void wl_region_destroy(struct wl_region *region);

/*! @brief Add the specified rectangle to the region. @param x region-local x coordinate @param y region-local y coordinate @param width rectangle width @param height rectangle height */
void wl_region_add(struct wl_region *region, int32_t x, int32_t y, int32_t width, int32_t height);

/*! @brief Subtract the specified rectangle from the region. @param x region-local x coordinate @param y region-local y coordinate @param width rectangle width @param height rectangle height */
void wl_region_subtract(struct wl_region *region, int32_t x, int32_t y, int32_t width, int32_t height);

/*! @brief Unbind from the subcompositor interface. This does not affect any other objects. */
void wl_subcompositor_destroy(struct wl_subcompositor *subcompositor);

/*! @brief Create a sub-surface interface for the given surface, associating it with the given parent surface. This turns a plain wl_surface into a sub-surface. @param id new wl_subsurface object ID 
 * @param surface the surface to be turned into a sub-surface @param parent the parent surface */
struct wl_subsurface *wl_subcompositor_get_subsurface(struct wl_subcompositor *subcompositor, struct wl_surface *surface, struct wl_surface *parent);

/*! @brief Remove sub-surface interface. The wl_surface's association to the parent is deleted and unmapped immediately. */
void wl_subsurface_destroy(struct wl_subsurface *subsurface);

/*! @brief Schedule a sub-surface position change. Coordinates are relative to the parent surface origin. @param x x coordinate in parent surface @param y y coordinate in parent surface */
void wl_subsurface_set_position(struct wl_subsurface *subsurface, int32_t x, int32_t y);

/*! @brief Restack the sub-surface above the reference surface. @param sibling reference surface (must be sibling or parent) */
void wl_subsurface_place_above(struct wl_subsurface *subsurface, struct wl_surface *sibling);

/*! @brief Restack the sub-surface below the reference surface. @param sibling reference surface (must be sibling or parent) */
void wl_subsurface_place_below(struct wl_subsurface *subsurface, struct wl_surface *sibling);

/*! @brief Set sub-surface to synchronized mode. wl_surface.commit caches state until the parent surface commits. */
void wl_subsurface_set_sync(struct wl_subsurface *subsurface);

/*! @brief Set sub-surface to desynchronized mode. wl_surface.commit applies state immediately, independently. */
void wl_subsurface_set_desync(struct wl_subsurface *subsurface);

/*! @brief */
void wl_fixes_destroy(struct wl_fixes *fixes);

/*!
 * @brief Destroy a wl_registry object.The client should no longer use the wl_registry after making this request. 
 * The compositor will emit a wl_display.delete_id event with the object ID of the registry and will no longer emit any events on the registry.
 * The client should re-use the object ID once it receives the wl_display.delete_id event. @param registry The registry object to destroy.
 */
void wl_fixes_destroy_registry(struct wl_fixes *fixes, struct wl_registry *registry);

/*! @brief */
void *wl_proxy_marshal_flags(struct wl_proxy *proxy, uint32_t opcode, const struct wl_interface *interface, uint32_t version, uint32_t flags, ...);

/*! @brief */
uint32_t wl_proxy_get_version(struct wl_proxy *proxy);


#ifdef  __cplusplus
}
#endif

#endif // FLORA_PROTOCALS_H
