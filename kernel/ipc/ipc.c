#include "ipc.h"

typedef struct terra_ipc_queue_entry {
    terra_ipc_message_t message;
    uint16_t next;
    uint8_t active;
} terra_ipc_queue_entry_t;

typedef struct terra_ipc_channel {
    terra_ipc_channel_t id;
    uint32_t owner;
    uint32_t flags;
    uint16_t head;
    uint16_t tail;
    uint32_t queued;
    uint64_t sent;
    uint64_t received;
    uint64_t dropped;
    uint64_t next_sequence;
    uint8_t active;
    uint8_t closed;
} terra_ipc_channel_state_t;

#define TERRA_IPC_NO_ENTRY 0xffffu

static terra_ipc_channel_state_t channels[TERRA_IPC_MAX_CHANNELS];
static terra_ipc_queue_entry_t messages[TERRA_IPC_MAX_MESSAGES];
static terra_ipc_channel_t next_channel_id;
static int ipc_ready;

static void ipc_copy(void *dest, const void *src, size_t size) {
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;
    for (size_t i = 0; i < size; ++i)
        d[i] = s[i];
}

static void ipc_clear_message(terra_ipc_message_t *message) {
    message->type = 0;
    message->sender = 0;
    message->receiver = 0;
    message->sequence = 0;
    message->cookie = 0;
    message->size = 0;
    for (size_t i = 0; i < TERRA_IPC_MAX_PAYLOAD; ++i)
        message->payload[i] = 0;
}

static void ipc_reset(void) {
    for (uint32_t i = 0; i < TERRA_IPC_MAX_CHANNELS; ++i) {
        channels[i].id = 0;
        channels[i].owner = 0;
        channels[i].flags = 0;
        channels[i].head = TERRA_IPC_NO_ENTRY;
        channels[i].tail = TERRA_IPC_NO_ENTRY;
        channels[i].queued = 0;
        channels[i].sent = 0;
        channels[i].received = 0;
        channels[i].dropped = 0;
        channels[i].next_sequence = 1;
        channels[i].active = 0;
        channels[i].closed = 0;
    }

    for (uint32_t i = 0; i < TERRA_IPC_MAX_MESSAGES; ++i) {
        ipc_clear_message(&messages[i].message);
        messages[i].next = TERRA_IPC_NO_ENTRY;
        messages[i].active = 0;
    }

    next_channel_id = 1;
}

static terra_ipc_channel_state_t *ipc_find_channel(terra_ipc_channel_t id) {
    if (!id)
        return 0;

    for (uint32_t i = 0; i < TERRA_IPC_MAX_CHANNELS; ++i) {
        if (channels[i].active && channels[i].id == id)
            return &channels[i];
    }

    return 0;
}

static int ipc_caller_allowed(const terra_ipc_channel_state_t *channel,
                              uint32_t caller) {
    if (!channel)
        return 0;

    if (!(channel->flags & TERRA_IPC_CHANNEL_KERNEL_ONLY))
        return 1;

    return caller == 0 || caller == channel->owner;
}

static uint16_t ipc_alloc_message(void) {
    for (uint32_t i = 0; i < TERRA_IPC_MAX_MESSAGES; ++i) {
        if (!messages[i].active) {
            messages[i].active = 1;
            messages[i].next = TERRA_IPC_NO_ENTRY;
            ipc_clear_message(&messages[i].message);
            return (uint16_t)i;
        }
    }

    return TERRA_IPC_NO_ENTRY;
}

static void ipc_free_message(uint16_t id) {
    if (id >= TERRA_IPC_MAX_MESSAGES)
        return;

    ipc_clear_message(&messages[id].message);
    messages[id].next = TERRA_IPC_NO_ENTRY;
    messages[id].active = 0;
}

static terra_ipc_status_t ipc_pop_matching(terra_ipc_channel_state_t *channel,
                                           uint32_t receiver,
                                           terra_ipc_message_t *out_message,
                                           int remove) {
    uint16_t prev = TERRA_IPC_NO_ENTRY;
    uint16_t current = channel->head;

    while (current != TERRA_IPC_NO_ENTRY) {
        terra_ipc_queue_entry_t *entry = &messages[current];
        uint16_t next = entry->next;
        int receiver_matches =
            (entry->message.receiver == 0) ||
            (receiver == 0) ||
            (entry->message.receiver == receiver) ||
            (channel->flags & TERRA_IPC_CHANNEL_BROADCAST);

        if (entry->active && receiver_matches) {
            if (out_message)
                *out_message = entry->message;

            if (remove) {
                if (prev == TERRA_IPC_NO_ENTRY)
                    channel->head = next;
                else
                    messages[prev].next = next;

                if (channel->tail == current)
                    channel->tail = prev;

                channel->queued--;
                channel->received++;
                ipc_free_message(current);
            }

            return TERRA_IPC_OK;
        }

        prev = current;
        current = next;
    }

    return TERRA_IPC_ERR_EMPTY;
}

void ipc_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    ipc_reset();
    ipc_ready = 1;
}

int ipc_is_ready(void) {
    return ipc_ready;
}

void ipc_shutdown(void) {
    ipc_reset();
    ipc_ready = 0;
}

