#include "backtrace.h"

#include <stdint.h>
#include <stdio.h>

#include "symbols.h"
#include "../status.h"

int terra_backtrace_capture(terra_backtrace_frame_t *frames, unsigned long max_frames) {
    if (!frames && max_frames != 0)
        return TERRA_STATUS_INVAL;
    if (max_frames == 0)
        return 0;
    frames[0].address = (void *)(uintptr_t)terra_backtrace_capture;
    if (terra_symbols_lookup(frames[0].address, frames[0].symbol, sizeof(frames[0].symbol)) != TERRA_STATUS_OK)
        snprintf(frames[0].symbol, sizeof(frames[0].symbol), "terra_backtrace_capture");
    return 1;
}
