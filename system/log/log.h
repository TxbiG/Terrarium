#ifndef TERRARIUM_SYSTEM_LOG_LOG_H
#define TERRARIUM_SYSTEM_LOG_LOG_H

typedef enum terra_log_level {
    TERRA_LOG_TRACE = 0,
    TERRA_LOG_DEBUG,
    TERRA_LOG_INFO,
    TERRA_LOG_WARN,
    TERRA_LOG_ERROR,
    TERRA_LOG_FATAL
} terra_log_level_t;

void terra_log(terra_log_level_t level, const char *component, const char *message);
void terra_logf(terra_log_level_t level, const char *component, const char *format, ...);

#endif
