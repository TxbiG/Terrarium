
#include "wayland-client-protocol.h"




typedef void (*wl_display_listener_error)(void *data, struct wl_display *wl_display, struct wl_object *object_id, uint32_t code, const char *message);

typedef void (*wl_display_listener_delete_id)(void *data, struct wl_display *wl_display, uint32_t id);

typedef void (*wl_registry_listener_global)(void *data,  struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version);

typedef void (*wl_registry_listener_global_remove)(void *data, struct wl_registry *registry, uint32_t name);

typedef void (*wl_callback_listener_done)(void *data, struct wl_callback *callback, uint32_t callback_data);

/* Event handler callback types */
typedef void (*wl_seat_capabilities_handler)(void *data, wl_seat *seat, uint32_t capabilities);

typedef void (*wl_seat_name_handler)(void *data, wl_seat *seat, const char *name);


/**
 * Pointer enter event.
 * Indicates pointer focus on a surface.
 * 
 * @param serial Serial number of the enter event.
 * @param surface Surface entered by the pointer.
 * @param surface_x Surface-local x coordinate (fixed-point).
 * @param surface_y Surface-local y coordinate (fixed-point).
 */
typedef void (*wl_pointer_enter_handler_t)(uint32_t serial, struct wl_surface *surface, int32_t surface_x, int32_t surface_y);

/**
 * Pointer leave event.
 * Indicates pointer focus leaves a surface.
 * 
 * @param serial Serial number of the leave event.
 * @param surface Surface left by the pointer.
 */
typedef void (*wl_pointer_leave_handler_t)(uint32_t serial, struct wl_surface *surface);

/**
 * Pointer motion event.
 * Indicates pointer movement relative to the focused surface.
 * 
 * @param time Timestamp in milliseconds.
 * @param surface_x Surface-local x coordinate (fixed-point).
 * @param surface_y Surface-local y coordinate (fixed-point).
 */
typedef void (*wl_pointer_motion_handler_t)(uint32_t time, int32_t surface_x, int32_t surface_y);

/**
 * Pointer button event.
 * Indicates a mouse button press or release.
 * 
 * @param serial Serial number of the button event.
 * @param time Timestamp in milliseconds.
 * @param button Button code (from linux/input-event-codes.h).
 * @param state Physical state of the button.
 */
typedef void (*wl_pointer_button_handler_t)(uint32_t serial, uint32_t time, uint32_t button, wl_pointer_button_state_t state);

/**
 * Pointer axis event.
 * Indicates scroll or other axis movement.
 * 
 * @param time Timestamp in milliseconds.
 * @param axis Axis type.
 * @param value Scroll length in surface-local coordinate space (fixed-point).
 */
typedef void (*wl_pointer_axis_handler_t)(uint32_t time, wl_pointer_axis_t axis, int32_t value);

/**
 * Pointer frame event (since version 5).
 * Indicates end of a logical group of pointer events.
 */
typedef void (*wl_pointer_frame_handler_t)(void);

/**
 * Pointer axis_source event (since version 5).
 * Indicates how an axis event was physically generated.
 * @param source Axis source type.
 */
typedef void (*wl_pointer_axis_source_handler_t)(wl_pointer_axis_source_t source);

/**
 * Pointer axis_stop event (since version 5).
 * Indicates the stopping of scrolling on an axis.
 * @param time Timestamp in milliseconds.
 * @param axis Axis type that stopped.
 */
typedef void (*wl_pointer_axis_stop_handler_t)(uint32_t time, wl_pointer_axis_t axis);

/**
 * Pointer axis_discrete event (since version 5, deprecated since 8).
 * Indicates discrete scroll steps.
 * @param axis Axis type.
 * @param discrete Number of steps (can be negative).
 */
typedef void (*wl_pointer_axis_discrete_handler_t)(wl_pointer_axis_t axis, int32_t discrete);

/**
 * Pointer axis_value120 event (since version 8).
 * Indicates high-resolution scroll value.
 * @param axis Axis type.
 * @param value120 Scroll distance as fraction of 120.
 */
typedef void (*wl_pointer_axis_value120_handler_t)(wl_pointer_axis_t axis, int32_t value120);


/**
 * Pointer axis_relative_direction event (since version 9).
 * Indicates relative physical direction of axis movement.
 * @param axis Axis type.
 * @param direction Relative direction.
 */
