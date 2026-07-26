#ifndef TERRARIUM_NET_TLS_LIB_H
#define TERRARIUM_NET_TLS_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tls_ctx tls_ctx_t;
typedef struct tls_conn tls_conn_t;

tls_ctx_t* tls_ctx_create(void);
tls_conn_t* tls_connect(tls_ctx_t*, net_socket_t socket);

ssize_t tls_write(tls_conn_t*, const void*, size_t);
ssize_t tls_read(tls_conn_t*, void*, size_t);
void tls_conn_close(tls_conn_t*);
void tls_ctx_destroy(tls_ctx_t*);

int tls_set_verify_mode(tls_ctx_t*, int mode);
int tls_set_ca_cert(tls_ctx_t*, const char *path);


#ifdef __cplusplus
}
#endif
#endif // TERRARIUM_NET_TLS_LIB_H
