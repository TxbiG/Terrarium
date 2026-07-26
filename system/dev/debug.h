#ifndef TERRARIUM_SYSTEM_DEV_DEBUG_H
#define TERRARIUM_SYSTEM_DEV_DEBUG_H

void terra_debug_break(void);
void terra_debug_print(const char *message);
void terra_debug_printf(const char *format, ...);

#endif