typedef void (*wl_pointer_axis_relative_direction_handler_t)(wl_pointer_axis_t axis, wl_pointer_axis_relative_direction_t direction);


typedef struct {
    rect* rects;     // Dynamic array of rectangles
    size_t count;      // Number of rectangles currently in the region
    size_t capacity;   // Allocated capacity for the rectangles array
} wl_region;

struct wl_surface {
    // Requests (methods called by client)
    void (*destroy)(struct wl_surface* surface);
    void (*attach)(struct wl_surface* surface, struct wl_buffer* buffer, int32_t x, int32_t y);
    void (*damage)(struct wl_surface* surface, int32_t x, int32_t y, int32_t width, int32_t height);
    void (*frame)(struct wl_surface* surface, struct wl_callback* callback);
    void (*set_opaque_region)(struct wl_surface* surface, struct wl_region* region);
    void (*set_input_region)(struct wl_surface* surface, struct wl_region* region);
    void (*commit)(struct wl_surface* surface);
    void (*set_buffer_transform)(struct wl_surface* surface, enum wl_output_transform transform);
    void (*set_buffer_scale)(struct wl_surface* surface, int32_t scale);
    void (*damage_buffer)(struct wl_surface* surface, int32_t x, int32_t y, int32_t width, int32_t height);
    void (*offset)(struct wl_surface* surface, int32_t x, int32_t y);

    // Events (callbacks from compositor)
    void (*enter)(struct wl_surface* surface, struct wl_output* output);
    void (*leave)(struct wl_surface* surface, struct wl_output* output);
    void (*preferred_buffer_scale)(struct wl_surface* surface, int32_t factor);
    void (*preferred_buffer_transform)(struct wl_surface* surface, uint32_t transform);
};

/* wl_data_offer requests */
struct wl_data_offer_requests {
    /**
     * accept - accept one of the offered mime types
     * @serial: serial number of the accept request
     * @mime_type: mime type accepted by the client, or NULL if none accepted
     */
    void (*accept)(void *data, uint32_t serial, const char *mime_type);

    /**
     * receive - request that the data is transferred
     * @mime_type: mime type desired by receiver
     * @fd: file descriptor for data transfer
     */
    void (*receive)(void *data, const char *mime_type, int fd);

    /**
     * destroy - destroy data offer
     */
    void (*destroy)(void *data);

    /**
     * finish - the offer will no longer be used (since version 3)
     */
    void (*finish)(void *data);

    /**
     * set_actions - set the available/preferred drag-and-drop actions (since version 3)
     * @dnd_actions: actions supported by the destination client
     * @preferred_action: action preferred by the destination client
     */
    void (*set_actions)(void *data, uint32_t dnd_actions, uint32_t preferred_action);
};

/* wl_data_offer events */
struct wl_data_offer_events {
    /**
     * offer - advertise offered mime type
     * @mime_type: offered mime type
     */
    void (*offer)(void *data, const char *mime_type);

    /**
     * source_actions - notify the source-side available actions (since version 3)
     * @source_actions: actions offered by the data source
     */
    void (*source_actions)(void *data, uint32_t source_actions);

    /**
     * action - notify the selected action (since version 3)
     * @dnd_action: action selected by the compositor
     */
    void (*action)(void *data, uint32_t dnd_action);
};

/* wl_data_source requests */
struct wl_data_source_requests {
    /**
     * offer - add an offered mime type
     * @mime_type: mime type offered by the data source
     *
     * This request adds a mime type to the set of mime types
     * advertised to targets. Can be called several times to offer
     * multiple types.
     */
    void (*offer)(void *data, const char *mime_type);

    /**
     * destroy - destroy the data source
     */
    void (*destroy)(void *data);

    /**
     * set_actions - set the available drag-and-drop actions (since version 3)
     * @dnd_actions: actions supported by the data source
     *
     * Sets the actions that the source side client supports for this
     * operation. This request may trigger wl_data_source.action and
     * wl_data_offer.action events if the compositor needs to change the
     * selected action.

     * The dnd_actions argument must contain only values expressed in the
     * wl_data_device_manager.dnd_actions enum, otherwise it will result
     * in a protocol error.

     * This request must be made once only, and can only be made on sources
     * used in drag-and-drop, so it must be performed before
     * wl_data_device.start_drag. Attempting to use the source other than
     * for drag-and-drop will raise a protocol error.
     */
    void (*set_actions)(void *data, uint32_t dnd_actions);
};

