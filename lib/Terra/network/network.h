#ifndef TERRARIUM_NET_LIB_H
#define TERRARIUM_NET_LIB_H

#include <stddef.h>
#include <stdint.h>

#if defined(_MSC_VER) && !defined(_SSIZE_T_DEFINED)
typedef intptr_t ssize_t;
#define _SSIZE_T_DEFINED
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define NET_POLL_IN   0x01
#define NET_POLL_OUT  0x02
#define NET_POLL_ERR  0x04

typedef int net_socket_t;

typedef enum {
    NET_OK = 0,
    NET_ERR_TIMEOUT,
    NET_ERR_REFUSED,
    NET_ERR_RESET,
    NET_ERR_NO_ROUTE
} net_error_t;

typedef enum {
    NET_AF_INET = 1,
    NET_AF_INET6 = 2
} net_addr_family_t;

typedef enum {
    NET_SOCK_STREAM,
    NET_SOCK_DGRAM
} net_socket_type_t;

typedef enum {
    NET_OPT_NONBLOCK,
    NET_OPT_RECV_TIMEOUT,
    NET_OPT_SEND_TIMEOUT,
    NET_OPT_REUSE_ADDR
} net_socket_option_t;

typedef struct {
    net_socket_t socket;
    uint32_t events;
    uint32_t revents;
} net_pollfd_t;

typedef struct {
    net_addr_family_t family;
    uint16_t port;
    union {
        uint32_t ipv4;
        uint8_t  ipv6[16];
    };
} net_addr_t;

int net_shutdown(net_socket_t s, int how);
net_socket_t net_socket(int domain, int type, int protocol);
int net_bind(net_socket_t s, const net_addr_t *addr);
int net_connect(net_socket_t s, const net_addr_t *addr);
ssize_t net_send(net_socket_t s, const void *data, size_t len);
ssize_t net_recv(net_socket_t s, void *buf, size_t len);
int net_close(net_socket_t s);


// TCP
int net_listen(net_socket_t s, int backlog);
net_socket_t net_accept(net_socket_t s, net_addr_t *out_addr);



ssize_t net_sendto(net_socket_t s, const void *data, size_t len, const net_addr_t *addr);

ssize_t net_recvfrom(net_socket_t s, void *buf, size_t len, net_addr_t *out_addr);
int net_setsockopt(net_socket_t s, net_socket_option_t opt, const void *value, size_t len);

net_error_t net_last_error(void);
net_error_t net_socket_error(net_socket_t s);
int net_poll(net_pollfd_t *fds, size_t count, int timeout_ms);

#ifdef __cplusplus
}
#endif

#endif // TERRARIUM_NET_LIB_H
