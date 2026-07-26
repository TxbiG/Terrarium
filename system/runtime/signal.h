#ifndef TERRARIUM_SYSTEM_RUNTIME_SIGNAL_H
#define TERRARIUM_SYSTEM_RUNTIME_SIGNAL_H

#include "process.h"

typedef void (*terra_signal_handler_t)(int signal);

int terra_signal_set_handler(int signal, terra_signal_handler_t handler);
int terra_signal_send(terra_pid_t pid, int signal);
int terra_signal_block(int signal);
int terra_signal_unblock(int signal);

#endif
