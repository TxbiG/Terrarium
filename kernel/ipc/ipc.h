#ifndef TERRARIUM_IPC_H
#define TERRARIUM_IPC_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_IPC_MAX_CHANNELS 64u
#define TERRA_IPC_MAX_MESSAGES 256u
#define TERRA_IPC_MAX_PAYLOAD 128u

typedef uint32_t terra_ipc_channel_t;

typedef enum terra_ipc_status {
    TERRA_IPC_OK = 0,
    TERRA_IPC_ERR_NOT_READY = -1,
    TERRA_IPC_ERR_INVALID = -2,
    TERRA_IPC_ERR_NO_SPACE = -3,
    TERRA_IPC_ERR_NOT_FOUND = -4,
    TERRA_IPC_ERR_CLOSED = -5,
    TERRA_IPC_ERR_DENIED = -6,
    TERRA_IPC_ERR_EMPTY = -7,
    TERRA_IPC_ERR_TOO_LARGE = -8
} terra_ipc_status_t;

typedef enum terra_ipc_channel_flags {
    TERRA_IPC_CHANNEL_NONE = 0,
    TERRA_IPC_CHANNEL_KERNEL_ONLY = 1u << 0,
    TERRA_IPC_CHANNEL_BROADCAST = 1u << 1
} terra_ipc_channel_flags_t;

typedef enum terra_ipc_message_type {
    TERRA_IPC_MSG_DATA = 0,
    TERRA_IPC_MSG_SIGNAL = 1,
    TERRA_IPC_MSG_REQUEST = 2,
    TERRA_IPC_MSG_REPLY = 3,
    TERRA_IPC_MSG_EVENT = 4
} terra_ipc_message_type_t;

typedef struct terra_ipc_message {
    uint32_t type;
    uint32_t sender;
    uint32_t receiver;
    uint64_t sequence;
    uint64_t cookie;
    size_t size;
    uint8_t payload[TERRA_IPC_MAX_PAYLOAD];
} terra_ipc_message_t;

typedef struct terra_ipc_channel_info {
    terra_ipc_channel_t id;
    uint32_t owner;
    uint32_t flags;
    uint32_t queued_messages;
    uint32_t capacity;
    uint64_t sent_messages;
    uint64_t received_messages;
    uint64_t dropped_messages;
    int closed;
} terra_ipc_channel_info_t;

void ipc_init(const terra_boot_info_t *boot_info);
int ipc_is_ready(void);
void ipc_shutdown(void);
terra_ipc_status_t ipc_channel_create(uint32_t owner,
                                      uint32_t flags,
                                      terra_ipc_channel_t *out_channel);
terra_ipc_status_t ipc_channel_close(terra_ipc_channel_t channel,
                                     uint32_t caller);
terra_ipc_status_t ipc_channel_info(terra_ipc_channel_t channel,
                                    terra_ipc_channel_info_t *out_info);
terra_ipc_status_t ipc_send(terra_ipc_channel_t channel,
                            uint32_t sender,
                            uint32_t receiver,
                            uint32_t type,
                            const void *payload,
                            size_t size,
                            uint64_t cookie);
terra_ipc_status_t ipc_receive(terra_ipc_channel_t channel,
                               uint32_t receiver,
                               terra_ipc_message_t *out_message);
terra_ipc_status_t ipc_peek(terra_ipc_channel_t channel,
                            uint32_t receiver,
                            terra_ipc_message_t *out_message);
terra_ipc_status_t ipc_drain(terra_ipc_channel_t channel, uint32_t caller);

#ifdef __cplusplus
}
#endif

#endif