/* wl_data_source events */
struct wl_data_source_events {
    /**
     * target - a target accepts an offered mime type
     * @mime_type: mime type accepted by the target, or NULL if none accepted
     *
     * Sent when a target accepts pointer_focus or motion events. If
     * a target does not accept any of the offered types, type is NULL.
     *
     * Used for feedback during drag-and-drop.
     */
    void (*target)(void *data, const char *mime_type);

    /**
     * send - send the data
     * @mime_type: mime type for the data
     * @fd: file descriptor for the data
     *
     * Request for data from the client. Send the data as the
     * specified mime type over the passed file descriptor, then
     * close it.
     */
    void (*send)(void *data, const char *mime_type, int fd);

    /**
     * cancelled - selection was cancelled
     *
     * This data source is no longer valid. Reasons:
     * - replaced by another data source
     * - drop destination did not accept any mime type
     * - drop destination did not select any action in mask
     * - drop did not happen over a surface
     * - compositor cancelled drag-and-drop operation
     *
     * Client should clean up and destroy this data source.
     *
     * For version 2 or older, emitted only if replaced by another source.
     */
    void (*cancelled)(void *data);

    /**
     * dnd_drop_performed - drag-and-drop operation physically finished (since version 3)
     *
     * The user performed the drop action. This event does not indicate
     * acceptance, wl_data_source.cancelled may still be emitted afterwards
     * if the drop destination does not accept any mime type.
     *
     * May not be received if compositor cancelled operation before event.
     *
     * Data source may still be used and should not be destroyed here.
     */
    void (*dnd_drop_performed)(void *data);

    /**
     * dnd_finished - drag-and-drop operation concluded (since version 3)
     *
     * Drop destination finished interoperating with this data source,
     * client is now free to destroy data source and free resources.
     *
     * If action was "move", source can now delete transferred data.
     */
    void (*dnd_finished)(void *data);

    /**
     * action - notify the selected action (since version 3)
     * @dnd_action: action selected by the compositor
     *
     * Indicates the action selected after matching source/destination.
     * Can be emitted multiple times during drag-and-drop operation.
     * Only possible after dnd_drop_performed if ended in "ask" action.
     *
     * Compositors may change selected action on the fly.
     *
     * Most recent action is always valid.
     */
    void (*action)(void *data, uint32_t dnd_action);
};

// Events callback interface for wl_surface
struct wl_surface_listener {
    // The surface enters an output.
    void (*enter)(void *data, struct wl_surface *surface, struct wl_output *output);

    // The surface leaves an output.
    void (*leave)(void *data, struct wl_surface *surface, struct wl_output *output);

    // Preferred buffer scale changed.
    void (*preferred_buffer_scale)(void *data, struct wl_surface *surface, int32_t factor);

    // Preferred buffer transform changed.
    void (*preferred_buffer_transform)(void *data, struct wl_surface *surface, uint32_t transform);
};


struct wl_proxy {
    uint32_t id // server-side object ID
    const struct wl_interface *iface;   // protocol interface
    uint32_t version;                   // negotiated version

    void (**listener)(void);            // event callbacks
    void *listener_data;                // user data

    struct wl_display *display;         // connection / transport owner
};



/* Event listener struct */
typedef struct wl_seat_listener {
    wl_seat_capabilities_handler capabilities;
    wl_seat_name_handler name;
} wl_seat_listener;
/* Events */
/* Interface struct holding event callbacks */
typedef struct wl_pointer_listener {
    wl_pointer_enter_handler_t enter;
    wl_pointer_leave_handler_t leave;
    wl_pointer_motion_handler_t motion;
    wl_pointer_button_handler_t button;
    wl_pointer_axis_handler_t axis;
    wl_pointer_frame_handler_t frame; /* since v5 */
    wl_pointer_axis_source_handler_t axis_source; /* since v5 */
    wl_pointer_axis_stop_handler_t axis_stop; /* since v5 */
    wl_pointer_axis_discrete_handler_t axis_discrete; /* since v5, deprecated v8 */
    wl_pointer_axis_value120_handler_t axis_value120; /* since v8 */
    wl_pointer_axis_relative_direction_handler_t axis_relative_direction; /* since v9 */
} wl_pointer_listener_t;

/* Event data structs */

/* keymap event */
struct wl_keyboard_keymap_event {
    uint32_t format;    /* keymap format (enum wl_keyboard_keymap_format) */
    int32_t  fd;/* file descriptor of the keymap */
    uint32_t size;      /* size in bytes of the keymap */
};

