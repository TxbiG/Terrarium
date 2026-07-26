#include "debug.h"

#include <stdarg.h>
#include <stdio.h>

void terra_debug_break(void) {
}

void terra_debug_print(const char *message) {
    if (message)
        fputs(message, stderr);
}

void terra_debug_printf(const char *format, ...) {
    if (!format)
        return;
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}
