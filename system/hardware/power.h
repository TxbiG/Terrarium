#ifndef TERRARIUM_SYSTEM_HARDWARE_POWER_H
#define TERRARIUM_SYSTEM_HARDWARE_POWER_H

typedef enum terra_power_action {
    TERRA_POWER_SHUTDOWN = 0,
    TERRA_POWER_REBOOT,
    TERRA_POWER_SUSPEND,
    TERRA_POWER_HIBERNATE
} terra_power_action_t;

int terra_power_request(terra_power_action_t action);

#endif
