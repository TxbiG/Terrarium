#ifndef TERRARIUM_SYSTEM_LOG_CRASH_H
#define TERRARIUM_SYSTEM_LOG_CRASH_H

#define TERRA_CRASH_REASON_MAX 256u
#define TERRA_CRASH_PROCESS_MAX 64u

typedef struct terra_crash_report {
    int pid;
    int signal;
    char process[TERRA_CRASH_PROCESS_MAX];
    char reason[TERRA_CRASH_REASON_MAX];
} terra_crash_report_t;

int terra_crash_write_report(const terra_crash_report_t *report);
int terra_crash_read_report(unsigned long index, terra_crash_report_t *out_report);
int terra_crash_report_count(void);

#endif
