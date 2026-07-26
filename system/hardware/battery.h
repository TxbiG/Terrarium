#ifndef TERRARIUM_SYSTEM_HARDWARE_BATTERY_H
#define TERRARIUM_SYSTEM_HARDWARE_BATTERY_H

typedef struct terra_battery_status {
    int present;
    int charging;
    int percent;
    int minutes_remaining;
} terra_battery_status_t;

int terra_battery_query(terra_battery_status_t *out_status);

#endif
