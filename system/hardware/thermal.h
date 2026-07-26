#ifndef TERRARIUM_SYSTEM_HARDWARE_THERMAL_H
#define TERRARIUM_SYSTEM_HARDWARE_THERMAL_H

typedef struct terra_thermal_zone {
    char name[32];
    int temperature_millic;
    int critical_millic;
} terra_thermal_zone_t;

int terra_thermal_list(terra_thermal_zone_t *zones, unsigned long max_zones);

#endif
