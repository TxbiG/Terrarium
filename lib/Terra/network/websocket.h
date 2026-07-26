#ifndef TERRARIUM_NET_WS_H
#define TERRARIUM_NET_WS_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ws_conn ws_conn_t;

typedef enum {
    WS_OK = 0,
    WS_ERR_CLOSED,
    WS_ERR_PROTOCOL,
    WS_ERR_INTERNAL
} ws_error_t;

typedef enum {
    WS_MSG_TEXT,
    WS_MSG_BINARY
} ws_message_type_t;

/* Client-side connect */
ws_conn_t* ws_connect(const char *url);

/* Send message */
ssize_t ws_send(
    ws_conn_t*,
    ws_message_type_t type,
    const void *data,
    size_t len
);

/* Receive message */
ssize_t ws_recv(
    ws_conn_t*,
    ws_message_type_t *out_type,
    void *buffer,
    size_t buffer_len
);

/* Close connection */
void ws_close(ws_conn_t*);

/* Errors */
ws_error_t ws_last_error(void);

#ifdef __cplusplus
}
#endif

#endif // TERRARIUM_NET_WS_H
