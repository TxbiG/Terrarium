#ifndef TERRARIUM_NET_HTTP_LIB_H
#define TERRARIUM_NET_HTTP_LIB_H


#ifdef __cplusplus
extern "C" {
#endif

typedef struct http_client_t http_client_t;

http_client_t* http_client_create(void);
http_response_t* http_get(http_client_t*, const char* url);
void http_client_destroy(http_client_t*);
http_response_t* http_request(http_client_t*, const char *method, const char *url, const http_headers_t *headers, const void *body, size_t body_len);
int http_response_status(http_response_t*);
const char* http_response_body(http_response_t*, size_t *len);
void http_response_destroy(http_response_t*);

#ifdef __cplusplus
}
#endif

#endif // TERRARIUM_NET_HTTP_LIB_H
