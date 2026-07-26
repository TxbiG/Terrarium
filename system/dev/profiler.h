#ifndef TERRARIUM_SYSTEM_DEV_PROFILER_H
#define TERRARIUM_SYSTEM_DEV_PROFILER_H

int terra_profiler_start(const char *name);
int terra_profiler_stop(const char *name);
int terra_profiler_dump(const char *path);

#endif
