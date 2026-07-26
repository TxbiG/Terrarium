#ifndef TERRARIUM_SYSTEM_RUNTIME_EXE_H
#define TERRARIUM_SYSTEM_RUNTIME_EXE_H

typedef enum terra_exe_format {
    TERRA_EXE_UNKNOWN = 0,
    TERRA_EXE_ELF,
    TERRA_EXE_SCRIPT
} terra_exe_format_t;

terra_exe_format_t terra_exe_probe(const char *path);
int terra_exe_can_run(const char *path);
int terra_exe_resolve(const char *name, char *out_path, unsigned long out_size);

#endif