terra_ipc_status_t ipc_channel_create(uint32_t owner,
                                      uint32_t flags,
                                      terra_ipc_channel_t *out_channel) {
    if (!ipc_ready)
        return TERRA_IPC_ERR_NOT_READY;
    if (!out_channel)
        return TERRA_IPC_ERR_INVALID;

    for (uint32_t i = 0; i < TERRA_IPC_MAX_CHANNELS; ++i) {
        if (channels[i].active)
            continue;

        channels[i].id = next_channel_id++;
        if (!next_channel_id)
            next_channel_id = 1;

        channels[i].owner = owner;
        channels[i].flags = flags;
        channels[i].head = TERRA_IPC_NO_ENTRY;
        channels[i].tail = TERRA_IPC_NO_ENTRY;
        channels[i].queued = 0;
        channels[i].sent = 0;
        channels[i].received = 0;
        channels[i].dropped = 0;
        channels[i].next_sequence = 1;
        channels[i].closed = 0;
        channels[i].active = 1;
        *out_channel = channels[i].id;
        return TERRA_IPC_OK;
    }

    return TERRA_IPC_ERR_NO_SPACE;
}

terra_ipc_status_t ipc_channel_close(terra_ipc_channel_t channel,
                                     uint32_t caller) {
    terra_ipc_channel_state_t *state = ipc_find_channel(channel);
    if (!state)
        return TERRA_IPC_ERR_NOT_FOUND;
    if (caller != 0 && caller != state->owner)
        return TERRA_IPC_ERR_DENIED;

    ipc_drain(channel, caller);
    state->closed = 1;
    state->active = 0;
    return TERRA_IPC_OK;
}

terra_ipc_status_t ipc_channel_info(terra_ipc_channel_t channel,
                                    terra_ipc_channel_info_t *out_info) {
    terra_ipc_channel_state_t *state = ipc_find_channel(channel);
    if (!state)
        return TERRA_IPC_ERR_NOT_FOUND;
    if (!out_info)
        return TERRA_IPC_ERR_INVALID;

    out_info->id = state->id;
    out_info->owner = state->owner;
    out_info->flags = state->flags;
    out_info->queued_messages = state->queued;
    out_info->capacity = TERRA_IPC_MAX_MESSAGES;
    out_info->sent_messages = state->sent;
    out_info->received_messages = state->received;
    out_info->dropped_messages = state->dropped;
    out_info->closed = state->closed != 0;
    return TERRA_IPC_OK;
}

terra_ipc_status_t ipc_send(terra_ipc_channel_t channel,
                            uint32_t sender,
                            uint32_t receiver,
                            uint32_t type,
                            const void *payload,
                            size_t size,
                            uint64_t cookie) {
    if (!ipc_ready)
        return TERRA_IPC_ERR_NOT_READY;
    if (size > TERRA_IPC_MAX_PAYLOAD)
        return TERRA_IPC_ERR_TOO_LARGE;
    if (size && !payload)
        return TERRA_IPC_ERR_INVALID;

    terra_ipc_channel_state_t *state = ipc_find_channel(channel);
    if (!state)
        return TERRA_IPC_ERR_NOT_FOUND;
    if (state->closed)
        return TERRA_IPC_ERR_CLOSED;
    if (!ipc_caller_allowed(state, sender))
        return TERRA_IPC_ERR_DENIED;

    uint16_t msg_id = ipc_alloc_message();
    if (msg_id == TERRA_IPC_NO_ENTRY) {
        state->dropped++;
        return TERRA_IPC_ERR_NO_SPACE;
    }

    terra_ipc_message_t *message = &messages[msg_id].message;
    message->type = type;
    message->sender = sender;
    message->receiver = receiver;
    message->sequence = state->next_sequence++;
    message->cookie = cookie;
    message->size = size;
    if (size)
        ipc_copy(message->payload, payload, size);

    if (state->tail == TERRA_IPC_NO_ENTRY) {
        state->head = msg_id;
        state->tail = msg_id;
    } else {
        messages[state->tail].next = msg_id;
        state->tail = msg_id;
    }

    state->queued++;
    state->sent++;
    return TERRA_IPC_OK;
}

terra_ipc_status_t ipc_receive(terra_ipc_channel_t channel,
                               uint32_t receiver,
                               terra_ipc_message_t *out_message) {
    if (!ipc_ready)
        return TERRA_IPC_ERR_NOT_READY;
    if (!out_message)
        return TERRA_IPC_ERR_INVALID;

    terra_ipc_channel_state_t *state = ipc_find_channel(channel);
    if (!state)
        return TERRA_IPC_ERR_NOT_FOUND;

    return ipc_pop_matching(state, receiver, out_message, 1);
}

terra_ipc_status_t ipc_peek(terra_ipc_channel_t channel,
                            uint32_t receiver,
                            terra_ipc_message_t *out_message) {
    if (!ipc_ready)
        return TERRA_IPC_ERR_NOT_READY;
    if (!out_message)
        return TERRA_IPC_ERR_INVALID;

    terra_ipc_channel_state_t *state = ipc_find_channel(channel);
    if (!state)
        return TERRA_IPC_ERR_NOT_FOUND;

    return ipc_pop_matching(state, receiver, out_message, 0);
}

terra_ipc_status_t ipc_drain(terra_ipc_channel_t channel, uint32_t caller) {
    terra_ipc_channel_state_t *state = ipc_find_channel(channel);
    if (!state)
        return TERRA_IPC_ERR_NOT_FOUND;
    if (caller != 0 && caller != state->owner)
        return TERRA_IPC_ERR_DENIED;

    uint16_t current = state->head;
    while (current != TERRA_IPC_NO_ENTRY) {
        uint16_t next = messages[current].next;
        ipc_free_message(current);
        current = next;
    }

    state->head = TERRA_IPC_NO_ENTRY;
    state->tail = TERRA_IPC_NO_ENTRY;
    state->dropped += state->queued;
    state->queued = 0;
    return TERRA_IPC_OK;
}