/* enter event */
struct wl_keyboard_enter_event {
    uint32_t serial;      /* serial number */
    struct wl_surface *surface; /* surface gaining keyboard focus */
    const uint32_t *keys; /* array of keys currently logically down */
    size_t keys_count;    /* number of keys in the keys array */
};

/* leave event */
struct wl_keyboard_leave_event {
    uint32_t serial;      /* serial number */
    struct wl_surface *surface; /* surface losing keyboard focus */
};

/* key event */
struct wl_keyboard_key_event {
    uint32_t serial;      /* serial number */
    uint32_t time;/* timestamp with millisecond granularity */
    uint32_t key; /* keycode */
    uint32_t state;* key state (enum wl_keyboard_key_state) */
};

/* modifiers event */
struct wl_keyboard_modifiers_event {
    uint32_t serial; /* serial number */
    uint32_t mods_depressed; /* depressed modifiers */
    uint32_t mods_latched;   /* latched modifiers */
    uint32_t mods_locked;    /* locked modifiers */
    uint32_t group;  /* keyboard layout group */
};

/* repeat_info event (since version 4) */
struct wl_keyboard_repeat_info_event {
    int32_t rate;  /* repeat rate in characters per second */
    int32_t delay; /* delay in ms before repeating starts */
};
/* wl_touch events parameters structs */
struct wl_touch_down_event {
    uint32_t serial;
    uint32_t time;
    void *surface;  /* wl_surface pointer */
    int32_t id;
    wl_fixed_t x;
    wl_fixed_t y;
};

struct wl_touch_up_event {
    uint32_t serial;
    uint32_t time;
    int32_t id;
};

struct wl_touch_motion_event {
    uint32_t time;
    int32_t id;
    wl_fixed_t x;
    wl_fixed_t y;
};

struct wl_touch_shape_event {
    int32_t id;
    wl_fixed_t major;
    wl_fixed_t minor;
};

struct wl_touch_orientation_event {
    int32_t id;
    wl_fixed_t orientation;
};

/* wl_output events parameters structs */
struct wl_output_geometry_event {
    int32_t x;
    int32_t y;
    int32_t physical_width;
    int32_t physical_height;
    enum wl_output_subpixel subpixel;
    const char *make;
    const char *model;
    enum wl_output_transform transform;
};

struct wl_output_mode_event {
    uint32_t flags; /* bitfield of wl_output_mode_flags */
    int32_t width;
    int32_t height;
    int32_t refresh; /* in mHz */
};

struct wl_output_scale_event { int32_t factor; };

struct wl_output_name_event { const char *name; };

struct wl_output_description_event { const char *description; };


struct wl_display_listener {
    wl_display_listener_error error;
    wl_display_listener_delete_id delete_id;
};

struct wl_registry_listener {
    wl_registry_listener_global global;
    wl_registry_listener_global_remove global_remove;
};

struct wl_callback_listener {
    wl_callback_listener_done done;
};

/* Event listener callbacks */
struct wl_keyboard_listener {
    void (*keymap)(void *data, struct wl_keyboard *keyboard, uint32_t format, int32_t fd, uint32_t size);
    void (*enter)(void *data, struct wl_keyboard *keyboard, uint32_t serial, struct wl_surface *surface, const uint32_t *keys, size_t keys_count);
    void (*leave)(void *data, struct wl_keyboard *keyboard, uint32_t serial, struct wl_surface *surface);
    void (*key)(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state);
    void (*modifiers)(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group);
    void (*repeat_info)(void *data, struct wl_keyboard *keyboard, int32_t rate, int32_t delay);
};


// Display
struct wl_callback *wl_display_sync(struct wl_display *display) {
    return wl_proxy_marshal_flags((struct wl_proxy *)display, 0, /* sync opcode */ &wl_callback_interface, wl_proxy_get_version((struct wl_proxy *)display), 0);
}

struct wl_registry *wl_display_get_registry(struct wl_display *display) {
    return wl_proxy_marshal_flags((struct wl_proxy *)display, 1, /* get_registry */ &wl_registry_interface, wl_proxy_get_version((struct wl_proxy *)display), 0);
}

int wl_display_add_listener(struct wl_display *display, const struct wl_display_listener *listener, void *data) {
    return wl_proxy_add_listener((struct wl_proxy *)display, (void (**)(void))listener, data);
}


