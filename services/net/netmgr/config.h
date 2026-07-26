#ifndef TERRARIUM_NETMGR_CONFIG_H
#define TERRARIUM_NETMGR_CONFIG_H

#define TERRARIUM_NET_CONFIG_PATH "/etc/terrarium/net/interfaces.cfg"

typedef struct terrarium_net_interface_config {
    const char *name;
    const char *mode;
    const char *address;
    const char *gateway;
} terrarium_net_interface_config_t;

#endif
