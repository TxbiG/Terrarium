#ifndef TERRARIUM_SYSTEM_DEV_BACKTRACE_H
#define TERRARIUM_SYSTEM_DEV_BACKTRACE_H

typedef struct terra_backtrace_frame {
    void *address;
    char symbol[128];
} terra_backtrace_frame_t;

int terra_backtrace_capture(terra_backtrace_frame_t *frames, unsigned long max_frames);

#endif