// registry
struct wl_proxy *wl_registry_bind(struct wl_registry *registry,    uint32_t name,    const struct wl_interface *interface,    uint32_t version) {
    return wl_proxy_marshal_flags((struct wl_proxy *)registry, 0, /* bind */ interface, version, 0, name, interface->name, version);
}

int wl_registry_add_listener(struct wl_registry *registry,onst struct wl_registry_listener *listener,oid *data) {
    return wl_proxy_add_listener((struct wl_proxy *)registry, (void (**)(void))listener, data);
}

// compositor
struct wl_surface *wl_compositor_create_surface(struct wl_compositor *compositor) {
    return wl_proxy_marshal_flags((struct wl_proxy *)compositor, 0, &wl_surface_interface, wl_proxy_get_version((struct wl_proxy *)compositor), 0);
}

struct wl_region *wl_compositor_create_region(struct wl_compositor *compositor) {
    return wl_proxy_marshal_flags((struct wl_proxy *)compositor, 1, &wl_region_interface, wl_proxy_get_version((struct wl_proxy *)compositor), 0);
}



// wayland surface
void wl_surface_destroy(struct wl_surface *surface) { 
    wl_proxy_marshal_flags((struct wl_proxy *)surface, WL_SURFACE_DESTROY, NULL, wl_proxy_get_version((struct wl_proxy *)surface), WL_MARSHAL_FLAG_DESTROY);
}

void wl_surface_attach(struct wl_surface *surface, struct wl_buffer *buffer, int32_t x, int32_t y) { wl_proxy_marshal((struct wl_proxy *)surface, WL_SURFACE_ATTACH, buffer, x, y); }

void wl_surface_damage(struct wl_surface *surface, int32_t x, int32_t y, int32_t width, int32_t height) { wl_proxy_marshal((struct wl_proxy *)surface, WL_SURFACE_DAMAGE, x, y, width, height); }

struct wl_callback *wl_surface_frame(struct wl_surface *surface) {
    return wl_proxy_marshal_flags((struct wl_proxy *)surface, WL_SURFACE_FRAME, &wl_callback_interface, wl_proxy_get_version((struct wl_proxy *)surface), 0);
}

void wl_surface_set_opaque_region(struct wl_surface *surface,    struct wl_region *region) {
    wl_proxy_marshal((struct wl_proxy *)surface, WL_SURFACE_SET_OPAQUE_REGION, region);
}

void wl_surface_set_input_region(struct wl_surface *surface,   struct wl_region *region) {
    wl_proxy_marshal((struct wl_proxy *)surface, WL_SURFACE_SET_INPUT_REGION, region);
}

void wl_surface_commit(struct wl_surface *surface) {
    wl_proxy_marshal((struct wl_proxy *)surface, WL_SURFACE_COMMIT);
}

void wl_surface_set_buffer_transform(struct wl_surface *surface,int32_t transform) {
    wl_proxy_marshal((struct wl_proxy *)surface, WL_SURFACE_SET_BUFFER_TRANSFORM, transform);
}

void wl_surface_set_buffer_scale(struct wl_surface *surface,   int32_t scale) {
    wl_proxy_marshal((struct wl_proxy *)surface, WL_SURFACE_SET_BUFFER_SCALE, scale);
}

void wl_surface_damage_buffer(struct wl_surface *surface,int32_t x,int32_t y,int32_t width,int32_t height) {
    wl_proxy_marshal((struct wl_proxy *)surface, WL_SURFACE_DAMAGE_BUFFER, x, y, width, height);
}

void wl_surface_offset(struct wl_surface *surface, int32_t x, int32_t y) {
    wl_proxy_marshal((struct wl_proxy *)surface, WL_SURFACE_OFFSET, x, y);
}

int wl_surface_add_listener(struct wl_surface *surface, const struct wl_surface_listener *listener, void *data) {
    return wl_proxy_add_listener((struct wl_proxy *)surface, (void (**)(void))listener, data);
}


// region
void wl_region_destroy(struct wl_region *region) {
    wl_proxy_marshal_flags((struct wl_proxy *)region, 0, NULL, wl_proxy_get_version((struct wl_proxy *)region), WL_MARSHAL_FLAG_DESTROY);
}

void wl_region_add(struct wl_region *region, int32_t x, int32_t y, int32_t width, int32_t height) {
    wl_proxy_marshal((struct wl_proxy *)region, 1, x, y, width, height);
}

