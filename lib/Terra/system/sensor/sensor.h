#ifndef TERRARIUM_SENSOR_LIB_H
#define TERRARIUM_SENSOR_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t sensor_id_t;

/* Sensor types */
typedef enum {
    SENSOR_ACCELEROMETER,
    SENSOR_GYROSCOPE,
    SENSOR_MAGNETOMETER,
    SENSOR_TEMPERATURE,
    SENSOR_LIGHT,
    SENSOR_PROXIMITY,
    SENSOR_HUMIDITY,
    SENSOR_PRESSURE,
    SENSOR_CUSTOM
} sensor_type_t;

typedef enum {
    SENSOR_OK = 0,
    SENSOR_ERR_INVALID_ID,
    SENSOR_ERR_NOT_SUPPORTED,
    SENSOR_ERR_BUSY,
    SENSOR_ERR_NO_MEMORY,
    SENSOR_ERR_INTERNAL
} sensor_error_t;

/* Sensor event structure */
typedef struct {
    sensor_id_t id;
    sensor_type_t type;
    uint64_t timestamp;     // nanoseconds
    union {
        struct { float x, y, z; } vec3;
        float value;       // single value sensors
    };
} sensor_event_t;

typedef struct {
    uint32_t sample_rate_hz;      // How frequently to sample
    uint8_t enabled;               // Enable / disable sensor
} sensor_config_t;

/* Sensor callback */
typedef void (*sensor_callback_t)(const sensor_event_t *event, void *user_data);

int sensor_init(void);                  // Initialize sensor subsystem
void sensor_shutdown(void);             // Shutdown sensor subsystem

size_t sensor_get_count(void);          // How many sensors exist
int sensor_get_info(size_t index, sensor_type_t *type, char *name, size_t name_len);

int sensor_subscribe(sensor_id_t id, sensor_callback_t cb, void *user_data);
int sensor_unsubscribe(sensor_id_t id, sensor_callback_t cb);

/*! Polling based */
int sensor_read(sensor_id_t id, sensor_event_t *out_event);

int sensor_set_config(sensor_id_t id, const sensor_config_t *cfg);
int sensor_get_config(sensor_id_t id, sensor_config_t *cfg);

/*! Error util */
sensor_error_t sensor_last_error(void);

#ifdef __cplusplus
}
#endif

#endif // TERRARIUM_SENSOR_LIB_H
