#ifndef LIBPOWER_H
#define LIBPOWER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Power states */
typedef enum {
    POWER_STATE_ERROR,           // error determining power status
    POWER_STATE_UNKNOWN,         // cannot determine power status
    POWER_STATE_ON_BATTERY,      // Not plugged in, running on the battery
    POWER_STATE_NO_BATTERY,      // Plugged in, no battery available
    POWER_STATE_CHARGING,        // Plugged in, charging battery
    POWER_STATE_CHARGED          // Plugged in, battery charged
} power_state_t;

/* Battery health */
typedef enum {
    BATTERY_HEALTH_UNKNOWN = 0,
    BATTERY_HEALTH_GOOD,
    BATTERY_HEALTH_DEGRADED,
    BATTERY_HEALTH_BAD
} battery_health_t;

/* Battery information */
typedef struct {
    int percentage;          /* 0 - 100 */
    int voltage_mv;          /* millivolts */
    int current_ma;          /* milliamps */
    int temperature_c;       /* Celsius */
    battery_health_t health;
    power_state_t state;
} battery_info_t;

/*! @brief Battery info  @returns 1 if has battery. 0 if there is no battery. */
int power_get_battery_info(battery_info_t *info);

/* System power control */
int power_suspend(void);
int power_resume(void);
int power_shutdown_system(void);
int power_reboot_system(void);

/* Power profiles */
int power_set_performance_mode(void);
int power_set_balanced_mode(void);
int power_set_power_saver_mode(void);

#ifdef __cplusplus
}
#endif

#endif /* LIBPOWER_H */
