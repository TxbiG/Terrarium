#ifndef TERRARIUM_NET_DNS_LIB_H
#define TERRARIUM_NET_DNS_LIB_H

// Add DNS caching policy, mDNS / service discovery

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dns_query dns_query_t;

typedef enum {
    DNS_OK = 0,
    DNS_ERR_TIMEOUT,
    DNS_ERR_NOT_FOUND,
    DNS_ERR_NO_RESPONSE,
    DNS_ERR_INTERNAL
} dns_error_t;

typedef enum {
    DNS_RECORD_A,
    DNS_RECORD_AAAA
} dns_record_type_t;

typedef struct {
    dns_record_type_t type;
    union {
        uint32_t ipv4;
        uint8_t  ipv6[16];
    };
} dns_result_t;

/* Resolve hostname → one or more addresses */
int dns_resolve(const char *hostname, dns_record_type_t type, dns_result_t *results, size_t max_results, size_t *out_count);

dns_error_t dns_last_error(void);

/* Callback signature */
typedef void (*dns_callback_t)(dns_query_t *query, dns_error_t status, dns_result_t *results, size_t count, void *user_data);

/* Submit async query */
dns_query_t* dns_resolve_async(const char *hostname, dns_record_type_t type, dns_callback_t cb, void *user_data);

/* Polling version (optional) */
int dns_poll(dns_query_t **queries, size_t count, int timeout_ms);

/* Cancel query */
void dns_query_cancel(dns_query_t* query);

int dns_resolve(const char *hostname, dns_record_type_t type, ...);


#ifdef __cplusplus
}
#endif

#endif // TERRARIUM_NET_DNS_LIB_H