void wl_region_subtract(struct wl_region *region,  int32_t x,  int32_t y,  int32_t width,  int32_t height) {
    wl_proxy_marshal((struct wl_proxy *)region, 2, x, y, width, height);
}


// wl_seat / pointer / keyboard / touch
wl_pointer *wl_seat_get_pointer(wl_seat *seat) { return wl_proxy_marshal_flags((struct wl_proxy *)seat, 0, &wl_pointer_interface, wl_proxy_get_version((struct wl_proxy *)seat), 0); }

wl_keyboard *wl_seat_get_keyboard(wl_seat *seat) { return wl_proxy_marshal_flags((struct wl_proxy *)seat, 1, &wl_keyboard_interface, wl_proxy_get_version((struct wl_proxy *)seat), 0); }

wl_touch *wl_seat_get_touch(wl_seat *seat) { return wl_proxy_marshal_flags((struct wl_proxy *)seat, 2, &wl_touch_interface, wl_proxy_get_version((struct wl_proxy *)seat), 0); }

void wl_seat_release(wl_seat *seat) { wl_proxy_marshal_flags((struct wl_proxy *)seat, 3, NULL, wl_proxy_get_version((struct wl_proxy *)seat), WL_MARSHAL_FLAG_DESTROY); }

// sub compositor
struct wl_subsurface* wl_subcompositor_get_subsurface(struct wl_subcompositor *subcompositor,  struct wl_surface *surface,  struct wl_surface *parent) {
    return wl_proxy_marshal_flags((struct wl_proxy *)subcompositor, 1, &wl_subsurface_interface, wl_proxy_get_version((struct wl_proxy *)subcompositor), 0, surface, parent);
}

void wl_subsurface_destroy(struct wl_subsurface *subsurface) {
    wl_proxy_marshal_flags((struct wl_proxy *)subsurface, 0, NULL, wl_proxy_get_version((struct wl_proxy *)subsurface), WL_MARSHAL_FLAG_DESTROY);
}





void wl_shm_pool_destroy(struct wl_shm_pool *pool) {
    wl_proxy_marshal_flags((struct wl_proxy *)pool, WL_SHM_POOL_DESTROY, NULL, 0, WL_MARSHAL_FLAG_DESTROY);
}

void wl_shm_pool_resize(struct wl_shm_pool *pool, int32_t size) {
    wl_proxy_marshal((struct wl_proxy *)pool, WL_SHM_POOL_RESIZE, size);
}


void wl_data_device_start_drag(void *data,   void *source,   void *origin,   void *icon,   uint32_t serial) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_DATA_DEVICE_START_DRAG, source, origin, icon, serial);
}

void wl_data_device_set_selection(void *data,      void *source,      uint32_t serial) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_DATA_DEVICE_SET_SELECTION, source, serial);
}

void wl_data_device_release(void *data) {
    wl_proxy_marshal_flags((struct wl_proxy *)data, WL_DATA_DEVICE_RELEASE, NULL, 0, WL_MARSHAL_FLAG_DESTROY);
}

void wl_data_device_data_offer(void *data, void *id) {
    /* event stub – dispatched via listener */
}

void wl_data_device_enter(void *data, uint32_t serial, void *surface, int32_t x_fixed, int32_t y_fixed,void *id) {
}

void wl_data_device_leave(void *data) {
}

void wl_data_device_motion(void *data, uint32_t time, int32_t x_fixed, int32_t y_fixed) {
}

void wl_data_device_drop(void *data) {
}

void wl_data_device_selection(void *data, void *id) {
}

void wl_data_device_manager_create_data_source(void *data, void *id) {
    wl_proxy_marshal_flags((struct wl_proxy *)data, WL_DATA_DEVICE_MANAGER_CREATE_DATA_SOURCE, &wl_data_source_interface, wl_proxy_get_version((struct wl_proxy *)data), 0, id);
}

void wl_data_device_manager_get_data_device(void *data,                void *id,                void *seat) {
    wl_proxy_marshal_flags((struct wl_proxy *)data, WL_DATA_DEVICE_MANAGER_GET_DATA_DEVICE, &wl_data_device_interface, wl_proxy_get_version((struct wl_proxy *)data), 0, seat, id);
}


void wl_shell_get_shell_surface(void *data,    void *id,    void *surface) {
    wl_proxy_marshal_flags((struct wl_proxy *)data, WL_SHELL_GET_SHELL_SURFACE, &wl_shell_surface_interface, wl_proxy_get_version((struct wl_proxy *)data), 0, surface, id);
}


