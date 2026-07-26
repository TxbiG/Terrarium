#ifndef TERRARIUM_LOG_SERVICE_H
#define TERRARIUM_LOG_SERVICE_H

int log_service_start(void);
int log_service_status(void);
int log_service_write(const char *message);

#endif
