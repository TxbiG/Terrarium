#ifndef TERRARIUM_SYSTEM_DEV_SANITIZER_H
#define TERRARIUM_SYSTEM_DEV_SANITIZER_H

void terra_sanitizer_report_leak(const char *object, unsigned long bytes);
void terra_sanitizer_report_bounds(const char *object, unsigned long index);

#endif