void wl_shell_surface_pong(void *data, uint32_t serial) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_PONG, serial);
}

void wl_shell_surface_move(void *data,void *seat,uint32_t serial) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_MOVE, seat, serial);
}

void wl_shell_surface_resize(void *data,  void *seat,  uint32_t serial,  uint32_t edges) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_RESIZE, seat, serial, edges);
}

void wl_shell_surface_set_toplevel(void *data) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_SET_TOPLEVEL);
}

void wl_shell_surface_set_transient(void *data, void *parent, int32_t x, int32_t y, uint32_t flags) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_SET_TRANSIENT, parent, x, y, flags);
}

void wl_shell_surface_set_fullscreen(void *data, uint32_t method, uint32_t framerate, void *output) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_SET_FULLSCREEN, method, framerate, output);
}

void wl_shell_surface_set_popup(void *data, void *seat, uint32_t serial, void *parent, int32_t x, int32_t y, uint32_t flags) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_SET_POPUP, seat, serial, parent, x, y, flags);
}

void wl_shell_surface_set_maximized(void *data, void *output) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_SET_MAXIMIZED, output);
}

void wl_shell_surface_set_title(void *data, const char *title) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_SET_TITLE, title);
}

void wl_shell_surface_set_class(void *data, const char *class_) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_SET_CLASS, class_);
}



void wl_shell_surface_pong(void *data, uint32_t serial) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_PONG, serial);
}

void wl_shell_surface_move(void *data,void *seat, uint32_t serial) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_MOVE, seat, serial);
}

void wl_shell_surface_resize(void *data,  void *seat,  uint32_t serial,  uint32_t edges) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_RESIZE, seat, serial, edges);
}

void wl_shell_surface_set_toplevel(void *data) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_SET_TOPLEVEL);
}

void wl_shell_surface_set_transient(void *data, void *parent, int32_t x, int32_t y, uint32_t flags) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_SET_TRANSIENT, parent, x, y, flags);
}

void wl_shell_surface_set_fullscreen(void *data, uint32_t method, uint32_t framerate, void *output) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_SET_FULLSCREEN, method, framerate, output);
}

void wl_shell_surface_set_popup(void *data, void *seat, uint32_t serial, void *parent, int32_t x, int32_t y, uint32_t flags) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_SET_POPUP, seat, serial, parent, x, y, flags);
}

void wl_shell_surface_set_maximized(void *data, void *output) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_SET_MAXIMIZED, output);
}

void wl_shell_surface_set_title(void *data, const char *title) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_SET_TITLE, title);
}

void wl_shell_surface_set_class(void *data, const char *class_) {
    wl_proxy_marshal((struct wl_proxy *)data, WL_SHELL_SURFACE_SET_CLASS, class_);
}

void wl_shell_surface_ping(void *data, uint32_t serial) {
}

void wl_shell_surface_configure(void *data, uint32_t edges, int32_t width, int32_t height) {
}

void wl_shell_surface_popup_done(void *data) {
}

void wl_subsurface_set_position(struct wl_subsurface *subsurface,    int32_t x,    int32_t y) {
    wl_proxy_marshal((struct wl_proxy *)subsurface, WL_SUBSURFACE_SET_POSITION, x, y);
}

void wl_subsurface_place_above(struct wl_subsurface *subsurface,   struct wl_surface *sibling) {
    wl_proxy_marshal((struct wl_proxy *)subsurface, WL_SUBSURFACE_PLACE_ABOVE, sibling);
}

void wl_subsurface_place_below(struct wl_subsurface *subsurface,   struct wl_surface *sibling) {
    wl_proxy_marshal((struct wl_proxy *)subsurface, WL_SUBSURFACE_PLACE_BELOW, sibling);
}

void wl_subsurface_set_sync(struct wl_subsurface *subsurface) {
    wl_proxy_marshal((struct wl_proxy *)subsurface, WL_SUBSURFACE_SET_SYNC);
}

void wl_subsurface_set_desync(struct wl_subsurface *subsurface) {
    wl_proxy_marshal((struct wl_proxy *)subsurface, WL_SUBSURFACE_SET_DESYNC);
}

void wl_fixes_destroy(struct wl_fixes *fixes) {
    wl_proxy_marshal_flags((struct wl_proxy *)fixes, WL_FIXES_DESTROY, NULL, 0, WL_MARSHAL_FLAG_DESTROY);
}

