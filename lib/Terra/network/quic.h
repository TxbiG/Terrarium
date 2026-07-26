#ifndef TERRARIUM_NET_QUIC_H
#define TERRARIUM_NET_QUIC_H

#include <stdint.h>
#include <stddef.h>
#include "net.h"

// Add HTTP/2 framing, HTTP/3 mapping

#ifdef __cplusplus
extern "C" {
#endif

typedef struct quic_ctx quic_ctx_t;
typedef struct quic_conn quic_conn_t;
typedef struct quic_stream quic_stream_t;

typedef struct http3_client http3_client_t;
typedef struct http3_request http3_request_t;
typedef struct http3_response http3_response_t;

typedef enum {
    QUIC_OK = 0,
    QUIC_ERR_TIMEOUT,
    QUIC_ERR_HANDSHAKE,
    QUIC_ERR_CLOSED,
    QUIC_ERR_INTERNAL
} quic_error_t;

/* Context (shared config, certs, etc) */
quic_ctx_t* quic_ctx_create(void);
void quic_ctx_destroy(quic_ctx_t*);

/* Client connection */
quic_conn_t* quic_connect(
    quic_ctx_t* ctx,
    const net_addr_t *addr,
    const char *server_name /* SNI */
);

/* Server-side accept (optional later) */
quic_conn_t* quic_accept(quic_ctx_t* ctx);

/* Connection lifecycle */
void quic_conn_close(quic_conn_t*);

/* Streams */
quic_stream_t* quic_stream_open(quic_conn_t*);
ssize_t quic_stream_write(quic_stream_t*, const void*, size_t);
ssize_t quic_stream_read(quic_stream_t*, void*, size_t);
void quic_stream_close(quic_stream_t*);

/* Error handling */
quic_error_t quic_last_error(void);


/* Create client (reuse libquic context) */
http3_client_t* http3_client_create(quic_ctx_t* ctx);

/* Submit request (async) */
http3_request_t* http3_request_async(http3_client_t* client, const char *method, const char *url, const http_headers_t *headers, 
    const void *body, size_t body_len, void (*callback)(http3_request_t*, http3_response_t*, void*), void *user_data);

/* Response helpers */
int http3_response_status(http3_response_t*);
const char* http3_response_body(http3_response_t*, size_t *len);
void http3_response_destroy(http3_response_t*);


#ifdef __cplusplus
}
#endif

#endif // TERRARIUM_NET_QUIC_H
