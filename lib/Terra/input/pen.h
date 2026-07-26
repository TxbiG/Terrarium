#pragma once

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef uint32_t pen_id_t;

typedef enum {
    PEN_AXIS_PRESSURE,  /**< Pen pressure.  Unidirectional: 0 to 1.0 */
    PEN_AXIS_XTILT,     /**< Pen horizontal tilt angle.  Bidirectional: -90.0 to 90.0 (left-to-right). */
    PEN_AXIS_YTILT,     /**< Pen vertical tilt angle.  Bidirectional: -90.0 to 90.0 (top-to-down). */
    PEN_AXIS_DISTANCE,  /**< Pen distance to drawing surface.  Unidirectional: 0.0 to 1.0 */
    PEN_AXIS_ROTATION,  /**< Pen barrel rotation.  Bidirectional: -180 to 179.9 (clockwise, 0 is facing up, -180.0 is facing down). */
    PEN_AXIS_SLIDER,    /**< Pen finger wheel or slider (e.g., Airbrush Pen).  Unidirectional: 0 to 1.0 */
    PEN_AXIS_TANGENTIAL_PRESSURE,    /**< Pressure from squeezing the pen ("barrel pressure"). */
    PEN_AXIS_COUNT       /**< Total known pen axis types in this version of Moss. This number may grow in future releases! */
} pen_axis_t;

typedef enum {
    PEN_TYPE_INVALID = -1, /**< Not a valid pen device. */
    PEN_TYPE_UNKNOWN,      /**< Don't know specifics of this pen. */
    PEN_TYPE_DIRECT,       /**< Pen touches display. */
    PEN_TYPE_INDIRECT      /**< Pen touches something that isn't the display. */
} pen_device_type_t;

typedef enum {
    PEN_INPUT_DOWN,
    PEN_INPUT_BUTTON_1,
    PEN_INPUT_BUTTON_2,
    PEN_INPUT_BUTTON_3,
    PEN_INPUT_BUTTON_4,
    PEN_INPUT_BUTTON_5,
    PEN_INPUT_BUTTON_TIP,
    PEN_INPUT_BUTTON_PROXIMITY,
} pen_input_flag_t;

typedef struct Moss_PenAxisEvent {
    Moss_EventType type;     /**< Moss_EVENT_PEN_AXIS */
    uint32 reserved;
    uint64 timestamp;       /**< In nanoseconds, populated using Moss_GetTicksNS() */
    pen_id_t which;        /**< The pen instance id */
    pen_input_flag_t pen_state;   /**< Complete pen input state at time of event */
    float x;                /**< X coordinate, relative to window */
    float y;                /**< Y coordinate, relative to window */
    pen_axis_t axis;       /**< Axis that has changed */
    float value;            /**< New value of axis */
} pen_axis_event_t;

typedef struct {
    Moss_EventType type; /**< Moss_EVENT_PEN_BUTTON_DOWN or Moss_EVENT_PEN_BUTTON_UP */
    uint32 reserved;
    uint64 timestamp;   /**< In nanoseconds, populated using Moss_GetTicksNS() */
    pen_id_t which;        /**< The pen instance id */
    pen_input_flag_t pen_state;   /**< Complete pen input state at time of event */
    float x;                /**< X coordinate, relative to window */
    float y;                /**< Y coordinate, relative to window */
    uint8 button;       /**< The pen button index (first button is 1). */
    bool down;      /**< true if the button is pressed */
} pen_button_event_t;

typedef struct {
    Moss_EventType type; /**< Moss_EVENT_PEN_MOTION */
    uint32 reserved;
    uint64 timestamp;   /**< In nanoseconds, populated using Moss_GetTicksNS() */
    pen_id_t which;        /**< The pen instance id */
    pen_input_flag_t pen_state;   /**< Complete pen input state at time of event */
    float x;                /**< X coordinate, relative to window */
    float y;                /**< Y coordinate, relative to window */
} pen_motion_event_t;

typedef struct {
    Moss_EventType type; /**< Moss_EVENT_PEN_PROXIMITY_IN or Moss_EVENT_PEN_PROXIMITY_OUT */
    uint32 reserved;
    uint64 timestamp;   /**< In nanoseconds, populated using Moss_GetTicksNS() */
    pen_id_t which;        /**< The pen instance id */
} pen_proximity_event_t;

typedef struct {
    Moss_EventType type;     /**< Moss_EVENT_PEN_DOWN or Moss_EVENT_PEN_UP */
    uint32 reserved;
    uint64 timestamp;       /**< In nanoseconds, populated using Moss_GetTicksNS() */
    pen_id_t which;        /**< The pen instance id */
    pen_input_flag_t pen_state;   /**< Complete pen input state at time of event */
    float x;                /**< X coordinate, relative to window */
    float y;                /**< Y coordinate, relative to window */
    bool eraser;        /**< true if eraser end is used (not all pens support this). */
    bool down;          /**< true if the pen is touching or false if the pen is lifted off */
} pen_touch_event_t;

#ifdef __cplusplus
}
#endif  // __cplusplus
