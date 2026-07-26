#ifndef TERRARIUM_SYSTEM_NET_SOCKET_UTILS_H
#define TERRARIUM_SYSTEM_NET_SOCKET_UTILS_H

int terra_socket_set_nonblocking(int socket, int enabled);
int terra_socket_set_reuseaddr(int socket, int enabled);
int terra_socket_last_error(void);

#endif
