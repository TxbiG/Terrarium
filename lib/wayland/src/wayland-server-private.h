#ifndef FLORA_SERVER_INTERNAL_H
#define FLORA_SERVER_INTERNAL_H

#include "server_core.h"

struct wl_priv_signal {
	struct wl_list listener_list;
	struct wl_list emit_list;
};

void wl_priv_signal_init(struct wl_priv_signal *signal);

void wl_priv_signal_add(struct wl_priv_signal *signal, struct wl_listener *listener);

struct wl_listener* wl_priv_signal_get(struct wl_priv_signal *signal, wl_notify_func_t notify);

void wl_priv_signal_emit(struct wl_priv_signal *signal, void *data);

void wl_priv_signal_final_emit(struct wl_priv_signal *signal, void *data);


#endif // FLORA_SERVER_H