void wl_fixes_destroy_registry(struct wl_fixes *fixes, struct wl_registry *registry) {
    wl_proxy_marshal((struct wl_proxy *)fixes, WL_FIXES_DESTROY_REGISTRY, registry);
}

void wl_subcompositor_destroy(struct wl_subcompositor *subcompositor) {
    wl_proxy_marshal_flags((struct wl_proxy *)subcompositor, WL_SUBCOMPOSITOR_DESTROY, NULL, 0, WL_MARSHAL_FLAG_DESTROY);
}




int wl_registry_add_listener(struct wl_registry *registry, const struct wl_registry_listener *listener, void *data)
{
    return wl_proxy_add_listener((struct wl_proxy *)registry, (void (**)(void))listener, data);
}



void wl_callback_destroy(struct wl_callback *callback)
{
    wl_proxy_destroy((struct wl_proxy *)callback);
}


int wl_callback_add_listener(struct wl_callback *callback, const struct wl_callback_listener *listener, void *data)
{
    return wl_proxy_add_listener((struct wl_proxy *)callback, (void (**)(void))listener, data);
}
















struct wl_wire_header {
    uint32_t object_id;
    uint16_t opcode;
    uint16_t size;   // total message size in bytes
};



uint32_t wl_proxy_get_version(struct wl_proxy *proxy) { return proxy ? proxy->version : 0; }


int wl_proxy_add_listener(struct wl_proxy *proxy,   void (**listener)(void),   void *data) {
    if (!proxy || !listener)
        return -1;

    proxy->listener = listener;
    proxy->listener_data = data;
    return 0;
}


void wl_proxy_marshal(struct wl_proxy *proxy, uint32_t opcode, ...) {
    va_list ap;
    va_start(ap, opcode);

    wl_proxy_marshal_flags( proxy, opcode, NULL, proxy->version, 0, ap);
    va_end(ap);
}

static struct wl_proxy * wl_proxy_create(struct wl_display *display, const struct wl_interface *iface,  uint32_t version) {
    struct wl_proxy *proxy = calloc(1, sizeof(*proxy));
    proxy->id = wl_display_allocate_id(display);
    proxy->iface = iface;
    proxy->version = version;
    proxy->display = display;
    return proxy;
}

void *wl_proxy_marshal_flags(struct wl_proxy *proxy, uint32_t opcode, const struct wl_interface *interface, uint32_t version, uint32_t flags, ...) {
    struct wl_proxy *new_proxy = NULL;

    if (interface) { new_proxy = wl_proxy_create(proxy->display, interface, version); }

    uint8_t buffer[512]; // simple fixed buffer for now
    struct wl_wire_header *hdr = (struct wl_wire_header *)buffer;

    hdr->object_id = proxy->id;
    hdr->opcode = opcode;
    hdr->size = sizeof(*hdr);

    uint8_t *p = buffer + sizeof(*hdr);

    va_list ap;
    va_start(ap, flags);

    /* Argument packing (simplified) */
    for (const char *sig = proxy->iface->requests[opcode].signature; *sig; sig++) {

        switch (*sig) {
        case 'u': { // uint32
            uint32_t v = va_arg(ap, uint32_t);
            memcpy(p, &v, 4);
            p += 4;
            hdr->size += 4;
            break;
        }
        case 'i': { // int32
            int32_t v = va_arg(ap, int32_t);
            memcpy(p, &v, 4);
            p += 4;
            hdr->size += 4;
            break;
        }
        case 'o': { // object
            struct wl_proxy *obj = va_arg(ap, struct wl_proxy *);
            uint32_t id = obj ? obj->id : 0;
            memcpy(p, &id, 4);
            p += 4;
            hdr->size += 4;
            break;
        }
        case 'n': { // new_id
            uint32_t id = new_proxy ? new_proxy->id : 0;
            memcpy(p, &id, 4);
            p += 4;
            hdr->size += 4;
            break;
        }
        /* strings, arrays omitted for now */
        }
    }

    va_end(ap);

    /* Send message */
    wl_connection_send(proxy->display, buffer, hdr->size);

    /* Handle destroy flag */
    if (flags & WL_MARSHAL_FLAG_DESTROY) { wl_proxy_destroy(proxy); }

    return new_proxy;
}


void wl_connection_send(struct wl_display *display, const void *data, size_t size);
