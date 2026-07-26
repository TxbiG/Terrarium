#ifndef TERRARIUM_SYSTEM_LOG_TRACE_H
#define TERRARIUM_SYSTEM_LOG_TRACE_H

int terra_trace_enable(const char *channel);
int terra_trace_disable(const char *channel);
int terra_trace_emit(const char *channel, const char *event);

#endif
